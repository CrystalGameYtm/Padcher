// custom_patcher.cpp

#include "asm_patcher.h"
#include <sstream>       
#include <cstring>       

// �������� ��������� ��� ����� �����. ���� �������� ��� MyForm.h
struct CustomPatch {
    long offset;
    byte_vector newData;
    byte_vector originalData;
};

// ��������� ���� ������� �������
std::string apply_custom_asm_patch(byte_vector& rom_data) {
    // ====================================================================
    // == �������� ��ز ���ײ ��� ==
    // ====================================================================
    // �� ����� ����, ��� ��� ������� ���������� ��� ���������/���� ������.
    std::vector<CustomPatch> my_patches = {
        // ������� 1: ������ ������� ����� � 3 �� 99 (����� ���)
        {
    0x1A40,
    {0xA9, 0x63},
    {}  // ��� �������� (������� ������)
},
        // ������� 2: "NOP-���" ���������� �� ������� 0x2B10 (����� ���)
       {
    0x2B10,
    {0xEA},
    {0xBD}  // <--- ������� D �� B
},
        // ������� ���� ������ ������, ������ �������...
    };
    // ====================================================================

    for (const auto& patch : my_patches) {
        // ��������, �� �� �������� ���� �� ��� �����
        if (patch.offset < 0 || patch.offset + patch.newData.size() > rom_data.size()) {
            std::stringstream ss;
            ss << "�������: ���� �� ������� 0x" << std::hex << patch.offset << " �������� �� ��� �����.";
            return ss.str(); // ��������� ����� � ��������
        }

        // ����������� �������� ����������� �����
        if (!patch.originalData.empty()) {
            if (memcmp(rom_data.data() + patch.offset, patch.originalData.data(), patch.originalData.size()) != 0) {
                std::stringstream ss;
                ss << "�������: ��������� ����� �� ������� 0x" << std::hex << patch.offset << " �� ����������.\n";
                ss << "�������� �������� � ����: 0x" << std::hex << (int)rom_data[patch.offset + 0] << "\n";
                ss << "� ��������� ��������: 0x" << std::hex << (int)patch.originalData[0] << "\n";
                ss << "���� �� �����������.";
                return ss.str();
            }
        }

        // ������������ �����: ������� ��� ���
        memcpy(rom_data.data() + patch.offset, patch.newData.data(), patch.newData.size());
    }

    return ""; // ����! ��������� ������� �����.
}