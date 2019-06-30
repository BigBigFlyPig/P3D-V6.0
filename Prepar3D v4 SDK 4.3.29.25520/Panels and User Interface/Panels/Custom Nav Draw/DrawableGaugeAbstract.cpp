///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///----------------------------------------------------------------------------

#include "DrawableGaugeAbstract.h"
#include "CommonUtils.h"

#include <math.h>

namespace CustomDraws
{
    DEFINE_PANEL_CALLBACK_REFCOUNT(DrawableGaugeAbstract)

        static const wchar_t* DRAWABLERESOURCES = L"Resources\\";

    /// Public constructor and destructor
    ///
    DrawableGaugeAbstract::DrawableGaugeAbstract(const IGaugeCDrawableCreateParameters* pParams)
        : m_Flags(0) //m_Flags(TAKES_DC)
        , m_bShow(false)
        , m_Hdc(NULL)
    {
        INIT_PANEL_CALLBACK_REFCOUNT(DrawableGaugeAbstract);
        m_Size.x = 1;
        m_Size.y = 1;

        //Explicitly setting default values for m_Flags as an example
        //m_Flags(TAKES_DC) - commented out above - defaults to these values
        SetDC(true);                //Drawing in hdc
        SetTakesImage(false);       //Not taking an image
        SetResizable(true);         //We want this to be resizable incase someone changes the window size
        SetDrawAlpha(false);        //Does nothing if TAKES_DC is true
        SetTransparent(true);       //When false, the transparent color draws black underneath
        SetMaskTransparency(false); //Does nothing if TAKES_DC is true
        SetDoubleBuffer(false);     //Allows for a background draw, not needed
    }

    DrawableGaugeAbstract::~DrawableGaugeAbstract(void)
    {
        /// Detor
    }

    HRESULT DrawableGaugeAbstract::FindResource(const wchar_t* fName, Gdiplus::Bitmap** out)
    {
        HRESULT err = E_FAIL;
        wchar_t cPath[256];
        wchar_t cPanelDir[256];
        static const wchar_t* FORMAT = L"%s%s%s";

        bool success = P3DInterop::GetXmlDir(cPanelDir, sizeof(cPanelDir));

        if (success && fName != NULL && wcslen(fName) > 0)
        {
            swprintf_s(cPath, sizeof(cPath), FORMAT, cPanelDir, DRAWABLERESOURCES, fName);
            if (FileExist(cPath))
            {
                wchar_t wc[256];
                size_t convertedChars = 0;
                *out = Gdiplus::Bitmap::FromFile(wc);
                if (*out != NULL)
                {
                    err = S_OK;
                }
            }
        }

        return err;
    }

    bool DrawableGaugeAbstract::FileExist(const wchar_t* sFullPath)
    {
        DWORD err = GetFileAttributes(sFullPath);

        if (INVALID_FILE_ATTRIBUTES == err)
        {
            return false;
        }
        return true;
    }


    /// supporting threaded draw calls
    bool DrawableGaugeAbstract::SetupDraw(PIXPOINT size, HDC hdc, PIMAGE pImage)
    {
        m_Size = size;
        m_Hdc = hdc;

        return true;
    }
    /// supporting threaded draw calls
    bool DrawableGaugeAbstract::GetDraw(IGaugeCDrawableDrawParameters* pParameters)
    {
        return Draw(pParameters, m_Size, m_Hdc, NULL);
    }
}