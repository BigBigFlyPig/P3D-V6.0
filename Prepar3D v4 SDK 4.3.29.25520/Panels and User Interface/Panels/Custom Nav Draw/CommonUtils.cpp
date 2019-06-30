///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///----------------------------------------------------------------------------

#include "CommonUtils.h"
#include <math.h>
#include <Windows.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


namespace P3DInterop
{
    /// Gets the path of Custom_Nav_Draw.dll
    bool GetDLLDir(wchar_t* sBuffer, int size)
    {
        static const wchar_t* Custom_Nav_Draw = L"Custom_Nav_Draw.dll";
        return GetBinDir(Custom_Nav_Draw, sBuffer, size);
    }

    bool GetBinDir(const wchar_t* name, wchar_t* sBuffer, int size)
    {
        memset(sBuffer, 0, size);

        GetModuleFileName(GetModuleHandle(name), sBuffer, size);

        std::wstring val = sBuffer;
        int index = val.find_last_of('\\');

        if (index > 0 && index < size)
        {
            memset(sBuffer + index + 1, 0, wcslen(name));

            return true;
        }
        return false;
    }

    /// Gets the path of Prepar3D\SimObjects\Airplanes\Custom_Nav_Draw\Panel
    bool GetXmlDir(wchar_t* sBuffer, int size)
    {
        static const wchar_t* MAIN = L"%sXMLData\\";
        memset(sBuffer, 0, size);
        wchar_t tempBuffer[256];
        int tempSize = 256;
        if (GetDLLDir(tempBuffer, tempSize))
        {
            swprintf_s(sBuffer, size, MAIN, tempBuffer);
            return true;
        }
        return false;
    }

    bool FileExist(const wchar_t* sFullPath)
    {
        DWORD err = GetFileAttributes(sFullPath);

        if (INVALID_FILE_ATTRIBUTES == err)
        {
            return false;
        }
        return true;
    }


    void NormalizeDegrees(int& value)
    {
        // return a value between 0 and 360
        while (value < 0)
        {
            value += 360;
        }
        while (value >= 360)
        {
            value -= 360;
        }
    }

    void NormalizeDegrees180(double& value)
    {
        // return a value between -180 and 180
        while (value <= -180)
        {
            value += 360;
        }
        while (value > 180)
        {
            value -= 360;
        }
    }

    int FindAllFiles(std::list< std::wstring > &files, const wchar_t* fileName)
    {
        HANDLE hNextFile;
        WIN32_FIND_DATA findFileData;
        hNextFile = FindFirstFile(fileName, &findFileData);
        BOOL res = TRUE;

        while (hNextFile != INVALID_HANDLE_VALUE && res)
        {
            files.push_back(findFileData.cFileName);
            res = FindNextFile(hNextFile, &findFileData); //Unhandled exception here!
        }
        return files.size();
    }
}