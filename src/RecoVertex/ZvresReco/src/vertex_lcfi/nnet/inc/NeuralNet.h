#ifndef NEURALNET_H
#define NEURALNET_H

#include "NeuralNetConfig.h"

#include <vector>
#include <iostream>
#include <string>
#include <utility>

#ifdef __CINT__
#include "NeuronLayer.h"
#include "NeuronBuilder.h"
#include "InputNormaliser.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
//! Neural Network Namespace
namespace nnet
{
class NeuronLayer;
class NeuronBuilder;
class InputNormaliser;
}
#endif

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
NeuralNet
{
public:
    typedef enum {PassthroughNormalised,GaussianNormalised} InputNormalisationSelect;
    typedef enum {XML,PlainText} SerialisationMode;

public:
	NeuralNet(const int numberOfInputs,const std::vector<int> &numberOfNeuronsPerLayer,NeuronBuilder *theNeuronBuilder,bool initialiseRandomSeed=true);
	NeuralNet(const int numberOfInputs,const std::vector<std::vector<std::string> > &namedNeuronsPerLayer,bool initialiseRandomSeed=true);
	NeuralNet(const NeuralNet &other);
	NeuralNet(const std::string &xmlfile,std::vector<NeuronBuilder *> &theNeuronBuilders,const SerialisationMode readMode=XML);
	NeuralNet(const char *xmlfile,std::vector<NeuronBuilder *> &theNeuronBuilders,const SerialisationMode readMode=XML);
	NeuralNet(const std::string &xmlfile,const SerialisationMode readMode=XML);
	NeuralNet(const char *xmlfile,const SerialisationMode readMode=XML);
	~NeuralNet(void);
	void serialise(std::ostream &os) const;
	std::vector<double> output(const std::vector<double> &inputValues) const;
	int numberOfWeights() const;
	int numberOfLayers() const {return _numberOfLayers;}
	int numberOfInputs() const {return _numberOfInputs;}
	std::vector<double> weights() const;
	void setWeights(const std::vector<double> &newWeights);
	NeuronLayer *layer(const int i) const;
	void setTargetNormalisationOffsets(const std::vector<double> &offsets);
	void setTargetNormalisationRanges(const std::vector<double> &ranges);
	std::vector<double> targetNormalisationOffsets() const {return _targetNormalisationOffsets;}
	std::vector<double> targetNormalisationRanges() const {return _targetNormalisationRanges;}
	std::vector<std::pair<double,double> > networkOutputRange() const;
	void setSerialisationPrecision(const int precision) {_serialisationPrecision = precision;}
	int getSerialisationPrecision() const {return _serialisationPrecision;}
    void setInputNormalisers(const std::vector<InputNormaliser *> &theNormalisers);
    std::vector<InputNormaliser *> inputNormalisers() const {return _inputNormalisers;}
    SerialisationMode getSerialisationMode() const {return _serialisationMode;}
    void setSerialisationMode(const SerialisationMode &mode) {_serialisationMode = mode;}

protected:
	void constructLayers(const int numberOfInputs,const int numberOfLayers,const std::vector<int> &numberOfNeuronsPerLayer,bool initialiseRandomSeed);
	void constructLayers(const std::vector<std::vector<std::string> > &namedLayers,bool initialiseRandomSeed);
	void clear();
	void buildFromUrl(const std::string &url,const std::vector<NeuronBuilder *> &theNeuronBuilders);
	void buildFromUrl(const std::string &url);
    void buildFromXML(const std::string &url,const std::vector<NeuronBuilder *> &theNeuronBuilders);
    void buildFromXML(const std::string &url);
    void buildFromPlainText(const std::string &url,const std::vector<NeuronBuilder *> &theNeuronBuilders);
    void buildFromPlainText(const std::string &url);

private:
	int _numberOfLayers;
	int _numberOfInputs;
	std::vector<NeuronLayer *> _theLayers;
	const NeuronBuilder *_theNeuronBuilder;
	std::vector<double> _targetNormalisationOffsets;
	std::vector<double> _targetNormalisationRanges;
    std::vector<InputNormaliser *> _inputNormalisers;
    SerialisationMode _serialisationMode;
#ifdef __CINT__
	int
#else
	std::streamsize
#endif
		_serialisationPrecision;
};

// Non member stream output operator
#ifndef __CINT__
NEURALNETDLL
#endif
std::ostream &operator<<(std::ostream &os,const NeuralNet &nn);

}//namespace nnet

#endif
