#ifdef _WIN32
#define NOMINMAX
#endif
#include "NeuralNetDataSet.h"

#include <valarray>
#include <algorithm>
#include <fstream>
#include <sstream>

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using nnet::NeuralNetDataSet;

NeuralNetDataSet::NeuralNetDataSet(void)
: _inputDataSize(0),_targetDataSize(0),_changed(true),_outputPrecision(12)
{
}

NeuralNetDataSet::NeuralNetDataSet(const std::string &fileName)
: _inputDataSize(0),_targetDataSize(0),_changed(true),_outputPrecision(12)
{
	initialiseFromFile(fileName);
}

NeuralNetDataSet::NeuralNetDataSet(const char *fileName)
: _inputDataSize(0),_targetDataSize(0),_changed(true),_outputPrecision(12)
{
	const std::string file = fileName;
	initialiseFromFile(file);
}

void NeuralNetDataSet::initialiseFromFile(const std::string &fileName)
{
	std::ifstream ifs;
	ifs.open(fileName.c_str(),std::ios::in);
	if (ifs.is_open())
	{
		int numberOfDataItems = 0;
		std::string line;
		if (std::getline(ifs,line))
		{
			std::istringstream iss(line);
			iss >> numberOfDataItems >> std::ws;
		}
		if (std::getline(ifs,line))
		{
			std::istringstream iss(line);
			iss >> _inputDataSize >> std::ws >> _targetDataSize >> std::ws;
		}
		for (int i=0;i<numberOfDataItems;++i)
		{
			std::vector<double> inputData;
			std::vector<double> targetData;
			if (std::getline(ifs,line))
			{
				std::istringstream iss(line);
				double dataItem;
				for (int j=0;j<(int)_inputDataSize;++j)
				{
					iss >> dataItem >> std::ws;
					inputData.push_back(dataItem);
				}
				for (int j=0;j<(int)_targetDataSize;++j)
				{
					iss >> dataItem >> std::ws;
					targetData.push_back(dataItem);
				}
				_theData.push_back(std::make_pair<std::vector<double>,std::vector<double> >(inputData,targetData));
			}
		}
	}
	else
		std::cerr << "NeuralNetDataSet:: Failed to open " << fileName << std::endl;
}

NeuralNetDataSet::~NeuralNetDataSet(void)
{
}

void NeuralNetDataSet::addDataItem(const std::vector<double> &inputData,const std::vector<double> &targetOutput)
{
	if ((int)_theData.size() == 0)
	{
		_inputDataSize = inputData.size();
		_targetDataSize = targetOutput.size();
	}

	if (inputData.size() != _inputDataSize)
	{
		std::cerr << "Size mismatch for input data. Item not added." << std::endl;
		return;
	}
	if (targetOutput.size() != _targetDataSize)
	{
		std::cerr << "Size mismatch for target output data. Item not added." << std::endl;
		return;
	}
	_theData.push_back(std::make_pair<std::vector<double>,std::vector<double> >(inputData,targetOutput));
	_changed = true;
}

void NeuralNetDataSet::getNormalisationData(std::vector<double> &inputNormalisationDataMeans,
		std::vector<double> &targetNormalisationDataOffsets,
		std::vector<double> &inputNormalisationDataVariances,
		std::vector<double> &targetNormalisationDataRanges,
        std::vector<double> &inputNormalisationDataOffsets,
        std::vector<double> &inputNormalisationDataRanges) const
{
	if (_changed)
	{
		runningInputSum.assign(_inputDataSize,0.0);
		runningInputSumSqr.assign(_inputDataSize,0.0);
		runningTargetMin.assign(_targetDataSize,0.0);
		runningTargetRange.assign(_targetDataSize,0.0);
        runningInputMin.assign(_inputDataSize,0.0);
        runningInputRange.assign(_inputDataSize,0.0);

		for (int i=0;i<(int)_theData.size();++i)
		{
			for (int j=0;j<(int)_theData[i].first.size();++j)
			{
				runningInputSum[j] += _theData[i].first[j];
				runningInputSumSqr[j] += (_theData[i].first[j])*(_theData[i].first[j]);
                runningInputMin[j] = std::min(runningInputMin[j],_theData[i].first[j]);
                runningInputRange[j] = std::max(runningInputRange[j],_theData[i].first[j]);
			}
			for (int j=0;j<(int)_theData[i].second.size();++j)
			{
				runningTargetMin[j] = std::min(runningTargetMin[j],_theData[i].second[j]);
				runningTargetRange[j] = std::max(runningTargetRange[j],_theData[i].second[j]);
			}
		}
		for (int j=0;j<(int)runningTargetMin.size();++j)
		{
            runningInputRange[j] -= runningInputMin[j];
			runningTargetRange[j] -= runningTargetMin[j];
		}

		double nDataItems = (double)_theData.size();
		for (int i=0;i<(int)runningInputSum.size();++i)
		{
			runningInputSum[i] /= nDataItems;
			runningInputSumSqr[i] = (runningInputSumSqr[i]/nDataItems)-(runningInputSum[i]*runningInputSum[i]);
		}
		_changed = false;
	}
	inputNormalisationDataMeans.assign(runningInputSum.begin(),runningInputSum.end());
	targetNormalisationDataOffsets.assign(runningTargetMin.begin(),runningTargetMin.end());
	inputNormalisationDataVariances.assign(runningInputSumSqr.begin(),runningInputSumSqr.end());
	targetNormalisationDataRanges.assign(runningTargetRange.begin(),runningTargetRange.end());
    inputNormalisationDataOffsets.assign(runningInputMin.begin(),runningInputMin.end());
    inputNormalisationDataRanges.assign(runningInputRange.begin(),runningInputRange.end());
}

void NeuralNetDataSet::getDataItem(const int item,std::vector<double> &inputData,std::vector<double> &targetData) const
{
	if ((item>=0)&&(item<(int)_theData.size()))
	{
		inputData.assign(_theData[item].first.begin(),_theData[item].first.end());
		targetData.assign(_theData[item].second.begin(),_theData[item].second.end());
	}
	else
	{
		inputData.clear();
		targetData.clear();
	}
}

NEURALNETDLL std::ostream &nnet::operator<<(std::ostream &os,const NeuralNetDataSet &ds)
{
	std::streamsize oldPrec = os.precision();
	os.precision(ds._outputPrecision);
	os << ds.numberOfDataItems() << std::endl;
	os << ds._inputDataSize << " " << ds._targetDataSize << std::endl;
	for (int i=0;i<ds.numberOfDataItems();++i)
	{
		std::vector<double> data(ds._inputDataSize);
		std::vector<double> target(ds._targetDataSize);
		ds.getDataItem(i,data,target);
		for (std::vector<double>::const_iterator iter=data.begin();iter!=data.end();++iter)
			os << (*iter) << " ";
		for (std::vector<double>::const_iterator iter=target.begin();iter!=target.end();++iter)
			os << (*iter) << " ";
		os << std::endl;
	}
	os.precision(oldPrec);
	return os;
}

