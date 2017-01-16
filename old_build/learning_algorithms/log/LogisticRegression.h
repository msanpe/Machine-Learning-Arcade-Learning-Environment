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
    float data_min;
    float data_max;

    static const float lr;
    static const float iden;
    static const float neutro;
    static const float kMAX_FLOAT;

    void addBiases();
    void addInputData(bool add_vectors, std::vector<std::string> tokens);
    void addOutputData(bool add_vectors, std::vector<std::string> tokens);
    void split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> parseLine(const std::string &s, char delim);
    float calculatePrediction(float output);
    float calculateOutputFromInstance(std::vector<float> inputs, int output_index);
    float calculateOutputFromImputFile(int input_index, int output_index);
    void recalculateBias(int input_index, int output_index, float pred);

  public:
    LogisticRegression();
    LogisticRegression(std::string filename);
    ~LogisticRegression();

    int getDatasetSize();
    int getInputSize();
    int getOutputSize();
    std::string getStringBias();
    float getMinFromData();
    float getMaxFromData();

    void setInputSize(int tam);
    void setOutputSize(int tam);

    bool toggleDiscrete();
    void normalizeData();
    void normalizeInstance(std::vector <float> &input);
    bool openFile(std::string filename);
    int trainModel();
    void testOnFile();
    std::vector<float> testOnInstance(std::vector <float> input);
};
