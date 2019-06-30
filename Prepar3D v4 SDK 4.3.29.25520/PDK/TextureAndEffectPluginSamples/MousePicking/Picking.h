#pragma once

#include "Pdk.h"                // From Prepar3D SDK
#include "WindowPlugin.h"
#include "RenderingPlugin.h"
#include "PdkPlugin.h"
#include "IUnknownHelper.h"
#include <atlcomcli.h>



class PickHandler;


// Window plugin used to request screenspace points for picking.
class PickWindowPlugin : public P3D::WindowPlugin
{
public:
   PickWindowPlugin(PickHandler* handler) : mPickHandler(handler) { }
   virtual void OnPreCameraUpdate(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera)  override;
private:
   PickHandler* mPickHandler;
};


// Rendering plugin used to get pick results
class PickingRenderPlugin : public P3D::RenderingPlugin
{
public:
    PickingRenderPlugin(PickHandler* handler);
    virtual void Render(P3D::IRenderDataV400* pRenderData) override;
private:
    PickHandler* mPickHandler;
};

// This plugin makes custom pick requests and visualizes the results
class PickHandler : public P3D::PdkPlugin
{
   static const int NUM_PICKS = 4;
public:
   PickHandler();

   // loop through pick list and add each pick request to the window
   // this will be called from a window callback
   void  RequestPicks(P3D::ICameraSystemV400* camera);
   // loop through pick list and request each result from the window
   // this should be called from the render plugin's Render() function
   void  RequestPickResults(P3D::ICameraSystemV400* camera, P3D::IWindowV400* window);

   //----------------------------------------------------------------------------
   //  PdkPlugin Callback overrides
   //----------------------------------------------------------------------------

   // This function will render a while sphere at the location of each pick result
   virtual void OnCustomRender(P3D::IParameterListV400* pParams) override;

   // Unregister the window and renderin plugin and free up resources
   virtual void OnApplicationShutdown(P3D::IParameterListV400* pParams) override;

   // A structure for stroing the data associated with a pick request and response
   struct PickRequest
   {
       PickRequest() : ID(-1), wasHit(false) {}
       ScreenCoord xyz;
       int   ID;
       bool  wasHit;
       LLADegreesMeters location;
   };

   // list of pick requests
   PickRequest mPicks[NUM_PICKS];

protected:
   ARGBColor            mPickColor;
   float                mPickSize;

   CComPtr<P3D::IWindowV400> mWindow;
   CComPtr<P3D::ICameraSystemV400> mCamera;
   CComPtr<PickWindowPlugin> mWindowPlugin;
   CComPtr<PickingRenderPlugin> mRenderPlugin;
};

