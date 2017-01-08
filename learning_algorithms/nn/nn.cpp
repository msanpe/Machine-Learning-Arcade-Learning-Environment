//
// Created by Miguel on 21/12/2016.
//
#include "nn.h"

NeuralNetwork::NeuralNetwork() {
    Error = 0;
    Inputs = 0;
    Hidden = 0;
    Outputs = 0;
    Target = 0;
    HBias = 0; // hidden bias
    OBias = 0; //output bias
    Delta = 0;
    HDelta = 0;
    DWeight = 0;
    HDWeight = 0;
    InputWeights = 0;
    HiddenWeights = 0;
}

NeuralNetwork::~NeuralNetwork() {
    delete[] Inputs;
    delete[] Hidden;
    delete[] Outputs;
    delete[] Target;

    delete[] HBias;
    delete[] OBias;

    delete[] Delta;
    delete[] HDelta;

    delete[] DWeight;
    delete[] HDWeight;

    delete[] InputWeights;
    delete[] HiddenWeights;
}

// variable initialization and space allocation
void NeuralNetwork::createNet(void) {
    Inputs = new double[inputNum];
    Hidden = new double[hiddenNum];

    Outputs = new double[outputNum];
    Target = new double[targetNum];

    HBias = new double[hiddenNum];
    OBias = new double[outputNum];

    Delta = new double[outputNum];
    HDelta = new double[hiddenNum];

    DWeight = new double[outputNum];
    HDWeight = new double[hiddenNum];

    InputWeights = createLayer(inputNum, hiddenNum);
    HiddenWeights = createLayer(hiddenNum, outputNum);

    randomWeights();
    zeroDeltas();
    randomBias();
}

// Weights initialization
void NeuralNetwork::randomWeights(void) {
    int i, j;

    for (i = 0; i < inputNum; i++) {
        for (j = 0; j < hiddenNum; j++) {
            InputWeights[i][j] = generateRandom(1, -1);
        }
    }

    for (i = 0; i < hiddenNum; i++) {
        for (j = 0; j < outputNum; j++) {
            HiddenWeights[i][j] = generateRandom(1, -1);
        }
    }
}

// bias initialization
void NeuralNetwork::randomBias(void) {
    int i;

    for (i = 0; i < outputNum; i++) {
        OBias[i] = generateRandom(1, 0);
    }

    for (i = 0; i < hiddenNum; i++) {
        HBias[i] = generateRandom(1, 0);
    }
}

void NeuralNetwork::zeroDeltas(void) {
    int i;

    for (i = 0; i < outputNum; i++) {
        DWeight[i] = 0;
    }

    for (i = 0; i < hiddenNum; i++) {
        HDWeight[i] = 0;
    }
}

// transfer function
double NeuralNetwork::sigmoid(double num) {
    return (1.0 / (1.0 + exp(-num)));
}

void NeuralNetwork::feedForward(void) {
    int i, j;
    double synapseSum = 0.0f; // weights * inputs

    // feed hidden layer
    for (i = 0; i < hiddenNum; i++) {
        for (j = 0; j < inputNum; j++) {
            synapseSum += InputWeights[j][i] * Inputs[j];
        }
        Hidden[i] = sigmoid(synapseSum + HBias[i]);

        synapseSum = 0.0f;
    }

    // feed output layer
    for (i = 0; i < outputNum; i++) {
        for (j = 0; j < hiddenNum; j++) {
            synapseSum += HiddenWeights[j][i] * Hidden[j];
        }
        Outputs[i] = sigmoid(synapseSum + OBias[i]);

        synapseSum = 0.0f;
    }
}

void NeuralNetwork::computeError(void) {
    int i, j;
    double errorSum = 0.0f;
    double sumSquaredVals = regSumSquaredVals();
    Error = 0;

    // output layer error
    for (i = 0; i < outputNum; i++) {
        Err = (Target[i] - Outputs[i]);
        Delta[i] = (1 - Outputs[i]) * Outputs[i] * Err; // partial derivative of the total error with respect to the net input of the neuron
        Error += (0.5f * Err * Err) + (0.5f * Lambda * sumSquaredVals); // error with regularization
    }

    // hidden layer error
    for (i = 0; i < hiddenNum; i++) {
        for (j = 0; j < outputNum; j++) {
            errorSum += Delta[j] * HiddenWeights[i][j];
        }
        HDelta[i] = (1 - Hidden[i]) * Hidden[i] * errorSum;

        errorSum = 0.0f;
    }
}

void NeuralNetwork::backpropagate(void) {
    int i, j;

    // output layer
    for (i = 0; i < hiddenNum; i++) {
        for (j = 0; j < outputNum; j++) {
            HiddenWeights[i][j] += LR * Delta[j] * Hidden[i] + Alpha * DWeight[j]; // backprop with momentum
            DWeight[j] = LR * Delta[j] * Hidden[i];
        }
    }

    // hidden layer
    for (i = 0; i < inputNum; i++) {
        for (j = 0; j < hiddenNum; j++) {
            InputWeights[i][j] += LR * HDelta[j] * Inputs[i] + Alpha * HDWeight[j];
            HDWeight[j] = LR * HDelta[j] * Inputs[i];
        }
    }

    // output bias
    for (i = 0; i < outputNum; i++) {
        OBias[i] += LR * Delta[i];
    }

    // hidden bias
    for (i = 0; i < hiddenNum; i++) {
        HBias[i] += LR * HDelta[i];
    }
}

double NeuralNetwork::regSumSquaredVals() {
  int i, j;
  double sumSquaredVals = 0.0;

  // output layer
  for (i = 0; i < hiddenNum; i++) {
      for (j = 0; j < outputNum; j++) {
          sumSquaredVals += (HiddenWeights[i][j] * HiddenWeights[i][j]);

      }
  }

  // hidden layer
  for (i = 0; i < inputNum; i++) {
      for (j = 0; j < hiddenNum; j++) {
          sumSquaredVals += (InputWeights[i][j] * InputWeights[i][j]);
      }
  }

  return sumSquaredVals;
}

// weights array creator
double **createLayer(int Row, int Col) {
    double **array = new double *[Row];
    for (int i = 0; i < Row; i++)
        array[i] = new double[Col];

    return array;
}

double generateRandom(int High, int Low) {
    srand((unsigned int) time(NULL));
    return ((double) rand() / RAND_MAX) * (High - Low) + Low;
}

void NeuralNetwork::trainNet(void) {
    feedForward();
    computeError();
    backpropagate();
}

void NeuralNetwork::testNet(void) {
    feedForward();
}
