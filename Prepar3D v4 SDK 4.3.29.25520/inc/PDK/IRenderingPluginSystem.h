///----------------------------------------------------------------------------
///
///  Copyright 2014 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: Public interface for Prepar3D Plugin system
///                
///----------------------------------------------------------------------------

#ifndef RS_IPLUGIN_SYSTEM_H
#define RS_IPLUGIN_SYSTEM_H

#include <windows.h>
#include <Helpers\ListBuilder.h>

struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;

namespace P3D
{
    /** @defgroup rendering Rendering Services
    *  The rendering services enables external applications to render custom content into a Prepar3D 3D view.
    *  The rendering plugin system allows for custom DirectX® 11 rendering into
    *  textures or overlay onto a view using rendering plug-ins.  The object rendering services provides a
    *  way to add custom objects such as lights to a 3d scene.
    *  @{
    */

    interface IRenderingPluginServiceV400;
    interface IRenderingPluginV400;
    interface IRenderDataV400;

    /** This class holds a set of flags that describe how the render data will be used
    *  during the Render() callback. These should be set using SetRenderFlags
    *  during the PreRender() callback.
    *  @remark The IRenderDataV400 provided to the Render() call will only contain resources that were
    *  requested with these flags.
    **/
    struct PdkRenderFlags
    {
        PdkRenderFlags() : RenderingIsEnabled(true), WillWriteColor(true), WillWriteDepthStencil(false),
            WillReadColor(true), WillReadDepthStencil(false), WillModifyDeviceState(true), Output8bpp(false) {}
        /** If false, the Render() function will not be called **/
        bool RenderingIsEnabled : 1;
        /** If false, OutputColor will not be provided **/
        bool WillWriteColor : 1;
        /** If false, OutputDepthStencil will not be provided **/
        bool WillWriteDepthStencil : 1;
        /** If false, InputColor will not be provided **/
        bool WillReadColor : 1;
        /** If false, InputDepthStencil will not be provided **/
        bool WillReadDepthStencil : 1;
        /** Set this to trun if any device/content states will be set by the render call. **/
        bool WillModifyDeviceState : 1;
        bool Output8bpp : 1;
    };

    enum PdkRenderPass {
        RenderPassPreVc = 0,
        RenderPassPreVcPanels,
        RenderPassDefault,
        RenderPassPreScene,
    };

    struct IWindowV400;
    struct ICameraSystemV400;

    #include "Legacy\IRenderingPluginSystem.legacy.h"

    /**
     * Plugin service used to register custom rendering plugins.  These plugins can be used to render into
     * a texture, or to render on top of an existing view.  If requested, a plugin can also read from the current
     * view output allowing it to implement post process effects that are too complex to be implemented through
     * the xml/hlsl based custom post process system.
     **/
    DECLARE_INTERFACE_(IRenderingPluginSystemV430, IRenderingPluginSystemV400)
    {
        /**
         * Create a new texture given name, size, and a callback function for each update
         * @param   name    Name of texture.  Models and gauges can map to this texture by name.
         * @param   width   Width of texture in pixels
         * @param   height  Height of texture in pixels
         * @param   plugin  plugin that will be used to render into the texture
         * @return          HRESULT, S_OK if function succeeds or E_FAIL if it fails
         **/
        virtual HRESULT CreateTexture(const WCHAR* name, unsigned int width, unsigned int height, IRenderingPluginV400* plugin) = 0;

        /**
         * Remove a texture given the texture name
         * @param   name    Name of plugin
         * @return          HRESULT, ERR_NONE if function succeeds or ERR_FAIL if it fails
         **/
        virtual HRESULT RemoveTexture(const WCHAR* name) = 0;

        /**
         * Get list of textures created externally
         * @param[out]   nameList    list of names of all plugins
         **/
        virtual HRESULT GetCreatedTextures(INameList& nameList) = 0;

        /**
         * Create a new effect with given name and a callback function for each update
         * @param   name    Name of Effect
         * @param   plugin  plugin that will be used to render the effect
         * @return HRESULT, ERR_NONE if function succeeds or ERR_FAIL if it fails
         **/
        virtual HRESULT CreateEffect(const WCHAR* name, IRenderingPluginV400* plugin) = 0;

        /**
         * Remove an effect with given name that was created externally
         * @param   name    Name of Effect
         * @return HRESULT, ERR_NONE if function succeeds or ERR_FAIL if it fails
         **/
        virtual HRESULT RemoveEffect(const WCHAR* name) = 0;

        /**
         * List of effects provided by the system
         * @param[out] names   list of names of effects provided by the system
         * @return HRESULT, ERR_NONE if function succeeds or ERR_FAIL if it fails
         **/
        virtual HRESULT GetSystemEffects(INameList& names) = 0;

        /**
        * Get a Win32 HWND by adapter ID.  This provides a window associated with a
        * specific GPU using the AdapterID provided by IRenderData.
        * @param uAdapterID   Adapter ID for a GPU
        * @param[out] hWindow   Reference to an HWND window handle
        * @return HRESULT, ERR_NONE if function succeeds or ERR_FAIL if it fails
        */
        virtual HRESULT GetDeviceWindow(UINT uAdapterID, HWND& hWindow) = 0;
    };

    /**
     *  Rendering plugin interface used to implement texture and effect plugins.
     *  These plugins can be used to render into a texture, or to render on top
     *   of an existing view.  If requested, a plugin can also read from the current
     *  view output allowing it to implement post process effects that are too complex 
     *  to be implemented through the xml/hlsl based custom post process system.
     **/
    DECLARE_INTERFACE_(IRenderingPluginV400, IUnknown)
    {
        /**
         * A callback to render content
         * @param  pRenderData   Interface to rendering device and resources used for rendering.
         * @remark Render will be called during each rendering pass unless the RenderingIsEnabled
         * flag is set to false in PreRender.
         **/
        virtual void Render(IRenderDataV400* pRenderData) abstract;

        /**
        * Called before calling Render.  This function should call 
        * pRenderData->SetRenderFlags() to let the plugin system know
        * what resources in the render data will be read or written.
        **/
        virtual void PreRender(IRenderDataV400* pRenderData) abstract;
    };

    DECLARE_INTERFACE_(IRenderDataV430, IRenderDataV400)
    {
        /**
        * Get Texture width
        * @return  Texture width in pixels
        **/
        virtual FLOAT GetTextureWidth() = 0;
        /**
        * Get Texture height
        * @return  Texture height in pixels
        **/
        virtual FLOAT GetTextureHeight() = 0;

        /**
        * Get Adapter id
        * @return  Current Adapter of RenderData
        **/
        virtual UINT GetAdapterID() = 0;

        /**
        * Get Device
        * @return  D3D11 device.
        **/
        virtual ID3D11Device* GetDevice() abstract;

        /**
        * Get Output Color
        * @return  Ouput color render target view.
        * @remark  Must set WillWriteColor render flag to true in PreRender to use this
        **/
        virtual ID3D11RenderTargetView* GetOutputColor() abstract;

        /**
        * Get Output Depth Stencil
        * @return  Ouput depth stencil view.
        * @remark  Must set WillWriteDepthStencil render flag to true in PreRender to use this
        **/
        virtual ID3D11DepthStencilView* GetOutputDepthStencil() abstract;

        /**
        * Get input color
        * @return  input color shader resource view.
        * @remark  Must set WillReadColor render flag to true in PreRender to use this
        **/
        virtual ID3D11ShaderResourceView* GetInputColor() abstract;

        /**
        * Get Input Depth Stencil
        * @return  Input depth stencil as shader resource view.
        * @remark  Must set WillReadDepthStencil render flag to true in PreRender to use this
        **/
        virtual ID3D11ShaderResourceView* GetInputDepthStencil() abstract;

        /**
        * Get Input Stencil
        * @return  Input stencil as shader resource view.
        * @remark  Will be provided only for RenderPassPreVc.
        * @remark  Stencil available through green channel as uint.
        **/
        virtual ID3D11ShaderResourceView* GetInputStencil() abstract;

        /**
        * Get texture by name
        * @param  name    name of texture
        * @return  shader resource view of texture
        **/
        virtual ID3D11ShaderResourceView* GetTexture(const char* name) abstract;

        /**
        * Get Window
        * @return  Window used for rendering
        **/
        virtual IWindowV400* GetWindow() abstract;

        /**
        * Get Camera
        * @return  Camera used for rendering
        **/
        virtual ICameraSystemV400* GetCamera() abstract;

        /**
        * Get PdkRenderFlags
        * @return  PdkRenderFlags
        **/
        virtual PdkRenderFlags GetRenderFlags() abstract;

        /**
        * Set PdkRenderFlags.  This must be called from PreRender() for Render() to work properly.
        * @param  flags  render flags associated with the current window and render pass
        **/
        virtual void SetRenderFlags(PdkRenderFlags flags) abstract;

        /**
        * Get Render Pass
        * @return  Current render pass.
        **/
        virtual PdkRenderPass GetRenderPass() abstract;
    };

    ///---------------------------------------------------------------------------
    /// Predefined UUIDs

    interface __declspec(uuid("{1336EB04-8030-4C51-A7B2-BD289200049B}")) IRenderingPluginSystemV430;
    extern __declspec(selectany) REFIID  IID_IRenderingPluginSystemV430 = __uuidof(IRenderingPluginSystemV430);
    extern __declspec(selectany) REFGUID SID_RenderingPluginSystem = __uuidof(IRenderingPluginSystemV400);

    interface __declspec(uuid("{03de3b83-f2af-11e1-aff1-0800200c9a66}")) IRenderingPluginV400;
    extern __declspec(selectany) REFIID  IID_IRenderingPluginV400 = __uuidof(IRenderingPluginV400);

    interface __declspec(uuid("{60D1DEEC-0EC3-4333-B423-E4320C8B6DE7}")) IRenderDataV430;
    extern __declspec(selectany) REFIID  IID_IRenderDataV430 = __uuidof(IRenderDataV430);


    //--------------------------------------------------------------------------------------------------
    // These typedefs and definitions exist to ease development using these interfaces.  
    // Update to keep in sync with the latest version.
    extern __declspec(selectany) REFIID  IID_IRenderingPlugin = IID_IRenderingPluginV400;
    extern __declspec(selectany) REFIID  IID_IRenderData = IID_IRenderDataV430;
    extern __declspec(selectany) REFIID  IID_IRenderingPluginSystem = IID_IRenderingPluginSystemV430;
    //--------------------------------------------------------------------------------------------------
    // These typedefs and definitions exist to ease development using these interfaces.  
    // Update to keep in sync with the latest version.
    typedef IRenderDataV430 IRenderData;
    typedef IRenderingPluginV400 IRenderingPlugin;
    typedef IRenderingPluginSystemV430 IRenderingPluginSystem;
    /** @} */ // end of group
}

#endif