#ifdef _WIN32
#define NOMINMAX
#endif
#include "NeuralNet.h"
#include "NeuronLayer.h"
#include "NeuronBuilder.h"
#include "Neuron.h"
#include "RandomNumberUtils.h"
#include "NeuronBuilderCatalogue.h"
#include "InputNormaliserBuilderCatalogue.h"
#include "InputNormaliserBuilder.h"
#include "PassthroughNormaliser.h"

#ifndef NEURALNETNOXMLREADER
#include "NeuralNetXMLReader.h"
#endif

#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <algorithm>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using nnet::NeuralNet;
using nnet::NeuronLayer;

NeuralNet::NeuralNet(const int numberOfInputs,const std::vector<int> &numberOfNeuronsPerLayer,
					 NeuronBuilder *theNeuronBuilder,bool initialiseRandomSeed)
: _numberOfInputs(numberOfInputs),
  _theNeuronBuilder(theNeuronBuilder),_serialisationPrecision(12)
{
    theNeuronBuilder->setNetwork(this);
#ifndef NEURALNETNOXMLREADER
    _serialisationMode = XML;
#else
    _serialisationMode = PlainText;
#endif
	_numberOfLayers = (int)numberOfNeuronsPerLayer.size();
	constructLayers(numberOfInputs,_numberOfLayers,numberOfNeuronsPerLayer,initialiseRandomSeed);
	for (int i=0;i<_numberOfInputs;++i)
	{
        _inputNormalisers.push_back(new PassthroughNormaliser(this));
	}
	for (int i=0;i<numberOfNeuronsPerLayer[_numberOfLayers-1];++i)
	{
		_targetNormalisationOffsets.push_back(0.0);
		_targetNormalisationRanges.push_back(1.0);
	}
}

NeuralNet::NeuralNet(const int numberOfInputs,const std::vector<std::vector<std::string> > &namedNeuronsPerLayer,
					 bool initialiseRandomSeed)
: _numberOfInputs(numberOfInputs),
  _theNeuronBuilder(NULL),_serialisationPrecision(12)
{
#ifndef NEURALNETNOXMLREADER
    _serialisationMode = XML;
#else
    _serialisationMode = PlainText;
#endif
	_numberOfLayers = (int)namedNeuronsPerLayer.size();
	constructLayers(namedNeuronsPerLayer,initialiseRandomSeed);
	for (int i=0;i<_numberOfInputs;++i)
	{
        _inputNormalisers.push_back(new PassthroughNormaliser(this));
	}
	for (int i=0;i<(int)namedNeuronsPerLayer[_numberOfLayers-1].size();++i)
	{
		_targetNormalisationOffsets.push_back(0.0);
		_targetNormalisationRanges.push_back(1.0);
	}
}

NeuralNet::NeuralNet(const NeuralNet &other)
{
	NeuralNet &theother=const_cast<NeuralNet &>(other);
	clear();
	_numberOfLayers = other.numberOfLayers();
	_numberOfInputs = other.numberOfInputs();
	_serialisationPrecision = other.getSerialisationPrecision();
	for (int i=0;i<_numberOfLayers;++i)
		_theLayers.push_back(new NeuronLayer(*(theother.layer(i)),this));
	_theNeuronBuilder = (NeuronBuilder *)0;

    std::vector<InputNormaliser *> otherNormalisers = other.inputNormalisers();
    std::vector<InputNormaliser *> myNewNormalisers;
    for (int i=0;i<(int)otherNormalisers.size();++i)
        myNewNormalisers.push_back(otherNormalisers[i]->clone(this));
    setInputNormalisers(myNewNormalisers);

    std::vector<double> othermeans = other.targetNormalisationOffsets();
	_targetNormalisationOffsets.assign(othermeans.begin(),othermeans.end());
    std::vector<double> othervariances = other.targetNormalisationRanges();
	_targetNormalisationRanges.assign(othervariances.begin(),othervariances.end());
    _serialisationMode = other.getSerialisationMode();
}

NeuralNet::NeuralNet(const std::string &xmlfile,std::vector<NeuronBuilder *> &theNeuronBuilders,const SerialisationMode readMode)
: _numberOfLayers(0),_numberOfInputs(0),_serialisationPrecision(12),_serialisationMode(readMode)
{
	buildFromUrl(xmlfile,theNeuronBuilders);
}

NeuralNet::NeuralNet(const char *xmlfile,std::vector<NeuronBuilder *> &theNeuronBuilders,const SerialisationMode readMode)
: _numberOfLayers(0),_numberOfInputs(0),_serialisationPrecision(12),_serialisationMode(readMode)
{
	std::string url = xmlfile;
	buildFromUrl(url,theNeuronBuilders);
}

NeuralNet::NeuralNet(const std::string &xmlfile,const SerialisationMode readMode)
: _numberOfLayers(0),_numberOfInputs(0),_serialisationPrecision(12),_serialisationMode(readMode)
{
	buildFromUrl(xmlfile);
}

NeuralNet::NeuralNet(const char *xmlfile,const SerialisationMode readMode)
: _numberOfLayers(0),_numberOfInputs(0),_serialisationPrecision(12),_serialisationMode(readMode)
{
	std::string url = xmlfile;
	buildFromUrl(url);
}

NeuralNet::~NeuralNet(void)
{
	clear();
}

void NeuralNet::buildFromXML(const std::string &url,const std::vector<NeuronBuilder *> &theNeuronBuilders)
{
#ifndef NEURALNETNOXMLREADER
	clear();
	_theNeuronBuilder = (NeuronBuilder *)0;
    std::vector<std::string> normaliserTypes;
    std::vector<std::vector<double> > normaliserConstructionData;

	NeuralNetXMLReader myReader(url);
	myReader.networkTopology(_numberOfInputs,_numberOfLayers);
	myReader.getNormalisationData(normaliserTypes,normaliserConstructionData,_targetNormalisationOffsets,
		_targetNormalisationRanges);

    for (int i=0;i<_numberOfInputs;++i)
    {
        InputNormaliserBuilder *theBuilder = InputNormaliserBuilderCatalogue::instance(this)->builderOf(normaliserTypes[i]);
        if (theBuilder != (InputNormaliserBuilder *)0)
        {
            _inputNormalisers.push_back(theBuilder->buildNormaliser(normaliserConstructionData[i]));
        }
		else
		{
			std::cerr << "No builder in catalogue for the requested input normaliser type!" << std::endl;
			std::cerr << "Normaliser type requested = " << normaliserTypes[i] << std::endl;
		}
    }

	if ((_numberOfInputs > 0)&&(_numberOfLayers > 0))
	{
		for (int i=0;i<_numberOfLayers;++i)
		{
			int numberOfNeuronsPerLayer = myReader.nextLayer();
			NeuronLayer *newLayer = new NeuronLayer(this);
			for (int j=0;j<numberOfNeuronsPerLayer;++j)
			{
				std::string neuronType;
				std::vector<double> constructionData;
				NeuronBuilder *theBuilder = (NeuronBuilder *)0;

				int numberOfInputs = myReader.nextNeuron(neuronType,constructionData);

				std::vector<NeuronBuilder *>::const_iterator builderIter = theNeuronBuilders.begin();
				while (builderIter != theNeuronBuilders.end())
				{
					if ( (*builderIter)->buildsType().compare(neuronType) == 0)
					{
						theBuilder = (*builderIter);
						break;
					}
					builderIter++;
				}

				if (theBuilder != (NeuronBuilder *)0)
				{
                    theBuilder->setNetwork(this);
					Neuron *newNeuron = theBuilder->buildNeuron(numberOfInputs,constructionData);
					newLayer->addNeuron(newNeuron);
				}
				else
				{
					std::cerr << "No builder supplied for the requested neuron type!" << std::endl;
					std::cerr << "Neuron type requested = " << neuronType << std::endl;
				}
			}
			_theLayers.push_back(newLayer);
		}
	}
#else
    std::cerr << "No XML support compiled in." << std::endl;
#endif
}

void NeuralNet::buildFromPlainText(const std::string &url,const std::vector<NeuronBuilder *> &theNeuronBuilders)
{
	clear();
	_theNeuronBuilder = (NeuronBuilder *)0;
    std::vector<std::string> normaliserTypes;
    std::vector<std::vector<double> > normaliserConstructionData;
	std::ifstream ifs;
	ifs.open(url.c_str(),std::ios::in);
	if (ifs.is_open())
	{
		std::string line;
		if (std::getline(ifs,line))
		{
			std::istringstream iss(line);
			iss >> _numberOfInputs >> _numberOfLayers;
		}

		for (int i=0;i<_numberOfInputs;++i)
		{
            std::getline(ifs,line);
            std::istringstream iss(line);
            std::string sdata; 

			iss >> std::ws >> sdata >> std::ws;
            std::vector<double> normaliserConstructionData;
            normaliserConstructionData.clear();
            std::string value;
            while (std::getline(iss,value,' '))
            {
			    double ddata;
                iss >> ddata;
                normaliserConstructionData.push_back(ddata);
            }
            InputNormaliserBuilder *theBuilder = InputNormaliserBuilderCatalogue::instance(this)->builderOf(sdata);
            if (theBuilder != (InputNormaliserBuilder *)0)
            {
                _inputNormalisers.push_back(theBuilder->buildNormaliser(normaliserConstructionData));
            }
		    else
		    {
			    std::cerr << "No builder in catalogue for the requested input normaliser type!" << std::endl;
			    std::cerr << "Normaliser type requested = " << normaliserTypes[i] << std::endl;
		    }
		}

		int numberOfNeurons;
		for (int i=0;i<_numberOfLayers;++i)
		{
			NeuronLayer *newLayer = new NeuronLayer(this);
			std::getline(ifs,line);
			std::istringstream iss(line);
			iss >> numberOfNeurons;

			for (int j=0;j<numberOfNeurons;++j)
			{
				std::string neuronType;
				std::vector<double> constructionData;
				double filedata;
				int numberOfInputs;
				ifs >> std::ws;

				std::getline(ifs,line);
				std::istringstream iss(line);

				iss >> neuronType >> numberOfInputs >> std::ws;

				std::string value;
				while (std::getline(iss,value,' '))
				{
					std::istringstream iss(value);
					iss >> filedata;
					constructionData.push_back(filedata);
				}

				NeuronBuilder *theBuilder = (NeuronBuilder *)0;
				std::vector<NeuronBuilder *>::const_iterator builderIter = theNeuronBuilders.begin();
				while (builderIter != theNeuronBuilders.end())
				{
					if ( (*builderIter)->buildsType().compare(neuronType) == 0)
					{
						theBuilder = (*builderIter);
						break;
					}
					builderIter++;
				}

				if (theBuilder != (NeuronBuilder *)0)
				{
                    theBuilder->setNetwork(this);
					Neuron *newNeuron = theBuilder->buildNeuron(numberOfInputs,constructionData);
					newLayer->addNeuron(newNeuron);
				}
				else
				{
					std::cerr << "No builder supplied for the requested neuron type!" << std::endl;
					std::cerr << "Neuron type requested = " << neuronType << std::endl;
				}
			}
			_theLayers.push_back(newLayer);
		}
		for (int i=0;i<numberOfNeurons;++i) // this should be the number of neurons in the output layer
		{
			double filedata;
			ifs >> std::ws >> filedata;
			_targetNormalisationOffsets.push_back(filedata);
			ifs >> std::ws >> filedata;
			_targetNormalisationRanges.push_back(filedata);
		}
		ifs.close();
	}
	else
		std::cerr << "Failed to open " << url << std::endl;
}

void NeuralNet::buildFromUrl(const std::string &url,const std::vector<NeuronBuilder *> &theNeuronBuilders)
{
    if (_serialisationMode == PlainText)
        buildFromPlainText(url,theNeuronBuilders);
    else if (_serialisationMode == XML)
        buildFromXML(url,theNeuronBuilders);
}

void NeuralNet::buildFromXML(const std::string &url)
{
#ifndef NEURALNETNOXMLREADER
	clear();
	_theNeuronBuilder = (NeuronBuilder *)0;
    std::vector<std::string> normaliserTypes;
    std::vector<std::vector<double> > normaliserConstructionData;

    NeuralNetXMLReader myReader(url);
	myReader.networkTopology(_numberOfInputs,_numberOfLayers);
	myReader.getNormalisationData(normaliserTypes,normaliserConstructionData,_targetNormalisationOffsets,
		_targetNormalisationRanges);

    for (int i=0;i<_numberOfInputs;++i)
    {
        InputNormaliserBuilder *theBuilder = InputNormaliserBuilderCatalogue::instance(this)->builderOf(normaliserTypes[i]);
        if (theBuilder != (InputNormaliserBuilder *)0)
        {
            _inputNormalisers.push_back(theBuilder->buildNormaliser(normaliserConstructionData[i]));
        }
		else
		{
			std::cerr << "No builder in catalogue for the requested input normaliser type!" << std::endl;
			std::cerr << "Normaliser type requested = " << normaliserTypes[i] << std::endl;
		}
    }

	if ((_numberOfInputs > 0)&&(_numberOfLayers > 0))
	{
		for (int i=0;i<_numberOfLayers;++i)
		{
			int numberOfNeuronsPerLayer = myReader.nextLayer();
			NeuronLayer *newLayer = new NeuronLayer(this);
			for (int j=0;j<numberOfNeuronsPerLayer;++j)
			{
				std::string neuronType;
				std::vector<double> constructionData;

				int numberOfInputs = myReader.nextNeuron(neuronType,constructionData);

				NeuronBuilder *theBuilder = NeuronBuilderCatalogue::instance()->builderOf(neuronType);

				if (theBuilder != (NeuronBuilder *)0)
				{
                    theBuilder->setNetwork(this);
					Neuron *newNeuron = theBuilder->buildNeuron(numberOfInputs,constructionData);
					newLayer->addNeuron(newNeuron);
				}
				else
				{
					std::cerr << "No builder in catalogue for the requested neuron type!" << std::endl;
					std::cerr << "Neuron type requested = " << neuronType << std::endl;
				}
			}
			_theLayers.push_back(newLayer);
		}
	}
#else
    std::cerr << "No XML support compiled in." << std::endl;
#endif
}

void NeuralNet::buildFromPlainText(const std::string &url)
{
	clear();
	_theNeuronBuilder = (NeuronBuilder *)0;
    std::vector<std::string> normaliserTypes;
    std::vector<std::vector<double> > normaliserConstructionData;
	std::ifstream ifs;
	ifs.open(url.c_str(),std::ios::in);
	if (ifs.is_open())
	{
		std::string line;
		if (std::getline(ifs,line))
		{
			std::istringstream iss(line);
			iss >> _numberOfInputs >> _numberOfLayers;
		}

		for (int i=0;i<_numberOfInputs;++i)
		{
            std::getline(ifs,line);
            std::istringstream iss(line);
            std::string sdata; 

			iss >> std::ws >> sdata >> std::ws;
            std::vector<double> normaliserConstructionData;
            normaliserConstructionData.clear();
            std::string value;
            while (std::getline(iss,value,' '))
            {
			    double ddata;
                iss >> ddata;
                normaliserConstructionData.push_back(ddata);
            }
            InputNormaliserBuilder *theBuilder = InputNormaliserBuilderCatalogue::instance(this)->builderOf(sdata);
            if (theBuilder != (InputNormaliserBuilder *)0)
            {
                _inputNormalisers.push_back(theBuilder->buildNormaliser(normaliserConstructionData));
            }
		    else
		    {
			    std::cerr << "No builder in catalogue for the requested input normaliser type!" << std::endl;
			    std::cerr << "Normaliser type requested = " << normaliserTypes[i] << std::endl;
		    }
		}

		int numberOfNeurons;
		for (int i=0;i<_numberOfLayers;++i)
		{
			NeuronLayer *newLayer = new NeuronLayer(this);
			std::getline(ifs,line);
			std::istringstream iss(line);
			iss >> numberOfNeurons;

			for (int j=0;j<numberOfNeurons;++j)
			{
				std::string neuronType;
				std::vector<double> constructionData;
				double filedata;
				int numberOfInputs;
				ifs >> std::ws;

				std::getline(ifs,line);
				std::istringstream iss(line);

				iss >> neuronType >> numberOfInputs >> std::ws;

				std::string value;
				while (std::getline(iss,value,' '))
				{
					std::istringstream iss(value);
					iss >> filedata;
					constructionData.push_back(filedata);
				}

				NeuronBuilder *theBuilder = NeuronBuilderCatalogue::instance()->builderOf(neuronType);

				if (theBuilder != (NeuronBuilder *)0)
				{
                    theBuilder->setNetwork(this);
					Neuron *newNeuron = theBuilder->buildNeuron(numberOfInputs,constructionData);
					newLayer->addNeuron(newNeuron);
				}
				else
				{
					std::cerr << "No builder in catalogue for the requested neuron type!" << std::endl;
					std::cerr << "Neuron type requested = " << neuronType << std::endl;
				}
			}
			_theLayers.push_back(newLayer);
		}
		for (int i=0;i<numberOfNeurons;++i) // this should be the number of neurons in the output layer
		{
			double filedata;
			ifs >> std::ws >> filedata;
			_targetNormalisationOffsets.push_back(filedata);
			ifs >> std::ws >> filedata;
			_targetNormalisationRanges.push_back(filedata);
		}
		ifs.close();
	}
	else
		std::cerr << "Failed to open " << url << std::endl;
}

void NeuralNet::buildFromUrl(const std::string &url)
{
    if (_serialisationMode == PlainText)
        buildFromPlainText(url);
    else if (_serialisationMode == XML)
        buildFromXML(url);
}

void NeuralNet::constructLayers(const int numberOfInputs,const int numberOfLayers,const std::vector<int> &numberOfNeuronsPerLayer,bool initialiseRandomSeed)
{
	if (numberOfLayers<(int)numberOfNeuronsPerLayer.size())
		std::cerr << "NeuralNet:: Not enough information to build network layers!" << std::endl;
	else
	{
		if (initialiseRandomSeed)
			NeuralNetRandom::SetRandomSeed(NeuralNetRandom::GetNewRandomSeed());
		for (int i=0;i<numberOfLayers;++i)
		{
			int numInputs;
			if (i==0) numInputs = numberOfInputs;
			else numInputs = numberOfNeuronsPerLayer[i-1];
			
			_theLayers.push_back(new NeuronLayer(numberOfNeuronsPerLayer[i],numInputs,*_theNeuronBuilder,this));
		}
	}
}

void NeuralNet::constructLayers(const std::vector<std::vector<std::string> > &namedLayers,bool initialiseRandomSeed)
{
	if (_numberOfLayers<(int)namedLayers.size())
		std::cerr << "NeuralNet:: Not enough information to build network layers!" << std::endl;
	else
	{
		if (initialiseRandomSeed)
			NeuralNetRandom::SetRandomSeed(NeuralNetRandom::GetNewRandomSeed());
		int numInputs = _numberOfInputs;
		for (int i=0;i<_numberOfLayers;++i)
		{
			if (i>0) numInputs = (int)namedLayers[i-1].size();
			_theLayers.push_back(new NeuronLayer(numInputs,namedLayers[i],this));
		}
	}
}

void NeuralNet::clear()
{
	if (_theLayers.size() > 0)
	{
		for (std::vector<NeuronLayer *>::iterator iter=_theLayers.begin();
			iter != _theLayers.end();++iter)
			(*iter)->destroy();
		_theLayers.clear();
	}
    if ((int)_inputNormalisers.size() != 0)
    {
        for (int i=0;i<(int)_inputNormalisers.size();++i)
        {
            if (_inputNormalisers[i] != (InputNormaliser *)0)
                delete _inputNormalisers[i];
        }
        _inputNormalisers.clear();
    }
	_targetNormalisationOffsets.clear();
	_targetNormalisationRanges.clear();
}

void NeuralNet::serialise(std::ostream &os) const
{
	std::streamsize oldPrec = os.precision();
	os.precision(_serialisationPrecision);
    if (_serialisationMode == XML)
    {
	    os << "<?xml version=\"1.0\"?>" << std::endl;
	    os << "<NeuralNet NumberOfInputs=\"" << _numberOfInputs << "\" NumberOfLayers=\"" << _numberOfLayers << "\">" << std::endl;
	    for (int i=0;i<_numberOfInputs;++i)
	    {
            os << "<InputNormalisationData>" <<std::endl;
            os << *_inputNormalisers[i] << std::endl;
            os << "</InputNormalisationData>" << std::endl;
	    }
	    for (int i=0;i<_theLayers[_numberOfLayers-1]->numberOfNeurons();++i)
	    {
		    os << "<TargetNormalisationData Offset=\"" << _targetNormalisationOffsets[i] << "\" Range=\"";
		    os << _targetNormalisationRanges[i] << "\"/>" << std::endl;
	    }
	    for (std::vector<NeuronLayer *>::const_iterator iter=_theLayers.begin();
		    iter != _theLayers.end();++iter)
		    (*iter)->serialise(os);
	    os << "</NeuralNet>" << std::endl;
    }
    else if (_serialisationMode == PlainText)
    {
	    os << _numberOfInputs << " " << _numberOfLayers << std::endl;
	    for (int i=0;i<_numberOfInputs;++i)
	    {
            os << *_inputNormalisers[i] << std::endl;
	    }
	    for (std::vector<NeuronLayer *>::const_iterator iter=_theLayers.begin();
		    iter != _theLayers.end();++iter)
		    (*iter)->serialise(os);
	    for (int i=0;i<_theLayers[_numberOfLayers-1]->numberOfNeurons();++i)
	    {
		    os << _targetNormalisationOffsets[i] << " ";
		    os << _targetNormalisationRanges[i] << std::endl;
	    }
    }
	os.precision(oldPrec);
}

std::vector<double> NeuralNet::output(const std::vector<double> &inputValues) const
{
	std::vector<double> inputs;

	if ((int)inputValues.size() < _numberOfInputs)
	{
		std::cerr << "NeuralNet:: Too few input values to evaluate result." << std::endl;
	}
	else
	{
		// normalise inputs
		for (int i=0;i<(int)inputValues.size();++i)
        {
            inputs.push_back(_inputNormalisers[i]->normalisedValue(inputValues[i]));
        }

		for (std::vector<NeuronLayer *>::const_iterator iter=_theLayers.begin();
				iter != _theLayers.end();++iter)
			inputs = (*iter)->output(inputs);
	}

	// Scale outputs
	for (int i=0;i<(int)inputs.size();++i)
		inputs[i] = (inputs[i]*_targetNormalisationRanges[i])+_targetNormalisationOffsets[i];
	return inputs;
}

int NeuralNet::numberOfWeights() const
{
	int totalWeights = 0;
	for (std::vector<NeuronLayer *>::const_iterator iter=_theLayers.begin();
			iter != _theLayers.end();++iter)
		totalWeights += (*iter)->numberOfWeights();
	return totalWeights;
}

std::vector<double> NeuralNet::weights() const
{
	std::vector<double> theWeights;
	for (int i=0;i<_numberOfLayers;++i)
	{
		std::vector<double> layerWeights = _theLayers[i]->weights();
		theWeights.insert(theWeights.end(),layerWeights.begin(),layerWeights.end());
	}
	return theWeights;
}

void NeuralNet::setWeights(const std::vector<double> &newWeights)
{
	if ((int)newWeights.size()<numberOfWeights())
		std::cerr << "NeuralNet:: Too few weights supplied to initialize network." << std::endl;
	else
	{
		std::vector<double> newLayerWeights;
		std::vector<double>::const_iterator iter = newWeights.begin();
		for (int i=0;i<_numberOfLayers;++i)
		{
			int weightsPerLayer = _theLayers[i]->numberOfWeights();
			newLayerWeights.assign(iter,iter+weightsPerLayer);
			_theLayers[i]->setWeights(newLayerWeights);
			iter+=weightsPerLayer;
		}
	}
}

NeuronLayer *NeuralNet::layer(const int i) const
{
	if ((i>=0)&&(i<(int)_theLayers.size()))
	{
		return _theLayers[i];
	}
	else
		throw std::out_of_range("Attempt to access non-existant network layer.");
}

void NeuralNet::setTargetNormalisationOffsets(const std::vector<double> &offsets)
{
	if ((int)offsets.size() == _theLayers[_numberOfLayers-1]->numberOfNeurons())
	{
		_targetNormalisationOffsets.assign(offsets.begin(),offsets.end());
	}
}

void NeuralNet::setTargetNormalisationRanges(const std::vector<double> &ranges)
{
	if ((int)ranges.size() == _theLayers[_numberOfLayers-1]->numberOfNeurons())
	{
		_targetNormalisationRanges.assign(ranges.begin(),ranges.end());
	}
}

std::vector<std::pair<double,double> > NeuralNet::networkOutputRange() const
{
	std::vector<std::pair<double,double> > outputdata;
	NeuronLayer *outputLayer = _theLayers[_numberOfLayers-1];
	for (int i=0;i<outputLayer->numberOfNeurons();++i)
	{
		double nmin,nmax;
		outputLayer->neuron(i)->outputRange(nmin,nmax);
		outputdata.push_back(std::make_pair<double,double>(nmin,nmax));
	}
	return outputdata;
}

void NeuralNet::setInputNormalisers(const std::vector<InputNormaliser *> &theNormalisers)
{
    if (theNormalisers.size() == _numberOfInputs)
    {
        if ((int)_inputNormalisers.size() != 0)
        {
            for (int i=0;i<(int)_inputNormalisers.size();++i)
            {
                if (_inputNormalisers[i] != (InputNormaliser *)0)
                    delete _inputNormalisers[i];
            }
            _inputNormalisers.clear();
        }
        for (int i=0;i<_numberOfInputs;++i)
            _inputNormalisers.push_back(theNormalisers[i]);
    }
    else
    {
        std::cerr << "NeuralNet:: Too few input normalisers supplied. No change" << std::endl;
    }
}

// Non-member output operator

NEURALNETDLL std::ostream &operator<<(std::ostream &os,const NeuralNet &nn)
{
	nn.serialise(os);
	return os;
}
