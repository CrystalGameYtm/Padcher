// custom_patcher.cpp

#include "asm_patcher.h"
#include <sstream>       
#include <cstring>       

// Внутрішня структура для опису патча. Вона невидима для MyForm.h
struct CustomPatch {
    long offset;
    byte_vector newData;
    byte_vector originalData;
};

// Реалізація нашої головної функції
std::string apply_custom_asm_patch(byte_vector& rom_data) {
    // ====================================================================
    // == ВИЗНАЧТЕ ВАШІ ПАТЧІ ТУТ ==
    // ====================================================================
    // Це єдине місце, яке вам потрібно редагувати для додавання/зміни патчів.
    std::vector<CustomPatch> my_patches = {
        // Приклад 1: Змінити кількість життів з 3 на 99 (умовні дані)
        {
    0x1A40,
    {0xA9, 0x63},
    {}  // БЕЗ перевірки (порожній вектор)
},
        // Приклад 2: "NOP-інг" інструкції за адресою 0x2B10 (умовні дані)
       {
    0x2B10,
    {0xEA},
    {0xBD}  // <--- Замінили D на B
},
        // Додайте сюди стільки патчів, скільки потрібно...
    };
    // ====================================================================

    for (const auto& patch : my_patches) {
        // Перевірка, чи не виходить патч за межі файлу
        if (patch.offset < 0 || patch.offset + patch.newData.size() > rom_data.size()) {
            std::stringstream ss;
            ss << "Помилка: Патч за адресою 0x" << std::hex << patch.offset << " виходить за межі файлу.";
            return ss.str(); // Повертаємо рядок з помилкою
        }

        // Опціональна перевірка оригінальних байтів
        if (!patch.originalData.empty()) {
            if (memcmp(rom_data.data() + patch.offset, patch.originalData.data(), patch.originalData.size()) != 0) {
                std::stringstream ss;
                ss << "Помилка: Оригінальні байти за адресою 0x" << std::hex << patch.offset << " не співпадають.\n";
                ss << "Програма побачила у файлі: 0x" << std::hex << (int)rom_data[patch.offset + 0] << "\n";
                ss << "А очікувала побачити: 0x" << std::hex << (int)patch.originalData[0] << "\n";
                ss << "Патч не застосовано.";
                return ss.str();
            }
        }

        // Застосування патча: копіюємо нові дані
        memcpy(rom_data.data() + patch.offset, patch.newData.data(), patch.newData.size());
    }

    return ""; // Успіх! Повертаємо порожній рядок.
}