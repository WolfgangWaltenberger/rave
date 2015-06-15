#include "RangeMappingNormaliser.h"
#include "NeuralNet.h"

//using namespace nnet added 15/08/06 by Mark Grimes (mark.grimes@bristol.ac.uk) for the LCFI vertex package
using namespace nnet;

RangeMappingNormaliser::RangeMappingNormaliser(const double inputRangeMin,const double inputRangeMax,
                                const double outputRangeMin,const double outputRangeMax,const NeuralNet *parentNetwork)
                                : InputNormaliser(parentNetwork),_inputMin(inputRangeMin),_outputMin(outputRangeMin)
{
    if (inputRangeMax < inputRangeMin)
    {
        std::cerr << "RangeMappingNormaliser:: input max < min! "
            << " Swapping min and max." << std::endl;
        _inputMin = inputRangeMax;
        _inputRange = inputRangeMin-inputRangeMax;
    }
    else
        _inputRange = inputRangeMax-inputRangeMin;
        
    if (outputRangeMax < outputRangeMin)
    {
        std::cerr << "RangeMappingNormaliser:: output max < min! "
            << " Swapping min and max." << std::endl;
        _outputMin = outputRangeMax;
        _outputRange = outputRangeMin-outputRangeMax;
    }
    else
        _outputRange = outputRangeMax-outputRangeMin;

    if (_inputRange == 0.0)
    {
        std::cerr << "RangeMappingNormaliser:: input range 0! Setting to 1.0" << std::endl;
        _inputRange = 1.0;
    }
    if (_outputRange == 0.0)
    {
        std::cerr << "RangeMappingNormaliser:: output range 0! Setting to 1.0" << std::endl;
        _outputRange = 1.0;
    }
}

RangeMappingNormaliser::~RangeMappingNormaliser(void)
{
}

double RangeMappingNormaliser::normalisedValue(const double input) const
{
    return (((input-_inputMin)/_inputRange)*_outputRange)+_outputMin;
}

void RangeMappingNormaliser::serialise(std::ostream &os) const
{
    if (_parentNetwork != (NeuralNet *)0)
    {
        NeuralNet::SerialisationMode mode = _parentNetwork->getSerialisationMode();
        if (mode == NeuralNet::XML)
        {
            os << "<InputNormaliser Type=\"" << name() << "\">" << std::endl;
            os << "<parameter Value=\"" << _inputMin << "\"/>" << std::endl;
            os << "<parameter Value=\"" << _inputRange << "\"/>" << std::endl;
            os << "<parameter Value=\"" << _outputMin << "\"/>" << std::endl;
            os << "<parameter Value=\"" << _outputRange << "\"/>" << std::endl;
            os << "</InputNormaliser>";
        }
        else if (mode == NeuralNet::PlainText)
        {
            os << name() << " " << _inputMin << " " << _inputRange << " "
                << _outputMin << " " << _outputRange;
        }
    }
}

InputNormaliser *RangeMappingNormaliser::clone(const NeuralNet *newNetwork) const
{
    return new RangeMappingNormaliser(_inputMin,_inputMin+_inputRange,
                                      _outputMin,_outputMin+_outputRange,
                                      newNetwork);
}
