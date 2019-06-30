///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This Example will render a cursor into Prepar3D as a new 
///               Texture or as an Effect.  
///               
///----------------------------------------------------------------------------

///----------------------------------------------------------------------------
/// README: 
/// SETUP Prepar3D Configuration
/// This example required additional setup in Prepar3D configuration
/// [dll.xml]
/// [panel.CFG]
/// [Camera.CFG]
///----------------------------------------------------------------------------

#include "PdkServices.h"
#include "D3D11SampleInlineRenderer.h"

#define     RTT_WIDTH       200
#define     RTT_HEIGHT      200

using namespace P3D;

extern "C" __declspec(dllexport) void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);

    CComPtr<IRenderingPluginSystemV400> spPluginSystem = nullptr;    // The Rendering Plugin System 
    CComPtr<IRenderingPluginV400> spTexturePlugin = nullptr;         // The Plugin to create a new texture
    CComPtr<IRenderingPluginV400> spEffectPlugin = nullptr;          // The Plugin to create a new effect

    // Request the Rendering System Plugin from the PDK Services.
    spPluginSystem = PdkServices::GetRenderingPluginSystem();

    // The plugin service is then used to Create a new Texture for Prepar3D
    // This texture will simply contain a Cursor on a blue background,
    // however anything could be rendered into it using DirectX11.

    // Create and register the sample rendering plugin as a texture.
    spTexturePlugin.Attach(new D3D11SampleInlineRenderer(false));
    spPluginSystem->CreateTexture(TEXT("CursorTexture"), RTT_WIDTH, RTT_HEIGHT, spTexturePlugin);

    // Create a new Effect in Prepar3D, this draw a cursor on top of a view
    // Again this is a simple example but anything can be rendered using DirectX11.
    // The current view output can also be requested allowing a custom effect to act as
    // an advanced post process which modifies the view output rather than simply
    // adding content on top of it

    // Create and register the sample rendering plugin as an effect.
    spEffectPlugin.Attach(new D3D11SampleInlineRenderer(true));
    spPluginSystem->CreateEffect(TEXT("CursorEffect"), spEffectPlugin);

    return;
}

extern "C" __declspec(dllexport) void __stdcall DLLStop(void)
{
    CComPtr<IRenderingPluginSystemV400> spPluginSystem = nullptr;   // The Rendering Plugin System 

    spPluginSystem = PdkServices::GetRenderingPluginSystem();

    // Unregister our texture and effect.
    spPluginSystem->RemoveTexture(TEXT("CursorTexture"));
    spPluginSystem->RemoveEffect(TEXT("CursorEffect"));

    PdkServices::Shutdown();
}
