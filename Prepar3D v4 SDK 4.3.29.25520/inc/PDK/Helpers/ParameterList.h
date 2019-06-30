#include "InterfaceServiceWrapper.h"
#include "IEventService.h"
#include "IUnknownHelper.h"
#pragma once
namespace P3D
{
    class P3dCallback : public ICallbackV400
    {
    public:
        P3dCallback() : m_RefCount(1)  { }
        
        virtual void Invoke(IParameterListV400* pParams) abstract;

        DEFAULT_REFCOUNT_INLINE_IMPL()
        DEFAULT_IUNKNOWN_QI_INLINE_IMPL(ICallbackV400, IID_ICallbackV400);
    };
    
    /**
    *  Basic parameter list with two parameters
    */
    class ParameterList : public IParameterListV400
    {
    public:
        ParameterList(IServiceProvider* pService, UINT64 uParam0, UINT64 uParam1) :
            m_RefCount(1),
            m_pServiceProvider(pService)
        {
            if (pService == nullptr)
            {
                m_pServiceProvider.Attach(new InterfaceServiceWrapper(nullptr));
            }
            m_Param0.Value = uParam0;
            m_Param1.Value = uParam1;
        }

        virtual IServiceProvider* GetServiceProvider()
        {
            return m_pServiceProvider;
        }

        virtual P3DParameter GetParameter(UINT32 index)
        {     
            switch (index)
            {
            case 0:
                return m_Param0;
                break;
            case 1:
                return m_Param1;
                break;
            }
            P3DParameter zeroParam = { 0 };
            return zeroParam;
        }

        virtual UINT32 GetCount()
        {
            return 2;
        }
    protected:
        CComPtr<IServiceProvider> m_pServiceProvider;
        P3DParameter m_Param0;
        P3DParameter m_Param1;
        static const P3DParameter PARAM_ZERO;
        DEFAULT_REFCOUNT_INLINE_IMPL()
        DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IParameterListV400, IID_IParameterListV400);
    };
}