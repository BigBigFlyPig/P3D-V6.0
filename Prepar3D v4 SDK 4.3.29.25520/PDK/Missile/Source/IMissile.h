/************************************************************************************************************** 
    IMissile.h

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: Sample interface in which other SDK sample objects can query for.
***************************************************************************************************************/

#pragma once

DECLARE_INTERFACE_(IMissile, IUnknown)
{
    STDMETHOD_(double, GetSpeed())                const PURE; //Feet per second
};

interface __declspec(uuid("{36AA6E5B-7BDC-4826-B516-ED2A84F913CD}")) IMissile;
extern __declspec(selectany) REFIID  IID_IMissile   =   __uuidof(IMissile);
extern __declspec(selectany) REFGUID SID_Missile    =   __uuidof(IMissile);

