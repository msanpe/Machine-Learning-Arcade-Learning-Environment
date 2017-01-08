//
// Created by Miguel on 21/12/2016.
//
#ifndef NeuralNetwork_H
#define NeuralNetwork_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

class NeuralNetwork {
public:
    NeuralNetwork();
    ~NeuralNetwork();

    void createNet(void);
    void trainNet(void);
    void testNet(void);

    void randomWeights(void);
    void randomBias(void);
    void zeroDeltas(void);
    void saveNet(char *p);

    double *Inputs;
    double *Outputs;
    double *Target;

    double Error;     //error total en la red
    double Alpha;     //momentum
    double LR;       //learning rate
    double Lambda;   //regularization strength

    int inputNum;    //numero de elementos de entrada
    int outputNum;   //elementos en la capa oculta
    int hiddenNum;   //elementos en la capa de salida
    int targetNum;   //numero de objetivos

private:
    double *Hidden;

    double *HBias;
    double *OBias;

    double **InputWeights;
    double **HiddenWeights;

    double *Delta;
    double *HDelta;
    double *DWeights;
    double *HDWeights;

    double Err;

    void feedForward(void);
    void computeError(void);
    void backpropagate(void);
    double sigmoid(double num);
    double regSumSquaredVals();
};

double **createLayer(int Row, int Col);
double Random( int High, int Low );

#endif
