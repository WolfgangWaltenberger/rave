/*! \page NeuralNet Neural Net Package
\section Acknowledgements 
All code in this neural net package was written by David Bailey of the University of Manchester.

\section AssumedKnowledge Assumed knowledge
- Standard Template Library (STL) vectors.

\section Remarks
- All neural net classes are in the namespace <tt>nnet</tt>.
- This is by no means a complete guide to every feature available, at present at least.

\section BasicPrinciplesOfAnArtificialNeuralNetwork Basic principles of an artificial neural network
This is a very basic introduction to the principles of a neural network (geared specifically at the way this package works).  If you have any experience with neural networks you can safely skip this section.<br>
<br>
Neurons are created to accept an arbitrary number of inputs, and based on these provide a single output value.  The output is given by the neurons \f$ threshold function \f$, which can be any given function of the neurons \f$ activation value \f$ (see the \ref NeuronDescriptions for the functions actually provided with this package).<br>
The activation value is given by multiplying each input by a pre calculated \f$ weight \f$ depending on how important that input is, and summing these results.  Each neuron can also be given a bias, depending on how important that neuron is to the network, but more on that later.<br>
Calculating these weights is the important part, and is what differentiates a well performing network from a bad one.  This process is known as \f$ training \f$, and is performed by a training algorithm (see \ref TrainingTheNetwork for the algorithms provided here).  Basically, you provide the training algorithm with a set of data that you know the answers to (the result you would want the network to give you), and it changes the weights to give the best possible results for all the elements in the data set.<br>
<br>
As a basic example, imagine a network composed of a single neuron that tells you if a food is bad for you or not.  Say it is set up with three inputs, fibre content, fat content and colour.  For simplicity, lets give the neuron a linear threshold function, so just a function that multiplies the activation value by a set constant, say \f$ k \f$. The output of the network would be
\f[
output=f(activation value)=k\times (fibrecontent\times weight_{fibre}+fatcontent\times weight_{fat}+colour\times weight_{colour})
\f]
The network is useless until the values of the weights are adjusted so that they give an accurate output.  To do this, a large database of foods is required where the properties of colour, fibre and fat content are known, as well as some reliable value as to how healthy the food is.  The training algorithm then modifies the weights to try and get the best match of the output to the expected value for each food in the database.  When somebody comes along with a new food, its properties can be put into the network and a (hopefully) reliable value as to how healthy it is pops out the other end.<br>
Ideally, once trained, the weight given to colour will be zero since that is completely irrelevant (ignoring artificial colourings).  However, if the training sample has just a few blue foods, which just happen to be bad for you, then the training algorithm will wrongly ascribe a high weight.to the colour input.  Also, if the training sample foods have pretty similar fat and fibre contents, but are radically differently healthy (say, maybe due to salt content), then the training algorithm will probably be unable to make any sense of the sample, and give useless weights. This emphasises the need to select a large and varied training sample (as well as setting up the network with meaningful inputs in the first place).<br>
<br>
Realistically, a network will be composed of many neurons so that all 'cross effects' between the inputs are taken into account (where a weighting for one input needs to depend on other inputs as well).  Here, the network would be built up with layers of neurons where the input for each neuron in a layer is the output from each neuron in the layer before.  The final layer would have just one neuron, so that you get just one output for the network.


\section CreatingAndTraining Creating and training a new neural net
The method used to create a new network varies slightly depending on the algorithm used to train it.  Sections \ref BuildingTheNeuronLayers and \ref CreatingTheNetwork describe how to setup the network ready for training, which is common to all training algorithms.  The <tt>BatchBackPropagationAlgorithm</tt>, <tt>BackPropagationCGAlgorithm</tt> and <tt>GeneticAlgorithm</tt> algorithms require the training data to be pre-stored in a <tt>nnet::NeuralNetDataSet</tt> class (section \ref BuildingTheTrainingSample), and will train themselves over the whole data set.  <tt>BackPropagationAlgorithm</tt> on the other hand performs one training step at a time to provide more control over each training step.<BR>
Descriptions of the algorithms are given in \ref TrainingTheNetwork.

\subsection BuildingTheNeuronLayers Building the neuron layers
Only simple nets can be built, where each neuron takes the outputs of all of the neurons in the previous layer as its inputs.  Details about the neurons behaviour are given in \ref NeuronDescriptions.<BR>
There are two methods, one where neurons can have different types, and a simpler one where all of the neurons have the same type.

\subsubsection AllNeuronsOfTheSameType All neurons of the same type
Building the neuron layers simply consists of creating an STL vector of integers with the number of neurons in each layer, including the output layer but excluding the input layer.  The type of all of the neurons is set later when the network is built.  So if a network takes 3 inputs, has two hidden layers with 6 neurons and 4 neurons respectively, and 2 outputs the layers would be set like this:

\code
std::vector<int> neuronsInLayer;
neuronsInLayer.push_back( 6 );
neuronsInLayer.push_back( 4 );
neuronsInLayer.push_back( 2 );
// The number of inputs is set later.
\endcode

\subsubsection NeuronsWithDifferentTypes Neurons with different types
These are set in a similar way, but instead of integers specifying the number of neurons in each layer, another STL vector of strings specifying the name of each neuron type is used, with the number of neurons set by the size of the vector.
Currently available types (descriptions are given in \ref NeuronDescriptions) are:

\code
LinearNeuron
SigmoidNeuron
TanSigmoidNeuron
\endcode

So if a network as in the previous example is to be built (with arbitrary neuron types):

\code
std::vector<std::string> layer1;
layer1.push_back( "LinearNeuron" );
layer1.push_back( "SigmoidNeuron" );
// ...and so on until there are six names in the vector -> six neurons in the layer

std::vector<std::string> layer2;
layer2.push_back( "TanSigmoidNeuron" );
// ... and so on another three times

std::vector<std::string> outputlayer;
outputlayer.push_back( "LinearNeuron" );
outputlayer.push_back( "LinearNeuron" );

std::vector< std::vector<std::string> > neuronNames;
neuronNames.push_back( layer1 );
neuronNames.push_back( layer2 );
neuronNames.push_back( outputlayer );
// The number of inputs is set later.
\endcode


\subsection CreatingTheNetwork Creating the network
Once the layer structure has been set up, the network can be created as follows, depending on which layer specification method was used.

\subsubsection AllNeuronsOfTheSameType All neurons of the same type
The type of the neurons is set by creating a neuron builder and passing its address to the network constructor.  The names of available builders are the same as for the neurons, but with ``<tt>Builder</tt>'' on the end, for example ``<tt>nnet::LinearNeuronBuilder</tt>'' will build ``<tt>LinearNeuron</tt>''s.

\code
int numberOfInputs=3; // number of inputs set when the network is created
nnet::SigmoidNeuronBuilder myNeuronBuilder; // the type of ALL the neurons is set here

// now create the network using the neuronsInLayer vector from before
nnet::NeuralNet sameNeuronsNet( numberOfInputs, neuronsInLayer, &myNeuronBuilder );
\endcode

\subsubsection NeuronsWithDifferentTypes Neurons with different types}
All that is needed here is the STL vectors of neuron names previously initialised and the number of inputs.

\code
int numberOfInputs=3; // number of inputs set when the network is created

// now create the network using the neuronNames vector from before
nnet::NeuralNet differentNeuronsNet( numberOfInputs, neuronNames );
\endcode

\subsubsection RandomNumberGeneration Random number generation
In each case, you can optionally use a random seed for the random number generator that sets the initial neuron weights by adding a boolean parameter at the end of the constructor arguments.  Default is to use a random seed.

\code
//don't use a random seed
nnet::NeuralNet sameNeuronsNet( numberOfInputs, neuronsInLayer, &myNeuronBuilder, false );

//use a random seed (default)
nnet::NeuralNet differentNeuronsNet( numberOfInputs, neuronNames, true );
\endcode

Note that currently <b>the default implementation uses <tt>rand()</tt> for random numbers</b>, and the random seed is taken from the current system time.  If you require something more sophisticated modify the ``<tt>RandomNumberUtils.h</tt>'' file.

\subsection BuildingTheTrainingSample Building the training sample
A network can be trained without setting out the data sample into a <tt>nnet::NeuralNetDataSet</tt> using the <tt>BackPropagationAlgorithm</tt>, but large scale training is easiest using the other algorithms so this will be covered here.<BR>
Data is added to the <tt>nnet::NeuralNetDataSet</tt> by calls to <tt>addDataItem</tt>, with a vector of inputs and a vector of the expected outputs as the arguments.  All items in the data set must have the same number of inputs and outputs; the first item you add sets these sizes for the whole data set.  If you try and add an item where the input or output vectors are not the correct size, then an error will be printed to standard error and the item will be ignored.

For example:

\code
// for a network to calculate the probability a given animal is a donkey
// with inputs, in order, of "number of legs", "height" and "length of tail"
nnet::NeuralNetDataSet \label{donkeyNetDataSet}animalSample;
std::vector<double> inputs;
std::vector<double> output;

// for donkey 1
output.push_back( 1 ); // I know for certain this animal is a donkey
inputs.push_back( 4 ); // it has four legs
inputs.push_back( 1.45 ); // it is 1.45m tall
inputs.push_back( 0.32 ); // it has a tail 32cm long

// This call sets the sample to demand 3 inputs and 1 output
animalSample.addDataItem( inputs, output ); // Add this animal to the training sample

inputs.clear(); // Clear all the data so that the vectors can be reused
output.clear();

//for Geoff
output.push_back( 0 ); // I know for certain Geoff isn't a donkey
inputs.push_back( 2 ); // he has 2 legs
inputs.push_back( 1.82 ); // he is 1.82m tall

// This will not be added, because there are not enough inputs!
animalSample.addDataItem( inputs, output ); // Add Geoff to the training sample

// To add Geoff to the training sample we need to match the number of inputs
inputs.push_back( 0 ); // Geoff's tail is 0cm long
animalSample.addDataItem( inputs, output ); // This will now work
\endcode

\subsection TrainingTheNetwork Training the network
To train the network, a training algorithm is created with the network to be trained as the constructor argument, and a call to train is made with the number of training epochs and the training data.
Currently available training algorithms are:<BR>

\code
nnet::BackPropagationAlgorithm
nnet::BackPropagationCGAlgorithm
nnet::BatchBackPropagationAlgorithm
nnet::GeneticAlgorithm
\endcode

\subsubsection TrainingWithBackPropagationAlgorithm Training with BackPropagationAlgorithm
The Back Propagation Algorithm uses the back propagation method for determining the gradient of the error, and then gradient descent to modify the weights to minimise the error.  It is very similar to the BatchBackPropagationAlgorithm except that it only performs one training step at a time to give more control over the training parameters at each step.<BR>
The algorithm class is constructed by giving it the network to be trained, and optionally values for \f$learningRate\f$ and \f$momentumConstant\f$ (defaults are 0.5 for both). The \f$learningRate\f$ parameter is just a multiplier applied to the calculated change required for each weight, larger values will mean the weights will change more rapidly with each step.  The previous steps' calculated change is also added to the current steps', but multiplied by the \f$momentumConstant\f$ value.  A value greater than or equal to one for this would stop the algorithm settling on a maximum because (at least) the full previous change is added as well.<BR>

The <tt>train</tt> method is used to perform one training run, and returns the error.  It takes a vector of the inputs and a vector of the required outputs, so if the first data item in the previous example is used for the step:

\code
std::vector<double> inputs;
std::vector<double> output;

// for donkey 1
output.push_back( 1 ); // I know for certain this animal is a donkey
inputs.push_back( 4 ); // it has four legs
inputs.push_back( 1.45 ); // it is 1.45m tall
inputs.push_back( 0.32 ); // it has a tail 32cm long

//set learningRate to 0.6 and momentumConstant to 0.4
nnet::BackPropagationAlgorithm myTrainer( myPreviouslyCreatedNetwork, 0.6, 0.4 );

double errorForThisStep=myTrainer.train( inputs, output );
\endcode

\subsubsection TrainingWithBatchPropagationAlgorithm Training with BatchBackPropagationAlgorithm
This is essentially the same as <tt>BackPropagationAlgorithm</tt>, except it is supplied with a training sample which it will loop over itself.  It can also be set do so repeatedly by specifying the number of epochs to run when calling the <tt>train</tt> method.  The error for the most recent epoch is returned by <tt>train</tt>, and the errors from previous epochs can be retrieved as a vector with the <tt>getTrainingErrorValuesPerEpoch</tt> method.

\code
//created in the same way as for the single step version
nnet::BatchBackPropagationAlgorithm myTrainer( myPreviouslyCreatedNetwork ) //use default learningRate and momentumConstant

//train using the sample in the previous example and 50 epochs
double finalError=myTrainer.train( 50, animalSample );

//get the errors from previous epochs to see how things are converging
std::vector<double> errors=myTrainer.getTrainingErrorValuesPerEpoch();
\endcode

\subsubsection TrainingWithBackPropagationCGAlgorithm Training with BackPropagationCGAlgorithm
This algorithm is similar to <tt>BatchBackPropagationAlgorithm</tt> except that it uses the conjugate gradient method to minimise the error instead of gradient descent.
It offers three types of function to calculate the \f$\beta\f$ coefficient (see any detailed description of conjugate gradients) selected using the <tt>setBetaFunction</tt> method.  These are ``FletcherReves'', ``PolakRibiere'', and ``ConjugateGradient'', used as an enumeration as quoted.  The default is FletcherReves.

\code
//created in the same way as for the single step version
nnet::BackPropagationCGAlgorithm myTrainer( myPreviouslyCreatedNetwork )

//set the beta function to Polak-Ribiere
myTrainer.setBetaFunction( nnet::BackPropagationCGAlgorithm::PolakRibiere );

//train using the sample in the previous example and 500 epochs
double finalError=myTrainer.train( 500, animalSample );
\endcode

\section ObtainingResults Obtaining results
To get results from the neural network, the output method takes the inputs as an STL vector of doubles, and provides the results as an STL vector of doubles.  So to determine if some animal is a donkey using a network trained from data of form of the data set in the previous example:

\code
// Using the previously trained net "donkeyNet" to
// find out if Fido is a donkey...
std::vector<double> inputs;
inputs.push_back( 4 ); // Fido has four legs
inputs.push_back( 0.54 ); // he is 54cm tall
inputs.push_back( 0.25 ); // his tail is 25cm long

std::vector<double> output=donkeyNet( inputs );
// The network was set to have only one output, if there were any
// more then they would be the higher elements in the vector.
std::cout << "Likelihood Fido is a donkey= " << output[0] << std::endl;
\endcode

\section SavingANeuralNetToDisk Saving a neural net to disk
Neural nets can either be saved as plain text or XML files, with the default being XML.  To choose between the two make a call to <tt>NeuralNet::setSerialisationMode</tt> with either <tt>nnet::NeuralNet::PlainText</tt> or <tt>nnet::NeuralNet::XML</tt>.<BR>
The network can then be saved to disk by passing a C++ stream to serialise.  For example:

\code
myNeuralNet.setSerialisationMode( nnet::NeuralNet::XML );
std::ofstream outputFile( "/home/me/myNeuralNet.xml" );
myNeuralNet.serialise( outputFile );
\endcode

The network can also of course be printed to standard output by calling <tt>serialise( std::cout )</tt>.

\section LoadingANeuralNetFromDisk Loading a neural net from disk
A network can be loaded from disk by simply passing the filename and the serialisation mode as the constructor arguments.  If the serialisation mode is not specified then XML is assumed.  For example:

\code
nnet::NeuralNet myXMLNet( "/home/me/myNeuralNet.xml", nnet::NeuralNet::XML );
nnet::NeuralNet anotherXMLNet( "/home/me/myOtherNeuralNet.xml" );//XML is the default
nnet::NeuralNet myTextNet( "/home/me/myNeuralNet.txt", nnet::NeuralNet::PlainText );
\endcode

Note that there is currently no error checking when loading XML nets, <b>if you try and load a plain text net as XML, or the file is not properly structured you will get a segmentation fault or runaway memory allocation</b>. This is still being looked into.

\section NeuronDescriptions Neuron Descriptions
The output from a neuron is given by its <em>threshold function</em> which is unique to each type of neuron.  This is a function of the neurons <em>activation value</em>, which is calculated the same way for each type.

The activation value \f$ a \f$ for a neuron with \f$ N \f$ inputs, \f$ i_{n}\f$ , each with weights \f$ w_{n} \f$ is given by
\f[
a=\sum_{n=1}^{N} i_{n}\times w_{n}+b\times w_{b}
\f]
Where \f$ b \f$ is a bias that can be assigned to a particular neuron (and \f$ w_{b}\f$ the bias' weight).
The weights are initially random, and are then fine tuned by the training algorithms to try and give the desired output.  The bias is set when the neuron is created but that process is done internally by the neuron builders.  All current neuron builders set the bias to -1.

Some of the neurons have methods to change their behaviour.  To get the neuron pointer to call these methods use ``<tt>NeuralNet::layer(layerNumber)->neuron(neuronNumber)</tt>'', where the numbers of available layers and neurons per layer can be found with ``<tt>NeuralNet::numberOfLayers()</tt>'' and ``<tt>NeuralNet::layer(layerNumber)->numberOfNeurons()</tt>'' respectively.

\subsection LinearNeuron Linear Neuron
Linear neurons give, as the name suggests, a linear output between -1 and +1 with a gradient of \f$1/slopeEnd\f$.  The value of <em>slopeEnd</em> can be set using the <tt>LinearNeuron::setSlopeEnd(newValue)</tt> method.  If the output is greater than +slopeEnd, then the output is limited to +1; any less than -slopeEnd and the output is limited to -1.  Anywhere in between gives the expected linear output of \f$activationvalue/slopeEnd\f$.

\subsection SigmoidNeuron Sigmoid Neuron
The sigmoid neuron gives sigmoid (sort of resembles a slanted ``S'') output, \f$ o \f$, of between 0 and 1 from the function

\f[
o=\frac{1}{1+e^{-a/r}}
\f]

Where \f$ r \f$, the ``response'', can be set with the <tt>SigmoidNeuron::setResponse(newValue)</tt> method.  The default is 1.

\subsection TanSigmoidNeuron Tan Sigmoid Neuron
This neuron gives a similar looking output to the sigmoid neuron, but between -1 and 1.  The value is given by

\f[
o=tanh(s\times a)
\f]

Where the value of \f$ s \f$ (the ``scale'') can be set with the <tt>TanSigmoidNeuron::setScale(newValue)</tt> method.  The default is 1.

@author Mark Grimes (mark.grimes@bristol.ac.uk)

*/

namespace vertex_lcfi
{
//!Neural Net namespace
namespace nnet{};
};
