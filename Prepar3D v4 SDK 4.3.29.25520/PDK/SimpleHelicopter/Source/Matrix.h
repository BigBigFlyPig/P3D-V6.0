/************************************************************************************************************** 
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple helicopter ISimObject.
***************************************************************************************************************/

#pragma once

#include <math.h>

#include "ISimObject.h"

//Body <-> World Matrix
class Matrix
{
public:

    Matrix()
    {
        memset(&m_dMatrix, 0, sizeof(m_dMatrix));
    }

    void BodyToWorld(__in const P3D::DXYZ& vBody, __out P3D::DXYZ& vWorld)
    {
        vWorld.dZ = m_dMatrix[0][0] * vBody.dZ + m_dMatrix[0][1] * vBody.dX + m_dMatrix[0][2] * vBody.dY;  //l1 m1  n1
        vWorld.dX = m_dMatrix[1][0] * vBody.dZ + m_dMatrix[1][1] * vBody.dX + m_dMatrix[1][2] * vBody.dY;  //l2 m2  n2
        vWorld.dY = m_dMatrix[2][0] * vBody.dZ + m_dMatrix[2][1] * vBody.dX + m_dMatrix[2][2] * vBody.dY;  //l3 m3  n3
    }

    void Update(const P3D::DXYZ& vdOrient)
    {
        double dCosPitch   = cos(vdOrient.dX);
        double dSinPitch   = sin(vdOrient.dX);
        double dCosBank    = cos(vdOrient.dZ);
        double dSinBank    = sin(vdOrient.dZ);
        double dCosHeading = cos(vdOrient.dY);
        double dSinHeading = sin(vdOrient.dY);

        m_dMatrix[0][0] = dCosPitch * dCosHeading;  // l1
        m_dMatrix[1][0] = dCosPitch * dSinHeading;  // l2
        m_dMatrix[2][0] = -dSinPitch;  // l3

        m_dMatrix[0][1] = dSinBank * dSinPitch * dCosHeading - dCosBank * dSinHeading; // m1
        m_dMatrix[1][1] = dSinBank * dSinPitch * dSinHeading + dCosBank * dCosHeading; // m2
        m_dMatrix[2][1] = dSinBank * dCosPitch;  // m3
                            
        m_dMatrix[0][2] = dCosBank * dSinPitch * dCosHeading + dSinBank * dSinHeading;  // n1
        m_dMatrix[1][2] = dCosBank * dSinPitch * dSinHeading - dSinBank * dCosHeading;  // n2
        m_dMatrix[2][2] = dCosBank * dCosPitch;  // n3
    }

    double m_dMatrix[3][3];
};
