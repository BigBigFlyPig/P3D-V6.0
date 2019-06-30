///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Custom drawable Interface
///
///----------------------------------------------------------------------------
#pragma once

#include "gauges.h"

#include <gdiplus.h>
#include <atlbase.h>
#include <WinError.h>

namespace CustomDraws
{
    /// Defines and implements a list of properties specifically to Sample page
    ///
    class DrawableGaugeAbstract abstract : public IGaugeCDrawable
    {
        DECLARE_PANEL_CALLBACK_REFCOUNT(DrawableGaugeAbstract)
    public:
        DrawableGaugeAbstract(const IGaugeCDrawableCreateParameters*);
        virtual ~DrawableGaugeAbstract(void);

        /// @see gauges.h
        virtual void Update() abstract;
        virtual bool Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage) abstract;
        virtual void Init(void) {}

        /// Define default implementation
        virtual FLAGS32 GetFlags() override sealed;
        virtual void Show(bool on) override sealed;
        /// Not allowing this yet. We dont need to implement double buffer unless we expect performance issues and need to thread out the draw calls
        virtual bool SetupDraw(PIXPOINT size, HDC hdc, PIMAGE pImage) override sealed;
        virtual bool GetDraw(IGaugeCDrawableDrawParameters* pParameters) override sealed;
        bool IsVisible(void);
        void SetDC(bool);
        void SetTakesImage(bool);
        void SetResizable(bool);
        void SetDrawAlpha(bool);
        void SetTransparent(bool);
        void SetMaskTransparency(bool);
        void SetDoubleBuffer(bool);
        /// Static utility functions
        static HRESULT FindResource(const wchar_t*, Gdiplus::Bitmap**);
        static bool FileExist(const wchar_t*);
    private:
        bool m_bShow;
        FLAGS32 m_Flags;
        PIXPOINT m_Size;
        HDC m_Hdc;
    };

    /// Return visible state
    inline bool DrawableGaugeAbstract::IsVisible()
    {
        return m_bShow;
    }

    inline void DrawableGaugeAbstract::SetDC(bool value)
    {
        if (value)
        {
            m_Flags |= TAKES_DC;
        }
        else
        {
            m_Flags &= ~TAKES_DC;
        }
    }

    inline void DrawableGaugeAbstract::SetTakesImage(bool value)
    {
        if (value)
        {
            m_Flags |= TAKES_PIMAGE;
        }
        else
        {
            m_Flags &= ~TAKES_PIMAGE;
        }
    }

    inline void DrawableGaugeAbstract::SetResizable(bool value)
    {
        if (value)
        {
            m_Flags &= ~NOT_RESIZABLE;
        }
        else
        {
            m_Flags |= NOT_RESIZABLE;
        }
    }

    inline void DrawableGaugeAbstract::SetDrawAlpha(bool value)
    {
        if (value)
        {
            m_Flags |= DRAWS_ALPHA;
        }
        else
        {
            m_Flags &= ~DRAWS_ALPHA;
        }
    }

    inline void DrawableGaugeAbstract::SetTransparent(bool value)
    {
        if (value)
        {
            m_Flags &= ~NO_TRANSPARENCY;
        }
        else
        {
            m_Flags |= NO_TRANSPARENCY;
        }
    }

    inline void DrawableGaugeAbstract::SetMaskTransparency(bool value)
    {
        if (value)
        {
            m_Flags |= MASK_TRANSPARENCY;
        }
        else
        {
            m_Flags &= ~MASK_TRANSPARENCY;
        }
    }

    inline void DrawableGaugeAbstract::SetDoubleBuffer(bool value)
    {
        if (value)
        {
            m_Flags &= ~DOUBLE_BUFFER;
        }
        else
        {
            m_Flags |= DOUBLE_BUFFER;
        }
    }

    /// Set visible state
    inline void DrawableGaugeAbstract::Show(bool on)
    {
        m_bShow = on;
    }

    /// Always take a dc
    inline FLAGS32 DrawableGaugeAbstract::GetFlags()
    {
        return m_Flags;
    }
}