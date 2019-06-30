//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  IUnknownHelper.h
//  
//  Description:
//              Helper class for IUnknown interface.
//------------------------------------------------------------------------------

#ifndef _IUNKNOWNHELPER_H
#define _IUNKNOWNHELPER_H

#pragma once

/***********************************************************************************************
***********************************************************************************************/
#define DECLARE_IUNKNOWN()\
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
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(__in REFIID riid, void **ppvObject);\

#endif // _IUNKNOWNHELPER_H

