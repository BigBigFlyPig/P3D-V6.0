///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: Public interface for Prepar3D Plugin system
///                
///----------------------------------------------------------------------------

#ifndef RS_IPLUGIN_SYSTEM_LEGACY_H
#define RS_IPLUGIN_SYSTEM_LEGACY_H



DECLARE_INTERFACE_(IRenderingPluginSystemV400, IUnknown)
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
};

DECLARE_INTERFACE_(IRenderDataV400, IUnknown)
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


interface __declspec(uuid("{03de3b80-f2af-11e1-aff1-0800200c9a66}")) IRenderingPluginSystemV400;
extern __declspec(selectany) REFIID  IID_IRenderingPluginSystemV400 = __uuidof(IRenderingPluginSystemV400);

interface __declspec(uuid("{af02252d-6a99-498e-86e5-59c736ad3e00}")) IRenderDataV400;
extern __declspec(selectany) REFIID  IID_IRenderDataV400 = __uuidof(IRenderDataV400);

#endif