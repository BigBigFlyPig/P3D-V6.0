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

#include "initpdk.h"
#include "D3D11SampleInlineRenderer.h"
#include "IWindowPluginSystem.h"

using namespace P3D;

D3D11SampleInlineRenderer::D3D11SampleInlineRenderer(bool bIsEffect) :
    mDevice(NULL),
    mIsEffect(bIsEffect),
    mVertexShader(NULL),
    mPixelShader(NULL),
    mVertexLayout(NULL),
    mVertexBuffer(NULL)
{
}

D3D11SampleInlineRenderer::~D3D11SampleInlineRenderer()
{
    CleanupDevice();
}

//--------------------------------------------------------------------------------------
// Render callback
//--------------------------------------------------------------------------------------
void D3D11SampleInlineRenderer::Render(IRenderDataV400* pRenderData)
{
    if (!pRenderData)
    {
        return;
    }

    // This will return the render target for the output of the view.
    // Plug-in can render anything into this target.  More advanced plug-ins
    // may wish to modify the existing view's output contents.  To do so,
    // an additional shader resource would need to be queried and the 
    // plugin would need to request double buffering which is required for a view
    // output to be both read and written by a plugin
    CComPtr<ID3D11RenderTargetView> spRenderTarget = pRenderData->GetOutputColor();
    if (spRenderTarget == NULL)
    {
        return;
    }

    CComPtr<ID3D11DeviceContext> pDeviceContext = NULL;
    mDevice->GetImmediateContext(&pDeviceContext);

    if (pDeviceContext == NULL)
    {
        return;
    }

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = pRenderData->GetTextureWidth();
    vp.Height = pRenderData->GetTextureHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    pDeviceContext->ClearState();
    pDeviceContext->RSSetViewports(1, &vp);

    static float ClearColor[4] = { 0.5f, 0.0f, 0.5f, 1.0f }; // red,green,blue,alpha

    if (mIsEffect)
    {
        /// Do anything specific for effect callback
    }
    else
    {
        /// This is a texture type so we want to clear out the RT and render everything
        pDeviceContext->ClearRenderTargetView(spRenderTarget, ClearColor);
    }

    pDeviceContext->OMSetRenderTargets(1, &spRenderTarget.p, NULL);

    pDeviceContext->IASetInputLayout(mVertexLayout);

    // Set vertex buffer
    UINT stride = sizeof(SIMPLEVERTEX);
    UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

    // Set primitive topology
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // Render a triangle
    pDeviceContext->VSSetShader(mVertexShader, NULL, 0);
    pDeviceContext->GSSetShader(NULL, NULL, 0);
    pDeviceContext->PSSetShader(mPixelShader, NULL, 0);

    pDeviceContext->Draw(8, 0);

    pDeviceContext->VSSetShader(NULL, NULL, 0);
    pDeviceContext->GSSetShader(NULL, NULL, 0);
    pDeviceContext->PSSetShader(NULL, NULL, 0);

    pDeviceContext->OMSetRenderTargets(0, NULL, NULL);
}

//--------------------------------------------------------------------------------------
// PreRender callback
//--------------------------------------------------------------------------------------
void D3D11SampleInlineRenderer::PreRender(P3D::IRenderDataV400* pRenderData)
{
    if (!pRenderData)
    {
        return;
    }

    // Disable rendering until our device is initialized.
    if (mDevice == NULL)
    {
        mRenderFlags.RenderingIsEnabled = false;
        pRenderData->SetRenderFlags(mRenderFlags);
    }

    CComPtr<ID3D11Device> spDevice = pRenderData->GetDevice();
    if (spDevice == NULL)
    {
        return;
    }

    // Create buffer/input layout and other resources
    if (FAILED(InitDevice(spDevice)))
    {
        return;
    }

    // Only want Render() called during the default render pass
    mRenderFlags.RenderingIsEnabled = (pRenderData->GetRenderPass() == RenderPassDefault);
    
    // Will not read the color render target.
    mRenderFlags.WillReadColor = false;

    pRenderData->SetRenderFlags(mRenderFlags);
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT D3D11SampleInlineRenderer::CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFileA(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
                               dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
    if (FAILED(hr))
    {
        if (pErrorBlob != NULL)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

static const char DLL_NAME[] = "DxGauge.dll";
static const char HLSL_FILE[] = "ShadersHLSL\\Example.fx";
//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT D3D11SampleInlineRenderer::InitDevice(ID3D11Device* pDevice)
{
    HRESULT hr = S_OK;

    if (mDevice == pDevice)
    {
        return hr;
    }

    // get full file path to this dll
    char path[2048];
    GetModuleFileNameA(GetModuleHandleA(DLL_NAME), path, 2048);
    // subtract length of dll filename to get the start index for a local file.
    size_t folderIndex = strlen(path) - strlen(DLL_NAME);
    // copy shader file name to create full path
    size_t uSize = max(ARRAYSIZE(path) - folderIndex - 1, 0);  //How much space available to write to.
    strcpy_s(&path[folderIndex], uSize, &HLSL_FILE[0]);

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile(path, "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        return hr;
    }

    // Create the vertex shader
    hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &mVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = pDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                    pVSBlob->GetBufferSize(), &mVertexLayout);
    pVSBlob->Release();
    if (FAILED(hr))
    {
        return hr;
    }

    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile(path, "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        return hr;
    }

    // Create the pixel shader
    hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &mPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
    {
        return hr;
    }

    // Create vertex buffer
    SIMPLEVERTEX vertices[] =
    {
        XMFLOAT3(0.0f, 0.5f, 0.0f),
        XMFLOAT3(0.0f, 0.1f, 0.0f),
        XMFLOAT3(0.5f, 0.0f, 0.0f),
        XMFLOAT3(0.1f, 0.0f, 0.0f),
        XMFLOAT3(-0.5f, 0.0f, 0.0f),
        XMFLOAT3(-0.1f, 0.0f, 0.0f),
        XMFLOAT3(0.0f, -0.5f, 0.0f),
        XMFLOAT3(0.0f, -0.1f, 0.0f),
    };


    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SIMPLEVERTEX) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = pDevice->CreateBuffer(&bd, &InitData, &mVertexBuffer);

    if (FAILED(hr))
    {
        return hr;
    }

    mDevice = pDevice;

    return S_OK;
}

void D3D11SampleInlineRenderer::CleanupDevice()
{
    if (mVertexBuffer) mVertexBuffer->Release();
    if (mVertexLayout) mVertexLayout->Release();
    if (mVertexShader) mVertexShader->Release();
    if (mPixelShader) mPixelShader->Release();
}
