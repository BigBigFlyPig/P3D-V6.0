///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Maintains navigation data for use by other property owners.
///
///----------------------------------------------------------------------------


#include "NavigationPropertyOwner.h"
#include "XMLData.h"
#include "gauges.h"
#include "Helpers\PdkServices.h"

#include <math.h>
#include <string>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace Nav
{
    /// Create the first instance
    static NavigationPropertyOwner s_NavigationPropertyOwner;

    NavigationPropertyOwner::NavigationPropertyOwner()
    {

    }

    HRESULT NavigationPropertyOwner::Init()
    {
        m_AreaData.Load(L"areas.xml");

        // convert areas.xml data to nav prop owner structures
        XMLToAreas(m_AreaData.Root());

        return S_OK;
    }

    HRESULT NavigationPropertyOwner::DeInit()
    {
        m_vAreas.clear();
        m_vCircles.clear();

        return S_OK;
    }

    //Getting Area data that was set in the areas.xml files
    HRESULT NavigationPropertyOwner::GetAreas(std::vector<Nav::Areas> *& vect)
    {
        if (m_vAreas.size() == 0)
        {
            return E_FAIL;
        }
        else
        {
            vect = &m_vAreas;
            return S_OK;
        }
    }

    //Getting Circle data that was set in the areas.xml files
    HRESULT NavigationPropertyOwner::GetCircles(std::vector<Nav::Circles> *& vect)
    {
        if (m_vCircles.size() == 0)
        {
            return E_FAIL;
        }
        else
        {
            vect = &m_vCircles;
            return S_OK;
        }
    }

    //Not setting location, only setting the variables for location comparisons.
    void NavigationPropertyOwner::SetLocationVariables(double lat, double lon, double heading)
    {
        m_dLatRad = lat;
        m_dLonRad = lon;
        m_dHdgRad = heading;
    }

    //Returns true if no change, false if change
    bool NavigationPropertyOwner::SameLocation(double lat, double lon)
    {
        if (lat == m_dLatRad && lon == m_dLonRad)
        {
            return true;
        }
        return false;
    }

    //Not setting map size, only setting the variables for map size comparisons.
    void NavigationPropertyOwner::SetMapSizeVariables(PIXPOINT size)
    {
        m_MapSize = size;
    }

    //Returns true if no change, false if change
    bool NavigationPropertyOwner::SameMapSize(PIXPOINT size)
    {
        if (size.x == m_MapSize.x && size.y == m_MapSize.y)
        {
            return true;
        }
        return false;
    }

    //Calculate the latlon positions into xy pixel positions
    HRESULT NavigationPropertyOwner::CalculateLatLontoXYPix(double dLatRad, double dLonRad, float fPixelScale, float fCenterX, float fCenterY, bool bNorthUp, float &X, float &Y, bool bOverrideRange)
    {
        const float  fDRAW_RANGE_RAD = 0.12217f;
        P3D::DXYZ   osPos;
        P3D::DXYZ   poiPos;
        P3D::DXYZ   relPos;
        P3D::DXYZ   rotPos;
        //Early check to see if it's within the range to be drawn
        if ((!bOverrideRange) && ((fabs(dLatRad - m_dLatRad) > fDRAW_RANGE_RAD || fabs(dLonRad - m_dLonRad) > fDRAW_RANGE_RAD)))
        {
            return E_FAIL;
        }
        //Calculate the position to draw in the map
        osPos.dX = m_dLonRad;
        osPos.dY = 0.0;
        osPos.dZ = m_dLatRad;
        poiPos.dX = dLonRad;
        poiPos.dY = 0.0;
        poiPos.dZ = dLatRad;
        relPos = ConvertLonAltLatToXYZ(poiPos, osPos);
        //If Orientation value = 1 in the xml for a north facing map.
        if (bNorthUp)
        {
            rotPos = RotateXYZtoHdg(relPos, m_dMagVar);
        }
        //Rotate map with the planes heading
        else
        {
            rotPos = RotateXYZtoHdg(relPos, -m_dHdgRad);
        }
        // Pixel positions to draw
        X = fCenterX + (float)(rotPos.dX) * fPixelScale;
        Y = fCenterY - (float)(rotPos.dZ) * fPixelScale;
        return S_OK;
    }

    void NavigationPropertyOwner::XMLToAreas(Common::XMLNode &sourceXML)
    {
        std::list<Common::XMLNode> childNode;
        wchar_t areaColor[20];
        wchar_t areaName[20];
        wchar_t circleID[20];
        wchar_t circleName[20];

        //iterate through all area/circle nodes and add to m_vAreas
        auto parentIter = sourceXML.Children.begin();
        auto parentEnd = sourceXML.Children.end();

        for (; parentIter != parentEnd; ++parentIter)
        {
            childNode = parentIter->Children;

            auto childIter = childNode.begin();
            auto childEnd = childNode.end();

            // Area Nodes
            if (parentIter->Name == L"Area")
            {
                wcscpy_s(areaName, parentIter->Attributes[L"name"].c_str());
                wcscpy_s(areaColor, parentIter->Attributes[L"color"].c_str());

                std::vector<LLA> vAreaVector;
                // Point Nodes
                for (; childIter != childEnd; ++childIter)
                {
                    LLA areaLLA;
                    areaLLA.latitude = wcstod(childIter->Attributes[L"lat"].c_str(), NULL);
                    areaLLA.longitude = wcstod(childIter->Attributes[L"lon"].c_str(), NULL);
                    vAreaVector.push_back(areaLLA);
                }

                m_vAreas.push_back(Areas(&vAreaVector, areaName, StringToIntSafe(areaColor, nullptr, 0)));
            }
            // Circle nodes
            else if (parentIter->Name == L"Circles")
            {
                for (; childIter != childEnd; ++childIter)
                {
                    wcscpy_s(circleID, childIter->Attributes[L"id"].c_str());
                    wcscpy_s(circleName, childIter->Attributes[L"name"].c_str());

                    Circles circle(wcstod(childIter->Attributes[L"lat"].c_str(), NULL),
                        wcstod(childIter->Attributes[L"lon"].c_str(), NULL),
                        wcstod(childIter->Attributes[L"radius"].c_str(), NULL),
                        circleID, circleName,
                        StringToIntSafe(childIter->Attributes[L"color"].c_str(), nullptr, 0));
                    m_vCircles.push_back(circle);
                }
            }
        }
    }

    int NavigationPropertyOwner::StringToIntSafe(const std::wstring& str, size_t *pos, int base)
    {
        // error checking version of stoi
        int iValue;
        try
        {
            iValue = std::stoi(str, pos, base);
        }
        catch (...)
        {
            iValue = 0;
        }
        return iValue;
    }
}
