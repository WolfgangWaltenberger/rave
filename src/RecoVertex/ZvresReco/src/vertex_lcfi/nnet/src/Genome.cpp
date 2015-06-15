#include "Genome.h"
#include "RandomNumberUtils.h"

#include <algorithm>
#include <iostream>
#include <cstdlib>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

double Genome::MutationRate = 0.1;
double Genome::CrossoverRate = 0.7;
double Genome::MaxMutationPerturbation = 0.3;

Genome::Genome(const int numberOfGenes,const bool setInitialRandomValues)
: _fitness(0.0),_numberOfGenes(numberOfGenes)
{
	_chromosome = std::vector<double>(numberOfGenes);
	if (setInitialRandomValues)
		std::generate(_chromosome.begin(),_chromosome.end(),NeuralNetRandom::RandomClamped);
}

Genome::Genome(const std::vector<double> &chromosome)
: _fitness(0.0)
{
	_chromosome = chromosome;
	_numberOfGenes = (int)_chromosome.size();
}

Genome::Genome(const std::vector<double> &chromosome,const double fitness)
: _fitness(fitness)
{
	_chromosome = chromosome;
	_numberOfGenes = (int)_chromosome.size();
}

Genome::~Genome(void)
{
}

void Genome::setChromosome(const std::vector<double> &newChromosome)
{
	if ((int)newChromosome.size()<_numberOfGenes)
	{
		std::cerr << "Genome::setChromosome - Too few genes in new chromosome." << std::endl;
		return;
	}
	else
	{
		_chromosome.assign(newChromosome.begin(),newChromosome.begin()+_numberOfGenes);
	}
}

void Genome::sex(const Genome &with,Genome &baby1,Genome &baby2)
{
	if (_chromosome.size() != with.chromosome().size())
	{
		std::cerr << "Genome::sex - incompatible chromosomes!" << std::endl;
		return;
	}
	if ((with==*this)||(NeuralNetRandom::RandomFloat()>CrossoverRate))
	{
		baby1 = *this;
		baby2 = with;
	}
	else
	{
		crossover(with,baby1,baby2);
	}
	mutate(baby1);
	mutate(baby2);
}

void Genome::crossover(const Genome &with,Genome &baby1,Genome &baby2)
{
	baby1.setFitness(0.0);
	baby2.setFitness(0.0);

	int crossoverPoint = NeuralNetRandom::RandomInt(0,numberOfGenes()-1);

	std::vector<double> newChromosome1;
	std::vector<double> newChromosome2;

	std::vector<double>::iterator myGenes = _chromosome.begin();
	std::vector<double> otherChromosome = with.chromosome();
	std::vector<double>::iterator otherGenes = otherChromosome.begin();

	if (crossoverPoint > 0)
	{
		newChromosome1.assign(myGenes,myGenes+crossoverPoint+1);
		newChromosome2.assign(otherGenes,otherGenes+crossoverPoint+1);
	}
	newChromosome1.insert(newChromosome1.end(),otherGenes+crossoverPoint,otherChromosome.end());
	newChromosome2.insert(newChromosome2.end(),myGenes+crossoverPoint,_chromosome.end());

	baby1.setChromosome(newChromosome1);
	baby2.setChromosome(newChromosome2);
}

void Genome::mutate(Genome &theGenome)
{
	std::vector<double> chromosome = theGenome.chromosome();
	for (int i=0;i<(int)chromosome.size();++i)
	{
		if (NeuralNetRandom::RandomFloat() < MutationRate)
			chromosome[i] += NeuralNetRandom::RandomClamped()*MaxMutationPerturbation;
	}
	theGenome.setChromosome(chromosome);
}
