#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

class Perceptron{
  private:
    FILE *fp;
    int tuple_tam;
    float *weights;
    int theta;
    int global_error;
    int NUM_INSTANCES;
    std::vector< std::vector<int> > inputs;
    std::vector<int> outputs;

    float randomFloat();

  public:
    Perceptron();
    Perceptron(char const * fichero);
    ~Perceptron();


};

Perceptron::Perceptron(){
  tuple_tam = 0;
  fp = NULL;
  weights = NULL;
}

Perceptron::~Perceptron(){
  if(fp!=NULL){
    fclose(fp);
  }
}

Perceptron::Perceptron(char const * fichero){
  int k;

  if ((fp = fopen(fichero, "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
  }
  fscanf(fp, "%i", tuple_tam);
  weights = new int[tuple_tam];

  for(int i=0; i<tuple_tam; i++){
    std::vector<int> v;
    inputs.push_back(v);
  }

  while (fscanf(fp, "(%i,%i,%i)", x1, x2, y) != EOF) {
      inputs[0].push_back(x1);
      inputs[1].push_back(x2);
      if (y == 0) {
          y = -1;
      }
      outputs.push_back(y);
      k++;
  }
  NUM_INSTANCES = k;
}

float Perceptron::randomFloat()
{
    return (float)rand() / (float)RAND_MAX;
}

char const* Perceptron::tupleFormat(int tam){

  std::string mistring = "(";
  for (int i =0; i<tam; i++){
    if(i!<tam-1)
      mistring.append("%i,");
    else
      mistring.append("%i");
  }
  mistring.append(")");
  char const * cstr = str.c_str();
  return cstr;
}

int calculateOutput(int x, int y)
{
    float sum = x * weights[0] + y * weights[1] + weights[2];
    return (sum >= 0) ? 1 : -1;
}

Perceptron::train(){

  char const * cstr = tupleFormat(tuple_tam);
  int i = tuple_tam;
  int * actual_weight = weights;
  int it_number = 0;
  int local_error;
  int x1, x2, y;


  while(i){
    --i;
    actual_weight = randomFloat();
    ++actual_weight;
  }

  do{
    it_number++;
    global_error = 0;
    for(int i =0 ; i<NUM_INSTANCES; i++){
      y = calculateOutput(inputs[0][i],inputs[1][i]);
      local_eror = outputs[i] - y
      weights[0] += LEARNING_RATE * local_error * inputs[0][i];
      weights[1] += LEARNING_RATE * local_error * inputs[1][i];
      weights[2] += LEARNING_RATE * local_error;

      global_error += (localError*localError);
    }
  }while(global_error != 0 && it_number<=100);
}


int main(int argc, char *argv[])
{
    char const *filename = "datos.txt";
    Perceptron tron(filename);
    std::cout << "jodeeeeeeeeer" << std::endl;
}
