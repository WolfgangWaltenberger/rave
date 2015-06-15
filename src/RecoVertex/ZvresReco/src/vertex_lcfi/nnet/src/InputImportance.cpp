#include "InputImportance.h"

#include "Neuron.h"
#include "NeuronLayer.h"
#include "InputNormaliser.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <string>

//using namespace nnet for the LCFI vertex package
using namespace nnet;


namespace NeuralNetUtils {
template <typename T>
class Square
{
public:
	T operator()(const T &elem) const
	{
		return elem*elem;
	}
};
}

std::vector<double> InputImportance::operator()(const NeuralNet &theNet,const NeuralNetDataSet &theData) const
{
	NeuronLayer *firstHiddenLayer = theNet.layer(0);
	const int numberOfNeurons = firstHiddenLayer->numberOfNeurons();

	//std::cout << "# neurons in hidden layer = " << numberOfNeurons << std::endl;

	if (numberOfNeurons > 0)
	{
		const int numberOfInputs = firstHiddenLayer->neuron(0)->numberOfWeights();
		std::vector<double> weightsSquared(numberOfInputs,0.0);

		for (int i=0;i<numberOfNeurons;++i)
		{
			//std::cout << "Getting Neuron " << i << std::endl;
			std::vector<double> theWeights = firstHiddenLayer->neuron(i)->weights();
			std::transform(theWeights.begin(),theWeights.end(),theWeights.begin(),
				NeuralNetUtils::Square<double>());
			std::transform(theWeights.begin(),theWeights.end(),weightsSquared.begin(),
				weightsSquared.begin(),std::plus<double>());
		}

		std::vector<InputNormaliser *> inputNormalisers = theNet.inputNormalisers();

		//std::cout << "Got input normalisers" << std::endl;

		std::vector<double> inputNormalisationDataMeans;
		std::vector<double> targetNormalisationDataOffsets;
		std::vector<double> inputNormalisationDataVariances;
		std::vector<double> targetNormalisationDataRanges;
        std::vector<double> inputNormalisationDataOffsets;
        std::vector<double> inputNormalisationDataRanges;

		theData.getNormalisationData(inputNormalisationDataMeans,
			targetNormalisationDataOffsets,inputNormalisationDataVariances,
			targetNormalisationDataRanges,inputNormalisationDataOffsets,
			inputNormalisationDataRanges);

		//std::cout << "Got dataset info" << std::endl;

		int i;
		std::vector<InputNormaliser *>::iterator iter;
		for ( iter = inputNormalisers.begin(),i=0; iter != inputNormalisers.end(); ++iter,++i)
		{
			std::string theName = (*iter)->name();
			//std::cout << theName << " " << i << std::endl;
			// Deal with cases where the data is manipulated before the neurons see it.
			// Gaussian normalised data will have a mean of zero in the net, so set the data mean to 1 to get a
			// meaningful answer.
			// Range normalised data will also have a different effective mean in the network, so adjust accordingly.
			if (theName == "GaussianNormaliser") inputNormalisationDataMeans[i]=1.0;
			else if (theName == "RangeMappingNormaliser")
			{
				// Do this on a case by cae basis in case the normalisers are mapping to different ranges
				InputNormaliser &theNormaliser = *(*iter);
				double c = theNormaliser.normalisedValue(inputNormalisationDataOffsets[i]);
				double d;
				if (inputNormalisationDataOffsets[i] != 0.0)
				{
					d = c-((theNormaliser.normalisedValue(0.0)-c)*inputNormalisationDataRanges[i])/inputNormalisationDataOffsets[i];
				}
				else
				{
					d = ((theNormaliser.normalisedValue(1.0)-c)*inputNormalisationDataRanges[i])+c;
				}
				inputNormalisationDataMeans[i] = (((d-c)/inputNormalisationDataRanges[i])*
					(inputNormalisationDataMeans[i]-inputNormalisationDataOffsets[i]))+c;
			}
		}

		std::transform(inputNormalisationDataMeans.begin(),inputNormalisationDataMeans.end(),
			inputNormalisationDataMeans.begin(),NeuralNetUtils::Square<double>());

		//std::cout << "Squared the means" << std::endl;

		std::vector<double> importances(theNet.numberOfInputs(),0.0);

		//std::cout << "weights size " << weightsSquared.size() << std::endl;
		//std::cout << "data means size " << inputNormalisationDataMeans.size() << std::endl;

		if (inputNormalisationDataMeans.size() >= weightsSquared.size()-1)
		{
			std::transform(weightsSquared.begin(),weightsSquared.end()-1,inputNormalisationDataMeans.begin(),
				importances.begin(),std::multiplies<double>());
		}
		else
		{
			std::cerr << "InputImportance:: Mismatch between data and neurons." <<
				std::endl << "Data has too few variables. Some values may be meaningless." << std::endl;
			int theDataSize = static_cast<int>(inputNormalisationDataMeans.size());
			std::transform(weightsSquared.begin(),weightsSquared.begin()+theDataSize,inputNormalisationDataMeans.begin(),
				importances.begin(),std::multiplies<double>());
		}

		//std::cout << "returning: Size of importances is " << importances.size() << std::endl;

		return importances;
	}
	return std::vector<double>();
}
