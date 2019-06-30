//----------------------------------------------------------------------------
//
//  Copyright 2017 Lockheed Martin Corporation 
//  Lockheed Martin Proprietary Information
//
//----------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <fstream>

#include "IUnknownHelper.h"
#include "IEventService.h"

class DataHarvester : P3D::ICallbackV400
{
    DEFAULT_REFCOUNT_INLINE_IMPL()
    DEFAULT_IUNKNOWN_QI_INLINE_IMPL(P3D::ICallbackV400, P3D::IID_ICallbackV400)

public:

    DataHarvester() :
        m_RefCount(1),
        m_bIsHarvesting(false)
    {
    }

    virtual void Invoke(P3D::IParameterListV400* pParams);

    static HRESULT StartCallback(void* pContext);
    static HRESULT StopCallback(void* pContext);

    HRESULT Init();
    HRESULT Deinit();

private:

    HRESULT Start();
    HRESULT Stop();

    void PrintHeader();

    template<class T>
    void PrintValue(T value, bool bIsValid = true, bool bIsLast = false);

    bool            m_bIsHarvesting;
    std::wstring    m_strOutputFilePath;
    std::wfstream   m_fs;

};
