///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Loads the navigation data arrays from XML files.
///
///----------------------------------------------------------------------------

#include <WinError.h>
#include "CommonUtils.h"
#include "XMLData.h"

#include <xmllite.h>
#include <list>
#include <string>

#define LOG_ERROR() 

namespace Common
{
    static const wchar_t* RESOURCES_LOCATION = L"Resources\\";

    /// Public constructor and destructor
    ///
    XMLData::XMLData()
    {
    }

    XMLData::~XMLData(void)
    {
    }

    //XML Parsing Functions
    XMLNode XMLData::Root()
    {
        // return the top level root node
        return m_xDataRoot;
    }

    HRESULT XMLData::Load(const wchar_t* fName)
    {
        // load the XML file
        HRESULT err = E_FAIL;

        err = OpenXMLResource(fName);
        if (m_pXMLReader != NULL)
        {
            err = S_OK;

            // start with root node
            err = ProcessRoot(m_xDataRoot);
        }
        return err;
    }

    HRESULT XMLData::ProcessRoot(XMLNode &xRootNode)
    {
        HRESULT err = E_FAIL;

        XmlNodeType nodeType;
        bool bLoop = true;

        // read until the first XmlNodeType_Element node
        while (S_OK == (err = m_pXMLReader->Read(&nodeType)))
        {
            switch (nodeType)
            {
                // first Element node should be the root node
            case XmlNodeType_Element:
            {
                err = ProcessElementNode(xRootNode);
                bLoop = false;
                break;
            }
            }
            if (!bLoop)
            {
                break;
            }
        }
        return err;
    }

    HRESULT XMLData::ProcessElementNode(XMLNode &xElementNode)
    {
        const wchar_t* pLName;
        const wchar_t* pLValue;

        HRESULT err = E_FAIL;

        xElementNode.Name = L"";

        if (FAILED(err = m_pXMLReader->GetLocalName(&pLName, NULL)))
        {
            LOG_ERROR("XMLData read error: ProcessElementNode");
        }
        if (FAILED(err = m_pXMLReader->GetValue(&pLValue, NULL)))
        {
            LOG_ERROR("XMLData read error: ProcessElementNode");
        }

        if (pLName != NULL)
        {
            xElementNode.Name = pLName;
        }
        else
        {
            xElementNode.Name = L"";
        }

        if (pLValue != NULL)
        {
            xElementNode.Value = pLValue;
        }
        else
        {
            xElementNode.Value = L"";
        }

        // get attributes
        UINT iCount;
        m_pXMLReader->GetAttributeCount(&iCount);
        if (iCount > 0)
        {
            std::map<std::wstring, std::wstring> lstAttributes;
            err = ProcessAttributes(lstAttributes);
            if (lstAttributes.size() > 0)
            {
                xElementNode.Attributes = lstAttributes;
            }
        }

        // move reader pointer back to element since we're in attributes now
        if (FAILED(err = m_pXMLReader->MoveToElement()))
        {
            LOG_ERROR("XMLData read error: ProcessElementNode");
        }
        // get children, if any
        if (!m_pXMLReader->IsEmptyElement())
        {
            std::list<XMLNode> lstChildNodes;
            err = ProcessChildren(xElementNode.Name.c_str(), lstChildNodes);
            if (lstChildNodes.size() > 0)
            {
                xElementNode.Children = lstChildNodes;
            }
        }
        return err;
    }

    HRESULT XMLData::ProcessChildren(const wchar_t* sParentNodeName, std::list<XMLNode>  &lstChildList)
    {
        XmlNodeType nodeType;
        const wchar_t* pLName;
        bool bLoop = true;

        HRESULT err = E_FAIL;

        // parse each node
        while (S_OK == (err = m_pXMLReader->Read(&nodeType)))
        {
            switch (nodeType)
            {
            case XmlNodeType_Element:
            {
                XMLNode xNode;
                err = ProcessElementNode(xNode);
                if (xNode.Name != L"")
                {
                    // add the node to the list
                    lstChildList.push_back(xNode);
                }
                break;
            }
            case XmlNodeType_EndElement:
            {
                if (FAILED(err = m_pXMLReader->GetLocalName(&pLName, NULL)))
                {
                    LOG_ERROR("XMLData read error: ProcessChildren");
                }
                // reached end of parent node, drop out of while loop
                if (pLName != NULL)
                {
                    if (wcscmp(pLName, sParentNodeName) == 0)
                    {
                        bLoop = false;
                    }
                }
                break;
            }
            }
            if (!bLoop)
            {
                break;
            }
        }
        return err;
    }

    HRESULT XMLData::ProcessAttributes(std::map<std::wstring, std::wstring> &mapAttributes)
    {
        const wchar_t* pAttName;
        const wchar_t* pValue;

        // check for attributes
        HRESULT err = E_FAIL;

        if (!FAILED(err = m_pXMLReader->MoveToFirstAttribute()))
        {
            //process each attribute
            while (TRUE) {
                if (!m_pXMLReader->IsDefault()) {

                    if (FAILED(err = m_pXMLReader->GetLocalName(&pAttName, NULL))) {
                        LOG_ERROR("XMLData read error: ProcessAttributes");
                    }
                    if (FAILED(err = m_pXMLReader->GetValue(&pValue, NULL))) {
                        LOG_ERROR("XMLData read error: ProcessAttributes");
                    }
                    if (pAttName != NULL)
                    {
                        if (pValue != NULL)
                        {
                            mapAttributes[pAttName] = pValue;
                        }
                        else
                        {
                            mapAttributes[pAttName] = L"";
                        }
                    }
                }
                if (S_OK != m_pXMLReader->MoveToNextAttribute())
                {
                    break;
                }
            }
        }
        return err;
    }

    // read in an external XML file 
    HRESULT XMLData::OpenXMLResource(const wchar_t* fName)
    {
        HRESULT err = E_FAIL;
        CComPtr<IStream> pFileStream;

        wchar_t cPath[256];
        wchar_t cPanelDir[256];
        static const wchar_t* FORMAT = L"%s%s%s";
        bool success = P3DInterop::GetXmlDir(cPanelDir, sizeof(cPanelDir));

        m_pXMLReader = nullptr;

        if (success && fName != NULL && wcslen(fName) > 0)
        {
            swprintf_s(cPath, sizeof(cPath), FORMAT, cPanelDir, RESOURCES_LOCATION, fName);
            if (FileExist(cPath))
            {
                wchar_t wc[256];
                size_t convertedChars = 0;

                // Open read-only input stream
                if (FAILED(err = SHCreateStreamOnFile(cPath, STGM_READ, &pFileStream)))
                {
                    LOG_ERROR("XMLData read error: OpenXMLResource");
                }

                if (FAILED(err = CreateXmlReader(__uuidof(IXmlReader), (void**)&m_pXMLReader, NULL)))
                {
                    LOG_ERROR("XMLData read error: OpenXMLResource");
                }

                m_pXMLReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);

                if (FAILED(err = m_pXMLReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
                {
                    LOG_ERROR("XMLData read error: OpenXMLResource");
                }

                if (FAILED(err = m_pXMLReader->SetInput(pFileStream)))
                {
                    LOG_ERROR("XMLData read error: OpenXMLResource");
                }

                if (m_pXMLReader != NULL)
                {
                    err = S_OK;
                }
            }
        }
        return err;
    }

    bool XMLData::FileExist(const wchar_t* sFullPath)
    {
        DWORD err = GetFileAttributes(sFullPath);

        if (INVALID_FILE_ATTRIBUTES == err)
        {
            return false;
        }
        return true;
    }
}