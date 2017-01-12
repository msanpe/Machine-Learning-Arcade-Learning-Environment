//
// Created by Miguel on 21/12/2016.
//
#include <time.h>
#include <iostream>
#include "nn.h"
#include "data.h"
#include "functions.h"

double localError,       //Variable de error local para el entrenamiento
       meanSquaredError,        //Suma total del error, para detener el entrenamiento
       *tmpIn,          //Array que contiene los patrones de entrada de forma temporal
       *tmpOut;         //Array que contiene los patrones de salida de forma temporal
int trainSize, j, i;
int inp, hid, outp;    // layer sizes
char f[32], n[32];     // file names
std::string train;
int m;
bool activateDiscrete = false;   // varia entre discreto y continuo
NeuralNetwork NN;


void trainNet(void) {
    std::cout << "Number of inputs: " << std::endl;
    std::cin >> inp;
    std::cout << "Number of elements in hidden layer: " << std::endl;
    std::cin >> hid;
    std::cout << "Number of outputs: " << std::endl;
    std::cin >> outp;
    std::cout << "Training data file: " << std::endl;
    std::cin >> train;
    std::cout << "Filename to save net: " << std::endl;
    std::cin.width (31);
    std::cin >> n;
    srand(time(NULL));

    NN.inputNum = inp;
    NN.outputNum = outp;
    NN.hiddenNum = hid;
    NN.targetNum = outp;
    NN.createNet();
    tmpIn = new double[NN.inputNum];
    tmpOut = new double[NN.outputNum];

    data dataSet(NN.inputNum, NN.outputNum, train);
    dataSet.readFile();
    trainSize = dataSet.setSize();


    NN.LR = 0.5f;      // learning rate
    NN.Alpha = 0.3f;    // Momentum
    NN.LambdaIH = 0;      // Regularization strength
    NN.LambdaHO = 0;      // Regularization strength
    meanSquaredError = 1000;        // para detener la red en el error deseado

    // entrenamiento
    for (i = 0; meanSquaredError > 0.01; ++i) { // entrena hasta error deseado
        std::cout << "Epoch " << i << " ----> ";
        if (i == 500000) {  // limite que evita loops infinitos
            //std::cout << "Training is taking too many epochs" << std::endl;
            //break;
        }
        //Pasa 1 a 1 los patrones de entrada y salida y entrena la red
        for (int current = 0; current < trainSize; current++) {
            dataSet.getTrainingData(current, tmpIn, tmpOut);
            for (j = 0; j < NN.inputNum; j++) {
                NN.Inputs[j] = tmpIn[j];
            }
            for (j = 0; j < NN.outputNum; j++) {
                NN.Target[j] = tmpOut[j];
            }
            NN.trainNet();
            NN.testNet();
            /*for (j = 0; j < NN.inputNum; j++) {
                std::cout << NN.Inputs[j] << " ";
            }
            std::cout << " ---> ";
            for (j = 0; j < NN.outputNum; j++) {
                std::cout << NN.Outputs[j] << " ";
            }
            std::cout << std::endl;*/
            for (j = 0; j < NN.outputNum; j++) {
                double delta = NN.Target[j] - NN.Outputs[j];
                localError += delta * delta;
            }

        }
        // epoch error calculation
        meanSquaredError = localError/NN.outputNum;
        std::cout << " " << "Error: " << meanSquaredError << std::endl;
        localError = 0;
    }
    std::cout << "Training ended in " << i << " epochs" << std::endl;
    NN.saveNet(n);
    std::cout << "Network weights saved in " << n << std::endl;
}


int menu(void) {
    std::cout << std::endl;
    std::cout << "Menu" << std::endl;
    std::cout << "1. Train new network" << std::endl;
    std::cout << "2. Switch between continuos and discrete outputs" << std::endl;
    std::cout << "3. Test network" << std::endl;
    std::cout << "4. Load network" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cin >> m;
    std::cout << std::endl;

    if (m == 1)      trainNet();
    else if (m == 2) toggleDiscrete();
    else if (m == 3) useNet();
    else if (m == 4) loadNet();
    else if (m == 5) return 1;
    else             return 0;
}

// cambia entre salidas discretas/continuas
void toggleDiscrete(void) {
    if (activateDiscrete == false) {
        activateDiscrete = true;
        std::cout << "Discrete data activated" << std::endl;
    } else {
        activateDiscrete = false;
        std::cout << "Discrete data deactivated" << std::endl;
    }
}

// convierte de continuo a discreto
double convertDiscrete(double a) {
    if (activateDiscrete == true) {
        if (a < 0.5) return 0;
        if (a >= 0.5) return 1;
    } else return a;
}

void useNet(void) {
    std::cout << "Ingresar los valores de entrada a la red:" << std::endl;
    for (i = 0; i < NN.inputNum; i++) {
        std::cout << "Input " << (i + 1) << std::endl;
        std::cin >> NN.Inputs[i];
        std::cout << std::endl;
    }
    NN.testNet();
    std::cout << "Output:" << std::endl;
    for (int i = 0; i < NN.outputNum; i++) {
        std::cout << convertDiscrete(NN.Outputs[i]) << std::endl;
    }
}

void loadNet() {
    std::cout << "Insert weights file: " << std::endl;
    std::cin.width(31);
    std::cin >> f;
    NN.loadNet(f);  //Recupera la red del archivo dado
    useNet();
}
