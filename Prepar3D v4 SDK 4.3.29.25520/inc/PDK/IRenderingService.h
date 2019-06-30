/**
* Copyright (C) Lockheed Martin 2016.
**/

#ifndef RENDERINGSERVICE_H
#define RENDERINGSERVICE_H

#include <ObjBase.h>

/** @addtogroup types 
 * @{ 
 */

/**
 *  Struct that stores world position lat/lon in degrees, and altitude in meters.
 */
struct LLADegreesMeters
{
    double Latitude;
    double Longitude;
    double Altitude;

    LLADegreesMeters() : Latitude(0.0), Longitude(0.0), Altitude(0.0) {}
    
    /** 
     *  @param   latitude     Latitude in degrees
     *  @param   longitude    longitude in degrees
     *  @param   altitude     altitude in meters
     */
    LLADegreesMeters(double latitude, double longitude, double altitude)
        : Latitude(latitude), Longitude(longitude), Altitude(altitude) {}
};

/**
 *  Struct that stores orientation PBH (pitch, bank, heading) in degrees
 */
struct PBHDegrees
{
    float Pitch;
    float Bank;
    float Heading;

    PBHDegrees() : Pitch(0.0f), Bank(0.0f), Heading(0.0f) {}
    /** 
     *  @param   pitch       pitch in degrees
     *  @param   bank        bank in degrees
     *  @param   heading     heading in degrees
     */
    PBHDegrees(float pitch, float bank, float heading)
        : Pitch(pitch), Bank(bank), Heading(heading) {}
};

struct XYZMeters
{
    float X;
    float Y;
    float Z;

    XYZMeters() : X(0.0f), Y(0.0f), Z(0.0f) {}
    /** 
     *  @param   x     x offset in meters
     *  @param   y     y offset in meters
     *  @param   z     z offset in meters
     */
    XYZMeters(float x, float y, float z) : X(x), Y(y), Z(z) {}

    XYZMeters operator - (const XYZMeters& rhs) const
    {
        return XYZMeters(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
    }

    XYZMeters& operator -= (const XYZMeters& rhs)
    {
        return *this = *this - (rhs);
    }

    XYZMeters operator + (const XYZMeters& rhs) const
    {
        return XYZMeters(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z);
    }

    XYZMeters& operator += (const XYZMeters& rhs)
    {
        return *this = *this + (rhs);
    }

    XYZMeters operator * (const XYZMeters& rhs) const
    {
        return XYZMeters(this->X * rhs.X, this->Y * rhs.Y, this->Z * rhs.Z);
    }

    XYZMeters& operator *= (const XYZMeters& rhs)
    {
        return *this = *this * (rhs);
    }

    XYZMeters operator * (const float& rhs) const
    {
        return XYZMeters(this->X * rhs, this->Y * rhs, this->Z * rhs);
    }

    XYZMeters& operator *= (const float& rhs)
    {
        return *this = *this * (rhs);
    }

    XYZMeters operator / (const XYZMeters& rhs) const
    {
        return XYZMeters(this->X / rhs.X, this->Y / rhs.Y, this->Z / rhs.Z);
    }

    XYZMeters& operator /= (const XYZMeters& rhs)
    {
        return *this = *this / (rhs);
    }

    XYZMeters operator / (const float& rhs) const
    {
        return XYZMeters(this->X / rhs, this->Y / rhs, this->Z / rhs);
    }

    XYZMeters& operator /= (const float& rhs)
    {
        return *this = *this / (rhs);
    }

    bool operator == (const XYZMeters& src) const
    {
        if (this->X != src.X || this->Y != src.Y || this->Z != src.Z)
        {
            return false;
        }
        return true;
    }

    bool operator != (const XYZMeters& src) const
    {
        return !(*this == src);
    }
};

struct ScreenCoord
{
    float XPixels;
    float YPixels;
    float DistanceMeters;

    ScreenCoord() : XPixels(0.0f), YPixels(0.0f), DistanceMeters(0.0f) {}
    /**
    *  @param   x     x offset in meters
    *  @param   y     y offset in meters
    *  @param   z     z offset in meters
    */
    ScreenCoord(float x, float y, float dist) 
        : XPixels(x), YPixels(y), DistanceMeters(dist) {}
};

/** 
 *  Defines a local transformation which includes a position and orientation offset
 */
struct ObjectLocalTransform
{
    XYZMeters XYZ;
    PBHDegrees PBH;

    ObjectLocalTransform(){}
    /** 
     *  @param   x           x offset in meters
     *  @param   y           y offset in meters
     *  @param   z           z offset in meters
     *  @param   pitch       pitch in degrees
     *  @param   bank        bank in degrees
     *  @param   heading     heading in degrees
     */
    ObjectLocalTransform(float x, float y, float z, float pitch, float bank, float heading)
        : XYZ(x, y, z), PBH(pitch, bank, heading) {}
};

/** 
 *  Defines a world transformation which includes an LLA for position and PBH for orientation.
 *  Altitude is in meters.  All angle values are in degrees.
 */
struct ObjectWorldTransform
{
    LLADegreesMeters LLA;
    PBHDegrees PBH;

    ObjectWorldTransform(){}
    /** 
     *  @param   latitude     Latitude in degrees
     *  @param   longitude    longitude in degrees
     *  @param   altitude     altitude in meters
     *  @param   pitch       pitch in degrees
     *  @param   bank        bank in degrees
     *  @param   heading     heading in degrees
     */
    ObjectWorldTransform(double latitude, double longitude, double altitude,
        float pitch, float bank, float heading)
        : LLA(latitude, longitude, altitude), PBH(pitch, bank, heading) {}
};

/**
*  Union that stores color in ARGB 8 bit format
*/
union ARGBColor
{
    unsigned int Color;
    struct
    {
        unsigned int Blue : 8;
        unsigned int Green : 8;
        unsigned int Red : 8;
        unsigned int Alpha : 8;
    };

    ARGBColor() : Color(0) {}

    /**
    *  @param   alpha     Alpha component of the color 
    *  @param   red       Red component of the color
    *  @param   green     Green component of the color
    *  @param   blue      Blue component of the color
    */
    ARGBColor(unsigned int alpha, unsigned int red, unsigned int green, unsigned int blue)
        :Alpha(alpha), Red(red), Green(green), Blue(blue) {}
};

/**
*  Union that stores RenderFlags to control drawing
*/
union RenderFlags
{
    unsigned int Flags;
    struct
    {
        bool DrawFromBase : 1; //Draw the object from the center of the base
        bool DrawWithVC : 1; //Draw the object with the virtual cockpit
        bool DepthReadDisable : 1; //Draw on top without reading depth
        bool AlphaWriteEnable : 1; //Enable writing alpha
        bool ActAsStencil : 1; //Use this object as a stencil. If DrawWithVC is used, will draw before VC parts.
    };

    RenderFlags() : Flags(0) {}

    RenderFlags(unsigned int flags) : Flags(flags) {}
};

/** @} */ // end of group
/** @addtogroup rendering
* @{
*/

/**
* Service for rendering into a view
*/
DECLARE_INTERFACE_(IObjectRendererV400, IUnknown)
{

    /**
    *  Draw a sphere
    *  @param    location           Location the object will be drawn
    *  @param    radius             Radius of the object in meters
    *  @param    color              Color of the object
    *  @param    RenderFlags        Render flags to control drawing
    */
    virtual HRESULT DrawSphere(const ObjectWorldTransform& location, 
                               float radius,
                               ARGBColor color,
                               RenderFlags renderFlags = 0) abstract;

    /**
    *  Draw a cylinder
    *  @param    location           Location the object will be drawn
    *  @param    radius             Radius of the object in meters
    *  @param    height             Height of the object in meters
    *  @param    color              Color of the object
    *  @param    RenderFlags        Render flags to control drawing
    */
    virtual HRESULT DrawCylinder(const ObjectWorldTransform& location,
                                 float radius, 
                                 float height, 
                                 ARGBColor color, 
                                 RenderFlags renderFlags = 0) abstract;

    /**
    *  Draw a line represented by a rectangular prism
    *  @param    startLocation      Start location the object will be drawn
    *  @param    endLocation        End location the object will be drawn
    *  @param    width              Width of the object in meters
    *  @param    height             Height of the object in meters
    *  @param    color              Color of the object
    *  @param    RenderFlags        Render flags to control drawing
    */
    virtual HRESULT DrawLine(const LLADegreesMeters& startLocation,
                             const LLADegreesMeters& endLocation,
                             float width,
                             float height,
                             ARGBColor color,
                             RenderFlags renderFlags = 0) abstract;

    /**
    *  Draw a rectangular prism
    *  @param    location           Location the object will be drawn
    *  @param    width              Width of the object in meters
    *  @param    height             Height of the object in meters
    *  @param    depth              Depth of the object in meters
    *  @param    color              Color of the object
    *  @param    RenderFlags        Render flags to control drawing
    */
    virtual HRESULT DrawRectangle(const ObjectWorldTransform& location,
                                  float width,
                                  float height, 
                                  float depth,
                                  ARGBColor color,
                                  RenderFlags renderFlags = 0) abstract;

    /**
    *  Draw a trianglular prism
    *  @param    location           Location the object will be drawn
    *  @param    width              Width of the object in meters
    *  @param    height             Height of the object in meters
    *  @param    depth              Depth of the object in meters
    *  @param    color              Color of the object
    *  @param    RenderFlags        Render flags to control drawing
    */
    virtual HRESULT DrawTriangle(const ObjectWorldTransform& location,
                                 float width,
                                 float height,
                                 float depth,
                                 ARGBColor color,
                                 RenderFlags renderFlags = 0) abstract;

   /**
    *  Add a light to the group
    *  @param    x                      x offset in meters from light group origin
    *  @param    y                      y offset in meters from light group origin
    *  @param    z                      z offset in meters from light group origin
    *  @param    lightType              Light type
    *  @param    color                  Light color
    *  @param    size                   Size of light
    *  @param    range                  Distance at which light should be visible
    *  @param    bAttenuateByAmbient    Attenuate light based on ambient light in the scene
    */
    virtual HRESULT AddLight(float x,
                             float y,
                             float z,
                             unsigned int lightType, 
                             unsigned int color,
                             float size, 
                             float range, 
                             bool bAttenuateByAmbient) abstract;

    /**
    * Begin a light group with a world transform as its origin
    * @param  groupOrigin   All lights in the group will be offset relative to this coordinate transformation
    */
    virtual HRESULT BeginLightGroup(ObjectWorldTransform& groupOrigin) abstract;

    /**
    * End a light group.
    * @param  sortGroup  If true, this group of lights will be sorted with other transparent objects
    * in the scene.  Lights placed up on poles or attached to aircraft should be sorted. Lights placed 
    * on the ground generally do not need to be sorted.  Unsorted groups are combined into a single 
    * draw call for better rendering performance.  
    */
    virtual HRESULT EndLightGroup(bool sortGroup) abstract;
    
    /**
    * Apply body relative local transformation to a world transform
    *  @param    llapbhAtOrigin         World transformation of origin or base object
    *  @param    offsetXyzPbh           Local body relative transformation to apply as an offset
    *  @param    llapbhAtOffset         World transformation rusting from applying the body relative offset
    */
    virtual void ApplyBodyRelativeOffset(const ObjectWorldTransform& llapbhAtOrigin,
                                         const ObjectLocalTransform& offsetXyzPbh,
                                               ObjectWorldTransform& llapbhAtOffset) abstract;
                                               
    /**
    * Calculate body relative offset between two world transforms
    *  @param    llapbhAtOrigin         World transformation of the origin or base object
    *  @param    llapbhAtOffset         World transformation to use as a reference for calculating the offset
    *  @param    offsetXyzPbh           Body-relative offset needed to base from the base transform to the reference transform
    */
    virtual void CalculateBodyRelativeOffset(const ObjectWorldTransform& llapbhAtOrigin,
                                             const ObjectWorldTransform& llapbhAtOffset,
                                                   ObjectLocalTransform& offsetXyzPbh) abstract;
};

interface __declspec(uuid("{c32762e6-b6c5-4e05-bad7-0d54da537139}")) IObjectRendererV400;
extern __declspec(selectany) REFIID  IID_IObjectRendererV400 = __uuidof(IObjectRendererV400);
extern __declspec(selectany) REFIID  SID_ObjectRenderer = IID_IObjectRendererV400;

#endif