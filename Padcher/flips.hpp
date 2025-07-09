#ifndef FLIPS_HPP
#define FLIPS_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm> // for std::min

namespace flips {

    // BPS VLV Reading Logic
    inline uint64_t read_vlv(const unsigned char*& data, const unsigned char* end) {
        uint64_t result = 0, shift = 1;
        while (data < end) {
            uint8_t x = *data++;
            result += (uint64_t)(x & 0x7f) * shift;
            if (x & 0x80) {
                break;
            }
            shift <<= 7;
            result += shift;
        }
        return result;
    }

    inline std::string apply_bps(
        const std::vector<unsigned char>& patch,
        const std::vector<unsigned char>& source,
        std::vector<unsigned char>& target)
    {
        const unsigned char* patch_ptr = patch.data();
        const unsigned char* patch_end = patch_ptr + patch.size();

        if (patch.size() < 16 || memcmp(patch_ptr, "BPS1", 4) != 0) {
            return "Patch is not a valid BPS file.";
        }
        patch_ptr += 4;

        uint64_t source_size = read_vlv(patch_ptr, patch_end);
        uint64_t target_size = read_vlv(patch_ptr, patch_end);
        uint64_t metadata_size = read_vlv(patch_ptr, patch_end);

        if (patch_ptr + metadata_size > patch_end - 12) {
            return "Patch metadata is corrupt.";
        }
        patch_ptr += metadata_size;

        target.assign(target_size, 0);

        int64_t source_relative_offset = 0;
        int64_t target_relative_offset = 0;

        while (patch_ptr < patch_end - 12) {
            uint64_t data = read_vlv(patch_ptr, patch_end);
            uint64_t command = data & 3;
            uint64_t length = (data >> 2) + 1;

            if (target.empty() && target_size > 0) return "Target vector was not allocated correctly.";

            switch (command) {
            case 0: // SourceRead
                for (uint64_t i = 0; i < length; i++) {
                    if (target.size() > target.size() - length)
                        if (target.size() > 0 && source.size() > 0)
                            target[target.size() - length + i] = source[target.size() - length + i];
                }
                break;
            case 1: // TargetRead
                if (patch_ptr + length > patch_end - 12) return "Patch file is corrupt (TargetRead overflow).";
                for (uint64_t i = 0; i < length; i++) {
                    if (target.size() - length >= 0)
                        target[target.size() - length + i] = patch_ptr[i];
                }
                patch_ptr += length;
                break;
            case 2: // SourceCopy
            {
                uint64_t offset_data = read_vlv(patch_ptr, patch_end);
                source_relative_offset += (offset_data & 1 ? -1 : 1) * (offset_data >> 1);
                for (uint64_t i = 0; i < length; i++) {
                    if (target.size() > 0 && source.size() > 0)
                        target[target.size() - length + i] = source[(uint64_t)source_relative_offset];
                    if (source_relative_offset < (int64_t)source.size() - 1)
                        source_relative_offset++;
                }
            }
            break;
            case 3: // TargetCopy
            {
                uint64_t offset_data = read_vlv(patch_ptr, patch_end);
                target_relative_offset += (offset_data & 1 ? -1 : 1) * (offset_data >> 1);
                for (uint64_t i = 0; i < length; i++) {
                    if (target.size() > 0)
                        target[target.size() - length + i] = target[(uint64_t)target_relative_offset];
                    if (target_relative_offset < (int64_t)target.size() - 1)
                        target_relative_offset++;
                }
            }
            break;
            }
        }
        return ""; 
    }

    // IPS Patching Logic
    inline std::string apply_ips(
        const std::vector<unsigned char>& patch,
        const std::vector<unsigned char>& source,
        std::vector<unsigned char>& target)
    {
        target = source;
        const unsigned char* patch_ptr = patch.data();
        size_t patch_offset = 0;
        if (patch.size() < 8 || memcmp(patch_ptr, "PATCH", 5) != 0) return "Patch is not a valid IPS file.";
        patch_offset += 5;
        while (patch_offset < patch.size() - 3) {
            uint32_t offset = (patch_ptr[patch_offset] << 16) | (patch_ptr[patch_offset + 1] << 8) | patch_ptr[patch_offset + 2];
            patch_offset += 3;
            if (offset == 0x454F46) return "";
            uint16_t length = (patch_ptr[patch_offset] << 8) | patch_ptr[patch_offset + 1];
            patch_offset += 2;
            if (length > 0) {
                if (offset + length > target.size()) target.resize(offset + length);
                if (patch_offset + length > patch.size()) return "Patch file is corrupt (standard record).";
                memcpy(target.data() + offset, patch_ptr + patch_offset, length);
                patch_offset += length;
            }
            else {
                uint16_t rle_length = (patch_ptr[patch_offset] << 8) | patch_ptr[patch_offset + 1];
                patch_offset += 2;
                unsigned char rle_byte = patch_ptr[patch_offset];
                patch_offset += 1;
                if (offset + rle_length > target.size()) target.resize(offset + rle_length);
                memset(target.data() + offset, rle_byte, rle_length);
            }
        }
        return "";
    }

    // CRC32 Calculation 
    inline uint32_t crc32_table[256] = { 0 };
    inline void build_crc32_table() {
        if (crc32_table[1]) return;
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t crc = i;
            for (int j = 0; j < 8; j++) crc = (crc & 1) ? ((crc >> 1) ^ 0xEDB88320) : (crc >> 1);
            crc32_table[i] = crc;
        }
    }
    inline uint32_t calculate_crc32(const unsigned char* data, size_t size) {
        build_crc32_table();
        uint32_t crc = 0xFFFFFFFF;
        for (size_t i = 0; i < size; i++) crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ data[i]];
        return ~crc;
    }
}
#endif //FLIPS_HPP