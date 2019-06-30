#include "initpdk.h"
#include "PdkServices.h"

#include "Picking.h"

using namespace P3D;

PickHandler::PickHandler() : mWindow(nullptr), mCamera(nullptr)
{
   // Set size and color of pick sphere to draw
   mPickColor.Red = 255;
   mPickColor.Green = 255;
   mPickColor.Blue = 255;
   mPickColor.Alpha = 128;
   mPickSize = 0.01f;
   // set up some default values for pick points
   for (int i = 0; i < NUM_PICKS; ++i)
   {
      mPicks[i].xyz.XPixels = 100.0f + 100.0f * i;
      mPicks[i].xyz.YPixels = 500.0f;
   }
   // Create the effect rendering plugin
   mRenderPlugin.Attach(new PickingRenderPlugin(this));
   PdkServices::GetRenderingPluginSystem()->CreateEffect(TEXT("MousePickingTest"), mRenderPlugin);
   // Create the window plugin
   mWindowPlugin.Attach(new PickWindowPlugin(this));
}

// Request picks
void  PickHandler::RequestPicks(ICameraSystemV400* camera)
{
    if (camera == mCamera)
    {
        for (int i = 0; i < NUM_PICKS; ++i)
        {
            mPicks[i].ID = camera->AddPickRequest((int)mPicks[i].xyz.XPixels, (int)mPicks[i].xyz.YPixels);
        }
    }
}

// Get pick results and calculate their world location 
void  PickHandler::RequestPickResults(ICameraSystemV400* camera, IWindowV400* window)
{
    if (mWindow == nullptr)
    {
        mCamera = camera;
        mWindow = window;
        mWindow->AddPlugin(mWindowPlugin);
    }

    if (camera == mCamera)
    {
       for (int i = 0; i < NUM_PICKS; ++i)
       {
           camera->GetPickResult(mPicks[i].ID, mPicks[i].wasHit, mPicks[i].xyz.DistanceMeters);
           camera->GetWorldCoord(mPicks[i].xyz, mPicks[i].location);
       }
    }
}

// Draw a white sphere at the location of each pick point
void  PickHandler::OnCustomRender(IParameterListV400* pParams)
{
    if (PdkServices::GetObjectRenderer())
    {
        RenderFlags flags;
        flags.DrawWithVC = true;
        ObjectWorldTransform trans;
        for (int i = 0; i < NUM_PICKS; ++i)
        {
            trans.LLA = mPicks[i].location;
            PdkServices::GetObjectRenderer()->DrawSphere(trans, mPickSize, mPickColor, flags);
        }
    }
}

// Unregister plugins and release resources
void PickHandler::OnApplicationShutdown(IParameterListV400 * pParams)
{
    if (mWindow && mWindowPlugin)
    {
        mWindow->RemovePlugin(mWindowPlugin);
    }
    mWindow = nullptr;
    mCamera = nullptr;
    mWindowPlugin = nullptr;
    // unregister the effect
    PdkServices::GetRenderingPluginSystem()->RemoveEffect(TEXT("MousePickingTest"));
    mRenderPlugin = nullptr;
}

// window callback used for requesting picks
void PickWindowPlugin::OnPreCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{
    mPickHandler->RequestPicks(pCamera);
}

// The post process effect registration is a convenient way to 
// enable this mouse visualization, but the actual rendering will use
// custom object rendering via PDK rather than the more complex method of
// custom DirectX11 rendering.  Let the plugin system know that these resources
// will not be read or written to. 
PickingRenderPlugin::PickingRenderPlugin(PickHandler* handler) : RenderingPlugin(), mPickHandler(handler)
{
    mRenderFlags.WillWriteDepthStencil = false;
    mRenderFlags.WillReadDepthStencil = false;
    mRenderFlags.WillWriteColor = false;
    mRenderFlags.WillReadColor = false;
}

// render callback used to get pick results
void PickingRenderPlugin::Render(IRenderDataV400* data)
{
    // get window and camera and make sure they are valid
    CComPtr<IWindowV400> window = data->GetWindow();
    CComPtr<ICameraSystemV400> camera = data->GetCamera();
    if (window == nullptr || camera == nullptr)
    {
        return;
    }

    // request results of the picks made by the PickHandlers window callback earlier this frame
    mPickHandler->RequestPickResults(camera, window);

    // Set the pick points to be requested next frame
    // get size and calculate center point in pixels
    UINT32 x, y;
    window->GetSize(x, y);
    x /= 2;
    y /= 2;
    // place first point at the center of the window
    mPickHandler->mPicks[0].xyz.XPixels = (float)x;
    mPickHandler->mPicks[0].xyz.YPixels = (float)y;
    // place second point 200 pixels left of center
    mPickHandler->mPicks[1].xyz.XPixels = (float)x - 200;
    mPickHandler->mPicks[1].xyz.YPixels = (float)y;
    // place third point 200 pixels right of center
    mPickHandler->mPicks[2].xyz.XPixels = (float)x + 200;
    mPickHandler->mPicks[2].xyz.YPixels = (float)y;
    // place fourth point 200 pixel below the center
    mPickHandler->mPicks[3].xyz.XPixels = (float)x;
    mPickHandler->mPicks[3].xyz.YPixels = (float)y + 200;
}

///----------------------------------------------------------------------------
/// Prepar3D DLL start and end entry points
///----------------------------------------------------------------------------
static PickHandler* s_pPickPlugin = nullptr;

extern "C" __declspec(dllexport) void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);
    s_pPickPlugin = new PickHandler();
}

extern "C" __declspec(dllexport) void __stdcall DLLStop(void)
{
    if (s_pPickPlugin != nullptr)
    {
        delete s_pPickPlugin;
    }
    PdkServices::Shutdown();
}