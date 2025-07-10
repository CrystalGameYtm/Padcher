#include "bps_patcher.h"
#include <stdexcept>
#include <cstring>
#include <numeric>
#include <algorithm> // для std::min

// Анонімний namespace для внутрішніх (хелперних) функцій
namespace {

    // --- РЕАЛІЗАЦІЯ CRC32 ---
    class Crc32 {
    private:
        uint32_t table[256];
    public:
        Crc32() {
            uint32_t polynomial = 0xEDB88320;
            for (uint32_t i = 0; i < 256; i++) {
                uint32_t c = i;
                for (size_t j = 0; j < 8; j++) {
                    c = (c & 1) ? (polynomial ^ (c >> 1)) : (c >> 1);
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
    Crc32 crc32_calculator;
    uint32_t calculate_crc32(const uint8_t* data, size_t length) {
        return crc32_calculator.calculate(data, length);
    }
    uint32_t calculate_crc32_vector(const byte_vector& data) {
        return crc32_calculator.calculate(data.data(), data.size());
    }

    // --- РЕАЛІЗАЦІЯ DataReader ---
    class DataReader {
    private:
        const byte_vector& data_;
        size_t offset_ = 0;
    public:
        DataReader(const byte_vector& data) : data_(data) {}
        uint8_t read_u8() {
            if (offset_ >= data_.size()) throw std::runtime_error("DataReader: read out of bounds");
            return data_[offset_++];
        }
        std::string read_string(size_t len) {
            if (offset_ + len > data_.size()) throw std::runtime_error("DataReader: string read out of bounds");
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
            if (offset_ + len > data_.size()) throw std::runtime_error("DataReader: bytes read out of bounds");
            byte_vector result(data_.begin() + offset_, data_.begin() + offset_ + len);
            offset_ += len;
            return result;
        }
        size_t position() const { return offset_; }
        void seek(size_t new_offset) { offset_ = new_offset; }
    };

    // --- РЕАЛІЗАЦІЯ парсера BPS ---
    BpsPatch parse_bps(const byte_vector& patch_data) {
        DataReader reader(patch_data);
        if (reader.read_string(4) != "BPS1") throw std::runtime_error("Invalid BPS signature");
        BpsPatch patch;
        patch.source_size = reader.read_vlv();
        patch.target_size = reader.read_vlv();
        uint64_t metadata_len = reader.read_vlv();
        if (metadata_len > 0) patch.metadata = reader.read_string(metadata_len);

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
        if (patch.patch_checksum_from_file != calculate_crc32(patch_data.data(), patch_data.size() - 4)) {
            throw std::runtime_error("Patch checksum mismatch");
        }
        return patch;
    }

    // --- ХЕЛПЕРИ для створення патчів ---
    void write_vlv(byte_vector& dest, uint64_t value) {
        while (true) {
            uint8_t x = value & 0x7f;
            value >>= 7;
            if (value == 0) {
                dest.push_back(0x80 | x);
                break;
            }
            dest.push_back(x);
            value--;
        }
    }
    void write_u32(byte_vector& dest, uint32_t value) {
        dest.push_back(value & 0xff);
        dest.push_back((value >> 8) & 0xff);
        dest.push_back((value >> 16) & 0xff);
        dest.push_back((value >> 24) & 0xff);
    }

} // кінець анонімного namespace

// --- РЕАЛІЗАЦІЯ ПУБЛІЧНИХ ФУНКЦІЙ ---

byte_vector apply_bps_patch(const byte_vector& source_rom, const byte_vector& patch_data, bool validate_checksums) {
    BpsPatch patch = parse_bps(patch_data);

    if (validate_checksums && patch.source_checksum != calculate_crc32_vector(source_rom)) {
        throw std::runtime_error("Source ROM checksum mismatch");
    }

    byte_vector target_rom(patch.target_size);
    size_t target_write_offset = 0;
    int64_t source_relative_offset = 0;
    int64_t target_relative_offset = 0;

    auto write_to_target = [&](const uint8_t* data, size_t len) {
        if (target_write_offset + len > target_rom.size()) len = target_rom.size() - target_write_offset;
        if (len > 0) {
            memcpy(&target_rom[target_write_offset], data, len);
            target_write_offset += len;
        }
        };

    for (const auto& action : patch.actions) {
        uint64_t length = action.length;
        switch (action.type) {
        case BpsAction::SourceRead:
            if (target_write_offset + length > source_rom.size()) throw std::runtime_error("SourceRead out of bounds");
            write_to_target(&source_rom[target_write_offset], length);
            break;
        case BpsAction::TargetRead:
            if (action.bytes_to_write.size() < length) throw std::runtime_error("Not enough data for TargetRead");
            write_to_target(action.bytes_to_write.data(), length);
            break;
        case BpsAction::SourceCopy:
            source_relative_offset += action.relative_offset;
            for (uint64_t i = 0; i < length; ++i) {
                if (source_relative_offset < 0 || (size_t)source_relative_offset >= source_rom.size()) throw std::runtime_error("SourceCopy out of bounds");
                write_to_target(&source_rom[source_relative_offset], 1);
                source_relative_offset++;
            }
            break;
        case BpsAction::TargetCopy:
            target_relative_offset += action.relative_offset;
            for (uint64_t i = 0; i < length; ++i) {
                if (target_relative_offset < 0 || (size_t)target_relative_offset >= target_write_offset) throw std::runtime_error("TargetCopy out of bounds");
                uint8_t byte_to_copy = target_rom[target_relative_offset];
                write_to_target(&byte_to_copy, 1);
                target_relative_offset++;
            }
            break;
        }
    }

    if (validate_checksums && patch.target_checksum != calculate_crc32_vector(target_rom)) {
        throw std::runtime_error("Target ROM checksum mismatch after patching");
    }
    return target_rom;
}


byte_vector create_bps_patch(const byte_vector& original_rom, const byte_vector& modified_rom, bool, const std::string& metadata) {
    byte_vector patch_data;

    // 1. Заголовок
    patch_data.insert(patch_data.end(), { 'B', 'P', 'S', '1' });

    // 2. Розміри файлів та метадані
    write_vlv(patch_data, original_rom.size());
    write_vlv(patch_data, modified_rom.size());
    write_vlv(patch_data, metadata.length());
    if (!metadata.empty()) {
        patch_data.insert(patch_data.end(), metadata.begin(), metadata.end());
    }

    // 3. Створення дій (проста лінійна різниця)
    size_t scan_pos = 0;
    const size_t max_len = std::min(original_rom.size(), modified_rom.size());

    while (scan_pos < modified_rom.size()) {
        // Знаходимо, скільки байт збігається з вихідним файлом
        size_t source_read_len = 0;
        if (scan_pos < original_rom.size()) {
            while (scan_pos + source_read_len < max_len &&
                original_rom[scan_pos + source_read_len] == modified_rom[scan_pos + source_read_len]) {
                source_read_len++;
            }
        }

        if (source_read_len > 4) { // Використовуємо SourceRead, якщо є сенс
            uint64_t command = (BpsAction::SourceRead) | ((source_read_len - 1) << 2);
            write_vlv(patch_data, command);
            scan_pos += source_read_len;
        }

        // Знаходимо, скільки байт відрізняється
        size_t target_read_start = scan_pos;
        while (scan_pos < modified_rom.size() && (scan_pos >= original_rom.size() || original_rom[scan_pos] != modified_rom[scan_pos])) {
            scan_pos++;
        }
        size_t target_read_len = scan_pos - target_read_start;

        if (target_read_len > 0) {
            uint64_t command = (BpsAction::TargetRead) | ((target_read_len - 1) << 2);
            write_vlv(patch_data, command);
            patch_data.insert(patch_data.end(),
                modified_rom.begin() + target_read_start,
                modified_rom.begin() + target_read_start + target_read_len);
        }
    }

    // 4. Контрольні суми
    write_u32(patch_data, calculate_crc32_vector(original_rom));
    write_u32(patch_data, calculate_crc32_vector(modified_rom));
    // CRC32 самого патчу (без останніх 4 байт)
    write_u32(patch_data, calculate_crc32(patch_data.data(), patch_data.size()));

    return patch_data;
}