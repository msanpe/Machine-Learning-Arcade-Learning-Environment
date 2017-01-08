//
// Created by Miguel on 21/12/2016.
//

#include "data.h"

data::data(int a, int b, const char *p) {
    in = a;       //Numero de entradas de la red
    out = b;      //Numero de salidas de la red;
    file =  const_cast<char*>(p);
}

data::~data() {
    delete[] Inputs;
    delete[] Outputs;
}

// lee valores de fichero y los almacena hasta que son pedidos
void data::readFile(void) {
    FILE *fw = fopen(file, "r");

    if (!fw) {
        perror(file);
        return;
    }

    fscanf(fw, "%d", &dataSize);
    createDataContainers();

    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < in; j++) {
            fscanf(fw, "%lf", &Inputs[j][i]);
        }
        for (int j = 0; j < out; j++) {
            fscanf(fw, "%lf", &Outputs[j][i]);
        }
    }
}

void data::getTrainingData(int n, double *p, double *s) { // fill containers with training data
    for (int i = 0; i < in; i++) {
        p[i] = Inputs[i][n];
    }
    for (int i = 0; i < out; i++) {
        s[i] = Outputs[i][n];
    }
}

void data::createDataContainers(void) {
    Inputs = createContainer(in, dataSize);    // inputs
    Outputs = createContainer(out, dataSize);  // targets
}

int data::setSize(void) {
    return dataSize;
}

double **createContainer(int Row, int Col) {
    double **array = new double *[Row];
    for (int i = 0; i < Row; i++)
        array[i] = new double[Col];
    return array;
}
