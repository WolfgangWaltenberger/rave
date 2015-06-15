#include <vector>
#include <utility>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

template<typename T>
class NNFInterfaceManager
{
public:
	~NNFInterfaceManager();
	static NNFInterfaceManager *instance();
	void remove(const int ID);
	int add(T *item);
	T *item(const int ID);

protected:
	NNFInterfaceManager() {}

private:
	static NNFInterfaceManager<T> *_instance;
	std::vector<std::pair<int,T*> > _managedItems;
};

template<typename T>
NNFInterfaceManager<T> *NNFInterfaceManager<T>::_instance = (NNFInterfaceManager<T> *)0;

template<typename T>
NNFInterfaceManager<T> *NNFInterfaceManager<T>::instance()
{
	if (_instance == (NNFInterfaceManager<T> *)0)
		_instance = new NNFInterfaceManager<T>;
	return _instance;
}

template<typename T>
NNFInterfaceManager<T>::~NNFInterfaceManager()
{
	if (_managedItems.size() > 0)
	{
		for (typename std::vector<std::pair<int,T *> >::iterator iter=_managedItems.begin();
			iter != _managedItems.end();++iter)
		{
			delete (*iter).second;
		}
		_managedItems.clear();
	}
}

template<typename T>
int NNFInterfaceManager<T>::add(T *item)
{
	int newid = 0;
	for (typename std::vector<std::pair<int,T *> >::iterator iter=_managedItems.begin();
			iter != _managedItems.end();++iter)
	{
		if ( (*iter).first > newid) newid= (*iter).first;
	}
	newid++;

#if (defined(__GNUC__) && (__GNUC__ < 3))
	std::pair<int,T *> tmpitem(newid,item);
	_managedItems.push_back(tmpitem);
#else
	_managedItems.push_back(std::make_pair<int,T *>(newid,item));
#endif

	return newid;
}

template<typename T>
T *NNFInterfaceManager<T>::item(const int ID)
{
	T *nn = (T *)0;
	for (typename std::vector<std::pair<int,T *> >::iterator iter=_managedItems.begin();
			iter != _managedItems.end();++iter)
	{
		if ( (*iter).first == ID)
		{
			nn= (*iter).second;
			break;
		}
	}
	return nn;
}

template<typename T>
void NNFInterfaceManager<T>::remove(const int ID)
{
	T *nn = (T *)0;
	for (typename std::vector<std::pair<int,T *> >::iterator iter=_managedItems.begin();
			iter != _managedItems.end();++iter)
	{
		if ( (*iter).first == ID)
		{
			nn= (*iter).second;
			if (nn != (T *)0)
				delete nn;
			_managedItems.erase(iter);
			break;
		}
	}
}

}//namespace nnet

#include <iostream>
#include <string>
#include <fstream>
#include <valarray>

#include "NeuralNetConfig.h"
#include "NeuralNet.h"
#include "BackPropagationAlgorithm.h"
#include "GeneticAlgorithm.h"
#include "BatchBackPropagationAlgorithm.h"
#include "NeuralNetDataSet.h"
#include "BackPropagationCGAlgorithm.h"
#include "SigmoidNeuronBuilder.h"
#include "LinearNeuronBuilder.h"
#include "TanSigmoidNeuronBuilder.h"
#include "PassthroughNormaliser.h"
#include "GaussianNormaliser.h"
#include "RangeMappingNormaliser.h"

#ifndef _WIN32
#define LOADNEURALNETFROMFILE loadneuralnetfromfile_
#define DELETENEURALNET deleteneuralnet_
#define SAVENEURALNET saveneuralnet_
#define NEURALNETOUTPUT neuralnetoutput_
#define CREATESIGMOIDNN createsigmoidnn_
#define CREATELINEARNN createlinearnn_
#define CREATETANSIGMOIDNN createtansigmoidnn_
#define CREATEBACKPROPALG createbackpropalg_
#define CREATEGENETICALG creategeneticalg_
#define TRAINWITHBACKPROP trainwithbackprop_
#define EVALPOPULATIONFITNESS evalpopulationfitness_
#define BATCHTRAIN batchtrain_
#define NEWGENERATION newgeneration_
#define DELETEGENALG deletegenalg_
#define DELETEBACKPROP deletebackprop_
#define SETELITEGENOMEREPCOUNT setelitegenomerepcount_
#define SETNUMELITEGENOMES setnumelitegenomes_
#define CREATEBATCHBACKPROPALG createbatchbackpropalg_
#define DELETEBATCHBACKPROPALG deletebatchbackpropalg_
#define TRAINWITHBATCHBACKPROP trainwithbatchbackprop_
#define CREATEDATASET createdataset_
#define LOADDATASET loaddataset_
#define SAVEDATASET savedataset_
#define DELETEDATASET deletedataset_
#define ADDTODATASET addtodataset_
#define CREATECGALG createcgalg_
#define DELETECGALG deletecgalg_
#define TRAINWITHCGALG trainwithcgalg_
#define GETCGEPOCHERRORS getcgepocherrors_
#define GETBATCHBACKPROPEPOCHERRORS getbatchbackpropepocherrors_
#define SETCGPRINTOUTFREQUENCY setcgprintoutfrequency_
#define SETBATCHBACKPROPPRINTOUTFREQUENCY setbatchbackpropprintoutfrequency_
#define SETGENALGPRINTOUTFREQUENCY setgenalgprintoutfrequency_
#define CREATEPASSTHROUGHNORMALISER createpassthroughnormaliser_
#define CREATEGAUSSIANNORMALISER creategaussiannormaliser_
#define CREATERANGEMAPPINGNORMALISER createrangemappingnormaliser_
#define SETNNINPUTNORMALISERS setnninputnormalisers_
#define BATCHTRAINSETNORMALISERS batchtrainsetnormalisers_
#define TRAINWITHCGALGSETNORMALISERS trainwithcgalgsetnormalisers_
#define TRAINWITHBATCHBACKPROPSETNORMALISERS trainwithbatchbackpropsetnormalisers_
#define SETSERIALISATIONMODE setserialisationmode_
#endif

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

extern "C" {

NEURALNETDLL int LOADNEURALNETFROMFILE(const char *filename,const int *format,const int length)
{
	std::string fn(filename,length);
    const NeuralNet::SerialisationMode mode = static_cast<const NeuralNet::SerialisationMode>(*format);
	NeuralNet *nn = new NeuralNet(fn,mode);
	return NNFInterfaceManager<NeuralNet>::instance()->add(nn);
}

NEURALNETDLL void DELETENEURALNET(const int *nn)
{
	NNFInterfaceManager<NeuralNet>::instance()->remove(*nn);
}

NEURALNETDLL void SAVENEURALNET(const int *nn,const char *filename,const int length)
{
	std::string fn(filename,length);
	std::ofstream ofile;
#if (defined(__GNUC__) && (__GNUC__ < 3))
	ofile.open(fn.c_str(),std::ios::out|std::ios::trunc);
#else
	ofile.open(fn.c_str(),std::ios_base::out|std::ios_base::trunc);
#endif
	NNFInterfaceManager<NeuralNet>::instance()->item(*nn)->serialise(ofile);
	ofile.close();
}

NEURALNETDLL void NEURALNETOUTPUT(const int *nn,const int *inputlen,const double *input,
								  int *outputlen,double *outputs)
{
	std::vector<double> inputs;
	std::vector<double> output;
	for (int i=0;i<*inputlen;++i)
		inputs.push_back(*(input+i));
	NeuralNet *thenet = NNFInterfaceManager<NeuralNet>::instance()->item(*nn);
	if (thenet != (NeuralNet *)0)
	{
		output = thenet->output(inputs);
		int ol = *outputlen;
		ol = std::min(ol,(int)output.size());
		for (int i=0;i<ol;++i)
			*(outputs+i) = output[i];
		*outputlen = ol;
	}
}

NEURALNETDLL int CREATESIGMOIDNN(const int *ninputs,const int *nlayers,const int *nodesperlayer)
{
	NeuronBuilder *theBuilder = new SigmoidNeuronBuilder();
	std::vector<int> nodesPerLayer;
	for (int i=0;i<*nlayers;++i)
	{
		nodesPerLayer.push_back(*(nodesperlayer+i));
	}
	NeuralNet *nn = new NeuralNet(*ninputs,nodesPerLayer,theBuilder);
	return NNFInterfaceManager<NeuralNet>::instance()->add(nn);
	delete theBuilder;
}

NEURALNETDLL int CREATELINEARNN(const int *ninputs,const int *nlayers,const int *nodesperlayer)
{
	NeuronBuilder *theBuilder = new LinearNeuronBuilder();
	std::vector<int> nodesPerLayer;
	for (int i=0;i<*nlayers;++i)
	{
		nodesPerLayer.push_back(*(nodesperlayer+i));
	}
	NeuralNet *nn = new NeuralNet(*ninputs,nodesPerLayer,theBuilder);
	return NNFInterfaceManager<NeuralNet>::instance()->add(nn);
	delete theBuilder;
}

NEURALNETDLL int CREATETANSIGMOIDNN(const int *ninputs,const int *nlayers,const int *nodesperlayer)
{
	NeuronBuilder *theBuilder = new TanSigmoidNeuronBuilder();
	std::vector<int> nodesPerLayer;
	for (int i=0;i<*nlayers;++i)
	{
		nodesPerLayer.push_back(*(nodesperlayer+i));
	}
	NeuralNet *nn = new NeuralNet(*ninputs,nodesPerLayer,theBuilder);
	return NNFInterfaceManager<NeuralNet>::instance()->add(nn);
	delete theBuilder;
}

NEURALNETDLL int CREATEBACKPROPALG(const int *nn,const double *learningrate,const double *momentumconstant)
{
	NeuralNet *nnp = NNFInterfaceManager<NeuralNet>::instance()->item(*nn);
	if (nnp != (NeuralNet *)0)
	{
		BackPropagationAlgorithm *bpa = new BackPropagationAlgorithm(*nnp,*learningrate,*momentumconstant);
		return NNFInterfaceManager<BackPropagationAlgorithm>::instance()->add(bpa);
	}
	else
		return -1;
}

NEURALNETDLL int CREATEGENETICALG(const int *nn,const int *populationsize,const double *mutationrate,const double *crossoverrate)
{
	NeuralNet *nnp = NNFInterfaceManager<NeuralNet>::instance()->item(*nn);
	if (nnp != (NeuralNet *)0)
	{
		GeneticAlgorithm *ga = new GeneticAlgorithm(*nnp,*populationsize,*mutationrate,*crossoverrate);
		return NNFInterfaceManager<GeneticAlgorithm>::instance()->add(ga);
	}
	else
		return -1;
}

NEURALNETDLL double TRAINWITHBACKPROP(const int *ibp,const int *ninputs,const double *inputs,const int *ntargets,const double *targets)
{
	BackPropagationAlgorithm *thebp = NNFInterfaceManager<BackPropagationAlgorithm>::instance()->item(*ibp);
	if (thebp != (BackPropagationAlgorithm *)0)
	{
		std::vector<double> in;
		std::vector<double> aim;
		for (int i=0;i< *ninputs;++i)
			in.push_back( *(inputs+i));
		for (int i=0;i< *ntargets;++i)
			aim.push_back( *(targets+i));
		return thebp->train(in,aim);
	}
	else
		return -1.0;
}

NEURALNETDLL void EVALPOPULATIONFITNESS(const int *iga,const int *ninputs,const double *inputs,const int *ntargets,const double *targets)
{
	GeneticAlgorithm *thega = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*iga);
	if (thega != (GeneticAlgorithm *)0)
	{
		std::vector<double> in;
		std::vector<double> aim;
		for (int i=0;i< *ninputs;++i)
			in.push_back( *(inputs+i));
		for (int i=0;i< *ntargets;++i)
			aim.push_back( *(targets+i));

		thega->evaluatePopulationFitness(in,aim);
	}
}

NEURALNETDLL void NEWGENERATION(const int *iga)
{
	GeneticAlgorithm *thega = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*iga);
	if (thega != (GeneticAlgorithm *)0)
		thega->newGeneration();
}

NEURALNETDLL void BATCHTRAIN(const int *iga,const int *iset,const int *nepochs,const int *normalise)
{
	GeneticAlgorithm *thega = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*iga);
	NeuralNetDataSet *ds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*iset);
	if ((thega != (GeneticAlgorithm *)0)&&(ds != (NeuralNetDataSet *)0))
	{
        thega->batchTrain(*nepochs,*ds,(NeuralNet::InputNormalisationSelect)(*normalise));
	}
}

NEURALNETDLL void DELETEGENALG(const int *iga)
{
	NNFInterfaceManager<GeneticAlgorithm>::instance()->remove(*iga);
}

NEURALNETDLL void DELETEBACKPROP(const int *ibp)
{
	NNFInterfaceManager<BackPropagationAlgorithm>::instance()->remove(*ibp);
}

NEURALNETDLL void SETNUMELITEGENOMES(const int *iga,const int *nelite)
{
	GeneticAlgorithm *thega = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*iga);
	if (thega != (GeneticAlgorithm *)0)
		thega->setNumberOfEliteGenomes(*nelite);
}

NEURALNETDLL void SETELITEGENOMEREPCOUNT(const int *iga,const int *count)
{
	GeneticAlgorithm *thega = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*iga);
	if (thega != (GeneticAlgorithm *)0)
		thega->setEliteGenomeReplicationCount(*count);
}

NEURALNETDLL int CREATEBATCHBACKPROPALG(const int *nn,const double *learningrate,const double *momentumconstant)
{
	NeuralNet *nnp = NNFInterfaceManager<NeuralNet>::instance()->item(*nn);
	if (nnp != (NeuralNet *)0)
	{
		BatchBackPropagationAlgorithm *bpa = new BatchBackPropagationAlgorithm(*nnp,*learningrate,*momentumconstant);
		return NNFInterfaceManager<BatchBackPropagationAlgorithm>::instance()->add(bpa);
	}
	else
		return -1;
}

NEURALNETDLL void DELETEBATCHBACKPROPALG(const int *i)
{
	NNFInterfaceManager<BatchBackPropagationAlgorithm>::instance()->remove(*i);
}

NEURALNETDLL double TRAINWITHBATCHBACKPROP(const int *ibp,const int *dataset,const int *epochs,const int *normalise)
{
	BatchBackPropagationAlgorithm *thebbp = NNFInterfaceManager<BatchBackPropagationAlgorithm>::instance()->item(*ibp);
	NeuralNetDataSet *theds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*dataset);
	if ((thebbp != (BatchBackPropagationAlgorithm *)0)&&(theds != (NeuralNetDataSet *)0))
	{
		return thebbp->train(*epochs,*theds,(NeuralNet::InputNormalisationSelect)(*normalise));
	}
	else
		return -1.0;
}

NEURALNETDLL int CREATEDATASET(void)
{
	return NNFInterfaceManager<NeuralNetDataSet>::instance()->add(new NeuralNetDataSet);
}

NEURALNETDLL int LOADDATASET(const char *fname,const int flen)
{
	std::string filename(fname,flen);
	return NNFInterfaceManager<NeuralNetDataSet>::instance()->add(new NeuralNetDataSet(filename));
}

NEURALNETDLL void SAVEDATASET(const int *ids,const char *fname,const int flen)
{
	NeuralNetDataSet *ds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*ids);
	if (ds != (NeuralNetDataSet *)0)
	{
		std::string filename(fname,flen);
		std::ofstream ofs;
#if (defined(__GNUC__) && (__GNUC__ < 3))
		ofs.open(filename.c_str(),std::ios::out | std::ios::trunc);
#else
		ofs.open(filename.c_str(),std::ios_base::out | std::ios_base::trunc);
#endif
		if (ofs.is_open())
		{
			ofs << (*ds);
			ofs.close();
		}
	}
}

NEURALNETDLL void DELETEDATASET(const int* i)
{
	NNFInterfaceManager<NeuralNetDataSet>::instance()->remove(*i);
}

NEURALNETDLL void ADDTODATASET(const int *iset, const int *dlength,const double *data,const int *tlength,const double *tdata)
{
	NeuralNetDataSet *theds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*iset);
	if (theds != (NeuralNetDataSet *)0)
	{
		std::vector<double> vdata;
		std::vector<double> vtdata;
		for (int i=0;i< *dlength;++i)
			vdata.push_back( *(data+i));
		for (int i=0;i< *tlength;++i)
			vtdata.push_back( *(tdata+i));
		theds->addDataItem(vdata,vtdata);
	}
}

NEURALNETDLL int CREATECGALG(const int *inet)
{
	NeuralNet *nn = NNFInterfaceManager<NeuralNet>::instance()->item(*inet);
	if (nn != (NeuralNet *)0)
	{
		BackPropagationCGAlgorithm *cg = new BackPropagationCGAlgorithm(*nn);
		return NNFInterfaceManager<BackPropagationCGAlgorithm>::instance()->add(cg);
	}
	else
		return -1;
}

NEURALNETDLL void DELETECGALG(const int *ialg)
{
	NNFInterfaceManager<BackPropagationCGAlgorithm>::instance()->remove(*ialg);
}

NEURALNETDLL double TRAINWITHCGALG(const int *ialg,const int *iset,const int *epochs,const int *normalise)
{
	BackPropagationCGAlgorithm *cg = NNFInterfaceManager<BackPropagationCGAlgorithm>::instance()->item(*ialg);
	NeuralNetDataSet *ds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*iset);
	if ((cg != (BackPropagationCGAlgorithm *)0)&&(ds != (NeuralNetDataSet *)0))
		return cg->train(*epochs,*ds,(NeuralNet::InputNormalisationSelect)(*normalise));
	else
		return -1.0;
}

NEURALNETDLL void GETCGEPOCHERRORS(const int *ialg,int *nEntries,double *results)
{
	BackPropagationCGAlgorithm *cg = NNFInterfaceManager<BackPropagationCGAlgorithm>::instance()->item(*ialg);
	if (cg != (BackPropagationCGAlgorithm *)0)
	{
		std::vector<double> theErrors = cg->getTrainingErrorValuesPerEpoch();
		int entriesToCopy = std::min((int)theErrors.size(),*nEntries);
		for (int i=0;i<entriesToCopy;++i)
			*(results+i) = theErrors[i];
		*nEntries = entriesToCopy;
	}
}

NEURALNETDLL void GETBATCHBACKPROPEPOCHERRORS(const int *ialg,int *nEntries,double *results)
{
	BatchBackPropagationAlgorithm *bbp = NNFInterfaceManager<BatchBackPropagationAlgorithm>::instance()->item(*ialg);
	if (bbp != (BatchBackPropagationAlgorithm *)0)
	{
		std::vector<double> theErrors = bbp->getTrainingErrorValuesPerEpoch();
		int entriesToCopy = std::min((int)theErrors.size(),*nEntries);
		for (int i=0;i<entriesToCopy;++i)
			*(results+i) = theErrors[i];
		*nEntries = entriesToCopy;
	}
}

NEURALNETDLL void SETCGPRINTOUTFREQUENCY(const int *ialg,const int *frequency)
{
	BackPropagationCGAlgorithm *cg = NNFInterfaceManager<BackPropagationCGAlgorithm>::instance()->item(*ialg);
	if (cg != (BackPropagationCGAlgorithm *)0)
		cg->setProgressPrintoutFrequency(*frequency);
}

NEURALNETDLL void SETBATCHBACKPROPPRINTOUTFREQUENCY(const int *ialg,const int *frequency)
{
	BatchBackPropagationAlgorithm *bbp = NNFInterfaceManager<BatchBackPropagationAlgorithm>::instance()->item(*ialg);
	if (bbp != (BatchBackPropagationAlgorithm *)0)
		bbp->setProgressPrintoutFrequency(*frequency);
}

NEURALNETDLL void SETGENALGPRINTOUTFREQUENCY(const int *ialg,const int *frequency)
{
	GeneticAlgorithm *ga = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*ialg);
	if (ga != (GeneticAlgorithm *)0)
		ga->setProgressPrintoutFrequency(*frequency);
}

NEURALNETDLL int CREATEPASSTHROUGHNORMALISER(const int *nnID)
{
    NeuralNet *theNet = NNFInterfaceManager<NeuralNet>::instance()->item(*nnID);
    return NNFInterfaceManager<InputNormaliser>::instance()->add(new PassthroughNormaliser(theNet));
}

NEURALNETDLL int CREATEGAUSSIANNORMALISER(const int *nnID,const double *mean,const double *variance)
{
    NeuralNet *theNet = NNFInterfaceManager<NeuralNet>::instance()->item(*nnID);
    return NNFInterfaceManager<InputNormaliser>::instance()->add(new GaussianNormaliser(*mean,*variance,theNet));
}

NEURALNETDLL int CREATERANGEMAPPINGNORMALISER(const int *nnID,
                                              const double *inputmin,const double *inputmax,
                                              const double *outputmin,const double *outputmax)
{
    NeuralNet *theNet = NNFInterfaceManager<NeuralNet>::instance()->item(*nnID);
    return NNFInterfaceManager<InputNormaliser>::instance()->add(new RangeMappingNormaliser(*inputmin,*inputmax,
                                                                        *outputmin,*outputmax,theNet));
}

NEURALNETDLL void SETNNINPUTNORMALISERS(const int *nnID,const int *nNormalisers,const int *normaliserIDs)
{
    NeuralNet *theNet = NNFInterfaceManager<NeuralNet>::instance()->item(*nnID);
    std::vector<InputNormaliser *> theNormalisers;
    for (int i=0;i<*nNormalisers;++i)
        theNormalisers.push_back(NNFInterfaceManager<InputNormaliser>::instance()->item(*(normaliserIDs+i)));
    theNet->setInputNormalisers(theNormalisers);
}

NEURALNETDLL void BATCHTRAINSETNORMALISERS(const int *iga,const int *iset,const int *nepochs,const int *nNormalisers,const int *normalisers)
{
	GeneticAlgorithm *thega = NNFInterfaceManager<GeneticAlgorithm>::instance()->item(*iga);
	NeuralNetDataSet *ds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*iset);
    std::vector<InputNormaliser *> theNormalisers;
    for (int i=0;i<*nNormalisers;++i)
        theNormalisers.push_back(NNFInterfaceManager<InputNormaliser>::instance()->item(*(normalisers+i)));
	if ((thega != (GeneticAlgorithm *)0)&&(ds != (NeuralNetDataSet *)0))
	{
        thega->batchTrain(*nepochs,*ds,theNormalisers);
	}
}

NEURALNETDLL double TRAINWITHCGALGSETNORMALISERS(const int *ialg,const int *iset,const int *epochs,const int *nNormalisers,const int *normalisers)
{
	BackPropagationCGAlgorithm *cg = NNFInterfaceManager<BackPropagationCGAlgorithm>::instance()->item(*ialg);
	NeuralNetDataSet *ds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*iset);
    std::vector<InputNormaliser *> theNormalisers;
    for (int i=0;i<*nNormalisers;++i)
        theNormalisers.push_back(NNFInterfaceManager<InputNormaliser>::instance()->item(*(normalisers+i)));
	if ((cg != (BackPropagationCGAlgorithm *)0)&&(ds != (NeuralNetDataSet *)0))
		return cg->train(*epochs,*ds,theNormalisers);
	else
		return -1.0;
}

NEURALNETDLL double TRAINWITHBATCHBACKPROPSETNORMALISERS(const int *ibp,const int *dataset,const int *epochs,const int *nNormalisers,const int *normalisers)
{
	BatchBackPropagationAlgorithm *thebbp = NNFInterfaceManager<BatchBackPropagationAlgorithm>::instance()->item(*ibp);
	NeuralNetDataSet *theds = NNFInterfaceManager<NeuralNetDataSet>::instance()->item(*dataset);
    std::vector<InputNormaliser *> theNormalisers;
    for (int i=0;i<*nNormalisers;++i)
        theNormalisers.push_back(NNFInterfaceManager<InputNormaliser>::instance()->item(*(normalisers+i)));
	if ((thebbp != (BatchBackPropagationAlgorithm *)0)&&(theds != (NeuralNetDataSet *)0))
	{
		return thebbp->train(*epochs,*theds,theNormalisers);
	}
	else
		return -1.0;
}

NEURALNETDLL void SETSERIALISATIONMODE(const int *nn,const int *mode)
{
    const NeuralNet::SerialisationMode theMode = static_cast<const NeuralNet::SerialisationMode>(*mode);
    NNFInterfaceManager<NeuralNet>::instance()->item(*nn)->setSerialisationMode(theMode);
}

}
