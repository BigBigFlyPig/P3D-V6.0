///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///----------------------------------------------------------------------------

#include "Conversions.h"
#include "NavigationDrawable.h"
#include "gauges.h"
#include "LLA.h"
#include "Helpers\PdkServices.h"

#include <math.h>

struct Positions
{
    double dLat;
    double dLon;
};
struct Areas
{
    Positions*     pArea;
    int            iPointCount;
    Gdiplus::Color color;
};
struct Circles
{
    Positions*     pCircle;
    double         dRadius;
    Gdiplus::Color color;
};

namespace Nav
{
    enum NAV_PARAMETER
    {
        PARAMETER_ORIENT = 0,
        PARAMETER_SCALE,
        PARAMETER_NAV_OVERLAY_ON,
        PARAMETER_LATITUDE,
        PARAMETER_LONGITUDE,
        PARAMETER_HEADING,
    };

    /// Public constructor and destructor
    ///
    NavigationDrawable::NavigationDrawable(const IGaugeCDrawableCreateParameters* pParams)
        : DrawableGaugeAbstract(pParams)
    {
        //pParams pulling data that was set in CustomNavDraw.xml
        pParams->SetParameterId("Latitude", PARAMETER_LATITUDE, IGaugeCDrawableCreateParameters::PARAMETER_TYPE_FLOAT);
        pParams->SetParameterId("Longitude", PARAMETER_LONGITUDE, IGaugeCDrawableCreateParameters::PARAMETER_TYPE_FLOAT);
        pParams->SetParameterId("Scale", PARAMETER_SCALE, IGaugeCDrawableCreateParameters::PARAMETER_TYPE_FLOAT);
        pParams->SetParameterId("OverlayOn", PARAMETER_NAV_OVERLAY_ON, IGaugeCDrawableCreateParameters::PARAMETER_TYPE_INT);
        pParams->SetParameterId("Orientation", PARAMETER_ORIENT, IGaugeCDrawableCreateParameters::PARAMETER_TYPE_INT);
        pParams->SetParameterId("Heading", PARAMETER_HEADING, IGaugeCDrawableCreateParameters::PARAMETER_TYPE_FLOAT);
        m_pNavigationPropertyOwner = new NavigationPropertyOwner();
        m_pNavigationPropertyOwner->Init();
    }

    NavigationDrawable::~NavigationDrawable(void)
    {
        if (m_pNavigationPropertyOwner)
        {
            delete m_pNavigationPropertyOwner;
            m_pNavigationPropertyOwner = NULL;
        }
    }

    void NavigationDrawable::Update()
    {

    }

    bool NavigationDrawable::Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage)
    {
        HRESULT err = S_OK;
        //Should we draw? or is the NavPropOwner avail?
        if (!IsVisible() || !m_pNavigationPropertyOwner)
        {
            return false;
        }

        //Get Parameters from XML
        FLOAT64     dMapScale = 0.0;
        float       fMapScale = 0.0f;
        int         iOrientation = 0;
        int         iOverlayOn = 0;
        double      lat = 0;
        double      lon = 0;
        double      heading = 0;

        //Ownship Position (due to the A:Latitude & A:Longitude in the CustomNavDraw.xml file)
        pParameters->GetParameter(PARAMETER_LATITUDE, &lat);
        pParameters->GetParameter(PARAMETER_LONGITUDE, &lon);
        pParameters->GetParameter(PARAMETER_HEADING, &heading);

        //Map Info
        pParameters->GetParameter(PARAMETER_SCALE, &dMapScale);
        pParameters->GetParameter(PARAMETER_ORIENT, &iOrientation);
        pParameters->GetParameter(PARAMETER_NAV_OVERLAY_ON, &iOverlayOn);

        //Skip draw if lat, lon, and map size is the same
        if (m_pNavigationPropertyOwner->SameLocation(lat, lon)
            && m_pNavigationPropertyOwner->SameMapSize(size))
        {
            return false;
        }

        //New location or map size, setup for draw.
        m_pNavigationPropertyOwner->SetLocationVariables(lat, lon, heading);
        m_pNavigationPropertyOwner->SetMapSizeVariables(size);

        //Calculate Center and Scale
        float       fCenterX = size.x * 0.5f;
        float       fCenterY = size.y * 0.5f;
        fMapScale = (float)dMapScale;
        fMapScale = min(size.x, size.y) * 0.5f / fMapScale;

        //Init Graphics
        Gdiplus::Graphics graphics(hdc);

        //Clear the canvas to transparent
        graphics.Clear(Gdiplus::Color::Transparent);

        //If orientation is set in the xml
        if (iOrientation) { m_bNorthUp = iOrientation; }

        //If INT Mode is on, draw Boundaries
        if (iOverlayOn)
        {
            DrawAreas(graphics, fMapScale, fCenterX, fCenterY);
            DrawCircles(graphics, fMapScale, fCenterX, fCenterY);
        }

        return true;
    }

    //Draw areas that were defined in areas.xml
    void NavigationDrawable::DrawAreas(Gdiplus::Graphics &g, float fMapScale, Gdiplus::REAL fCenterX, Gdiplus::REAL fCenterY)
    {
        const int    iMAX_PIXELS_PER_POLY = 10;
        Gdiplus::PointF  points[iMAX_PIXELS_PER_POLY];

        std::vector<Nav::Areas> *pvAreas = NULL;
        if (SUCCEEDED(m_pNavigationPropertyOwner->GetAreas(pvAreas)))
        {
            //Go through each area
            for (std::vector<Nav::Areas>::iterator itArea = pvAreas->begin(); itArea != pvAreas->end(); ++itArea)
            {
                int pointCount = 0;
                Gdiplus::Pen pen(Gdiplus::Color(0xFF000000 | itArea->iColor), 5.0f);
                //Go through each set of vector points
                for (std::vector<LLA>::iterator itPoints = itArea->vPoints.begin(); itPoints != itArea->vPoints.end(); ++itPoints)
                {
                    if (!SUCCEEDED(m_pNavigationPropertyOwner->CalculateLatLontoXYPix(itPoints->latitude, itPoints->longitude, fMapScale,
                        fCenterX, fCenterY, m_bNorthUp, points[pointCount].X, points[pointCount].Y)))
                    {
                        break;
                    }
                    if (pointCount < iMAX_PIXELS_PER_POLY)
                    {
                        pointCount++;
                    }
                }
                //Draw polygon
                g.DrawPolygon(&pen, points, pointCount);
            }
        }
        return;
    }

    //Draw circles that were defined in areas.xml
    void NavigationDrawable::DrawCircles(Gdiplus::Graphics &g, float fMapScale, Gdiplus::REAL fCenterX, Gdiplus::REAL fCenterY)
    {
        std::vector<Nav::Circles> *pvCircles = NULL;
        if (SUCCEEDED(m_pNavigationPropertyOwner->GetCircles(pvCircles)))
        {
            //Go through each circle
            for (std::vector<Nav::Circles>::iterator itCircle = pvCircles->begin(); itCircle != pvCircles->end(); ++itCircle)
            {
                Gdiplus::REAL X, Y;
                int pointCount = 0;
                Gdiplus::Pen pen(Gdiplus::Color(0xFF000000 | itCircle->iColor), 3.0f);
                if (SUCCEEDED(m_pNavigationPropertyOwner->CalculateLatLontoXYPix(itCircle->dLat, itCircle->dLon, fMapScale,
                    fCenterX, fCenterY, m_bNorthUp, X, Y)))
                {
                    //Draw circle
                    Gdiplus::REAL R = (float)itCircle->dRadius * 2000.0f *  fMapScale;
                    g.DrawEllipse(&pen, X - R / 2.0f, Y - R / 2.0f, R, R);
                }
            }
        }
        return;
    }
}
