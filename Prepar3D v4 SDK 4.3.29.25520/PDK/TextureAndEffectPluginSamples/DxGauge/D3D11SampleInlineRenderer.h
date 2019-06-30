///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This Example will render a cursor into Prepar3D as a new 
///               Texture or as an Effect.  
///               **Borrowed and Refactored from Microsoft DX11 Example
///                
///----------------------------------------------------------------------------

#ifndef D3D11SAMPLEINLINERENDERER_H
#define D3D11SAMPLEINLINERENDERER_H

#pragma once

#include "IUnknownHelper.h"
#include "RenderingPlugin.h"

#pragma warning ( push )
#pragma warning (disable : 4005)
#include <d3d11.h>
#include <d3dx11.h> // Ensure DirectX Software Development Kit (SDK) is installed and linked
#include <d3dcompiler.h>
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4838)
#include <xnamath.h>
#pragma warning( pop )

// Simple d3d11 renderer which implements the custom render callback interface
// so that it can be registered with Prepar3D for rendering custom textures
// and effects
class D3D11SampleInlineRenderer : public P3D::RenderingPlugin
{
public:

    D3D11SampleInlineRenderer(bool bIsEffect);
    ~D3D11SampleInlineRenderer();

    /* IRenderingPluginV400 */
    virtual void Render(P3D::IRenderDataV400* pRenderData) override;
    virtual void PreRender(P3D::IRenderDataV400* pRenderData) override;

    // Free resources
    void CleanupDevice(void);

private:

    typedef struct
    {
        XMFLOAT3 Pos;
    } SIMPLEVERTEX;

    // Compile custom sahder
    HRESULT CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

    // Init resources and buffers
    HRESULT InitDevice(ID3D11Device* pDevice);

    // Call render and with the RT passed in
    void Render(ID3D11RenderTargetView* pRenderTarget, FLOAT width, FLOAT height);

    ID3D11Device*           mDevice;
    ID3D11VertexShader*     mVertexShader;
    ID3D11PixelShader*      mPixelShader;
    ID3D11InputLayout*      mVertexLayout;
    ID3D11Buffer*           mVertexBuffer;

    bool mIsEffect;
};

#endif
