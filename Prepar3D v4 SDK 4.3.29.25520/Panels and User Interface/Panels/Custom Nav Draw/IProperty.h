///----------------------------------------------------------------------------
///
///  Copyright (C) Lockheed Martin Corporation 2013
///  Lockheed Martin Proprietary Information
///  
///  @contract BWC PC21
///  
///  Interface to Prepar3D Window System
///----------------------------------------------------------------------------
#pragma once

//#include "IUnknownImpl.h"
//#include "ISerialize.h"
//#include "P3DInteropTypes.h"

#include <WinError.h>

#if defined(_DLLEXPORT_PROPERTY_IPROPERTY)
#define INTERFACE_IPROPERTY __declspec(dllexport)
#else
#define INTERFACE_IPROPERTY
#endif

namespace P3DInterop 
{
    class IProperty;
    class TrafficAircraftQuery;
    class KeyEventProperty;
    class SimConnectProperty;
    class LocalProperty;

    class IUpdateCallback
    {
    public:
        virtual void OnChanged( IProperty*, int ) {}
        virtual void OnChanged( IProperty*, double ) {}
        virtual void OnChanged( IProperty*, const char* ) {}
    };

    /// This is a public interface for all custom, aircraft and local properties.
    ///
    class IProperty// : public IUnknownImpl
    {
    public:
        /// Get the property int value
        virtual HRESULT GetPropertyValue( int* iValue ) const abstract;

        /// Get the property float value
        virtual HRESULT GetPropertyValue( double* fValue ) const abstract;

        /// Get the property string value
        virtual HRESULT GetPropertyValue( const char** sValue ) const abstract;
                
        /// Get the property as int
        virtual int GetIntValue( void ) const abstract;

        /// Get the property as double
        virtual double GetDoubleValue( void ) const abstract;
        
        /// Get the property as string
        virtual const char* GetStringValue( void ) const abstract;

        /// Set the property int value
        virtual HRESULT SetPropertyValue( int iValue) abstract;

        /// Set the property float value
        virtual HRESULT SetPropertyValue( double fValue) abstract;
        
        /// Set the property string value
        virtual HRESULT SetPropertyValue( const char* sValue) abstract;
        
        /// Add a callback when a change has occured
        virtual HRESULT AddChangedCallback( IUpdateCallback* ) abstract;

        /// Remove callback
        virtual HRESULT RemoveChangedCallback( IUpdateCallback* ) abstract;
        
        /// Trigger update on this propery
        virtual HRESULT Update( double fTimeStamp ) abstract;

        /// Get units id converted to p3d enum
        virtual int GetPropertyUnits( void ) const abstract;

        /// Get the units in string format
        virtual const char* GetPropertyUnitsAsString( void ) const abstract;

        /// Get the name of the property
        virtual const char* GetName( void ) const abstract;

        /// Gets the property Type
        virtual PROPERTY_TYPE GetPropertyType() const abstract;

        /// The property has changed since previoys frame
        virtual bool HasChanged( void ) const abstract;
        virtual double GetLastChanged( void ) const abstract;

        /// Indicate someone is listening
        virtual bool HasOnChangedListeners() const abstract;

        virtual bool HasUpdated( void ) const abstract;
        virtual HRESULT SetUpdated( bool ) abstract;

        /// Provide down cast interface
        virtual TrafficAircraftQuery* AsTrafficAircraftQuery() abstract;
        virtual KeyEventProperty* AsKeyEventProperty() abstract;
        virtual SimConnectProperty* AsSimConnectProperty() abstract;
        virtual LocalProperty* AsLocalProperty() abstract;
        
    };

    /// static accessor to query for a custom property
    INTERFACE_IPROPERTY IProperty* GetCustomProperty( const char * sName, const char* sType = S_UNKNOWN );

    /// static accessor to query for a Local property
    INTERFACE_IPROPERTY IProperty* GetLocalProperty( const char * sName, const char* sType = S_UNKNOWN );

    /// static accessor to query for an aircraft property
    INTERFACE_IPROPERTY IProperty* GetAircraftProperty( const char * sName, const char* sType = S_UNKNOWN );

    /// static accessor to query for specific aircraft and its property
    INTERFACE_IPROPERTY IProperty* GetAircraftProperty( unsigned int uAircraftId, const char * sName, const char* sType = S_UNKNOWN );

    /// static accessor to query for specific aircraft and its property
    INTERFACE_IPROPERTY IProperty* GetAircraftQuery( const char * sName, const char* sType = S_UNKNOWN );

    /// static accessor to query for a Local property
    INTERFACE_IPROPERTY IProperty* GetKeyEventProperty( int iKeyEventId );

    /// static accessor to query for a simple string property
    INTERFACE_IPROPERTY IProperty* GetSimpleStringProperty( const char * sName );

    /// static accessor to query for a simple string property
    INTERFACE_IPROPERTY IProperty* GetSimpleDoubleProperty( const char * sName );

    /// static accessor to query for a calculatro property
    /// This call is less efficient then get local or aircrat
    INTERFACE_IPROPERTY IProperty* GetCalculatorProperty( const char * sName, const char* sUnits = S_UNKNOWN );
    
    /// Query for a list of aircraft ids within the user ac radius
    /// Params : radius in meters
    /// Params : i/o list size
    /// Params : out buffer of ac ids
    INTERFACE_IPROPERTY HRESULT QueryAircraftNearUser( double dMeters, UINT& uMax, UINT* uArray );

    /// static accessor to query for a Multiplayer property
    INTERFACE_IPROPERTY IProperty* GetSimConnectProperty( const char* sEventName );
}