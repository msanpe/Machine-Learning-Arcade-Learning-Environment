#include "LogisticRegression.h"

/* log_regr.cpp*/

const float LogisticRegression::lr = 0.3;
const float LogisticRegression::iden = 1.0;
const float LogisticRegression::neutro = 0.0;


LogisticRegression::LogisticRegression(){
  input_tuple_tam = 0;
  output_tuple_tam = 0;
  NUM_INSTANCES = 0;
  discrete = false;
}

LogisticRegression::LogisticRegression(std::string filename){
  input_tuple_tam = 0;
  output_tuple_tam = 0;
  NUM_INSTANCES = 0;
  discrete = false;
  openFile(filename);
}

LogisticRegression::~LogisticRegression(){
}


std::vector<std::string> LogisticRegression::parseLine(std::string line){

  char c_line[40];
  std::vector<std::string> tokens;
  char * token;
  char seps[]   = " ,\t\n";

  strcpy(c_line, line.c_str());

  token = strtok(c_line, seps );
  while( token != NULL )
  {
     tokens.push_back(token);
     token = strtok( NULL, seps );
  }

  return tokens;
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
  for(int i=0; i<input_tuple_tam; i++){
    if(add_vectors){
      std::vector<float> v;
      v.push_back((float)atof(tokens[i].c_str()));
      inputs.push_back(v);
    }else{
      inputs[i].push_back((float)atof(tokens[i].c_str()));
    }
  }
}

void LogisticRegression::addOutputData(bool add_vectors, std::vector<std::string> tokens){
  for(int i=0; i<output_tuple_tam; i++){
    if(add_vectors){
      std::vector<float> v;
      v.push_back((float)atof(tokens[i].c_str()));
      outputs.push_back(v);
    }else{
      outputs[i].push_back((float)atof(tokens[i].c_str()));
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

float LogisticRegression::calculatePrediction(float output)
{
  return iden / (iden + exp(-output));
}

void LogisticRegression::recalculateBias(int input_index, int output_index, float pred){
    for(int j = 0; j<=input_tuple_tam; ++j){
      if(j < input_tuple_tam){
        bias[output_index][j] = bias[output_index][j] + lr * (outputs[output_index][input_index] - pred) * pred * (1 - pred) * inputs[j][input_index];
      }else if(j == input_tuple_tam){
        bias[output_index][j] = bias[output_index][j] + lr * (outputs[output_index][input_index] - pred) * pred * (1 - pred) * iden;
      }
    }
}

bool LogisticRegression::toggleDiscrete(){
  discrete = !discrete;
  return discrete;
}

int LogisticRegression::getDatasetSize(){
  return NUM_INSTANCES;
}
std::string LogisticRegression::getStringBias(){
  std::stringstream ss;

  for(int i =0; i<output_tuple_tam; i++){
    for(int j=0; j<=input_tuple_tam; j++){
      ss << "Bias (" << i << "," << j << "): " << bias[i][j] << " |-| ";
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
  return 0;
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
          if(!first_read){
            first_read = true;

            tokens =  parseLine(line);
            input_tuple_tam = tokens.size();
            addInputData(true, tokens);

            tokens.clear();
             // el bias de desplazamiento

            std::getline(fp, line);
            tokens = parseLine(line);
            output_tuple_tam = tokens.size();
            addOutputData(true, tokens);
            tokens.clear();
            addBiases();

          }else{

            tokens = parseLine(line);
            addInputData(false, tokens);
            tokens.clear();

            std::getline(fp, line);
            tokens = parseLine(line);
            addOutputData(false, tokens);
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
    */
  return salida;
}
