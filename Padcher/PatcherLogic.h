#pragma once

#include "bps_patcher.h"
#include "flips.hpp"
#include <msclr/marshal_cppstd.h>
#include <fstream>
#include <string>
#include <vector>

// Використовуємо стандартні простори імен, щоб не писати System::...
using namespace System;
using namespace System::IO;
using namespace System::Security::Cryptography;
using namespace System::Text;

// --- ДОПОМІЖНІ ТИПИ ТА ФУНКЦІЇ, ЩО ПРАЦЮЮТЬ З НАТИВНИМ C++ ---

// Визначаємо byte_vector, щоб не писати std::vector<unsigned char>
typedef std::vector<unsigned char> byte_vector;

// Функція для читання файлу в нативний вектор байтів
inline byte_vector read_file_native(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Не вдалося відкрити файл: " + filepath);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    byte_vector buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Не вдалося прочитати файл: " + filepath);
    }
    return buffer;
}

// Функція для запису нативного вектора байтів у файл
inline void write_file_native(const std::string& filepath, const byte_vector& data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не вдалося створити або відкрити для запису файл: " + filepath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// --- ФУНКЦІЇ ДЛЯ КОНТРОЛЬНИХ СУМ (працюють з .NET Streams) ---

inline String^ ComputeHash(Stream^ stream, HashAlgorithm^ hasher) {
    array<Byte>^ hash = hasher->ComputeHash(stream);
    StringBuilder^ sb = gcnew StringBuilder(hash->Length * 2);
    for each (Byte b in hash) {
        sb->Append(b.ToString("x2"));
    }
    return sb->ToString()->ToUpper();
}

inline String^ ComputeCrc32(Stream^ stream) {
    array<Byte>^ buffer = gcnew array<Byte>(stream->Length);
    stream->Read(buffer, 0, (int)stream->Length);

    uint32_t crc = 0xFFFFFFFF;
    for each (Byte b in buffer) {
        crc ^= b;
        for (int j = 0; j < 8; j++) {
            uint32_t mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return (~crc).ToString("X8");
}


// --- ГОЛОВНА ФУНКЦІЯ ПАТЧИНГУ (НОВА) ---

// Ця функція застосовує один будь-який патч (IPS, BPS, ASM)
inline byte_vector ApplySinglePatch(const byte_vector& source_rom, String^ patchPath, bool ignoreChecksums, String^ applicationPath) {
    String^ extension = Path::GetExtension(patchPath)->ToLower();
    std::string nativePatchPath = msclr::interop::marshal_as<std::string>(patchPath);

    if (extension == ".ips" || extension == ".bps") {
        // Ця частина залишається без змін...
        byte_vector patch_data = read_file_native(nativePatchPath);
        byte_vector target_data;
        if (extension == ".ips") {
            std::string error = flips::apply_ips(patch_data, source_rom, target_data);
            if (!error.empty()) throw std::runtime_error(error);
        }
        else { // .bps
            target_data = apply_bps_patch(source_rom, patch_data, !ignoreChecksums);
        }
        return target_data;
    }
    else if (extension == ".asm") {
        // Для .asm ми працюємо з тимчасовими файлами
        String^ tempRomPath = Path::GetTempFileName();
        // ВИКОРИСТОВУЄМО ПЕРЕДАНИЙ ШЛЯХ
        String^ asarPath = Path::Combine(applicationPath, "asar.exe");

        if (!File::Exists(asarPath)) {
            throw gcnew System::IO::FileNotFoundException("Не знайдено файл asar.exe в папці: " + applicationPath);
        }
        // ... інша частина логіки для asar залишається без змін ...
        try {
            std::string nativeTempRomPath = msclr::interop::marshal_as<std::string>(tempRomPath);
            write_file_native(nativeTempRomPath, source_rom);

            String^ commandLine = String::Format("\"{0}\" \"{1}\" \"{2}\"", asarPath, patchPath, tempRomPath);

            STARTUPINFOW si = { sizeof(si) };
            PROCESS_INFORMATION pi = { 0 };
            wchar_t* lpCommandLine = (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(commandLine).ToPointer();

            if (!CreateProcessW(NULL, lpCommandLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
                System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)lpCommandLine);
                throw gcnew Exception("Не вдалося запустити процес asar.exe. Помилка WinAPI: " + System::Runtime::InteropServices::Marshal::GetLastWin32Error());
            }

            System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)lpCommandLine);
            WaitForSingleObject(pi.hProcess, INFINITE);

            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            if (exitCode != 0) {
                throw gcnew Exception("Asar повідомив про помилку (код " + exitCode + ").");
            }

            return read_file_native(nativeTempRomPath);
        }
        finally {
            if (File::Exists(tempRomPath)) {
                File::Delete(tempRomPath);
            }
        }
    }
    else {
        throw gcnew NotSupportedException("Непідтримуваний формат патча: " + extension);
    }
}