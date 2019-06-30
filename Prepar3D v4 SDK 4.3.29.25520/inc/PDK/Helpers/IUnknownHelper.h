
#ifndef _IUNKNOWNHELPER_H
#define _IUNKNOWNHELPER_H

#pragma once
/** @addtogroup types */ /** @{ */
/***********************************************************************************************
***********************************************************************************************/
#define DEFAULT_REFCOUNT_INLINE_IMPL()\
public:\
    volatile LONG   m_RefCount;\
    virtual ULONG STDMETHODCALLTYPE AddRef() {return InterlockedIncrement(&m_RefCount) ;}\
    virtual ULONG STDMETHODCALLTYPE Release()\
    {\
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);\
        if      (RetVal == 0)         {delete this;}\
        else if (RetVal & 0x80000000) {__debugbreak();}\
        return RetVal;\
    }\

/***********************************************************************************************
***********************************************************************************************/
#define DECLARE_IUNKNOWN_WITH_INLINE_REFCOUNT_IMPL()\
public:\
    DEFAULT_REFCOUNT_INLINE_IMPL()\
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(__in REFIID riid, void **ppvObject);\

/***********************************************************************************************
***********************************************************************************************/

#define DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IClassName, IID_IClassName)\
STDMETHODIMP QueryInterface(REFIID riid, PVOID* ppv)\
{\
    HRESULT hr = E_NOINTERFACE;\
\
    *ppv = NULL;\
\
    if (IsEqualIID(riid , IID_IClassName))\
    {\
        *ppv = static_cast<IClassName*>(this);\
    }\
    else if (IsEqualIID(riid , IID_IUnknown))\
    {\
        *ppv = static_cast<IUnknown*>(this);\
    }\
\
    if (*ppv)\
    {\
        hr = S_OK;\
        AddRef();\
    }\
\
    return hr;\
}\

#define NO_COPY_IUNKNOWN_IMPL(IClassName) \
    private:  \
        IClassName(const IClassName&); \
        IClassName& operator=(const IClassName&);

/** @} */

#endif // _IUNKNOWNHELPER_H
