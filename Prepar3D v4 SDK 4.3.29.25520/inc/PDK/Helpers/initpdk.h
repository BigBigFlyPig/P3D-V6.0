#pragma once
#include <atlcomcli.h>
#include "initguid.h"
#include "PdkServices.h"

namespace P3D
{
    P3D::PdkServices* P3D::PdkServices::m_pServices = nullptr;

    void PdkServices::Init(IPdk* pPdk)
    {
        m_pServices = new PdkServices(pPdk);
    }

    void PdkServices::Shutdown()
    {
        if (m_pServices)
        {
            delete m_pServices;
            m_pServices = nullptr;
        }
    }

    IPdk* PdkServices::GetPdk() 
    { 
        if (m_pServices)
        {
            return m_pServices->m_spPdk;
        }
        return nullptr;
    }

    IEventServiceV400* PdkServices::GetEventService() 
    {
        if (m_pServices)
        {
            return m_pServices->m_spEventService;
        }
        return nullptr;
    }

    IVisualEffectManagerV430* PdkServices::GetVisualEffectManager()
    {
        if (m_pServices)
        {
            return m_pServices->m_spVisualEffectManager; 
        }
        return nullptr;
    }

    IDataLoadHelperV400* PdkServices::GetDataLoadHelper()
    {
        if (m_pServices)
        {
            return m_pServices->m_spDataLoadHelper; 
        }
        return nullptr;
    }

    IRenderingPluginSystemV430* PdkServices::GetRenderingPluginSystem()
    {
        if (m_pServices)
        {
            return m_pServices->m_spRenderingPluginSystem;
        }
        return nullptr;
    }
    
    IWindowPluginSystemV420* PdkServices::GetWindowPluginSystem()
    {
        if (m_pServices)
        {
            return m_pServices->m_spWindowPluginSystem;
        }
        return nullptr;
    }

    IGlobalDataV430* PdkServices::GetGlobalData()
    {
        if (m_pServices)
        {
            return m_pServices->m_spGlobalData;
        }
        return nullptr;
    }

    IObjectRendererV400* PdkServices::GetObjectRenderer()
    {
        if (m_pServices)
        {
            return m_pServices->m_spObjectRenderer;
        }
        return nullptr;
    }

    IWeatherSystemV430* PdkServices::GetWeatherSystem()
    {
        if (m_pServices)
        {
            return m_pServices->m_spWeatherSystem;
        }
        return nullptr;
    }

    ISimObjectManagerV430* PdkServices::GetSimObjectManager()
    {
        if (m_pServices)
        {
            return m_pServices->m_spSimObjectManager;
        }
        return nullptr;
    } 

    IReportingServiceV400* PdkServices::GetReportingService()
    {
        if (m_pServices)
        {
            return m_pServices->m_spReportingService;
        }
        return nullptr;
    }

    IPanelSystemV400* PdkServices::GetPanelSystem()
    {
        if (m_pServices)
        {
            return m_pServices->m_spPanelSystem;
        }
        return nullptr;
    }

    IIconServiceV410* PdkServices::GetIconService()
    {
        if (m_pServices)
        {
            return m_pServices->m_spIconService;
        }
        return nullptr;
    }

    IMenuServiceV410* PdkServices::GetMenuService()
    {
        if (m_pServices)
        {
            return m_pServices->m_spMenuService;
        }
        return nullptr;
    }

    IMultiplayerServiceV430* PdkServices::GetMultiplayerService()
    {
        if (m_pServices)
        {
            return m_pServices->m_spMultiplayerService;
        }
        return nullptr;
    }

    ICigiServiceV430* PdkServices::GetCigiService()
    {
        if (m_pServices)
        {
            return m_pServices->m_spCigiService;
        }
        return nullptr;
    }
}
