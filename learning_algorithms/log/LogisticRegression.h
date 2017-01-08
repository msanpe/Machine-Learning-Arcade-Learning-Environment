#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <sstream>

/* log_regr.h */

class LogisticRegression{
  private:
    int input_tuple_tam;
    int output_tuple_tam;
    int NUM_INSTANCES;
    bool discrete;

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
    float calculatePrediction(float output);
    float calculateOutputFromInstance(std::vector<float> inputs, int output_index);
    float calculateOutputFromImputFile(int input_index, int output_index);
    void recalculateBias(int input_index, int output_index, float pred);

  public:
    LogisticRegression();
    LogisticRegression(std::string filename);
    ~LogisticRegression();

    int getDatasetSize();
    std::string getStringBias();
    bool toggleDiscrete();

    bool openFile(std::string filename);
    int trainModel();
    void testOnFile();
    std::vector<float> testOnInstance(std::vector <float> input);



};
