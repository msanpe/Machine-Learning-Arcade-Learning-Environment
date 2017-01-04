/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: cristina
 *
 * Created on January 3, 2017, 10:14 PM
 */

#include <cstdlib>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

class LinearRegression{
  private:
    int tam;
    int N;
    vector< vector<float> > inputs;
    vector<float> outputs;
    vector<float> b;
    int cont;
    
  public:
    LinearRegression();
    ~LinearRegression();
    int getN();
    void init(string file);
    float getP(int idx);
    float getB(int n);
    float getErr(int idx);
    void actualizarB(float alpha, float err, int idx);
    void train(float alpha);


};
int LinearRegression::getN(){
    return N;
}
LinearRegression::LinearRegression(){
    tam = 0;
    N = 0;
}
LinearRegression::~LinearRegression(){
    
}

float LinearRegression::getP(int idx){
    return b[0] + b[1] * inputs[0][idx] + b[2] * inputs[1][idx];
}

float LinearRegression::getErr(int idx){
    return outputs[idx];
}

void LinearRegression::init(string file){
    cont = 1;
    ifstream ifs;
    ifs.open(file.c_str(), ifstream::in);
    
    ifs >> tam;
    inputs.resize(tam - 1);
    //outputs.resize(tam); 
    b.resize(tam); 
    
    float x0, x1, y;
    int n = 0;
    while(ifs >> x0 >> x1 >> y){
        inputs[0].push_back(x0);
        inputs[1].push_back(x1);
        outputs.push_back(y);
        n++;
    }
    N = n;   
}
void LinearRegression::actualizarB(float alpha, float err, int idx){    
    b[0] = b[0] - alpha * err;
    b[1] = b[1] - alpha * err * inputs[0][idx];
    b[2] = b[2] - alpha * err * inputs[1][idx];
}
float LinearRegression::getB(int n){
    return b[n];
}

void LinearRegression::train(float alpha){
    float p;
    float err;
    for (int i = 0; i < N; i ++){
        p = this->getP(i);
        err = p - this->getErr(i);
        this->actualizarB(alpha, err, i);
    }
    cout << "b0 = " << this->getB(0) ;
    cout << ", b1 = " << this->getB(1) ;
    cout << ", b2 = " << this->getB(2) ;
    cout << ", error " << cont++ << " " << err << endl;
}
int main(int argc, char** argv) {

    string file = "datos.txt";
    LinearRegression lin_reg;
    lin_reg.init(file);
    
    float alpha = 0.001;

    for (int i = 0; i < 20; i ++) {
        lin_reg.train(alpha);
    }
    cout << "\nFunción regresión lineal:    y = " << lin_reg.getB(0) << " + " << lin_reg.getB(1) << " * x1 + " << lin_reg.getB(2) << " * x2" << endl;
    return 0;
}

