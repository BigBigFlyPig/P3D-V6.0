///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Implement all utility functions here
///----------------------------------------------------------------------------
#pragma once

#include <string>
#include <list>

#define LENGTHOF(array) (sizeof(array)/sizeof(array[0]))

namespace P3DInterop
{
    /// Gets the path of Custom_Nav_Draw.dll
    bool GetDLLDir(wchar_t* sBuffer, int size);
    /// Gets the path of Gauges\Custom_Nav_Draw
    bool GetXmlDir(wchar_t* sBuffer, int size);
    /// Gets the path of Gauges\Custom_Nav_Draw\Bin\Release
    bool GetBinDir(const wchar_t* name, wchar_t* sBuffer, int size);

    /// Normalize angle to 0-360
    void NormalizeDegrees(int& value);

    /// Normalize angle to -180 to 180
    void NormalizeDegrees180(double& value);

    bool FileExist(const wchar_t* sFullPath);

    int FindAllFiles(std::list< std::wstring > &files, const wchar_t* dir);
}
