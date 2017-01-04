#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <sstream>

/* log_regr.h */

class LogisticRegresion{
  private:

    std::ifstream fp;
    int input_tuple_tam;
    int output_tuple_tam;
    int NUM_INSTANCES;

    std::vector< std::vector<float> > inputs;
    std::vector< std::vector<float> > outputs;
    std::vector< std::vector<float> > bias;

    static const float lr;
    static const float iden;
    static const float neutro;
    void addBiases();
    void addInputData(bool add_vectors, std::vector<std::string> tokens);
    void addOutputData(bool add_vectors, std::vector<std::string> tokens);
    std::vector<std::string> parseLine(std::string line);

  public:
    LogisticRegresion();
    LogisticRegresion(char const * fichero);
    ~LogisticRegresion();
    std::string getStringBias();
    float getInput(int i0, int i1);
    float calculatePrediction(float output);
    float calculateOutput(int input_index, int output_index);
    void recalculateBias(int input_index, int output_index, float pred);
    int trainModel();



};
