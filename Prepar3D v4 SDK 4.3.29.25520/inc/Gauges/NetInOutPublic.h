//-----------------------------------------------------------------------------
// NetInPublic
//
// Description: Interface to Prepar3D Serialization methods
//-----------------------------------------------------------------------------
//
// Copyright (c) 2014 by Lockheed Martin, all rights reserved
//
//

#ifndef _NETINOUT_PUBLIC_H
#define _NETINOUT_PUBLIC_H

interface NetInPublic
{
public:
    virtual const void*     Read        (unsigned int sz)  abstract;
    virtual bool            ReadBool    (void)             abstract;
    virtual unsigned char   ReadByte    (void)             abstract;
    virtual short           ReadShort   (void)             abstract;
    virtual INT32           ReadInt32   (void)             abstract;
    virtual float           ReadFloat   (void)             abstract;
    virtual double          ReadDouble  (void)             abstract;
    virtual unsigned short  ReadUShort  (void)             abstract;
    virtual unsigned long   ReadDword   (void)             abstract;

    /// Read the current location of the stream without affecting the current pointer.
    virtual const byte*     GetCurBuffer(void)  const abstract;
    virtual unsigned int    BytesLeft   (void)  const abstract;

    // TODO Unicode - Enable wide multiplayer strings
#ifdef _UNICODE
    virtual const WCHAR*    ReadString  (void) abstract;
#else
    virtual const CHAR*     ReadString  (void) abstract;
#endif 

};

interface NetOutPublic
{
public:
    virtual void WriteData(const void* pv, unsigned int len) abstract;
    virtual void WriteBool(bool b)              abstract;
    virtual void WriteByte(unsigned char b)     abstract;
    virtual void WriteShort(short value)        abstract;
    virtual void WriteInt32(INT32 value)        abstract;
    virtual void WriteUShort(unsigned short )   abstract;
    virtual void WriteFloat(float value)        abstract;
    virtual void WriteDouble(double value)      abstract;
    virtual void WriteDword(DWORD value)        abstract;
#ifdef _UNICODE
    virtual void WriteString(LPCWSTR pszStr)    abstract;
#else
    virtual void WriteString(LPCSTR pszStr)    abstract;
#endif
};


#endif //_NETINOUT_PUBLIC_H

