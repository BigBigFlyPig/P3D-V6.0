///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Loads the navigation data arrays from XML files.
///
///----------------------------------------------------------------------------
#pragma once

#include <xmllite.h>
#include <list>
#include <map>
#include <atlbase.h>
#include <string>

namespace Common
{
    struct XMLAttribute
    {
        std::wstring id;
        std::wstring value;

        XMLAttribute()
        {
            id = L"";
            value = L"";
        }

        XMLAttribute(wchar_t* sid, wchar_t* sValue)
        {
            id = sid;
            value = sValue;
        }
    };

    struct XMLNode
    {
        std::wstring Name;
        std::wstring Value;
        std::map<std::wstring, std::wstring> Attributes;
        std::list<XMLNode> Children;

        XMLNode()
        {
            Name = L"";
            Value = L"";
        }

        XMLNode(const wchar_t* sName, const wchar_t* sValue)
        {
            Name = sName;
            Value = sValue;
        }

        XMLNode(const wchar_t* sName, const wchar_t* sValue, std::map<std::wstring, std::wstring> &lstAttributes)
        {
            Name = sName;
            Value = sValue;
            Attributes = lstAttributes;
        }
    };

    class XMLData
    {

    public:
        XMLData(void);
        ~XMLData(void);
        XMLNode Root(void);
        HRESULT Load(const wchar_t* fName);

    private:
        HRESULT OpenXMLResource(const wchar_t*);
        bool FileExist(const wchar_t*);
        HRESULT ProcessRoot(XMLNode &xRootNode);
        HRESULT ProcessElementNode(XMLNode &xElementNode);
        HRESULT ProcessAttributes(std::map<std::wstring, std::wstring> &mapAttributes);
        HRESULT ProcessChildren(const wchar_t* sParentNodeName, std::list<XMLNode>  &lstChildren);

        CComPtr<IXmlReader> m_pXMLReader;

        XMLNode m_xDataRoot;
    };
}
