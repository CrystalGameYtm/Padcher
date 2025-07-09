#include "bps_patcher.h"
#include <stdexcept>
#include <cstring>
#include <numeric> // Для std::accumulate

namespace {

    // =========================================================================
    // ↓↓↓ СТАНДАРТНА, ПЕРЕВІРЕНА РЕАЛІЗАЦІЯ CRC32 ↓↓↓
    // =========================================================================
    class Crc32 {
    private:
        uint32_t table[256];
    public:
        Crc32() {
            uint32_t polynomial = 0xEDB88320;
            for (uint32_t i = 0; i < 256; i++) {
                uint32_t c = i;
                for (size_t j = 0; j < 8; j++) {
                    if (c & 1) {
                        c = polynomial ^ (c >> 1);
                    }
                    else {
                        c >>= 1;
                    }
                }
                table[i] = c;
            }
        }

        uint32_t calculate(const uint8_t* data, size_t length) {
            uint32_t crc = 0xFFFFFFFF;
            for (size_t i = 0; i < length; ++i) {
                crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
            }
            return crc ^ 0xFFFFFFFF;
        }
    };

    // Створюємо глобальний екземпляр, щоб таблиця генерувалася один раз
    Crc32 crc32_calculator;

    // Обгортки для зручності
    uint32_t calculate_crc32(const uint8_t* data, size_t length) {
        return crc32_calculator.calculate(data, length);
    }

    uint32_t calculate_crc32_vector(const byte_vector& data) {
        return crc32_calculator.calculate(data.data(), data.size());
    }
    // =========================================================================
    // ↑↑↑ КІНЕЦЬ СЕКЦІЇ CRC32 ↑↑↑
    // =========================================================================


    class DataReader {
        // ... (код класу DataReader залишається БЕЗ ЗМІН з попередньої версії) ...
    private:
        const byte_vector& data_;
        size_t offset_ = 0;
    public:
        DataReader(const byte_vector& data) : data_(data) {}

        uint8_t read_u8() {
            if (offset_ >= data_.size()) throw std::runtime_error("Читання за межами файлу");
            return data_[offset_++];
        }

        std::string read_string(size_t len) {
            if (offset_ + len > data_.size()) throw std::runtime_error("Читання за межами файлу");
            std::string result(reinterpret_cast<const char*>(&data_[offset_]), len);
            offset_ += len;
            return result;
        }

        uint64_t read_vlv() {
            uint64_t result = 0, shift = 1;
            while (true) {
                uint8_t x = read_u8();
                result += (uint64_t)(x & 0x7f) * shift;
                if (x & 0x80) break;
                shift <<= 7;
                result += shift;
            }
            return result;
        }

        uint32_t read_u32() {
            uint32_t value = 0;
            value |= (uint32_t)read_u8() << 0;
            value |= (uint32_t)read_u8() << 8;
            value |= (uint32_t)read_u8() << 16;
            value |= (uint32_t)read_u8() << 24;
            return value;
        }

        byte_vector read_bytes(size_t len) {
            if (offset_ + len > data_.size()) throw std::runtime_error("Читання за межами файлу");
            byte_vector result(data_.begin() + offset_, data_.begin() + offset_ + len);
            offset_ += len;
            return result;
        }

        size_t position() const { return offset_; }
        void seek(size_t new_offset) { offset_ = new_offset; }
    };

    BpsPatch parse_bps(const byte_vector& patch_data) {
        // ... (код функції parse_bps залишається БЕЗ ЗМІН) ...
        DataReader reader(patch_data);
        if (reader.read_string(4) != "BPS1") {
            throw std::runtime_error("Неправильна сигнатура BPS-файлу.");
        }
        BpsPatch patch;
        patch.source_size = reader.read_vlv();
        patch.target_size = reader.read_vlv();
        uint64_t metadata_len = reader.read_vlv();
        if (metadata_len > 0) {
            patch.metadata = reader.read_string(metadata_len);
        }
        size_t actions_end_offset = patch_data.size() - 12;
        while (reader.position() < actions_end_offset) {
            uint64_t data = reader.read_vlv();
            BpsAction action;
            action.type = static_cast<BpsAction::Type>(data & 0b11);
            action.length = (data >> 2) + 1;
            if (action.type == BpsAction::TargetRead) {
                action.bytes_to_write = reader.read_bytes(action.length);
            }
            else if (action.type == BpsAction::SourceCopy || action.type == BpsAction::TargetCopy) {
                uint64_t offset_data = reader.read_vlv();
                action.relative_offset = (offset_data & 1 ? -1 : 1) * (int64_t)(offset_data >> 1);
            }
            patch.actions.push_back(action);
        }
        reader.seek(actions_end_offset);
        patch.source_checksum = reader.read_u32();
        patch.target_checksum = reader.read_u32();
        patch.patch_checksum_from_file = reader.read_u32();

        // Перевірка контрольної суми самого патчу
        if (patch.patch_checksum_from_file != calculate_crc32(patch_data.data(), patch_data.size() - 4)) {
            throw std::runtime_error("Невідповідність контрольної суми самого патчу.");
        }
        return patch;
    }

} // кінець анонімного namespace

// ... (apply_bps_patch та create_bps_patch залишаються БЕЗ ЗМІН) ...
byte_vector apply_bps_patch(const byte_vector& source_rom, const byte_vector& patch_data, bool validate_checksums) {
    BpsPatch patch = parse_bps(patch_data);

    if (validate_checksums) {
        if (patch.source_checksum != calculate_crc32_vector(source_rom)) {
            throw std::runtime_error("Невідповідність контрольної суми вихідного ROM-файлу.");
        }
    }

    byte_vector target_rom(patch.target_size);
    size_t target_write_offset = 0; // Єдиний курсор для запису в цільовий файл

    int64_t source_relative_offset = 0;
    int64_t target_relative_offset = 0;

    auto write_to_target = [&](const uint8_t* data, size_t len) {
        if (target_write_offset + len > target_rom.size()) {
            // Можна кидати помилку, або просто ігнорувати, якщо патч намагається записати зайве
            len = target_rom.size() - target_write_offset;
        }
        if (len > 0) {
            memcpy(&target_rom[target_write_offset], data, len);
            target_write_offset += len;
        }
        };

    for (const auto& action : patch.actions) {
        uint64_t length = action.length;
        switch (action.type) {
        case BpsAction::SourceRead:
            // Читаємо з вихідного файлу з того ж зміщення, куди пишемо в цільовий
            if (target_write_offset + length > source_rom.size()) {
                throw std::runtime_error("SourceRead: спроба читання за межами вихідного файлу.");
            }
            write_to_target(&source_rom[target_write_offset], length);
            break;

        case BpsAction::TargetRead:
            // Просто пишемо байти з патчу
            if (action.bytes_to_write.size() < length) throw std::runtime_error("Недостатньо даних для TargetRead.");
            write_to_target(action.bytes_to_write.data(), length);
            break;

        case BpsAction::SourceCopy:
            source_relative_offset += action.relative_offset;
            for (uint64_t i = 0; i < length; ++i) {
                if (source_relative_offset < 0 || (size_t)source_relative_offset >= source_rom.size()) throw std::runtime_error("Вихід за межі при SourceCopy.");
                write_to_target(&source_rom[source_relative_offset], 1);
                source_relative_offset++;
            }
            break;

        case BpsAction::TargetCopy: // RLE-подібне копіювання
            target_relative_offset += action.relative_offset;
            for (uint64_t i = 0; i < length; ++i) {
                if (target_relative_offset < 0 || (size_t)target_relative_offset >= target_write_offset) throw std::runtime_error("Вихід за межі при TargetCopy.");
                // Читаємо байт з уже записаної частини цільового файлу
                uint8_t byte_to_copy = target_rom[target_relative_offset];
                write_to_target(&byte_to_copy, 1);
                target_relative_offset++;
            }
            break;
        }
    }

    if (validate_checksums) {
        if (patch.target_checksum != calculate_crc32_vector(target_rom)) {
            throw std::runtime_error("Невідповідність контрольної суми цільового ROM-файлу. Патчинг міг пройти з помилкою.");
        }
    }

    return target_rom;
}

byte_vector create_bps_patch(const byte_vector&, const byte_vector&, bool, const std::string&) {
    throw std::runtime_error("Функція create_bps_patch не реалізована.");
}