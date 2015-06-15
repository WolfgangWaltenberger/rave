#ifdef _WIN32
#define NOMINMAX
#endif
#include "GeneticAlgorithm.h"
#include "Genome.h"
#include "RandomNumberUtils.h"
#include "InputNormaliserBuilder.h"
#include "InputNormaliserBuilderCatalogue.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

namespace NeuralNetUtils {
bool GenAlgSortGenomePointers(Genome *ele1,Genome *ele2)
{
	return (*ele1)>(*ele2);
}

double GenAlgDiff(double ele1,double ele2)
{
	return ((ele1-ele2)*(ele1-ele2));
}
}

GeneticAlgorithm::GeneticAlgorithm(NeuralNet &theNetwork,const int populationSize,const double mutationRate,
								   const double crossoverRate)
								   : _theNetwork(theNetwork),_populationSize(populationSize),
								     _numberOfEliteCrossGenerationGenomes(4),_numberOfCopiesOfEliteGenomes(1),
									 _numberOfEvaluations(0),_progressPrintoutFrequency(100),
                                     _maxGenomeFitness(1.0E11)
{
	Genome::MutationRate = mutationRate;
	Genome::CrossoverRate = crossoverRate;

	// population must be an even number
	if (_populationSize < _numberOfEliteCrossGenerationGenomes)
		_populationSize = (_numberOfEliteCrossGenerationGenomes/2)*2+2; // Allow extra space for sex!
	if (_populationSize%2 != 0) _populationSize++;

	// Create an initial population. Take the current networks genome in case it has already been trained
	// and add a random set of others.
	_thePopulation.push_back(new Genome(_theNetwork.weights()));
	for (int i=0;i<_populationSize-1;++i)
		_thePopulation.push_back(new Genome(theNetwork.numberOfWeights()));
}

GeneticAlgorithm::~GeneticAlgorithm(void)
{
	exterminate();
}

void GeneticAlgorithm::setEliteGenomeReplicationCount(const int count)
{
	if ((count*_numberOfEliteCrossGenerationGenomes)%2 == 0)
		_numberOfCopiesOfEliteGenomes = count;
	else
		_numberOfCopiesOfEliteGenomes = count+1;
}

void GeneticAlgorithm::newGeneration()
{
	std::vector<Genome *> newPopulation;

	// Normalise the fitness parameters
	if (_numberOfEvaluations!=0)
		for (int i=0;i<_populationSize;++i)
		{
			_thePopulation[i]->setFitness(calculateGenomeFitness(_thePopulation[i]->fitness()/(double)_numberOfEvaluations));
		}

	// Sort the population in descending order of fitness. Best is first.
	// Best defined by largest (positive) fitness value.
	pickBest();

	// Allow addition of elite genomes from the current generation
	addEliteGenomesToNextGeneration(newPopulation);

	while ((int)newPopulation.size()<=_populationSize)
	{
		Genome *baby1 = new Genome(_theNetwork.numberOfWeights(),false);
		Genome *baby2 = new Genome(_theNetwork.numberOfWeights(),false);

		_thePopulation[pickGenomeByRoulette()]->sex(*(_thePopulation[pickGenomeByRoulette()]),
			*baby1,*baby2);

		newPopulation.push_back(baby1);
		newPopulation.push_back(baby2);
	}
	exterminate();
	_thePopulation = newPopulation;
	_numberOfEvaluations = 0;
}

double GeneticAlgorithm::calculateGenomeFitness(const double error)
{
	if (error>1.0/_maxGenomeFitness)
		return 1.0/error;
	else
		return _maxGenomeFitness;
}


void GeneticAlgorithm::evaluatePopulationFitness(const std::vector<double> &inputValues,
												 const std::vector<double> &desiredOutput)
{
	_numberOfEvaluations++;
	for (int i=0;i<_populationSize;++i)
	{
		_theNetwork.setWeights(_thePopulation[i]->chromosome());
		std::vector<double> netOutput = _theNetwork.output(inputValues);
		std::vector<double> differences(netOutput.size());

		std::transform(netOutput.begin(),netOutput.end(),
						desiredOutput.begin(),differences.begin(),
						NeuralNetUtils::GenAlgDiff);

		double msd = std::accumulate(differences.begin(),differences.end(),0.0);
		
		if (_numberOfEvaluations == 1)
			_thePopulation[i]->setFitness(msd);
		else
			_thePopulation[i]->setFitness(_thePopulation[i]->fitness()+msd);
	}
}

void GeneticAlgorithm::pickBest()
{
	std::sort(_thePopulation.begin(),_thePopulation.end(),NeuralNetUtils::GenAlgSortGenomePointers);
	_theNetwork.setWeights(_thePopulation[0]->chromosome());
}

void GeneticAlgorithm::batchTrain(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
                                  const NeuralNet::InputNormalisationSelect normaliseTrainingData)
{
	std::vector<double> inputmean;
	std::vector<double> targetoffset;
	std::vector<double> inputvariance;
	std::vector<double> targetrange;
    std::vector<double> inputoffset;
    std::vector<double> inputrange;
	dataSet.getNormalisationData(inputmean,targetoffset,inputvariance,targetrange,inputoffset,inputrange);

    if (normaliseTrainingData==NeuralNet::GaussianNormalised)
	{

		if (((int)inputmean.size() != _theNetwork.numberOfInputs())||((int)inputvariance.size() != _theNetwork.numberOfInputs()))
			std::cerr << "Normalisation error: input size mismatch" << std::endl;

        InputNormaliserBuilder *theBuilder = InputNormaliserBuilderCatalogue::instance(&_theNetwork)->builderOf("GaussianNormaliser");
        std::vector<InputNormaliser *> theNormalisers;
        for (int i=0;i<_theNetwork.numberOfInputs();++i)
        {
            std::vector<double> constructionData;
            constructionData.clear();
            constructionData.push_back(inputmean[i]);
            constructionData.push_back(inputvariance[i]);
            theNormalisers.push_back(theBuilder->buildNormaliser(constructionData));
        }
        _theNetwork.setInputNormalisers(theNormalisers);
	}

	std::vector<std::pair<double,double> > netoutranges = _theNetwork.networkOutputRange();
	std::vector<double> netoffsets;
	std::vector<double> netranges;
	for (int i=0;i<(int)targetoffset.size();++i)
	{
		double targetRange = netoutranges[i].second-netoutranges[i].first;
		netoffsets.push_back( -((targetrange[i]/targetRange)*netoutranges[i].first+targetoffset[i]));
		netranges.push_back( targetrange[i]/targetRange );
	}
	_theNetwork.setTargetNormalisationOffsets(netoffsets);
	_theNetwork.setTargetNormalisationRanges(netranges);

    trainWithDataSet(numberOfEpochs,dataSet);
}

void GeneticAlgorithm::batchTrain(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
                                  const std::vector<InputNormaliser *> &inputNormalisers)
{
    _theNetwork.setInputNormalisers(inputNormalisers);

    std::vector<double> inputmean;
	std::vector<double> targetoffset;
	std::vector<double> inputvariance;
	std::vector<double> targetrange;
    std::vector<double> inputoffset;
    std::vector<double> inputrange;
	dataSet.getNormalisationData(inputmean,targetoffset,inputvariance,targetrange,inputoffset,inputrange);

	std::vector<std::pair<double,double> > netoutranges = _theNetwork.networkOutputRange();
	std::vector<double> netoffsets;
	std::vector<double> netranges;

	for (int i=0;i<(int)targetoffset.size();++i)
	{
		double targetRange = netoutranges[i].second-netoutranges[i].first;
		netoffsets.push_back( -((targetrange[i]/targetRange)*netoutranges[i].first+targetoffset[i]));
		netranges.push_back( targetrange[i]/targetRange );
	}
	_theNetwork.setTargetNormalisationOffsets(netoffsets);
	_theNetwork.setTargetNormalisationRanges(netranges);

    trainWithDataSet(numberOfEpochs,dataSet);
}

void GeneticAlgorithm::trainWithDataSet(const int numberOfEpochs,const NeuralNetDataSet &dataSet)
{
    _savedEpochErrorValues.clear();
	for (int epoch=0;epoch<numberOfEpochs;++epoch)
	{
		processDataSet(dataSet);
		newGeneration();
        double epochError = error(dataSet);
        _savedEpochErrorValues.push_back(epochError);
		if (_progressPrintoutFrequency > 0)
		{
			if (epoch%_progressPrintoutFrequency == 0)
			{
				std::cout << "Epoch " << epoch << "/" << numberOfEpochs << " : Error function " << epochError << std::endl;
			}
		}
	}
    std::cout << "Final error at end of training cycle : "
        << error(dataSet) << std::endl;
}

void GeneticAlgorithm::processDataSet(const NeuralNetDataSet &dataSet)
{
	std::vector<double> inputs;
	std::vector<double> target;
	for (int i=0;i<dataSet.numberOfDataItems();++i)
	{
		dataSet.getDataItem(i,inputs,target);
		evaluatePopulationFitness(inputs,target);
	}
}

double GeneticAlgorithm::error(const NeuralNetDataSet &dataSet) const
{
	std::vector<double> inputs;
	std::vector<double> target;
	std::vector<double> output;
	double error = 0.0;
	for (int i=0;i<dataSet.numberOfDataItems();++i)
	{
		dataSet.getDataItem(i,inputs,target);
		std::vector<double> output = _theNetwork.output(inputs);
		std::vector<double> differences(output.size());
		std::transform(output.begin(),output.end(),target.begin(),differences.begin(),
			NeuralNetUtils::GenAlgDiff);
		double totalSqError = std::accumulate(differences.begin(),differences.end(),0.0);
		error+=totalSqError;
	}
	return error/(2.0*(double)dataSet.numberOfDataItems());
}

void GeneticAlgorithm::exterminate()
{
	for (int i=0;i<_populationSize;++i)
		delete _thePopulation[i];
	_thePopulation.clear();
}

double GeneticAlgorithm::totalPopulationFitness()
{
	double total=0.0;
	for (int i=0;i<_populationSize;++i)
		total+=(_thePopulation[i])->fitness();
	return total;
}

int GeneticAlgorithm::pickGenomeByRoulette()
{
	double fitnessSoFar = 0.0;
	double cutoff = NeuralNetRandom::RandomFloat()*totalPopulationFitness();
	int i;
	for (i=0;i<_populationSize;++i)
	{
		fitnessSoFar += (_thePopulation[i])->fitness();
		if (fitnessSoFar >= cutoff)
		{
			break;
		}
	}
	return i;
}

void GeneticAlgorithm::addEliteGenomesToNextGeneration(std::vector<Genome *> &nextGen)
{
	if (_numberOfEliteCrossGenerationGenomes == 0) return;
	std::vector<Genome *> theLuckyOnes;
	for (int i=0;i<_numberOfEliteCrossGenerationGenomes;++i)
	{
		theLuckyOnes.push_back(new Genome(*(_thePopulation[i])));
		Genome &lastOne = *theLuckyOnes.back();
		lastOne.setFitness(0.0);
	}

	for (int i=0;i<_numberOfCopiesOfEliteGenomes;++i)
		nextGen.insert(nextGen.begin(),theLuckyOnes.begin(),
			theLuckyOnes.end());
}

