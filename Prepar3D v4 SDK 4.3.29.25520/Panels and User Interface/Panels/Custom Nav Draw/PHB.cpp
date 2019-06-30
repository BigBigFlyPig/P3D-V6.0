///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Representation of  pitch, heading, and bank.
///
///----------------------------------------------------------------------------

#include "PHB.h"

// ========================================================================
// Name: PHB
//
// Desc: Constructor.
// ========================================================================
PHB::PHB()
{
    pitch = 0.0;
    heading = 0.0;
    bank = 0.0;
}

// ========================================================================
// Name: PHB
//
// Desc: Constructor that populates the PHB from a DXYZ.
//       dX -> Pitch
//       dY -> Heading
//       dZ -> Bank
// ========================================================================
PHB::PHB(const P3D::DXYZ &phb)
{
    pitch = phb.dX;
    heading = phb.dY;
    bank = phb.dZ;
}

// ========================================================================
// Name: ToDXYZ
//
// Desc: Populates a DXYZ from the PHB.
//       Pitch   -> dX
//       Heading -> dY
//       Bank    -> dZ
// ========================================================================
P3D::DXYZ PHB::ToDXYZ() const
{
    P3D::DXYZ phb;

    phb.dX = pitch;
    phb.dY = heading;
    phb.dZ = bank;

    return phb;
}

// ========================================================================
// Name: FromDXYZ
//
// Desc: Populates the PHB from a DXYZ.
//       dX -> Pitch
//       dY -> Heading
//       dZ -> Bank
// ========================================================================
void PHB::FromDXYZ(const P3D::DXYZ &lonAltLat)
{
    pitch = lonAltLat.dX;
    heading = lonAltLat.dY;
    bank = lonAltLat.dZ;
}