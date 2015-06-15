#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "NeuralNetConfig.h"

#include "NeuralNet.h"
#include "NeuralNetDataSet.h"

#ifdef __CINT__
#include "Genome.h"
#include "InputNormaliser.h"
#else
//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{
class Genome;
class InputNormaliser;
}
#endif

#include <vector>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
GeneticAlgorithm
{
public:
	GeneticAlgorithm(NeuralNet &theNetwork,const int populationSize,const double mutationRate=0.1,
		const double crossoverRate=0.7);
	~GeneticAlgorithm(void);
	void setNumberOfEliteGenomes(const int n)
	{ _numberOfEliteCrossGenerationGenomes = n;}
	void setEliteGenomeReplicationCount(const int count);
	void newGeneration();
	void evaluatePopulationFitness(const std::vector<double> &inputValues,
		const std::vector<double> &desiredOutput);
	void batchTrain(const int numberOfEpochs,const NeuralNetDataSet &dataSet,
        const NeuralNet::InputNormalisationSelect normaliseTrainingData=NeuralNet::PassthroughNormalised);
    void batchTrain(const int numberOfEpochs,const NeuralNetDataSet &dataSet,const std::vector<InputNormaliser *> &inputNormalisers);
	void setProgressPrintoutFrequency(const int frequency) {_progressPrintoutFrequency = frequency;}
    void setMaximumGenomeFitness(const double fitness) {_maxGenomeFitness = fitness;}
	std::vector<double> getTrainingErrorValuesPerEpoch() const {return _savedEpochErrorValues;}

protected:
    void trainWithDataSet(const int numberOfEpochs,const NeuralNetDataSet &dataSet);
	void pickBest();
	void exterminate();
	double totalPopulationFitness();
	int pickGenomeByRoulette();
	void addEliteGenomesToNextGeneration(std::vector<Genome *> &nextGen);
	virtual double calculateGenomeFitness(const double error);
	void processDataSet(const NeuralNetDataSet &dataSet);
	double error(const NeuralNetDataSet &dataSet) const;


private:
	NeuralNet &_theNetwork;
	std::vector<Genome *> _thePopulation;
	int _populationSize;
	int _numberOfEvaluations;

	// Parameters for elitism.
	// Allow this many of the best genomes a free ride
	// into the next generation.
	int _numberOfEliteCrossGenerationGenomes;

	// Allow this many copies of them
	int _numberOfCopiesOfEliteGenomes;

	int _progressPrintoutFrequency;
    double _maxGenomeFitness;
    std::vector<double> _savedEpochErrorValues;
};

}//namespace nnet

#endif
