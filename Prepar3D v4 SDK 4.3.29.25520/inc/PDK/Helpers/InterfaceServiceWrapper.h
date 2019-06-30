#include <ObjBase.h>
#include <atlcomcli.h>
#include <IUnknownHelper.h>
#pragma once
namespace P3D
{
    /** Small service provider wrapper that provides access to an IUnknown object by
     *  returning it as service when queried using an interface.
     */ 
    class InterfaceServiceWrapper : public IServiceProvider
    {
    public:
        InterfaceServiceWrapper(IUnknown* pInterface) : m_RefCount(1), m_pInterface(pInterface)
        {
        }

        HRESULT STDMETHODCALLTYPE QueryService(REFGUID guidService, REFIID riid, void **ppvObject)
        {
            HRESULT hr = E_NOINTERFACE;
            if (m_pInterface != nullptr)
            {
                hr = m_pInterface->QueryInterface(riid, ppvObject);
            }
            return hr;
        }

        CComPtr<IUnknown> m_pInterface;
        DEFAULT_REFCOUNT_INLINE_IMPL()
        DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IServiceProvider, IID_IServiceProvider)
    };
}