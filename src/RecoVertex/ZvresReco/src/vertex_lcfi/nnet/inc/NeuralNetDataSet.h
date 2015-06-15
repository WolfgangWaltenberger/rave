#ifndef NEURALNETDATASET_H
#define NEURALNETDATASET_H

#include "NeuralNetConfig.h"

#include <vector>
#include <utility>
#include <iostream>
#include <string>

namespace nnet
{
class NeuralNetDataSet;
}

//namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
namespace nnet
{

class
#ifndef __CINT__
NEURALNETDLL
#endif
NeuralNetDataSet
{
public:
	NeuralNetDataSet(void);
	NeuralNetDataSet(const std::string &fileName);
	NeuralNetDataSet(const char *fileName);
	~NeuralNetDataSet(void);
	void addDataItem(const std::vector<double> &inputData,const std::vector<double> &targetOutput);
	void getNormalisationData(std::vector<double> &inputNormalisationDataMeans,
		std::vector<double> &targetNormalisationDataOffsets,
		std::vector<double> &inputNormalisationDataVariances,
		std::vector<double> &targetNormalisationDataRanges,
        std::vector<double> &inputNormalisationDataOffsets,
        std::vector<double> &inputNormalisationDataRanges) const;
	void getDataItem(const int item,std::vector<double> &inputData,std::vector<double> &targetData) const;
	int numberOfDataItems() const { return (int)_theData.size(); }
	void setSerialisationPrecision(const int precision) {_outputPrecision = precision;}

protected:
	void initialiseFromFile(const std::string &fileName);

private:
	typedef std::pair<std::vector<double>,std::vector<double> > DataSetItem;

	std::vector<DataSetItem> _theData;
	std::vector<double>::size_type _inputDataSize;
	std::vector<double>::size_type _targetDataSize;
	mutable std::vector<double> runningInputSum;
	mutable std::vector<double> runningInputSumSqr;
	mutable std::vector<double> runningTargetMin;
	mutable std::vector<double> runningTargetRange;
    mutable std::vector<double> runningInputMin;
    mutable std::vector<double> runningInputRange;
	mutable bool _changed;
#ifndef __CINT__
	std::streamsize
#else
	int
#endif
		_outputPrecision;

public:
	friend
#ifndef __CINT__
		NEURALNETDLL
#endif
		std::ostream &operator<<(std::ostream &os,const NeuralNetDataSet &ds);
};

#ifndef __CINT__
NEURALNETDLL
#endif
std::ostream &operator<<(std::ostream &os,const NeuralNetDataSet &ds);

}//namespace nnet

#endif
