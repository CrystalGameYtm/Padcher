#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace Padcher {

    public ref class PatchComponent
    {
    public:
        property String^ FileName;
        property String^ Base64Data;
    };

    public ref class PadcherPack
    {
    public:
        property String^ BaseRomFileName;
        property List<PatchComponent^>^ Components;
        property String^ FullBaseRomPath; // Робоче поле, не зберігається напряму

        PadcherPack() {
            Components = gcnew List<PatchComponent^>();
        }
    };
}