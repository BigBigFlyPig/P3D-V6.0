#pragma once
#include "IEventService.h"      //From Prepar3D SDK
#include "Pdk.h"                //From Prepar3D SDK
#include "IWindowPluginSystem.h"
#include "IUnknownHelper.h"

#include "IRenderingService.h"  //From Prepar3D SDK
#include "IVisualEffects.h"
#include "IDataLoader.h"
#include "IRenderingPluginSystem.h"
#include "IGlobalData.h"
#include "IRenderingService.h"
#include "IWeatherSystem.h"
#include "ISimObject.h"
#include "IReportingService.h"
#include "IPanelSystem.h"
#include "IIconService.h"
#include "IMenuService.h"
#include "INetworkServices.h"

namespace P3D
{
    /** @addtogroup pdk */ /** @{ */

    /**
     * Provides static access to all of Prepar3D's core PDK services.
     * @remark  To use this in a plugin:
     * - Include initpdk.h from the cpp file where dll start and stop functions are defined
     * - Call Init() in the dll start function
     * - Call Shutdown() from the dll stop function  
     * ```
     *      #include "initpdk.h"
     *     
     *      void DLLStart(__in __notnull IPdk* pPdk)
     *      {
     *          PdkServices::Init(pPdk);
     *          // init code for your plugin
     *      }
     *      
     *      void DLLStop(void)
     *      {
     *          // deinit code for your plugin
     *          PdkServices::Shutdown();
     *      }
     * ```
     */
    class PdkServices
    {
    public:
        PdkServices(IPdk* pPdk) : m_spPdk(pPdk)
        {
            m_spPdk->QueryService(SID_EventService, IID_IEventServiceV400, (void**)&m_spEventService);
            m_spPdk->QueryService(SID_VisualEffectManager, IID_IVisualEffectManagerV430, (void**)&m_spVisualEffectManager);
            m_spPdk->QueryService(SID_DataLoadHelper, IID_IDataLoadHelperV400, (void**)&m_spDataLoadHelper);
            m_spPdk->QueryService(SID_GlobalData, IID_IGlobalDataV430, (void**)&m_spGlobalData);

            m_spPdk->QueryService(SID_ObjectRenderer, IID_IObjectRendererV400, (void**)&m_spObjectRenderer);
            m_spPdk->QueryService(SID_WeatherSystem, IID_IWeatherSystemV400, (void**)&m_spWeatherSystem);
            m_spPdk->QueryService(SID_SimObjectManager, IID_ISimObjectManagerV430, (void**)&m_spSimObjectManager);
            m_spPdk->QueryService(SID_WindowPluginSystem, IID_IWindowPluginSystemV420, (void**)&m_spWindowPluginSystem);
            m_spPdk->QueryService(SID_RenderingPluginSystem, IID_IRenderingPluginSystemV430, (void**)&m_spRenderingPluginSystem);
            m_spPdk->QueryService(SID_ReportingService, IID_IReportingServiceV400, (void**)&m_spReportingService);
            m_spPdk->QueryService(SID_PanelSystem, IID_IPanelSystemV400, (void**)&m_spPanelSystem);
            m_spPdk->QueryService(SID_IconService, IID_IIconServiceV410, (void**)&m_spIconService);
            m_spPdk->QueryService(SID_MenuService, IID_IMenuServiceV410, (void**)&m_spMenuService);
            m_spPdk->QueryService(SID_MultiplayerService, IID_IMultiplayerServiceV430, (void**)&m_spMultiplayerService);
            m_spPdk->QueryService(SID_CigiService, IID_ICigiServiceV430, (void**)&m_spCigiService);
        }
        /** 
         * Initialize static instance of PdkServices.  This should be called from the dll start function.
         */
        static void Init(IPdk* pPdk);

        /** 
         * Shutdown static instance of PdkServices.  This should be called from the dll stop function.
         */
        static void Shutdown();

        ~PdkServices()
        {
            m_spPdk = nullptr;
            m_spDataLoadHelper = nullptr;
            m_spGlobalData = nullptr;
            m_spEventService = nullptr;
            m_spVisualEffectManager = nullptr;         
            m_spRenderingPluginSystem = nullptr;
            m_spWindowPluginSystem = nullptr;     
            m_spObjectRenderer = nullptr;
            m_spWeatherSystem = nullptr;
            m_spSimObjectManager = nullptr;
            m_spReportingService = nullptr;
            m_spPanelSystem = nullptr;
            m_spIconService = nullptr;
            m_spMenuService = nullptr;
            m_spMultiplayerService = nullptr;
            m_spCigiService = nullptr;
        }
        /// Get Pdk
        static IPdk*                        GetPdk();
        /// Get Event Service
        static IEventServiceV400*           GetEventService();
        /// Get Visual Effect Manager
        static IVisualEffectManagerV430*    GetVisualEffectManager();
        /// Get Data Load Helper
        static IDataLoadHelperV400*         GetDataLoadHelper();
        /// Get Rendering Plugin System
        static IRenderingPluginSystemV430*  GetRenderingPluginSystem();
        /// Get Window Plugin System
        static IWindowPluginSystemV420*     GetWindowPluginSystem();
        /// Get Global Data
        static IGlobalDataV430*             GetGlobalData();
        /// Get Object Renderer
        static IObjectRendererV400*         GetObjectRenderer();
        /// Get Weather System
        static IWeatherSystemV430*          GetWeatherSystem();
        /// Get Simulation Object Manager
        static ISimObjectManagerV430*       GetSimObjectManager();
        /// Get Reporting Service
        static IReportingServiceV400*       GetReportingService();
        /// Get Panel System
        static IPanelSystemV400*            GetPanelSystem();
        /// Get Icon Service
        static IIconServiceV410*            GetIconService();
        /// Get Menu Service
        static IMenuServiceV410*            GetMenuService();
        /// Get Multiplayer Service
        static IMultiplayerServiceV430*     GetMultiplayerService();
        /// Get CIGI Service
        static ICigiServiceV430*            GetCigiService();
    protected:
        static PdkServices*                 m_pServices;
        CComPtr<IPdk>                       m_spPdk;
        CComPtr<IEventServiceV400>          m_spEventService;
        CComPtr<IVisualEffectManagerV430>   m_spVisualEffectManager;
        CComPtr<IDataLoadHelperV400>        m_spDataLoadHelper;
        CComPtr<IRenderingPluginSystemV430> m_spRenderingPluginSystem;
        CComPtr<IWindowPluginSystemV420>    m_spWindowPluginSystem;
        CComPtr<IGlobalDataV430>            m_spGlobalData;
        CComPtr<IObjectRendererV400>        m_spObjectRenderer;
        CComPtr<IWeatherSystemV430>         m_spWeatherSystem;
        CComPtr<ISimObjectManagerV430>      m_spSimObjectManager;
        CComPtr<IReportingServiceV400>      m_spReportingService;
        CComPtr<IPanelSystemV400>           m_spPanelSystem;
        CComPtr<IIconServiceV410>           m_spIconService;
        CComPtr<IMenuServiceV410>           m_spMenuService;
        CComPtr<IMultiplayerServiceV430>    m_spMultiplayerService;
        CComPtr<ICigiServiceV430>           m_spCigiService;
    };
    /** @} */
}