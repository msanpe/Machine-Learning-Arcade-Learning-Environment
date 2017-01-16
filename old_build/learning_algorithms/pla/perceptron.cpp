//
// Created by Miguel on 20/12/2016.
//

#include <iostream>
#include <math.h>

using namespace std;

#define EPOCHS 200  // Number of epochs
#define K 0.2f     // Learning rate

float trainSLP(float x0, float x1, float target);
float runSLP(float x0, float x1);
float sigmoid(float s);
void initWeights();


float weights[2];
float bias = 1.0f;

int main() {
    int i = 0;

    initWeights();

    while (i < EPOCHS) {
        i++;

        // AND function
        cout << "Output" << endl;
        cout << "1, 1 = " << trainSLP(1, 1, 1) << endl;
        cout << "1, 0 = " << trainSLP(1, 0, 0) << endl;
        cout << "0, 1 = " << trainSLP(0, 1, 0) << endl;
        cout << "0, 0 = " << trainSLP(0, 0, 0) << endl;

        cout << "\n";

        // Weights per epoch
        cout << "Weight[0] = " << weights[0] << endl;
        cout << "Weight[1] = " << weights[1] << endl;

        cout << "\nbias = " << bias << endl;

        cout << "\n\n" << "epoch = " << i << endl;

    }

    // test net
    cout << "Test:" << endl;
    cout << "1, 1 = " << runSLP(1, 1) << endl;
    cout << "1, 0 = " << runSLP(1, 0) << endl;
    cout << "0, 1 = " << runSLP(0, 1) << endl;
    cout << "0, 0 = " << runSLP(0, 0) << endl;

    cout << "\n";

    // final weights
    cout << "Final weight[0] = " << weights[0] << endl;
    cout << "Final weight[1] = " << weights[1] << endl;
    cout << "\nbias = " << bias << endl;

    cout << "\n\n" << "In " << i << " epochs" << endl;

    return 0;
}

float trainSLP(float x0, float x1, float target) {
    float net = 0;
    float delta[2];
    float Error;

    net = weights[0] * x0 + weights[1] * x1 - bias;
    net = sigmoid(net);

    Error = target - net;

    bias -= K * Error;

    delta[0] = K * Error * x0;  // synaptic weight variation
    delta[1] = K * Error * x1;  // error * input * LR

    weights[0] += delta[0];  // weight adjust

    return net;
}

float runSLP(float x0, float x1) {
    float net = 0;

    net = weights[0] * x0 + weights[1] * x1 - bias;
    net = sigmoid(net);

    return net;
}


void initWeights(void) {
    for (int i = 0; i < 2; i++) {
        weights[i] = (float) rand() / RAND_MAX;
    }
}

float sigmoid(float s) {
    return (1.0 / (1 + exp(-1 * s)));
}
