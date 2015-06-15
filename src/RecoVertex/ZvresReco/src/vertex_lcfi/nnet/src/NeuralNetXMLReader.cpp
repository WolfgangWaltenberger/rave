#include "NeuralNetXMLReader.h"
#ifndef NEURALNETNOXMLREADER
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML) // MSXML
#include <windows.h>
#include <objbase.h>
#elif defined(USEXERCES)&&!defined(USETINYXML) // Xerces
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
XERCES_CPP_NAMESPACE_USE
#else // TINYXML
#endif

#include <iostream>
#include <cstdlib>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

NeuralNetXMLReader::NeuralNetXMLReader(const std::string &xmlFileName)
: _xmlFileName(xmlFileName),_ok(false),_currentLayer(-1),_currentNeuron(-1)
{
	init();
}

NeuralNetXMLReader::NeuralNetXMLReader(const char *xmlFileName)
: _ok(false),_currentLayer(-1),_currentNeuron(-1)
{
	_xmlFileName = xmlFileName;
	init();
}

NeuralNetXMLReader::~NeuralNetXMLReader(void)
{
	clean();
}

void NeuralNetXMLReader::clean()
{
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)

	if (_pLayers != NULL)
		_pLayers.Release();
	if (_pNeurons != NULL)
		_pNeurons.Release();
	if (_pXMLDoc != NULL)
		_pXMLDoc.Release();
	_pXMLDoc = NULL;
	_pLayers = NULL;
	_pNeurons = NULL;
	CoUninitialize();
	_ok = false;
#elif defined(USEXERCES)&&!defined(USETINYXML)
	if (_pDOMParser != NULL)
		delete _pDOMParser;
	if (_errorHandler != NULL)
		delete _errorHandler;
	XMLPlatformUtils::Terminate();
	_ok = false;
#else
    if (_pNeurons != NULL)
        delete _pNeurons;
    if (_pLayers != NULL)
        delete _pLayers;
    if (_pXMLDoc != NULL)
        delete _pXMLDoc;
    _ok = false;
#endif
}

void NeuralNetXMLReader::init()
{
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
	_pXMLDoc = NULL;
	_pLayers = NULL;
	_pNeurons = NULL;
	CoInitializeEx((LPVOID)NULL,(DWORD)COINIT_APARTMENTTHREADED);
	HRESULT hr = _pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
	if (FAILED(hr))
	{
		std::cerr << "Failed to instantiate DOMDocument4 class" << std::endl;
		std::cerr << "Is MSXML4 installed?" << std::endl;
		return;
	}
	_pXMLDoc->async = VARIANT_FALSE;
	_pXMLDoc->validateOnParse = VARIANT_FALSE;
	hr = _pXMLDoc->load(_xmlFileName.c_str());
	if (hr != VARIANT_TRUE)
	{
		MSXML2::IXMLDOMParseErrorPtr pError;
		pError = _pXMLDoc->parseError;
		_bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
		std::cerr << parseError << std::endl;
		clean();
		return;
	}
	_ok = true;
#elif defined(USEXERCES)&&!defined(USETINYXML)
	XMLPlatformUtils::Initialize();
	_pDOMParser = new XercesDOMParser();
	_errorHandler = (ErrorHandler *)new HandlerBase();
	_pDOMParser->setErrorHandler(_errorHandler);

	try {
		_pDOMParser->parse(_xmlFileName.c_str());
	}
	catch (const XMLException &toCatch)
	{
		char *message = XMLString::transcode(toCatch.getMessage());
		std::cerr << message << std::endl;
		XMLString::release(&message);
		clean();
		return;
	}
	catch (const DOMException &toCatch)
	{
		char *message = XMLString::transcode(toCatch.msg);
		std::cerr << message << std::endl;
		XMLString::release(&message);
		clean();
		return;
	}
	_pDoc = _pDOMParser->getDocument();
	_ok = true;
#else
    _pXMLDoc = new TiXmlDocument(_xmlFileName.c_str());
    _ok = _pXMLDoc->LoadFile();
    if (!_ok)
        clean();
    _pLayers = NULL;
    _pNeurons = NULL;
#endif
}

void NeuralNetXMLReader::networkTopology(int &numberOfInputs,int &numberOfLayers)
{
	numberOfInputs = -1;
	numberOfLayers = -1;
	if (_ok)
	{
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
		MSXML2::IXMLDOMNodePtr pNode = _pXMLDoc->selectSingleNode("/NeuralNet");
		if (pNode == NULL)
		{
			std::cerr << "Error fetching network definition node" << std::endl;
			std::cerr << _pXMLDoc->parseError->Getreason() << std::endl;
			clean();
			return;
		}
		MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = pNode->attributes;
		if (pAttributes == NULL)
		{
			std::cerr << "Error getting network attributes" << std::endl;
			std::cerr << _pXMLDoc->parseError->Getreason() << std::endl;
			clean();
			return;
		}
		else
		{
			MSXML2::IXMLDOMNodePtr pAttr = pAttributes->getNamedItem("NumberOfInputs");
			if (pAttr != NULL)
			{
				_bstr_t numtxt = pAttr->nodeValue.bstrVal;
				numberOfInputs = atoi((const char *)numtxt);
				pAttr.Release();
			}
			pAttr = pAttributes->getNamedItem("NumberOfLayers");
			if (pAttr != NULL)
			{
				_bstr_t numtxt = pAttr->nodeValue.bstrVal;
				numberOfLayers = atoi((const char *)numtxt);
				pAttr.Release();
			}
		}
		getLayerCollection();
		pAttributes.Release();
		pNode.Release();
#elif defined(USEXERCES)&&!defined(USETINYXML)
		XMLCh *netname = XMLString::transcode("NeuralNet");
		DOMNodeList *pNodes = _pDoc->getElementsByTagName(netname);
		if (pNodes != NULL)
		{
			if (pNodes->getLength()>0)
			{
				DOMNode *pNode = pNodes->item(0); // The first NeuralNet
				DOMNamedNodeMap *pAttributes = pNode->getAttributes();
				if (pAttributes != NULL)
				{
				    XMLCh *attr = XMLString::transcode("NumberOfInputs");
					const XMLCh *text = pAttributes->getNamedItem(attr)->getNodeValue();
					char *ctxt = XMLString::transcode(text);
					numberOfInputs = atoi(ctxt);
					XMLString::release(&ctxt);
					XMLString::release(&attr);

					attr = XMLString::transcode("NumberOfLayers");
					text = pAttributes->getNamedItem(attr)->getNodeValue();
					ctxt = XMLString::transcode(text);
					numberOfLayers = atoi(ctxt);
					XMLString::release(&ctxt);
					XMLString::release(&attr);
				}
				getLayerCollection();
			}
		}
		XMLString::release(&netname);
#else
        _pDocRoot = _pXMLDoc->RootElement();
        if (_pDocRoot == NULL)
        {
            clean();
            return;
        }
        _pDocRoot->Attribute("NumberOfInputs",&numberOfInputs);
        _pDocRoot->Attribute("NumberOfLayers",&numberOfLayers);
        getLayerCollection();
#endif
	}
}

void NeuralNetXMLReader::getNormalisationData(std::vector<std::string> &normaliserTypes,std::vector<std::vector<double> > &constructionData,
		                                      std::vector<double> &targetoffsets,std::vector<double> &targetranges)
{
	if (_ok)
	{
		normaliserTypes.clear();
        if ((int)constructionData.size() != 0)
            for (int i=0;i<(int)constructionData.size();++i)
                constructionData[i].clear();
		constructionData.clear();
		targetoffsets.clear();
		targetranges.clear();

#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
		MSXML2::IXMLDOMNodeListPtr pInputVals = _pXMLDoc->selectNodes("/NeuralNet/InputNormalisationData");
		if (pInputVals == NULL)
		{
			MSXML2::IXMLDOMParseErrorPtr pError;
			pError = _pXMLDoc->parseError;
			_bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
			std::cerr << parseError << std::endl;
			clean();
			return;
		}

        for (int i=0;i<pInputVals->length;++i)
        {
            MSXML2::IXMLDOMNodeListPtr pNodes = pInputVals->item[i]->childNodes;
            if (pNodes == NULL)
		    {
			    MSXML2::IXMLDOMParseErrorPtr pError;
			    pError = _pXMLDoc->parseError;
			    _bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
			    std::cerr << parseError << std::endl;
			    clean();
			    return;
		    }

            for (int j=0;j<pNodes->length;++j)
            {
                MSXML2::IXMLDOMNodePtr pNormaliser = pNodes->item[j];
                MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = pNormaliser->attributes;
                if (pAttributes != NULL)
                {
                    MSXML2::IXMLDOMNodePtr pAttr = pAttributes->getNamedItem("Type");
                    if (pAttr != NULL)
                    {
                        _bstr_t typetext = pAttr->nodeValue.bstrVal;
                        std::string normtype = (const char *)typetext;
                        normaliserTypes.push_back(normtype);
                        pAttr.Release();
                    }
                }
                MSXML2::IXMLDOMNodeListPtr pParams = pNormaliser->childNodes;
                if (pParams == NULL)
                {
			        MSXML2::IXMLDOMParseErrorPtr pError;
			        pError = _pXMLDoc->parseError;
			        _bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
			        std::cerr << parseError << std::endl;
			        clean();
			        return;
                }
                std::vector<double> nodeConstructionData;
                nodeConstructionData.clear();
                if (pParams->length != 0)
                {
                    for (int k=0;k<pParams->length;++k)
                    {
                        MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = pParams->item[k]->attributes;
                        if (pAttributes != NULL)
                        {
                            MSXML2::IXMLDOMNodePtr pAttr = pAttributes->getNamedItem("Value");
                            if (pAttr != NULL)
                            {
                                char *stopconv;
                                _bstr_t numtxt = pAttr->nodeValue.bstrVal;
                                nodeConstructionData.push_back(strtod((const char *)numtxt,&stopconv));
                                pAttr.Release();
                            }
                        }
                    }
                }
                constructionData.push_back(nodeConstructionData);
            }
        }
		pInputVals.Release();

		pInputVals = _pXMLDoc->selectNodes("/NeuralNet/TargetNormalisationData");
		if (pInputVals == NULL)
		{
			MSXML2::IXMLDOMParseErrorPtr pError;
			pError = _pXMLDoc->parseError;
			_bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
			std::cerr << parseError << std::endl;
			clean();
			return;
		}
		for (int i=0;i<pInputVals->length;++i)
		{
			MSXML2::IXMLDOMNodePtr pNode = pInputVals->item[i];
			MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = pNode->attributes;
			if (pAttributes != NULL)
			{
				MSXML2::IXMLDOMNodePtr pAttr = pAttributes->getNamedItem("Offset");
				if (pAttr != NULL)
				{
					char *stopconv;
					_bstr_t numtxt = pAttr->nodeValue.bstrVal;
					targetoffsets.push_back(strtod((const char *)numtxt,&stopconv));
					pAttr.Release();
				}
				pAttr = pAttributes->getNamedItem("Range");
				if (pAttr != NULL)
				{
					char *stopconv;
					_bstr_t numtxt = pAttr->nodeValue.bstrVal;
					targetranges.push_back(strtod((const char *)numtxt,&stopconv));
					pAttr.Release();
				}
				pAttributes.Release();
				pNode.Release();
			}
		}
		pInputVals.Release();
#elif defined(USEXERCES)&&!defined(USETINYXML)
		XMLCh *collectionName = XMLString::transcode("InputNormalisationData");
		DOMNodeList *pInputVals = _pDoc->getDocumentElement()->getElementsByTagName(collectionName);
		XMLString::release(&collectionName);
		for (int i=0;i<pInputVals->getLength();++i)
		{
            DOMNodeList *pNormalisers = pInputVals->item(i)->getChildNodes();
            for (int j=0;j<pNormalisers->getLength();++j)
            {
                DOMNode *pNormaliser = pNormalisers->item(j);
                if (pNormaliser->getNodeType() == DOMNode::ELEMENT_NODE)
                {
                    DOMNamedNodeMap *pAttributes = pNormaliser->getAttributes();
                    if (pAttributes != NULL)
                    {
                        XMLCh *attr = XMLString::transcode("Type");
                        const XMLCh *text = pAttributes->getNamedItem(attr)->getNodeValue();
                        char *ctxt = XMLString::transcode(text);
                        std::string normtype = ctxt;
                        normaliserTypes.push_back(normtype);
                        XMLString::release(&ctxt);
                        XMLString::release(&attr);
                    }
                    std::vector<double> tmpdata;
                    tmpdata.clear();
                    if (pNormaliser->hasChildNodes())
                    {
                        DOMNodeList *pParams = pNormaliser->getChildNodes();
                        for (int k=0;k<pParams->getLength();++k)
                        {
                            DOMNode *pParam = pParams->item(k);
                            if (pParam->getNodeType() == DOMNode::ELEMENT_NODE)
                            {
                                DOMNamedNodeMap *pAttributes = pParam->getAttributes();
                                if (pAttributes != NULL)
                                {
                                    XMLCh *attr = XMLString::transcode("Value");
                                    const XMLCh *text = pAttributes->getNamedItem(attr)->getNodeValue();
                                    char *ctxt = XMLString::transcode(text);
                                    char *stopconv;
                                    tmpdata.push_back(strtod(ctxt,&stopconv));
                                    XMLString::release(&ctxt);
                                    XMLString::release(&attr);
                                }
                            }
                        }
                    }
                    constructionData.push_back(tmpdata);
                }
            }
        }

		collectionName = XMLString::transcode("TargetNormalisationData");
		pInputVals = _pDoc->getDocumentElement()->getElementsByTagName(collectionName);
		XMLString::release(&collectionName);
		for (int i=0;i<pInputVals->getLength();++i)
		{
			DOMNode *pNode = pInputVals->item(i);
			DOMNamedNodeMap *pAttributes = pNode->getAttributes();
			if (pAttributes != NULL)
			{
				char *stopconv;
			    XMLCh *attr = XMLString::transcode("Offset");
				const XMLCh *text = pAttributes->getNamedItem(attr)->getNodeValue();
				char *ctxt = XMLString::transcode(text);
				targetoffsets.push_back(strtod(ctxt,&stopconv));
				XMLString::release(&ctxt);
				XMLString::release(&attr);

				attr = XMLString::transcode("Range");
				text = pAttributes->getNamedItem(attr)->getNodeValue();
				ctxt = XMLString::transcode(text);
				targetranges.push_back(strtod(ctxt,&stopconv));
				XMLString::release(&ctxt);
				XMLString::release(&attr);
			}
		}
#else
        TiXmlNode *thenode = 0;
        std::vector<double> tmpdata;
        while (thenode = _pDocRoot->IterateChildren("InputNormalisationData",thenode))
        {
            TiXmlNode *theNormaliser = thenode->FirstChild("InputNormaliser");
            tmpdata.clear();
            std::string name = theNormaliser->ToElement()->Attribute("Type");
            normaliserTypes.push_back(name);
            TiXmlNode *params = 0;
            while (params = theNormaliser->IterateChildren(params))
            {
                double p;
                params->ToElement()->Attribute("Value",&p);
                tmpdata.push_back(p);
            }
            constructionData.push_back(tmpdata);
        }
        thenode = 0;
        while (thenode = _pDocRoot->IterateChildren("TargetNormalisationData",thenode))
        {
            double offset, range;
            thenode->ToElement()->Attribute("Offset",&offset);
            thenode->ToElement()->Attribute("Range",&range);
            targetoffsets.push_back(offset);
            targetranges.push_back(range);
        }
#endif
	}
}

void NeuralNetXMLReader::getLayerCollection()
{
	if (!_ok) return;
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
	_pLayers = _pXMLDoc->selectNodes("/NeuralNet/NeuronLayer");
	if (_pLayers == NULL)
	{
		MSXML2::IXMLDOMParseErrorPtr pError;
		pError = _pXMLDoc->parseError;
		_bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
		std::cerr << parseError << std::endl;
		clean();
	}
#elif defined(USEXERCES)&&!defined(USETINYXML)
	XMLCh *layername = XMLString::transcode("NeuronLayer");
    _pLayers = _pDoc->getDocumentElement()->getElementsByTagName(layername);
	XMLString::release(&layername);
#else
    _pLayers = new std::vector<TiXmlNode *>;
    TiXmlNode *node = 0;
    while (node = _pDocRoot->IterateChildren("NeuronLayer",node))
        _pLayers->push_back(node);
#endif
}

int NeuralNetXMLReader::nextLayer()
{

	int numberOfNeurons = -1;
	if (!_ok) return -1;
	++_currentLayer;
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
	if (_currentLayer<_pLayers->length)
	{
		_currentNeuron = -1;
		MSXML2::IXMLDOMNodePtr pNode = _pLayers->item[_currentLayer];
		MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = pNode->attributes;
		if (pAttributes == NULL)
		{
			MSXML2::IXMLDOMParseErrorPtr pError;
			pError = _pXMLDoc->parseError;
			_bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
			std::cerr << parseError << std::endl;
		}
		MSXML2::IXMLDOMNodePtr pAttr = pAttributes->getNamedItem("NumberOfNeurons");
		if (pAttr != NULL)
		{
			_bstr_t numtxt = pAttr->nodeValue.bstrVal;
			numberOfNeurons = atoi((const char *)numtxt);
			pAttr.Release();
		}
		else
		{
			MSXML2::IXMLDOMParseErrorPtr pError;
			pError = _pXMLDoc->parseError;
			_bstr_t parseError = _bstr_t("At line ")+_bstr_t(pError->Getline())+_bstr_t(" : ")+_bstr_t(pError->Getreason());
			std::cerr << parseError << std::endl;
		}
		getNeuronCollection();
		pAttributes.Release();
		pNode.Release();
		return numberOfNeurons;
	}
	else
	{
		std::cerr << "Attempt to access non-existant NeuronLayer" << std::endl;
		std::cerr << "#layers found " << _pLayers->length << std::endl;
		std::cerr << "request access to layer " << _currentLayer << std::endl;
		return -1;
	}
#elif defined(USEXERCES)&&!defined(USETINYXML)
	if (_currentLayer<_pLayers->getLength())
	{
		_currentNeuron = -1;
		DOMNode *pNode = _pLayers->item(_currentLayer);
		DOMNamedNodeMap *pAttributes = pNode->getAttributes();
		if (pAttributes != NULL)
		{
		    XMLCh *attr = XMLString::transcode("NumberOfNeurons");
			const XMLCh *text = pAttributes->getNamedItem(attr)->getNodeValue();
			char *ctxt = XMLString::transcode(text);
			numberOfNeurons = atoi(ctxt);
			XMLString::release(&ctxt);
			XMLString::release(&attr);
		}
		getNeuronCollection();
		return numberOfNeurons;
	}
	else
	{
		std::cerr << "Attempt to access non-existant NeuronLayer" << std::endl;
		std::cerr << "#layers found " << _pLayers->getLength() << std::endl;
		std::cerr << "request access to layer " << _currentLayer << std::endl;
		return -1;
	}
#else
    if (_currentLayer < (int)_pLayers->size())
    {
        _currentNeuron = -1;
        (*_pLayers)[_currentLayer]->ToElement()->Attribute("NumberOfNeurons",&numberOfNeurons);
        getNeuronCollection();
        return numberOfNeurons;
    }
    else
    {
		std::cerr << "Attempt to access non-existant NeuronLayer" << std::endl;
		std::cerr << "#layers found " << (int)_pLayers->size() << std::endl;
		std::cerr << "request access to layer " << _currentLayer << std::endl;
		return -1;
    }
#endif
}

void NeuralNetXMLReader::getNeuronCollection()
{
	if (!_ok) return;
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
	_pNeurons = _pLayers->item[_currentLayer]->childNodes;
#elif defined(USEXERCES)&&!defined(USETINYXML)
	_pNeurons = _pLayers->item(_currentLayer)->getChildNodes();
#else
    if (_pNeurons != NULL)
        delete _pNeurons;
    _pNeurons = new std::vector<TiXmlNode *>;
    TiXmlNode *parent = (*_pLayers)[_currentLayer];
    TiXmlNode *thenode = 0;
    while (thenode = parent->IterateChildren("Neuron",thenode))
        _pNeurons->push_back(thenode);
#endif
}

int NeuralNetXMLReader::nextNeuron(std::string &neuronType,std::vector<double> &constructionData)
{
	if (!_ok) return -1;
	int numberOfInputs = -1;
	constructionData.clear();
	_currentNeuron++;
#if defined(_WIN32)&&!defined(USEXERCES)&&!defined(USETINYXML)
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = _pNeurons->item[_currentNeuron]->attributes;
	MSXML2::IXMLDOMNodePtr pAttr = pAttributes->getNamedItem("Type");
	if (pAttr != NULL)
	{
		_bstr_t typetxt = pAttr->nodeValue.bstrVal;
		neuronType = (const char *)typetxt;
		pAttr.Release();
	}
	pAttr = pAttributes->getNamedItem("NumberOfInputs");
	if (pAttr != NULL)
	{
		_bstr_t numtxt = pAttr->nodeValue.bstrVal;
		numberOfInputs = atoi((const char *)numtxt);
		pAttr.Release();
	}
	pAttributes.Release();

	MSXML2::IXMLDOMNodeListPtr pNeuronData = _pNeurons->item[_currentNeuron]->childNodes;
	char *stopconv;
	for (int i=0;i<pNeuronData->length;++i)
	{
		MSXML2::IXMLDOMNodePtr dataelement = pNeuronData->item[i];
		_bstr_t txtval = dataelement->text;
		constructionData.push_back(strtod((const char *)txtval,&stopconv));
		dataelement.Release();
	}
	pNeuronData.Release();
	return numberOfInputs;
#elif defined(USEXERCES)&&!defined(USETINYXML)
	DOMNode *pNode = _pNeurons->item(_currentNeuron);
	while ((pNode->getNodeType() != DOMNode::ELEMENT_NODE)&&_currentNeuron<_pNeurons->getLength())
	{
		_currentNeuron++;
		pNode = _pNeurons->item(_currentNeuron);
	}
	if (_currentNeuron<_pNeurons->getLength())
	{

	    DOMNamedNodeMap *pAttributes = pNode->getAttributes();
	    if (pAttributes != NULL)
		{
			XMLCh * attr = XMLString::transcode("NumberOfInputs");
			const XMLCh *text = pAttributes->getNamedItem(attr)->getNodeValue();
			char *ctxt = XMLString::transcode(text);
			numberOfInputs = atoi(ctxt);
			XMLString::release(&ctxt);
			XMLString::release(&attr);

			attr = XMLString::transcode("Type");
			text = pAttributes->getNamedItem(attr)->getNodeValue();
			ctxt = XMLString::transcode(text);
			neuronType = ctxt;
			XMLString::release(&ctxt);
			XMLString::release(&attr);
		}
	    DOMNodeList *pNeuronData = pNode->getChildNodes();
	    char *stopconv;
	    for (int i=0;i<pNeuronData->getLength();++i)
		{
			DOMNode *dataelement = pNeuronData->item(i);

			if (dataelement->getNodeType() == DOMNode::ELEMENT_NODE)
			{
				DOMNodeList *attr = dataelement->getChildNodes();
				if (attr != NULL)
				{
					for (int j=0;j<attr->getLength();++j)
					{
						const XMLCh *text = attr->item(j)->getNodeValue();
						char *ctxt = XMLString::transcode(text);
						constructionData.push_back(strtod(ctxt,&stopconv));
						XMLString::release(&ctxt);
					}
				}
			}
	    }
	}
	return numberOfInputs;
#else
    TiXmlNode *theNeuron = (*_pNeurons)[_currentNeuron];
    neuronType = theNeuron->ToElement()->Attribute("Type");
    theNeuron->ToElement()->Attribute("NumberOfInputs",&numberOfInputs);
    // build construction data by reading all child nodes
    TiXmlNode *theNode = 0;
    while (theNode = theNeuron->IterateChildren(theNode))
    {
        std::string thevalue = theNode->FirstChild()->ToText()->ValueStr();
        char *stopconv;
        constructionData.push_back(strtod(thevalue.c_str(),&stopconv));
    }
    return numberOfInputs;
#endif
}
#endif
