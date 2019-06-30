///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  @contract BWC PC21
///  
///  Representation of  pitch, heading, and bank.
///
///----------------------------------------------------------------------------

#pragma once

#include <atlcomcli.h>
#include "InitGuid.h"

#include "Types.h"

typedef double RADIANS;

struct PHB
{
    PHB();
    PHB(const P3D::DXYZ &phb);

    P3D::DXYZ ToDXYZ() const;
    void FromDXYZ(const P3D::DXYZ&);

    RADIANS pitch;
    RADIANS heading;
    RADIANS bank;
};