#ifndef GENOME_H
#define GENOME_H

#include "NeuralNetConfig.h"

#include <vector>

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
Genome
{
public:
	Genome(const int numberOfGenes,const bool setInitialRandomValues=true);
	Genome(const std::vector<double> &chromosome);
	Genome(const std::vector<double> &chromosome,const double fitness);
	~Genome(void);
	double fitness() const {return _fitness;}
	void setFitness(double fitness) {_fitness = fitness;}
	std::vector<double> chromosome() const {return _chromosome;}
	void setChromosome(const std::vector<double> &newChromosome);
	void sex(const Genome &with,Genome &baby1,Genome &baby2);
	int numberOfGenes() {return _numberOfGenes;}

protected:
	void crossover(const Genome &with,Genome &baby1,Genome &baby2);
	void mutate(Genome &theGenome);

private:
	std::vector<double> _chromosome;
	double _fitness;
	int _numberOfGenes;

public:
	friend
#ifndef __CINT__
NEURALNETDLL
#endif
		bool operator<(const Genome &lhs,const Genome &rhs)
			{ return lhs.fitness() < rhs.fitness();}
	friend
#ifndef __CINT__
NEURALNETDLL
#endif
		bool operator>(const Genome &lhs,const Genome &rhs)
			{ return lhs.fitness() > rhs.fitness();}
	friend
#ifndef __CINT__
NEURALNETDLL
#endif
		bool operator==(const Genome &lhs,const Genome &rhs)
			{ return lhs.chromosome() == rhs.chromosome();}

public:
	static double MutationRate;
	static double CrossoverRate;
	static double MaxMutationPerturbation;
};

}//namespace nnet

#endif
