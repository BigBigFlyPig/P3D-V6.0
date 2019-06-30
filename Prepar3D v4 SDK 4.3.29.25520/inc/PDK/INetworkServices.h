///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: Interface to Prepar3D Weather System
///                
///----------------------------------------------------------------------------

#pragma once

#include <atlcomcli.h>

namespace P3D
{
    /** \defgroup networkservices   Network Services
     *
     * This PDK service allows callers to manipulate various networks systems during runtime.
     * \{
     */

     /**
      * This is the interface to the core Prepar3D multiplayer system. An example on how to query
      * for a PDK service can be found in the DLLStart() function of the Camera PDK Sample.
      */
    DECLARE_INTERFACE_(IMultiplayerServiceV430, IUnknown)
    {
    public:

        /**
        * Returns true if the client is currently connected to a multiplayer session.
        * @remarks The client may still be in the lobby at this point.
        */
        STDMETHOD_(bool, InSession)() const abstract;

        /**
        * Returns true if the client is currently connected to a multiplayer session and is the host.
        * @remarks The client may still be in the lobby at this point.
        */
        STDMETHOD_(bool, IsHosting)() const abstract;

        /**
        * Gets the number of players in the session.
        * @return               Returns the number of players in the current session.
        */
        STDMETHOD_(UINT, GetPlayerCount)() const abstract;

        /**
        * Gets the object ID of the player at the given index.
        * @param uIndex         The index of the player.
        * @return               Returns the player's object ID.
        * @remarks              A return value of 0 is an invalid object ID.
        *                       Indexes are 0 based and the max index will be GetPlayerCount() - 1.
        *                       Multiplayer role indexes may changed as users join and leave the session.
        */
        STDMETHOD_(UINT, GetPlayerObjectID)(__in UINT uIndex) const abstract;

        /**
        * Gets the name of the player at the given index.
        * @param uIndex         The index of the player.
        * @param pszName        The buffer to copy the player name into.
        * @param uLength        The length of the buffer to copy the player name into.
        * @return               Returns S_OK if the name was successfully retrieved and copied, E_FAIL otherwise.
        * @remarks              Indexes are 0 based and the max index will be GetPlayerCount() - 1.
        *                       Multiplayer role indexes may changed as users join and leave the session.
        */
        STDMETHOD(GetPlayerName)(__in UINT uIndex, __out LPWSTR pszName, __in UINT uLength) const abstract;

        /**
        * Gets the player role GUID of the player at the given index.
        * @param uIndex         The index of the player.
        * @param guidRole       The player role GUID.
        * @return               Returns S_OK if the multiplayer role GUID for the given player index was successfully found, E_FAIL otherwise.
        * @remarks              Indexes are 0 based and the max index will be GetPlayerCount() - 1.
        *                       Multiplayer role indexes may changed as users join and leave the session.
        *                       This function only succeeds when in a multiplayer structured scenario.
        *                       
        */
        STDMETHOD(GetPlayerRoleGUID)(__in UINT uIndex, __out GUID& guidRole) const abstract;
    };

    /// MultiplayerService interface ID
    DEFINE_GUID(IID_IMultiplayerServiceV430, 0xda7f0157, 0x45b8, 0x423a, 0xb1, 0xfc, 0x8c, 0x1, 0xd7, 0x48, 0xb1, 0x63);
    /// MultiplayerService service ID
    DEFINE_GUID(SID_MultiplayerService, 0x7db88f0f, 0x13f8, 0x4ee0, 0xa1, 0xf8, 0x23, 0x9a, 0x29, 0x23, 0x6b, 0xaf);

    /**
    * The byte position in a component data word where BYTE_POS_0 is the least significant byte
    * and BYTE_POS_3 is the most significat byte.
    */
    enum BYTE_POS
    {
        /// Most Significant Byte
        BYTE_POS_3 = 3,
        BYTE_POS_2 = 2,
        BYTE_POS_1 = 1,
        /// Least Significant Byte
        BYTE_POS_0 = 0,
    };

    /**
    * The short position in a component data word where SHORT_POS_1 is the most significant short
    * and SHORT_POS_0 is the least significant short.
    */
    enum SHORT_POS
    {
        /// Most Significant Short
        SHORT_POS_1 = 1,
        /// Least Significant Short
        SHORT_POS_0 = 0,
    };

    /**
    * This interface is used by a plugin to populate Component Control data when sending and receiving packets.
    * This interface allows for setting/getting of bytes, shorts, integers, floats, doubles, and 64-bit integers.
    * @remarks  When setting a byte or short, the position of the byte and short must also be given.
    * @remarks  When setting a 64-bit value, valid word indexes are [0,2].
    * See also the CigiComponentControl PDK sample.
    **/
    DECLARE_INTERFACE_(IComponentControlV430, IUnknown)
    {
        /// Signals Prepar3D to queue the current Component Control packet data.
        /// This function should be called once per Component Control packet,
        /// but may also be called more than once per IComponentControlCallbackV430::OnSend()
        /// callback to send multiple packets.
        STDMETHOD_(void, SendData)() abstract;
        /// Resets the IComponentControl data to the default values.
        /// Useful for resetting the structure between SendData() calls, if necessary.
        STDMETHOD_(void, ClearData)() abstract;
        /// Sets the Short Component Control flag. If true, Prepar3D will queue a
        /// Short Component Control packet with a call to SendData().
        STDMETHOD_(void, SetShortComponent)(bool bIsShort) abstract;
        /// Gets the short component flag.
        STDMETHOD_(bool, GetShortComponent)() const abstract;
        /// Sets the Component ID.
        STDMETHOD_(void, SetComponentID)(UINT16 uID) abstract;
        /// Gets the Component ID.
        STDMETHOD_(UINT16, GetComponentID)() const abstract;
        /// Sets the Component Class.
        STDMETHOD_(void, SetComponentClass)(UINT8 uClass) abstract;
        /// Gets the Component Class.
        STDMETHOD_(UINT8, GetComponentClass)() const abstract;
        /// Sets the Component State.
        STDMETHOD_(void, SetComponentState)(UINT8 uState) abstract;
        /// Gets the Component State.
        STDMETHOD_(UINT8, GetComponentState)() const abstract;
        /// Sets the Instance ID.
        STDMETHOD_(void, SetInstanceID)(UINT16 uInstanceID) abstract;
        /// Gets the Instance ID.
        STDMETHOD_(UINT16, GetInstanceID)() const abstract;
        /// Sets an unsigned byte for the given word in the given byte position.
        STDMETHOD_(void, SetUCharData)(UINT8 uData, UINT uWord, P3D::BYTE_POS ePos) abstract;
        /// Gets an unsigned byte from the given word in the given byte position.
        STDMETHOD_(UINT8, GetUCharData)(UINT uWord, P3D::BYTE_POS ePos) const abstract;
        /// Sets a byte for the given word in the given byte position.
        STDMETHOD_(void, SetCharData)(INT8 iData, UINT uWord, P3D::BYTE_POS ePos) abstract;
        /// Gets a byte from the given word in the given byte position.
        STDMETHOD_(INT8, GetCharData)(UINT uWord, P3D::BYTE_POS ePos) const abstract;
        /// Sets a 16-bit unsigned short for the given word in the given short position.
        STDMETHOD_(void, SetUShortData)(UINT16 uData, UINT uWord, P3D::SHORT_POS ePos) abstract;
        /// Gets a 16-bit unsigned short from the given word in the given short position.
        STDMETHOD_(UINT16, GetUShortData)(UINT uWord, P3D::SHORT_POS ePos) const abstract;
        /// Sets a 16-bit short for the given word in the given short position.
        STDMETHOD_(void, SetShortData)(INT16 iData, UINT uWord, P3D::SHORT_POS ePos) abstract;
        /// Gets a 16-bit short from the given word in the given short position.
        STDMETHOD_(INT16, GetShortData)(UINT uWord, P3D::SHORT_POS ePos) const abstract;
        /// Sets a 32-bit unsigned integer for the given word.
        STDMETHOD_(void, SetUIntData)(UINT32 uData, UINT uWord) abstract;
        /// Gets a 32-bit unsigned integer from the given word.
        STDMETHOD_(UINT32, GetUIntData)(UINT uWord) const abstract;
        /// Sets a 32-bit integer for the given word.
        STDMETHOD_(void, SetIntData)(INT32 iData, UINT uWord) abstract;
        /// Gets a 32-bit integer from the given word.
        STDMETHOD_(INT32, GetIntData)(UINT uWord) const abstract;
        /// Sets a 32-bit floating point value for the given word.
        STDMETHOD_(void, SetFloatData)(float fData, UINT uWord) abstract;
        /// Gets a 32-bit floating point value from the given word.
        STDMETHOD_(float, GetFloatData)(UINT uWord) const abstract;
        /// Sets a 64-bit unsigned integer value for the given word.
        /// Valid word values for 64-bit data fields are [0,2].
        STDMETHOD_(void, SetUInt64Data)(UINT64 uData, UINT uWord) abstract;
        /// Gets a 64-bit unsigned integer value from the given word.
        /// Valid word values for 64-bit data fields are [0,2].
        STDMETHOD_(UINT64, GetUInt64Data)(UINT uWord) const abstract;
        /// Sets a 64-bit double floating point value for the given word.
        /// Valid word values for 64-bit data fields are [0,2].
        STDMETHOD_(void, SetDoubleData)(double dData, UINT uWord) abstract;
        /// Gets a 64-bit double floating point value from the given word.
        /// Valid word values for 64-bit data fields are [0,2].
        STDMETHOD_(double, GetDoubleData)(UINT uWord) const abstract;
    };

    /// ComponentControl interface ID
    DEFINE_GUID(IID_IComponentControlV430, 0xddb1f3e4, 0x2c8e, 0x44b5, 0x99, 0x92, 0x34, 0xcc, 0x64, 0x49, 0x27, 0xe4);

    /**
    * This interface is implemeted by a plugin to receive Component Control callbacks.
    * Callbacks can be registered/unregistered using the CIGI PDK Service
    * ICigiServiceV430::RegisterComponentControlCallback() and
    * ICigiServiceV430::UnregisterComponentControlCallback() functions.
    **/
    DECLARE_INTERFACE_(IComponentControlCallbackV430, IUnknown)
    {
        /**
        * This function is called once per frame while Prepar3D is an active CIGI host.
        * Plugins should fill the IComponentControl interface with the necessary Compontent Control packet data.
        * The plugin should then call IComponentControlV430::SendData() to signal to Prepar3D that the current
        * data should be queued as an outgoing message. Plugins are able to call SendData() multiple times during
        * this callback to send more than one Component Control packet per frame. The IComponentControlV430::ClearData()
        * function may be called to reset the packet data between calls to SendData(). The ClearData() function is
        * there mainly to ease development and is not required to be called.
        **/
        virtual HRESULT OnSend(IComponentControlV430& CompCtrl) abstract;

        /**
        * This function is called once per frame while Prepar3D is an active CIGI IG.
        * Plugins should implement this function and determine if they are to process the received packet.
        **/
        virtual HRESULT OnReceive(const IComponentControlV430& CompCtrl) abstract;
    };

    /// ComponentControlCallback interface ID
    DEFINE_GUID(IID_IComponentControlCallbackV430, 0xb77c047e, 0x793c, 0x4210, 0x81, 0x72, 0x13, 0x22, 0x9a, 0x14, 0x33, 0x3d);

    /**
    * This is the interface to the core Prepar3D CIGI plugin. An example on how to query
    * for a PDK service can be found in the DLLStart() function of the Camera PDK Sample.
    */
    DECLARE_INTERFACE_(ICigiServiceV430, IUnknown)
    {
    public:

        /**
        * Returns true if the application is currently connected to a CIGI session.
        */
        STDMETHOD_(bool, InSession)() const abstract;

        /**
        * Returns true if the application is currently connected to a CIGI session and is the host.
        */
        STDMETHOD_(bool, IsHosting)() const abstract;

        /**
        * Returns true if the application is currently connected to a CIGI session and is the IG.
        */
        STDMETHOD_(bool, IsIG)() const abstract;

        /**
        * Returns the CIGI major version for the current session.
        */
        STDMETHOD_(UINT, GetMajorVersion)() const abstract;

        /**
        * Returns the CIGI minor version for the current session.
        */
        STDMETHOD_(UINT, GetMinorVersion)() const abstract;

        /**
        * Gets the CIGI Entity ID for the given Prepar3D Object ID.
        * @param    uObjectID   The Prepar3D Object ID.
        * @param    usEntityID  The CIGI Entity ID for the given Prepar3D Object ID.
        * @return               Returns S_OK if the CIGI Entity ID for the given Prepar3D Object ID was found, E_FAIL otherwise.
        */
        STDMETHOD(GetEntityID)(__in UINT uObjectID, __out USHORT& usEntityID) const abstract;

        /**
        * Gets the Prepar3D Object ID for the given CIGI Entity ID.
        * @param    usEntityID  The CIGI Entity ID.
        * @param    uObjectID   The Prepar3D Object ID for the given CIGI Entity ID.
        * @return               Returns S_OK if the Prepar3D Object ID for the given CIGI Entity ID was found, E_FAIL otherwise.
        */
        STDMETHOD(GetObjectID)(__in USHORT usEntityID, __out UINT& uObjectID) const abstract;

        /**
        * Registers the given IComponentControlCallback.
        * @param    pCallback   The callback to be registered.
        * @return               Returns S_OK if the callback was successfully registered, E_FAIL otherwise.
        * @remarks              Plugins will only receive callbacks when in an active CIGI session.
        */
        STDMETHOD(RegisterComponentControlCallback)(__in __notnull P3D::IComponentControlCallbackV430* pCallback) abstract;

        /**
        * Unregisters the given IComponentControlCallback.
        * @param    pCallback   The callback to be unregistered.
        * @return               Returns S_OK if the callback was successfully unregistered, E_FAIL otherwise.
        */
        STDMETHOD(UnregisterComponentControlCallback)(__in __notnull P3D::IComponentControlCallbackV430* pCallback) abstract;
    };

    /// CigiService interface ID
    DEFINE_GUID(IID_ICigiServiceV430, 0x1c500b49, 0x35ea, 0x49eb, 0xaa, 0x90, 0xfa, 0x90, 0x4f, 0xd2, 0x54, 0xb9);

    /// CigiService service ID
    DEFINE_GUID(SID_CigiService, 0x837698ae, 0x6cb7, 0x4ead, 0x9d, 0x34, 0x0, 0x17, 0x5, 0xdc, 0x8e, 0xf4);

    /** \} */

};
