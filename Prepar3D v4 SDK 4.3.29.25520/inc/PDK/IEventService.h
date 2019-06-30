//-----------------------------------------------------------------------------
// Copyright (c) Lockheed Martin Corporation. All Rights Reserved
//-----------------------------------------------------------------------------
#pragma once

#include <ObjBase.h>

namespace P3D
{
    /** \defgroup    eventservice      Event Service
     This service is provided through the PDK interface and allows callers to register for events
     which are signaled by Prepar3D. Event GUIDs are defined in the interface header. Currently
     supported events are:

     - EVENTID_Frame: This event is signaled once per frame.
     - EVENTID_1Hz: This event is signaled once per second.
     - EVENTID_CustomObjectDraw: This event is signaled once per view each frame.

     See PdkPlugin for an example of registering for multiple events and messages.
     \{
     */

     /**
      *  Parameter values used for IParameterListV400
      **/
    struct P3DParameter
    {
        UINT64 Value;
    };

    /**
    *   Parameter list interface used by ICallbackV400
    **/
    DECLARE_INTERFACE_(IParameterListV400, IUnknown)
    {
        /**
        *   Get service provider.  Use QueryService to request services from it.
        *   @return Currently all callbacks pass
        *   an IPdk pointer as the service provider, but some callbacks may
        *   provide callback-specific providers in the future.
        **/
        virtual IServiceProvider* GetServiceProvider() abstract;
        /**
        *   Get P3DParameter by index.
        *   @param    index   parameter index;
        *   @return   The parameter requested.
        **/
        virtual P3DParameter GetParameter(UINT32 index) abstract;
        /**
        *   Get parameter count.
        *   @param    index   parameter index;
        *   @return   number of parameters in the params list.
        **/
        virtual UINT32 GetCount() abstract;
    };

    interface __declspec(uuid("{075551e8-0885-4711-9f7f-a6eb6a61da6c}")) IParameterListV400;
    extern __declspec(selectany) REFIID IID_IParameterListV400 = __uuidof(IParameterListV400);
    /**
    *   Callback interface which must be implemented to register for events.
    *   @param    index   parameter index;
    *   @return   number of parameters in the params list.
    **/
    DECLARE_INTERFACE_(ICallbackV400, IUnknown)
    {
        /**
        *   Called when the registered event is fired.
        *   @param    pParams   Callback parameters interface pointer.
        **/
        virtual void Invoke(IParameterListV400* pParams) abstract;
    };

    interface __declspec(uuid("{aef008ce-84d1-4a54-b136-04dcb4fafd22}")) ICallbackV400;
    extern __declspec(selectany) REFIID IID_ICallbackV400 = __uuidof(ICallbackV400);
    /** Service used to register for Prepar3D event callbacks
     *
     */
    DECLARE_INTERFACE_(IEventServiceV400, IUnknown)
    {
        /**
        *   Register an event callback.
        *   @param    eventID      GUID id of the event to register for
        *   @param    pCallback    Callback to register
        *   @return   S_OK if succeeded and E_FAIL if it failed.
        **/
        virtual HRESULT RegisterCallback(const GUID& eventID, ICallbackV400* pCallback) abstract;
        /**
        *   Unregister an event callback.
        *   @param    eventID      GUID id of the event to register for
        *   @param    pCallback    Callback to register
        *   @return   S_OK if succeeded and E_FAIL if it failed.
        **/
        virtual HRESULT UnregisterCallback(const GUID& eventID, ICallbackV400* pCallback) abstract;
        /**
        *   Send a message event.
        *   @param    messageID  message ID
        *   @param    PVOID      message parameter
        **/
        virtual HRESULT SendMessageEvent(UINT32 messageID, PVOID messageParam) abstract;
    };

    interface __declspec(uuid("{5b3bac47-d372-4a51-a666-d0bc839484c7}")) IEventServiceV400;
    extern __declspec(selectany) REFIID IID_IEventServiceV400 = __uuidof(IEventServiceV400);
    extern __declspec(selectany) REFIID SID_EventService = __uuidof(IEventServiceV400);
    /// Event that is signaled once per frame
    DEFINE_GUID(EVENTID_Frame,
        0xc98b247b, 0xc8ea, 0x4f70, 0x8e, 0xd7, 0x2d, 0xfb, 0x19, 0x17, 0x51, 0xe3);
    /// This event is signaled once per frame.
    DEFINE_GUID(EVENTID_1Hz,
        0xaf64e7c8, 0x81df, 0x46dd, 0x8f, 0x46, 0x8e, 0x40, 0x9e, 0x42, 0xef, 0x6f);
    /**
    * This event is signaled once per view each frame.
    * The IParameterListV400 can be used to request the IObjectRendererV400 service, which
    * can be used to render custom objects into the scene. See the CustomLights example
    * for more details. (SDK\Utilities\PDK\CustomObjectRenderingSamples\CustomLights)
    */
    DEFINE_GUID(EVENTID_CustomObjectDraw,
        0x24f5b477, 0xcfcb, 0x4ac1, 0x8f, 0xee, 0xdd, 0x60, 0x14, 0xf8, 0xe9, 0x16);
    /**
    *  Event that is fired whenever a Prepar3D message is sent.
    *  The EVENT_MESSAGE_ message ID will be the first parameter of the EVENTID_Message
    *  callback. A sample message handler is included below:
    *```
    *      virtual void Invoke(IP3DCallbackParams* pParams)
    *      {
    *          UINT64 messageID = pParams->GetParameter(0).Value;
    *          if(messageID == EVENT_MESSAGE_APPLICATION_SHUTDOWN)
    *          {
    *              // handle app shutdown
    *          }
    *      }
    *```
    **/
    DEFINE_GUID(EVENTID_Message,
        0x5dafff1b, 0x8e70, 0x47e9, 0x87, 0x2b, 0x9f, 0x71, 0x7b, 0x11, 0x28, 0xbb);

    // application
#define EVENT_MESSAGE_APPLICATION_STARTUP               0x0036  ///< Application is starting up
#define EVENT_MESSAGE_APPLICATION_SHUTDOWN              0x0037  ///< Application is shutting down
#define EVENT_MESSAGE_APPLICATION_RESTART               0x003a  ///< Application is being "restarted" by a second instance
#define EVENT_MESSAGE_APPLICATION_ACTIVATED             0x003b  ///< Application just received WM_ACTIVATEAPP
#define EVENT_MESSAGE_APPLICATION_DEACTIVATED           0x003c  ///< Application just received WM_ACTIVATEAPP
#define EVENT_MESSAGE_MAIN_WINDOW_CHANGE                0x003d  ///< Application main window maximized, etc
#define EVENT_MESSAGE_MENU_MODE_START                   0x0040  ///< Main menu is getting ready to be displayed
#define EVENT_MESSAGE_MENU_MODE_STOP                    0x0041  ///< Main menu is done being displayed
#define EVENT_MESSAGE_CONTEXT_MENU_MODE_START           0x0042  ///< Context menu is getting ready to be displayed
#define EVENT_MESSAGE_CONTEXT_MENU_MODE_STOP            0x0043  ///< Context menu is done being displayed
#define EVENT_MESSAGE_DIALOG_MODE_START                 0x0044  ///< A dialogue is getting ready to be displayed
#define EVENT_MESSAGE_DIALOG_MODE_STOP                  0x0045  ///< A dialogue is done being displayed
#define EVENT_MESSAGE_MEMORY_LOW                        0x0046  ///< A malloc has failed, if you see this free up some memory
#define EVENT_MESSAGE_DEVICE_RESET_COMPLETED            0x0181  ///< Rendering system finished remapping windows to devices
#define EVENT_MESSAGE_MAIN_LOADING_STATE_CHANGE         0x0191  ///< On MainLoadingState change send new state
#define EVENT_MESSAGE_LOADING_COMPLETE                  0x0197  ///< Done loading
#define EVENT_MESSAGE_APPLICATION_DLLS_LOADED           0x003e  ///< Application dlls and plugin dlls are done loading

// sim
#define EVENT_MESSAGE_SIM_PAUSED                        0x0038  ///< Simulation is paused
#define EVENT_MESSAGE_SIM_UNPAUSED                      0x0039  ///< Simulation is unpaused
#define EVENT_MESSAGE_SIMCONNECT_SIMULATION_START       0x0022  ///< SimConnect has detected "simulating" has started
#define EVENT_MESSAGE_SIMCONNECT_SIMULATION_STOP        0x0023  ///< SimConnect has detected "simulating" has stopped
#define EVENT_MESSAGE_SCENARIO_LOADING                  0x0027  ///< Sent at beginning of scenario load sequence
#define EVENT_MESSAGE_SCENARIO_LOADED                   0x0028  ///< Sent at end of scenario load sequence
#define EVENT_MESSAGE_CURRENT_SCENARIO_CHANGED          0x002F  ///< Global current scenario changed
#define EVENT_MESSAGE_SCENARIO_POST_LOADED              0x0030  ///< After scenario PostLoad is completed
#define EVENT_MESSAGE_CRASH_START                       0x0081  ///< The user's plane just started crash sequence
#define EVENT_MESSAGE_CRASH_COMPLETE                    0x0082  ///< The user plane crashed and the crash sequence is done
#define EVENT_MESSAGE_TIMESEASON_CHANGED_10MIN          0x0110  ///< 10 minute time change
#define EVENT_MESSAGE_TIMESEASON_CHANGED_1MIN           0x0112  ///< 1 minute time change
#define EVENT_MESSAGE_TIMESEASON_CHANGED_BY_USER        0x0113  ///< User has changed the time via the time/season dialogue
#define EVENT_MESSAGE_REQUESTED_SIM_RATE_CHANGED        0x011A  ///< Sends the new value of the sim speed
#define EVENT_MESSAGE_WEATHER_CHANGED                   0x0133  ///< If there's any change in the weather, this message will get sent
#define EVENT_MESSAGE_WEATHER_MODE_CHANGED              0x0138  ///< Weather system mode (i.e. RWW, theme, custom) has been changed
#define EVENT_MESSAGE_CHANGE_USER_VEHICLE_START         0x240   ///< User vehicle is changing
#define EVENT_MESSAGE_CHANGE_USER_VEHICLE_CREATED       0x241   ///< New User Vehicle is created but hasn't been registered
#define EVENT_MESSAGE_CHANGE_USER_VEHICLE_FINISH        0x242   ///< User vehicle has finished changing
#define EVENT_MESSAGE_SIM_FROZEN                        0x270   ///< Sim frozen
#define EVENT_MESSAGE_SIM_UNFROZEN                      0x271   ///< Sim unfrozen
#define EVENT_MESSAGE_AVATAR_ATTACH_CHANGED             0x280   ///< The user's avatar has either attached or detached.

// windows
#define EVENT_MESSAGE_DESTROY_WINDOW                    0x0150  ///< Window is about to be destroyed
#define EVENT_MESSAGE_CREATE_WINDOW                     0x0151  ///< New window was created
#define EVENT_MESSAGE_VIEW_ACTIVATE                     0x0160  ///< Window activated
#define EVENT_MESSAGE_VIEW_MODE_CHANGED                 0x0161  ///< View changed on a window
#define EVENT_MESSAGE_VIEW_CREATE                       0x0163  ///< Window created
#define EVENT_MESSAGE_VIEW_DESTROY                      0x0164  ///< Window destroyed

// Multiplayer
#define EVENT_MESSAGE_MULTIPLAYER_SERVER_STARTED        0x0192  ///< When the host has created the server
#define EVENT_MESSAGE_MULTIPLAYER_CLIENT_STARTED        0x0193  ///< When a client has joined a session
#define EVENT_MESSAGE_MULTIPLAYER_LAUNCHED              0x0194  ///< When the multiplayer session is launched
#define EVENT_MESSAGE_MULTIPLAYER_SESSION_ENDED         0x0195  ///< When the multiplayer session is ended

// HMD Notifications
#define EVENT_MESSAGE_HMD_CONNECTED                     0x01A3   ///< Sent when an HMD is connected
#define EVENT_MESSAGE_HMD_DISCONNECTED                  0x01A4   ///< Sent when an HMD is disconnected
#define EVENT_MESSAGE_HMD_VIEW_OPENED                   0x01A5   ///< Sent when an HMD view is turned on
#define EVENT_MESSAGE_HMD_VIEW_CLOSED                   0x01A6   ///< Sent when an HMD view is turned off
#define EVENT_MESSAGE_HMD_VIEW_DISABLE                  0x01A7   ///< Send to disable current HMD view and revert to previous

// panel windows
#define EVENT_MESSAGE_PANEL_OPENED                      0x0231   ///< Panel opened
#define EVENT_MESSAGE_PANEL_CLOSED                      0x0232   ///< Panel closed

//SimDirector
#define EVENT_MESSAGE_SIMDIRECTOR_ACTIVE                0x022D   /// Sim Director changed from inactive to active
#define EVENT_MESSAGE_SIMDIRECTOR_INACTIVE              0x022E   /// Sim Director changed from active to inactive

//Virtual Reality
#define EVENT_MESSAGE_ENABLE_VR_ON_STARTUP              0x300    ///Enable VR by default

#define EVENT_MESSAGE_RENDERING_SYSTEM_PRESENT_BEGIN    0x310   /// Just before rendering system presents windows
#define EVENT_MESSAGE_RENDERING_SYSTEM_PRESENT_END      0x311   /// Just after rendering system presents windows

/** \} */ // end doc group
}