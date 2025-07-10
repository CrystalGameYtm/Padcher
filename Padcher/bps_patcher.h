#ifndef BPS_PATCHER_H
#define BPS_PATCHER_H

#include <vector>
#include <string>
#include <cstdint>

// Тип для бінарних даних
using byte_vector = std::vector<uint8_t>;

// Структура, що описує одну дію в BPS-патчі
struct BpsAction {
    enum Type : uint8_t {
        SourceRead,
        TargetRead,
        SourceCopy,
        TargetCopy
    };
    Type type;
    uint64_t length = 0;
    byte_vector bytes_to_write;
    int64_t relative_offset = 0;
};

// Структура, що представляє розпарсений BPS-патч
struct BpsPatch {
    uint64_t source_size = 0;
    uint64_t target_size = 0;
    std::string metadata;
    std::vector<BpsAction> actions;
    uint32_t source_checksum = 0;
    uint32_t target_checksum = 0;
    uint32_t patch_checksum_from_file = 0;
};

// --- ОГОЛОШЕННЯ ФУНКЦІЙ (ПРОТОТИПИ) ---

/**
 * @brief Застосовує BPS-патч до вихідного файлу.
 */
byte_vector apply_bps_patch(const byte_vector& source_rom, const byte_vector& patch_data, bool validate_checksums = true);

/**
 * @brief Створює BPS-патч на основі різниці між двома файлами.
 */
byte_vector create_bps_patch(const byte_vector& original_rom, const byte_vector& modified_rom, bool use_delta_mode, const std::string& metadata = "");

#endif // BPS_PATCHER_H