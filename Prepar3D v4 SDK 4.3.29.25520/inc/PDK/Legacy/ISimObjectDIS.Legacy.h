//-----------------------------------------------------------------------------------
// ISimObjectDIS.Legacy.h
// Copyright (C) Lockheed Martin.  All rights reserved.   
// Provides public interfaces for dll implementations
//-----------------------------------------------------------------------------------

#pragma once

#include "..\Types.h"

namespace P3D
{

/** ## *Professional Plus Only*
* This service allows the developer to interact and retrieve information with a distributed interactive simulation (DIS) session. 
* Developers integrating with this interface should be familiar with and are expected to follow DIS IEEE standards. This service is 
* provided by the IPdk interface.*/
DECLARE_INTERFACE_(IDISManagerV400, IUnknown)
{
    /** Returns TRUE if a DIS connection is active, FALSE otherwise. */
    STDMETHOD_(BOOL, IsConnected)() const PURE;

    /** Provides the EntityType for the given object ID if successful. */
    STDMETHOD (GetEntityTypeById)(__in UINT32 uID, __out P3D::EntityType& EntityType) const PURE;

    /** Used to issue a Fire PDU. */
    STDMETHOD (NotifyMunitionFired)(__in UINT32 uAttackerID,                        /**< The object ID of the firing entity */
                                    __in UINT32 uTargetID,                          /**< The object ID of the target entity if available, 0 otherwise */
                                    __in UINT32 uMunitionID,                        /**< The object ID of the munition entity if available, 0 otherwise */
                                    __in const P3D::EntityType& EntityType,         /**< The EntityType of the munition */
                                    __in const P3D::DXYZ& xyzLonAltLat,             /**< World location in radians and feet */
                                    __in const P3D::DXYZ& xyzLinearVelocity,        /**< World velocity in feet per second */
                                    __in unsigned short usWarheadType,              /**< The warhead type */
                                    __in unsigned short usFuseType,                 /**< The fuse type */
                                    __in unsigned short usQuantity,                 /**< The quantity of munitions represented */
                                    __in unsigned short usRate,                     /**< Rounds per minute */
                                    __in float fRange,                              /**< Meters */
                                    __inout unsigned short& usEventID               /**< Set to 0 for new Fire PDU or previously returned value to signify continuous firing */
                                    ) PURE;

    /** Used to issue a Detonation PDU. */
    STDMETHOD (NotifyMunitionDetonated)(__in UINT32 uAttackerID,                    /**< The object ID of the firing entity */
                                        __in UINT32 uTargetID,                      /**< The object ID of the target entity if available, 0 otherwise */
                                        __in UINT32 uMunitionID,                    /**< The object ID of the munition entity if available, 0 otherwise */
                                        __in const P3D::EntityType& EntityType,     /**< The EntityType of the munition */
                                        __in unsigned short usEventID,              /**< The event ID from an associated Fire PDU if available, 0 otherwise */
                                        __in const P3D::DXYZ& xyzLonAltLat,         /**< Radians/feet */
                                        __in const P3D::DXYZ& xyzLinearVelocity,    /**< World/FPS */
                                        __in unsigned short usWarheadType,          /**< The warhead type */
                                        __in unsigned short usFuseType,             /**< The fuse type */
                                        __in unsigned short usQuantity,             /**< The quantity of munitions represented */
                                        __in unsigned short usRate,                 /**< The rate of fire in rounds per minute */
                                        __in unsigned char yDetonationResult        /**< The result of the detonation */
                                        ) PURE;
};

DEFINE_GUID(IID_IDISManagerV400,  0xcfd6eb04, 0xf23a, 0x4f00, 0x85, 0xe7, 0x6c, 0x55, 0xba, 0xcd, 0x27, 0xf7);

/**************************************************************************************************************
**************************************************************************************************************/

/** @} */

} //End namespace P3D
