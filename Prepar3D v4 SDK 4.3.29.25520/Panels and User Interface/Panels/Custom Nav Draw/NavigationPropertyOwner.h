///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Maintains navigation data for use by other property owners.
///
///----------------------------------------------------------------------------
#pragma once
#include "Conversions.h"
#include "Location.h"
#include "XMLData.h"
#include "gauges.h"

#include <list>
#include <vector>
#include <array>
#include <atlbase.h>
#include <WinError.h>

namespace Nav
{

    struct Areas
    {
        std::vector<LLA> vPoints;
        int  iColor;

        Areas(std::vector<LLA> *vPoints1, wchar_t* name, int color)
        {
            vPoints = *vPoints1;
            iColor = color;
        }
    };

    struct Circles
    {
        double dLat;
        double dLon;
        double dRadius;
        int  iColor;

        Circles(double dLat1, double dLon1, double dRadius1, wchar_t* id, wchar_t* name, int color)
        {
            dLat = dLat1;
            dLon = dLon1;
            dRadius = dRadius1;
            iColor = color;
        }
    };

    class ChartDrawable;

    class NavigationPropertyOwner
    {
    public:

        NavigationPropertyOwner();
        virtual HRESULT Init();
        virtual HRESULT DeInit();

        //NAV Draw Functions
        HRESULT GetAreas(std::vector<Areas> *& vect);
        HRESULT GetCircles(std::vector<Circles> *& vect);
        HRESULT CalculateLatLontoXYPix(double dLatRad, double dLonRad, float fPixelScale, float fCenterX, float fCenterY, bool bNorthUp, float &X, float &Y, bool bOverrideMaxRange = false);

        void XMLToAreas(Common::XMLNode &sourceXML);

        int StringToIntSafe(const std::wstring& str, size_t *pos = 0, int base = 10);

        void SetLocationVariables(double lat, double lon, double heading);
        bool SameLocation(double lat, double lon);
        void SetMapSizeVariables(PIXPOINT size);
        bool SameMapSize(PIXPOINT size);

    private:
        std::vector<Areas> m_vAreas;
        std::vector<Circles> m_vCircles;

        Common::XMLData m_AreaData;

        //Ownship Position data due to how the CustomNavDraw.xml was setup. A:Longitude, A:Latitude, etc.
        double m_dLatRad;
        double m_dLonRad;
        double m_dHdgRad;
        double m_dMagVar;

        //Map
        PIXPOINT m_MapSize;
    };
}