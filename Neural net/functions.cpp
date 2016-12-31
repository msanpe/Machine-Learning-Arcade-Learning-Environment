//
// Created by Miguel on 21/12/2016.
//
#include <time.h>
#include <iostream>
#include "nn.h"
#include "data.h"
#include "functions.h"

double localError,       //Variable de error local para el entrenamiento
       errorSum,        //Suma total del error, focalizado para detener el entrenamiento
       *tmpIn,          //Array que contiene los patrones de entrada de forma temporal
       *tmpOut;         //Array que ontiene los patrones de salida de forma temporal
int trainSize, j, i;
int inp, hid, outp;    // layer sizes
char f[32];     // file names
int m;
bool activateDiscrete = false;   // varia entre discreto y continuo
NeuralNetwork NN;

using namespace std;

void trainNet(void) {
    cout << "Number of inputs: " << endl;
    cin >> inp;
    cout << "Number of elements in hidden layer: " << endl;
    cin >> hid;
    cout << "Number of outputs: " << endl;
    cin >> outp;
    cout << "Training data file: " << endl;
    cin.width(31);
    cin >> f;
    srand(time(NULL));

    NN.inputNum = inp;
    NN.outputNum = outp;
    NN.hiddenNum = hid;
    NN.targetNum = outp;
    NN.CreateNet();
    tmpIn = new double[NN.inputNum];
    tmpOut = new double[NN.outputNum];

    data dataSet(NN.inputNum, NN.outputNum, f);
    dataSet.readFile();
    trainSize = dataSet.setSize();


    NN.LR = 0.5f;      //learning rate
    NN.Alpha = 0.3f;    //Momentum
    errorSum = 1000;        // para detener la red en el error deseado

    // entrenamiento
    for (i = 0; errorSum > 0.01; ++i) { // entrena hasta error deseado

        if (i == 100000) {  // limite que evita loops infinitos
            cout << "Training is taking too many epochs" << endl;
            break;
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
            NN.TestNet();
            for (j = 0; j < NN.inputNum; j++) {
                cout << NN.Inputs[j] << " ";
            }
            cout << " ---> ";
            for (j = 0; j < NN.outputNum; j++) {
                cout << NN.Outputs[j] << " ";
            }
            cout << endl;
            for (j = 0; j < NN.outputNum; j++) {
                double delta = NN.Target[j] - NN.Outputs[j];
                localError += delta * delta;
            }
            NN.TrainNet();
        }
        // epoch error calculation
        errorSum = sqrt((localError) * (localError));
        localError = 0;
    }
    cout << "Training ended in " << i << " epochs" << endl;
}


int menu(void) {
    cout << endl;
    cout << "Menu" << endl;
    cout << "1. Train new network" << endl;
    cout << "2. Switch between continuos and discrete outputs" << endl;
    cout << "3. Test network" << endl;
    cout << "4. Exit" << endl;
    cin >> m;
    cout << endl;

    if (m == 1)      trainNet();
    else if (m == 2) toggleDiscrete();
    else if (m == 3) useNet();
    else if (m == 4) return 1;
    else             return 0;
}

// cambia entre salidas discretas/continuas
void toggleDiscrete(void) {
    if (activateDiscrete == false) {
        activateDiscrete = true;
        cout << "Discrete data activated" << endl;
    } else {
        activateDiscrete = false;
        cout << "Discrete data deactivated" << endl;
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
    cout << "Ingresar los valores de entrada a la red:" << endl;
    for (i = 0; i < NN.inputNum; i++) {
        cout << "Input " << (i + 1) << endl;
        cin >> NN.Inputs[i];
        cout << endl;
    }
    NN.TestNet();
    cout << "Output:" << endl;
    for (int i = 0; i < NN.outputNum; i++) {
        cout << convertDiscrete(NN.Outputs[i]) << endl;
    }
}
