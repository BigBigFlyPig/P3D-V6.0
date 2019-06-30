///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Maintains navigation data for use by other property owners.
///
///----------------------------------------------------------------------------
#pragma once

#include "DrawableGaugeAbstract.h"
#include "NavigationPropertyOwner.h"

#include <atlbase.h>
#include <WinError.h>
#include <gdiplus.h>

namespace Nav
{
    enum GAUGE_ID
    {
        GAUGE_NAV = 0,
        GAUGE_MAX
    };
    /// Defines and implements a list of properties specifically to Sample page
    ///
    class NavigationDrawable sealed : public CustomDraws::DrawableGaugeAbstract
    {
    public:
        NavigationDrawable(const IGaugeCDrawableCreateParameters* pParams);
        ~NavigationDrawable(void);
        virtual void Update() override;
        virtual bool Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage) override;

    private:
        Nav::NavigationPropertyOwner* m_pNavigationPropertyOwner;

        void    DrawAreas(Gdiplus::Graphics &g, float fMapScale, Gdiplus::REAL fCenterX, Gdiplus::REAL fCenterY);
        void    DrawCircles(Gdiplus::Graphics &g, float fMapScale, Gdiplus::REAL fCenterX, Gdiplus::REAL fCenterY);

        bool    m_bNorthUp;
    };
}