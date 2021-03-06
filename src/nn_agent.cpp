#include <time.h>
#include <iostream>
#include <cmath>

#include "../include/nn.h"
#include "../include/data.h"

#include "../include/utils.h"


#define NONE_ID 0
#define TENNIS_ID 1
#define FREEWAY_ID 2

std::string path="datos/";
const int maxSteps = 7500;
char const * game_name;
int game_id;
int lastLives;
data dataSet;
float totalReward;
ALEInterface alei;

double localError,       //Variable de error local para el entrenamiento
       meanSquaredError,        //mean sqared error, para detener el entrenamiento
       *tmpIn,          //Array que contiene los patrones de entrada de forma temporal
       *tmpOut;         //Array que contiene los patrones de salida de forma temporal
int trainSize, j, i;
int inp, hid, outp;    // layer sizes
std::string f, n, train;     // file names
int m;
int c = 0; // control del main loop
bool activateDiscrete = true;   // varia entre discreto y continuo
NeuralNetwork NN;

int doAction(int action,  float &reward){

  if(action == PLAYER_A_LEFT){
    reward += alei.act(PLAYER_A_LEFT);
    return PLAYER_A_LEFT;
  }else if(action == PLAYER_A_RIGHT){
    reward += alei.act(PLAYER_A_RIGHT);
    return PLAYER_A_RIGHT;
  }else if(action == PLAYER_A_UP){
    reward += alei.act(PLAYER_A_UP);
    return PLAYER_A_UP;
  }else if(action == PLAYER_A_DOWN){
    reward += alei.act(PLAYER_A_DOWN);
    return PLAYER_A_DOWN;
  }
  else{
    reward += alei.act(PLAYER_A_NOOP);
    return PLAYER_A_NOOP;
  }

  return PLAYER_A_NOOP;
}

void init(){

  alei.setInt("random_seed", 0);
  alei.setFloat("repeat_action_probability", 0);
  alei.setBool("display_screen", true);
  alei.setBool("sound", true);

  alei.loadROM(game_name);

  srand(time(NULL));
  lastLives = alei.lives();
  totalReward = .0f;

}

void trainNet(void) {
  std::cout << "Number of inputs: " << std::endl;
  std::cin >> inp;
  std::cout << "Number of elements in hidden layer: " << std::endl;
  std::cin >> hid;
  std::cout << "Number of outputs: " << std::endl;
  std::cin >> outp;
  std::cout << "Training data file: " << std::endl;
  std::cin >> train;
  path.append(train);
  path.append(".txt");
  std::cout << "Filename to save net: " << std::endl;
  std::cin.width (31);
  std::cin >> n;
  srand(time(NULL));

  NN.inputNum = inp;
  NN.outputNum = outp;
  NN.hiddenNum = hid;
  NN.targetNum = outp;
  NN.createNet();
  tmpIn = new double[NN.inputNum];
  tmpOut = new double[NN.outputNum];

  dataSet.setData(NN.inputNum, NN.outputNum, path);
  dataSet.readFile();
  trainSize = dataSet.setSize();


  NN.LR = 0.5f;      // learning rate
  NN.Alpha = 0.1f;    // Momentum
  NN.LambdaIH = 0;      // Regularization strength
  NN.LambdaHO = 0;      // Regularization strength
  meanSquaredError = 1000;        // para detener la red en el error deseado

  // entrenamiento
  for (i = 0; meanSquaredError > 0.01; ++i) { // entrena hasta error deseado
      std::cout << "Epoch " << i << " ----> ";
      if (i == 10000) {  // limite que evita loops infinitos
          std::cout << "Training is taking too many epochs" << std::endl;
          break;
      }
      //Pasa 1 a 1 los patrones de entrada y salida y entrena la red
      for (int current = 0; current < trainSize; current++) {
          dataSet.getTrainingData(current, tmpIn, tmpOut);
          for (j = 0; j < NN.inputNum; j++) {
              NN.Inputs[j] = tmpIn[j];
          }
          for (j = 0; j < NN.outputNum; j++) {
              NN.Target[j] = tmpOut[j];
          }
          NN.trainNet();
          NN.testNet();
          /*for (j = 0; j < NN.inputNum; j++) {
              std::cout << NN.Inputs[j] << " ";
          }
          std::cout << " ---> ";
          for (j = 0; j < NN.outputNum; j++) {
              std::cout << NN.Outputs[j] << " ";
          }
          std::cout << std::endl;*/
          for (j = 0; j < NN.outputNum; j++) {
              double delta = NN.Target[j] - NN.Outputs[j];
              localError += delta * delta;
          }

      }
      // epoch error calculation
      meanSquaredError = localError/NN.outputNum;
      std::cout << " " << "Error: " << meanSquaredError << std::endl;
      localError = 0;
  }
  std::cout << "Training ended in " << i << " epochs" << std::endl;
  NN.saveNet(const_cast<char *>(n.c_str()));
  std::cout << "Network weights saved in " << n << std::endl;
}




// cambia entre salidas discretas/continuas
void toggleDiscrete(void) {
    if (activateDiscrete == false) {
        activateDiscrete = true;
        std::cout << "Discrete data activated" << std::endl;
    } else {
        activateDiscrete = false;
        std::cout << "Discrete data deactivated" << std::endl;
    }
}

// convierte de continuo a discreto
double convertDiscrete(double a) {
    if (activateDiscrete == true) {
        if (a < 0.5) return 0;
        if (a >= 0.5) return 1;
    } else return a;
}

void useNet(void) {
    float val;
    std::cout << "Ingresar los valores de entrada a la red:" << std::endl;
    for (i = 0; i < NN.inputNum; i++) {
        std::cout << "Input " << (i + 1) << std::endl;
        std::cin >> val;
        NN.Inputs[i] = (val - dataSet.getMinVal()) / (dataSet.getMaxVal() - dataSet.getMinVal());
        std::cout << std::endl;
    }
    NN.testNet();
    std::cout << "Output:" << std::endl;
    for (int i = 0; i < NN.outputNum; i++) {
        std::cout << convertDiscrete(NN.Outputs[i]) << std::endl;
    }
}

std::vector<float> testOnInstance(std::vector <float> input){
  std::vector<float> outputs;
  float z_value;
  int i;

  for (i = 0; i < NN.inputNum; i++) {
      NN.Inputs[i] = input[i];
  }
  NN.testNet();
  for (i = 0; i < NN.outputNum; i++) {
      outputs.push_back(convertDiscrete(NN.Outputs[i]));
  }
  return outputs;
}

float agentStep() {

  float z_value;
  float max = -1;
  int action = 0;
  std::vector<float> outputs;

  std::vector<float> v_inputs;

  float reward = 0;

  switch(game_id){
    case TENNIS_ID:
         addTennisInputData(v_inputs, alei);
         break;
    case FREEWAY_ID:
         addFreewayInputData(v_inputs, alei);
         break;


    default:
         v_inputs.push_back(0);
         break;
  }

  for(int i=0; i<v_inputs.size(); i++){
    z_value = ((v_inputs[i] - dataSet.getMinVal()) / (dataSet.getMaxVal() - dataSet.getMinVal()));
    v_inputs[i] = z_value;
  }

  outputs = testOnInstance(v_inputs);

  for(int i=0; i<outputs.size(); i++){
    if(outputs[i] == 1){
      if(game_id == FREEWAY_ID){
        if(i == 0){
          action = 2;
        }else if(i == 1){
          action = 5;
        }
      }else{
          action = i+3;
      }
    }
  }

  doAction(action, reward);

  return reward;
}

void useNetOnGame(void) {
  init();

  int step;

  for (step = 0; !alei.game_over() && step < maxSteps; ++step)
  {  if((step%100) == 0) alei.act(PLAYER_A_FIRE);
     totalReward += agentStep();
  }
}

void loadNet();

int menu(void) {
    std::cout << std::endl;
    std::cout << "Menu" << std::endl;
    std::cout << "1. Train new network" << std::endl;
    std::cout << "2. Switch between continuos and discrete outputs" << std::endl;
    std::cout << "3. Test network" << std::endl;
    std::cout << "4. Test on game" << std::endl;
    std::cout << "5. Load network" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cin >> m;
    std::cout << std::endl;

    if (m == 1){
      trainNet();
      return 10;
    }
    else if (m == 2) toggleDiscrete();
    else if (m == 3) useNet();
    else if (m == 4) useNetOnGame();
    else if (m == 5) loadNet();
    else if (m == 6) return 1;
    else             return 0;
}

void loadNet() {
    std::cout << "Insert weights file: " << std::endl;
    std::cin.width(31);
    std::cin >> f;
    NN.loadNet(const_cast<char *>(f.c_str()));  //Recupera la red del archivo dado
    dataSet.setData(NN.inputNum, NN.outputNum, f.append("_data"));
    dataSet.readFile();
    menu();
}

int main(int argc, char **argv) {
  int c = 0;

  if (argc != 2)
     usage(argv[0]);
  game_name = argv[1];
  game_id=parseArgs(argc, argv, path);
  std::cout << path << std::endl;

    while (c != 1) {
        c = menu();
    }
}
