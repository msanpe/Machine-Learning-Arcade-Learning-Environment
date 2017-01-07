
/* 
 * Author: cristina
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
    int getTam();
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
    
    int n = 0;
    string s;
    while(ifs >> s){
        for(int i=0; i<tam-1; i++){
            inputs[i].push_back(atof(s.c_str()));
            ifs >> s;
        }
        outputs.push_back(atof(s.c_str()));
        n++;
    }
    N = n;   
}
void LinearRegression::actualizarB(float alpha, float err, int idx){    
    b[0] = b[0] - alpha * err;
    for(int i=1; i<tam; i++){
        b[i] = b[i] - alpha * err * inputs[i-1][idx];
    }
}
float LinearRegression::getB(int n){
    return b[n];
}

int LinearRegression::getTam(){
    return tam;
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
    for (int j = 1; j <getTam(); j++){
       cout << ", b" << j << " = " <<  this->getB(j) ;
    }
    cout << ", error " << cont++ << " " << err << endl;
}
int main(int argc, char** argv) {

    string file = "datos.txt";
    LinearRegression lin_reg;
    lin_reg.init(file);
    
    float alpha = 0.001;
    int epoches = 20;
    for (int i = 0; i < epoches; i ++) {
        lin_reg.train(alpha);
    }
    
    cout << "\nFunción regresión lineal:    y = (" ;
    cout << lin_reg.getB(0) << ")" ;
    for(int j = 1; j<lin_reg.getTam(); j++){
        cout << " + (" << lin_reg.getB(j) << ") * x" << j;
    }
    cout << endl;
    
    return 0;
}

