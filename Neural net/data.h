//
// Created by Miguel on 21/12/2016.
//

#ifndef NEURAL_NET_DATA_H
#define NEURAL_NET_DATA_H

#include <stdlib.h>
#include <stdio.h>

//Leer un archivo con los patrones de entrada y salida usados por la red
class data {
private:
    int dataSize,
            in,
            out;

    double **Inputs, //Contenedores de los datos de la muestras
            **Outputs;

    char *file;
public:
    data(int a, int b, char *p);
    ~data();
    void readFile(void);
    void getTrainingData(int n, double *p, double *s);
    void createDataContainers(void);
    int setSize(void);
};

double **createContainer(int Row, int Col);

#endif //NEURAL_NET_DATA_H
