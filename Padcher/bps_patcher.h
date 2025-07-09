#ifndef BPS_PATCHER_H
#define BPS_PATCHER_H

#include <vector>
#include <string>
#include <cstdint>

// ��� ��� ������� �����
using byte_vector = std::vector<uint8_t>;

// ���������, �� ����� ���� �� � BPS-�����
struct BpsAction {
    enum Type : uint8_t {
        SourceRead,
        TargetRead,
        SourceCopy,
        TargetCopy
    };

    Type type;
    uint64_t length = 0;

    // ���, ��������� ��� ������ ���� ��
    byte_vector bytes_to_write; // ��� TargetRead
    using byte_vector = std::vector<uint8_t>;
    int64_t relative_offset = 0;  // ��� SourceCopy, TargetCopy
};

// ���������, �� ����������� ����������� BPS-����
struct BpsPatch {
    uint64_t source_size = 0;
    uint64_t target_size = 0;
    std::string metadata;
    std::vector<BpsAction> actions;

    uint32_t source_checksum = 0;
    uint32_t target_checksum = 0;
    uint32_t patch_checksum_from_file = 0;
};

/**
 * @brief ��������� BPS-���� �� ��������� �����.
 * @param source_rom ��� ������������ ROM-�����.
 * @param patch_data ��� BPS-�����.
 * @param validate_checksums ���� true, ��������������� ��������� ����.
 * @return ��� ������������ ROM-�����.
 * @throw std::runtime_error � ������� ������� (������������ ������, ������������ CRC).
 */
byte_vector apply_bps_patch(const byte_vector& source_rom, const byte_vector& patch_data, bool validate_checksums = true);
byte_vector create_bps_patch(const byte_vector& original_rom, const byte_vector& modified_rom, bool use_delta_mode, const std::string& metadata = "");

#endif // BPS_PATCHER_H#pragma once
