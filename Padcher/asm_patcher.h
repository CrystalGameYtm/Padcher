// custom_patcher.h

#pragma once

#include <vector>
#include <string>

// Використовуємо псевдонім для зручності, такий самий, як у вашому проекті
using byte_vector = std::vector<unsigned char>;

/**
 * @brief Застосовує вбудований набір ASM-патчів до даних ROM.
 *
 * Модифікує вектор даних `rom_data` на місці.
 *
 * @param rom_data Вектор байтів ROM-файлу, який буде змінено.
 * @return Порожній рядок "" при успіху, або рядок з описом помилки, якщо щось пішло не так.
 */
std::string apply_custom_asm_patch(byte_vector& rom_data);