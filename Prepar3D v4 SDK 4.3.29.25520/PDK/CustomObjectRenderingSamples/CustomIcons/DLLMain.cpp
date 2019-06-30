///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This example uses the PDK to render custom lights.
///                
///----------------------------------------------------------------------------
#include "initpdk.h"
#include <WinError.h>
#include "PdkPlugin.h"

#include <string>

#define PI                  3.141592653589793238462643
#define PI_OVER_2           (PI / 2)
#define TWO_PI              (2*PI)
#define DegToRad(deg)       ((deg) * (TWO_PI/360)) // To convert degrees to radians Feet

using namespace P3D;

enum class TextureSwap
{
    SWAP_TO_BLUE,
    SWAP_TO_GREEN,
    SWAP_TO_RED,
    SWAP_TO_YELLOW
};

IIconInstanceV410*      g_IconInstances[8] = { 0 }; // Array to properly register/unregister icon instances

class CustomIconsPlugin : public PdkPlugin
{
public:

    HRESULT CreateAndRegisterIconInstanceArray()
    {
        HRESULT hr = E_FAIL;
        if (PdkServices::GetIconService())
        {
            for (UINT i = 0; i < 8; i++)
            {
                g_IconInstances[i] = PdkServices::GetIconService()->CreateIconInstance();
                PdkServices::GetIconService()->Register(g_IconInstances[i]);
            }
            hr = S_OK;
        }
        return hr;
    }

    HRESULT DestroyAndUnregisterIconInstanceArray()
    {
        HRESULT hr = E_FAIL;
        if (PdkServices::GetIconService())
        {
            for (UINT i = 0; i < 8; i++)
            {
                if (g_IconInstances[i] != nullptr)
                {
                    PdkServices::GetIconService()->Unregister(g_IconInstances[i]);
                    g_IconInstances[i]->SetObjectId(0);
                    g_IconInstances[i] = nullptr;
                }
            }
            hr = S_OK;
        }
        return hr;
    }

    // Draw icons in a square around the default starting location
    void DrawIconsInSquare()
    {
        // Lat Lon positions for 3x3 square
        double xAxis[3] = { DegToRad(-86.75), DegToRad(-86.5), DegToRad(-86.25) };
        double zAxis[3] = { DegToRad(30.75), DegToRad(30.5), DegToRad(30.25) };

        // Initialize dxyz
        DXYZ dxyz;
        dxyz.dX = xAxis[0];
        dxyz.dY = 0;
        dxyz.dZ = zAxis[0];

        // Texture Definition for icons (GUIDs linked up in IconConfig.xml)
        GUID guid = { 0x37b0bdeb, 0x15c0, 0x4ae4,{ 0x8b, 0xab, 0x6c, 0xd3, 0x79, 0x13, 0x0c, 0x34 } };

        // Create and register the array of icon instances
        if(SUCCEEDED(CreateAndRegisterIconInstanceArray()))
        {
            // Icon Instance array index
            UINT iconInstanceIndex = 0;

            // Traverse array to place and set texture of each icon
            for (UINT xIndex = 0; xIndex < (sizeof(xAxis) / sizeof(xAxis[0])); xIndex++)
            {
                for (UINT zIndex = 0; zIndex < (sizeof(zAxis) / sizeof(zAxis[0])); zIndex++)
                {
                    // Skip center of 3x3 square
                    if (xIndex == 1 && zIndex == 1)
                    {
                        continue;
                    }
                    if (g_IconInstances[iconInstanceIndex] != nullptr)
                    {
                        // Set proper lat/long
                        dxyz.dX = xAxis[xIndex];
                        dxyz.dZ = zAxis[zIndex];
                        g_IconInstances[iconInstanceIndex]->SetPosition(dxyz);

                        // Set Icon
                        g_IconInstances[iconInstanceIndex]->SetTextureDefinition(guid);
                    }
                    iconInstanceIndex++;
                }
            }
        }
    }

    // Change icon over ownship
    void SwapTextureDefinition(TextureSwap swapTo)
    {
        GUID guid;

        // These GUIDs are linked up in IconConfig.xml
        switch (swapTo)
        {
        case TextureSwap::SWAP_TO_YELLOW:
            guid = { 0x6279a926, 0x6a3d, 0x4c69,{ 0xb5, 0xed, 0x10, 0xf9, 0x46, 0x75, 0x9d, 0x88 } };
            m_pIconInstance->SetTextureDefinition(guid);
            break;
        case TextureSwap::SWAP_TO_RED:
            guid = { 0xb9553bfd, 0x43c0, 0x462b,{ 0xa8, 0x66, 0x93, 0x3c, 0xad, 0x3f, 0x2f, 0x72 } };
            m_pIconInstance->SetTextureDefinition(guid);
            break;
        case TextureSwap::SWAP_TO_GREEN:
            guid = { 0xcc98cf1f, 0x32a7, 0x43e4,{ 0xa9, 0xb6, 0x0e, 0x24, 0xb7, 0x11, 0xa1, 0x67 } };
            m_pIconInstance->SetTextureDefinition(guid);
            break;
        case TextureSwap::SWAP_TO_BLUE:
            guid = { 0x8e109987, 0x72b4, 0x4c75,{ 0xba, 0xee, 0xdf, 0xea, 0x74, 0x62, 0x9a, 0x99 } };
            m_pIconInstance->SetTextureDefinition(guid);
            break;
        default:
            break;
        }
    }

    ///----------------------------------------------------------------------------
    ///  PdkPlugin Callback overrides
    ///----------------------------------------------------------------------------
    virtual void OnOneHz(IParameterListV400* pParams) override
    {
        CComPtr<IBaseObjectV400> spUserObject;

        // Get the user object
        if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetUserObject(&spUserObject)))
        {

            // Set this m_pIconInstance to user object and get its position
            UINT iconInstanceObjectId = 0;
            const UINT userObjectId = spUserObject->GetId();

            // Get the IconInstanceList 
            IconInstanceList list;
            IIconInstanceV410* pIconInstanceIterItem = nullptr;

            // Ensure icon service exists before calling on it
            if (PdkServices::GetIconService())
            {
                bool ownshipInstanceFound = false;
                PdkServices::GetIconService()->GetIconInstanceList(list);

                // Iterate through the IconInstanceList to set m_pIconInstance to the user object's instance
                for (auto pIconInstanceIterItem : list.Items)
                {
                    if (pIconInstanceIterItem != nullptr)
                    {
                        iconInstanceObjectId = pIconInstanceIterItem->GetObjectId();
                        if (iconInstanceObjectId == userObjectId)
                        {
                            // Set m_pIconInsance to the Ownship Icon Instance
                            m_pIconInstance = pIconInstanceIterItem;
                            ownshipInstanceFound = true;
                            continue;
                        }
                        // Turn off visiblity for all Icons besides Ownship Icon and Positioned Icons
                        else if (iconInstanceObjectId > 0) // Positioned icons have no object id association
                        {
                            if (pIconInstanceIterItem->IsVisible()) // Unwanted icons set to not draw
                            {
                                pIconInstanceIterItem->SetVisibility(false);
                            }
                        }
                    }
                }

                // m_pIconInstance is now set to ownship's IconInstance
                if (ownshipInstanceFound && m_pIconInstance != nullptr)
                {
                    // Change texture definition every 2 seconds
                    m_uOneSecondTick++;
                    if (m_uOneSecondTick >= 8)
                    {
                        m_uOneSecondTick = 0;
                        SwapTextureDefinition(TextureSwap::SWAP_TO_YELLOW);
                    }
                    else if (m_uOneSecondTick == 6)
                    {
                        SwapTextureDefinition(TextureSwap::SWAP_TO_RED);
                    }
                    else if (m_uOneSecondTick == 4)
                    {
                        SwapTextureDefinition(TextureSwap::SWAP_TO_GREEN);
                    }
                    else if (m_uOneSecondTick == 2)
                    {
                        SwapTextureDefinition(TextureSwap::SWAP_TO_BLUE);
                    }
                }
            }
        }
    }

protected:

    IIconInstanceV410* m_pIconInstance;
    UINT m_uOneSecondTick = 0;
};


///----------------------------------------------------------------------------
/// Prepar3D DLL start and end entry points
///----------------------------------------------------------------------------

static CustomIconsPlugin* s_pCustomIconsPlugin = nullptr;

void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);

    // Create an instance of the class
    s_pCustomIconsPlugin = new CustomIconsPlugin;

    // Draw stationary square around the default location
    s_pCustomIconsPlugin->DrawIconsInSquare();
}

// Properly destroy memory of IconInstances
void __stdcall DLLStop(void)
{
    if (s_pCustomIconsPlugin)
    {
        s_pCustomIconsPlugin->DestroyAndUnregisterIconInstanceArray();
    }
    if (s_pCustomIconsPlugin != nullptr)
    {
        delete s_pCustomIconsPlugin;
    }
    PdkServices::Shutdown();
}