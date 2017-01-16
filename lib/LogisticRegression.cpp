#include "LogisticRegression.h"

/* log_regr.cpp*/

const float LogisticRegression::lr = 0.4;
const float LogisticRegression::iden = 1.0;
const float LogisticRegression::neutro = 0.0;
const float LogisticRegression::kMAX_FLOAT = 9999;


LogisticRegression::LogisticRegression(){
  input_tuple_tam = 0;
  output_tuple_tam = 0;
  NUM_INSTANCES = 0;
  data_min = kMAX_FLOAT;
  data_max = -kMAX_FLOAT;
  discrete = true;
  trained = false;
}

LogisticRegression::LogisticRegression(std::string filename){
  input_tuple_tam = 0;
  output_tuple_tam = 0;
  NUM_INSTANCES = 0;
  data_min = kMAX_FLOAT;
  data_max = -kMAX_FLOAT;
  discrete = true;
  trained = false;
  openFile(filename);
}

LogisticRegression::~LogisticRegression(){
}


void LogisticRegression::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> LogisticRegression::parseLine(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void LogisticRegression::addBiases(){
  for(int i=0; i<output_tuple_tam; i++){
    std::vector<float> v;
    for(int j=0; j<=input_tuple_tam; j++){
      v.push_back(neutro);
    }
    bias.push_back(v);
  }
}

void LogisticRegression::addInputData(bool add_vectors, std::vector<std::string> tokens){

  float valor;

  for(int i=0; i<input_tuple_tam; i++){
    if(add_vectors){
      std::vector<float> v;
      valor = (float)atof(tokens[i].c_str());
      v.push_back(valor);
      inputs.push_back(v);
    }else{
      valor = (float)atof(tokens[i].c_str());
      inputs[i].push_back(valor);
    }
  }
}

void LogisticRegression::addOutputData(bool add_vectors, std::vector<std::string> tokens){
  for(int i=0; i<output_tuple_tam; i++){
    if(add_vectors){
      std::vector<float> v;
      v.push_back((float)atof(tokens[i + input_tuple_tam].c_str()));
      outputs.push_back(v);
    }else{
      outputs[i].push_back((float)atof(tokens[i + input_tuple_tam].c_str()));
    }
  }
}

float LogisticRegression::calculateOutputFromInstance(std::vector<float> inputs, int output_index)
{
  float output = 0;
  for(int i = 0; i<=input_tuple_tam; i++){

    if(i < input_tuple_tam){

        output += bias[output_index][i] * inputs[i];

    }else{
        output += bias[output_index][i];
    }
  }
  return output;
}

float LogisticRegression::calculateOutputFromImputFile(int input_index, int output_index)
{
  float output = 0;
  for(int i = 0; i<=input_tuple_tam; i++){

    if(i < input_tuple_tam){

        output += bias[output_index][i] * inputs[i][input_index];

    }else{
        output += bias[output_index][i];
    }
  }
  return output;
}

void LogisticRegression::setInputSize(int tam){
  input_tuple_tam = tam;
}
void LogisticRegression::setOutputSize(int tam){
  output_tuple_tam = tam;
}

void LogisticRegression::setBotInfo(float min, float max, std::vector< std::vector<float> > &biases){
  data_min = min;
  data_max = max;
  bias = biases;
}

float LogisticRegression::calculatePrediction(float output)
{
  return iden / (iden + exp(-output));
}

void LogisticRegression::recalculateBias(int input_index, int output_index, float pred){

    float error = outputs[output_index][input_index] - pred;
    for(int j = 0; j<=input_tuple_tam; ++j){
      if(j < input_tuple_tam){
        bias[output_index][j] = bias[output_index][j] + lr * error * pred * (iden - pred) * inputs[j][input_index];
      }else if(j == input_tuple_tam){
        bias[output_index][j] = bias[output_index][j] + lr * error * pred * (iden - pred) * iden;
      }
    }

}

void LogisticRegression::loadBiases(std::string filename){
  std::ifstream bias_file;
  std::string line;
  std::vector<float> v;
  std::vector<std::string> tokens;
  int cont = 0;

  bias_file.open(filename.c_str(), std::ifstream::in);

  if(bias_file.is_open()){
    while (std::getline(bias_file, line))
    {
        const std::string s = line;
        if(cont == 0){
          setInputSize(atoi(line.c_str()));
        }else if(cont == 1){
          setOutputSize(atoi(line.c_str()));
        }else if(cont == 2){
          data_min = (atof(line.c_str()));
        }else if(cont == 3){
          data_max = (atof(line.c_str()));
        }else{
          tokens = parseLine(s, ';');
          if(!trained){
            v.clear();

            for(int i=0; i<tokens.size(); i++){
              v.push_back((float)atof(tokens[i].c_str()));
            }
            bias.push_back(v);
          }else{
            for(int i=0; i<tokens.size(); i++){
              bias[cont-2][i] = (float)atof(tokens[i].c_str());
            }
          }
        }
        cont++;
    }
  }else{
    std::cout << "error al cargar el archivo" << std::endl;
  }
}

bool LogisticRegression::toggleDiscrete(){
  discrete = !discrete;
  return discrete;
}

int LogisticRegression::getInputSize(){
  return input_tuple_tam;
}

int LogisticRegression::getOutputSize(){
  return output_tuple_tam;
}

int LogisticRegression::getDatasetSize(){
  return NUM_INSTANCES;
}

float LogisticRegression::getMinFromData(){
  float min = kMAX_FLOAT;
  if(data_min == kMAX_FLOAT){
    for(int i = 0; i<input_tuple_tam; i++){
      for(int j = 0; j< NUM_INSTANCES; j++){
          if(inputs[i][j] < min){
              min = inputs[i][j];
          }
        }
      }
    data_min = min;
    return min;
  }else{
    return data_min;
  }

}

float LogisticRegression::getMaxFromData(){
  float max = -kMAX_FLOAT;

  if(data_max == -kMAX_FLOAT){
    for(int i = 0; i<input_tuple_tam; i++){
      for(int j = 0; j< NUM_INSTANCES; j++){
        if(inputs[i][j] > max){
            max = inputs[i][j];
        }
      }
    }
    data_max = max;
    return max;
  }else{
    return data_max;
  }
}

std::string LogisticRegression::getStringBias(){
  std::stringstream ss;

  for(int i =0; i<output_tuple_tam; i++){
    for(int j=0; j<=input_tuple_tam; j++){
      ss << bias[i][j] << ";";
    }
    ss << "\n";
  }

  return ss.str();//bias[index];
}


int LogisticRegression::trainModel(){
  float output;
  float p;

  for(int i = 0; i < NUM_INSTANCES; ++i){
    for(int j = 0; j< output_tuple_tam; j++){

      output = calculateOutputFromImputFile(i, j);

      p = calculatePrediction(output);

      recalculateBias(i, j, p);
    }
  }
  trained = true;

  return 0;
}

void LogisticRegression::normalizeInstance(std::vector <float> &input){

    float z_value;

    for(int i =0; i<input.size(); i++){
      z_value = ((input[i] - getMinFromData()) / (getMaxFromData() - getMinFromData()));
      input[i] = z_value;
    }
}

void LogisticRegression::normalizeData(){

  float z_value;
  for(int i =0; i < input_tuple_tam; i++){
    for(int j = 0; j < NUM_INSTANCES; j++){
      z_value = ((inputs[i][j] - getMinFromData()) / (getMaxFromData() - getMinFromData()));
      inputs[i][j] = z_value;
    }
  }

  std::cout << "Data normalized" << std::endl;
}

bool LogisticRegression::openFile(std::string filename){

  char const * fichero = filename.c_str();
  bool tam_read = false;
  bool first_read = false;
  std::ifstream fp;
  std::string line;
  std::vector<std::string> tokens;

  fp.open(fichero, std::ifstream::in);

  if(fp.is_open()){
    while (std::getline(fp, line))
    {

        if(!tam_read){
          tam_read = true;
          NUM_INSTANCES = atoi(line.c_str());
        }else{
          const std::string s = line;
          if(!first_read){
            first_read = true;
            tokens =  parseLine(s, ',');
            addInputData(first_read, tokens);
            addOutputData(first_read, tokens);
            tokens.clear();
             // el bias de desplazamiento
            addBiases();
          }else{
            tokens = parseLine(s, ',');
            addInputData(!first_read, tokens);
            addOutputData(!first_read, tokens);
            tokens.clear();
          }
      }
    }
  }else{
    return false;
  }

  return true;
}

void LogisticRegression::testOnFile(){

  std::ofstream rfile;
  std::stringstream results;
  std::string filename = "log_regr_results";

  float prediction;

  for(int j=0; j<NUM_INSTANCES; j++){
    for(int i = 0; i<output_tuple_tam; i++){
      prediction = calculatePrediction(calculateOutputFromImputFile(j, i));
      if(discrete){
        if(prediction < 0.5){
          prediction = 0;
        }else{
          prediction = 1;
        }
      }
      if(i == 0){
        results << "Instance: " << j << "-----> Prediction for output y" << i << ": " << prediction << "\n";
      }else{
        results << "           " << "-----> Prediction for output y" << i << ": " << prediction << "\n";
      }
    }
    results <<'\n';
  }

  if(discrete)
    filename.append("_d_on");
  else
    filename.append("_d_off");

  filename.append(".txt");

  rfile.open(filename.c_str(), std::ios::out | std::ios::trunc );

  if(rfile.is_open()){
    rfile << results.str();
    std::cout << "Archivo escrito" << std::endl;
    rfile.close();
  }else{
    std::cout << "Error al abrir el archivo" << std::endl;
  }
}

std::vector<float> LogisticRegression::testOnInstance(std::vector <float> input){


  std::vector<float> salida;
  int max = -9999;
  float prediction;

    for(int i = 0; i<output_tuple_tam; i++){
      prediction = calculatePrediction(calculateOutputFromInstance(input, i));
      if(discrete){
        if(prediction < 0.5){
          prediction = 0;
        }else{
          prediction = 1;
        }
      }
      salida.push_back(prediction);
    }/*


    for(int i = 0; i<salida.size(); i++){
      if(salida[i] > max){
        max = i;
      }
    }

    for(int i = 0; i<salida.size(); i++){
      if(i == max){
        salida[i] = 1;
      }else{
        salida[i] = 0;
      }
    }*/
  return salida;
}
