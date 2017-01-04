#include "LogisticRegresion.h"

/* log_regr.cpp*/

const float LogisticRegresion::lr = 0.3;
const float LogisticRegresion::iden = 1.0;
const float LogisticRegresion::neutro = 0.0;


LogisticRegresion::LogisticRegresion(){
  input_tuple_tam = 0;
  NUM_INSTANCES = 0;
}

LogisticRegresion::~LogisticRegresion(){

}

std::vector<std::string> LogisticRegresion::parseLine(std::string line){

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

void LogisticRegresion::addBiases(){
  for(int i=0; i<output_tuple_tam; i++){
    std::vector<float> v;
    for(int j=0; j<=input_tuple_tam; j++){
      v.push_back(neutro);
    }
    bias.push_back(v);
  }
}

void LogisticRegresion::addInputData(bool add_vectors, std::vector<std::string> tokens){
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

void LogisticRegresion::addOutputData(bool add_vectors, std::vector<std::string> tokens){
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

LogisticRegresion::LogisticRegresion(char const * fichero){

  bool tam_read = false;
  bool first_read = false;

  std::string line;
  std::vector<std::string> tokens;

  fp.open(fichero, std::ifstream::in);

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
          addBiases();
          tokens.clear();
           // el bias de desplazamiento

          std::getline(fp, line);
          tokens = parseLine(line);
          output_tuple_tam = tokens.size();
          addOutputData(true, tokens);
          tokens.clear();

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
}

std::string LogisticRegresion::getStringBias(){
  std::stringstream ss;

  for(int i =0; i<output_tuple_tam; i++){
    for(int j=0; j<=input_tuple_tam; j++){
      ss << "Bias (" << i << "," << j << "): " << bias[i][j] << " |-| ";
    }
    ss << "\n";
  }

  return ss.str();//bias[index];
}

float LogisticRegresion::getInput(int i0, int i1){
  return 0;//inputs[i0][i1];
}

float LogisticRegresion::calculateOutput(int input_index, int output_index)
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

float LogisticRegresion::calculatePrediction(float output)
{
  return iden / (iden + exp(-output));
}

void LogisticRegresion::recalculateBias(int input_index, int output_index, float pred){
    for(int j = 0; j<=input_tuple_tam; ++j){
      if(j < input_tuple_tam){
        bias[output_index][j] = bias[output_index][j] + lr * (outputs[output_index][input_index] - pred) * pred * (1 - pred) * inputs[j][input_index];
      }else if(j == input_tuple_tam){
        bias[output_index][j] = bias[output_index][j] + lr * (outputs[output_index][input_index] - pred) * pred * (1 - pred) * iden;
      }
    }
}

int LogisticRegresion::trainModel(){
  float output;
  float p;

  for(int i = 0; i < NUM_INSTANCES; ++i){
    for(int j = 0; j< output_tuple_tam; j++){

      output = calculateOutput(i, j);

      p = calculatePrediction(output);

      recalculateBias(i, j, p);
    }
  }
  std::cout << output_tuple_tam << "\n";
  return 0;
}
