#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace Padcher {

    public ref class PatchComponent
    {
    public:
        property String^ Type;
        property String^ FileName;
        property String^ Base64Data;
        property String^ RelativePath;
    };

    public ref class PadcherPack
    {
    public:
        property String^ BaseRomFileName;
        property List<PatchComponent^>^ Components;
        property String^ FullBaseRomPath; 

        PadcherPack() {
            Components = gcnew List<PatchComponent^>();
        }
    };
}