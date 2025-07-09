// custom_patcher.h

#pragma once

#include <vector>
#include <string>

// ������������� �������� ��� ��������, ����� �����, �� � ������ ������
using byte_vector = std::vector<unsigned char>;

/**
 * @brief ��������� ���������� ���� ASM-������ �� ����� ROM.
 *
 * �������� ������ ����� `rom_data` �� ����.
 *
 * @param rom_data ������ ����� ROM-�����, ���� ���� ������.
 * @return ������� ����� "" ��� �����, ��� ����� � ������ �������, ���� ���� ���� �� ���.
 */
std::string apply_custom_asm_patch(byte_vector& rom_data);