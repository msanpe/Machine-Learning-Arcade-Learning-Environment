#include <time.h>
#include <iostream>
#include <cmath>

#include "learning_algorithms/nn/nn.h"
#include "learning_algorithms/nn/data.h"

#include "utils/utils.h"


#define NONE_ID 0
#define TENNIS_ID 1
#define FREEWAY_ID 2

std::string path="datos/";
const int maxSteps = 7500;
char const * game_name;
int game_id;
int lastLives;
float totalReward;
ALEInterface alei;

double localError,       //Variable de error local para el entrenamiento
       meanSquaredError,        //mean sqared error, para detener el entrenamiento
       *tmpIn,          //Array que contiene los patrones de entrada de forma temporal
       *tmpOut;         //Array que contiene los patrones de salida de forma temporal
int trainSize, j, i;
int inp, hid, outp;    // layer sizes
std::string f, n;     // file names
int m;
int c = 0; // control del main loop
bool activateDiscrete = false;   // varia entre discreto y continuo
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
    std::cin.width(31);
    std::cin >> f;
    path.append(f);
    path.append(".txt");
    std::cout << "Filename to save net: " << std::endl;
    std::cin.width(31);
    std::cin >> n;


    srand(time(NULL));
    std::cout << path << std::endl;
    NN.inputNum = inp;
    NN.outputNum = outp;
    NN.hiddenNum = hid;
    NN.targetNum = outp;
    NN.createNet();
    tmpIn = new double[NN.inputNum];
    tmpOut = new double[NN.outputNum];

    data dataSet(NN.inputNum, NN.outputNum, const_cast<char *>(path.c_str()));
    dataSet.readFile();
    trainSize = dataSet.setSize();


    NN.LR = 0.5f;      // learning rate
    NN.Alpha = 0.3f;    // Momentum
    NN.Lambda = 1;      // Regularization strength
    meanSquaredError = 1000;        // para detener la red en el error deseado

    // entrenamiento
    for (i = 0; meanSquaredError > 0.01; ++i) { // entrena hasta error deseado
          std::cout << i << "-->" << meanSquaredError << std::endl;
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
            NN.testNet();
            for (j = 0; j < NN.inputNum; j++) {
              //  std::cout << NN.Inputs[j] << " ";
            }
            //std::cout << " ---> ";
            for (j = 0; j < NN.outputNum; j++) {
            //  std::cout << NN.Outputs[j] << " ";
            }
            //std::cout << std::endl;
            for (j = 0; j < NN.outputNum; j++) {
                double delta = NN.Target[j] - NN.Outputs[j];
                localError += delta * delta;
            }
            NN.trainNet();
        }
        // epoch error calculation
        meanSquaredError = localError/NN.outputNum;
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
    std::cout << "Ingresar los valores de entrada a la red:" << std::endl;
    for (i = 0; i < NN.inputNum; i++) {
        std::cout << "Input " << (i + 1) << std::endl;
        std::cin >> NN.Inputs[i];
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

  for (i = 0; i < NN.inputNum; i++) {
      NN.Inputs[i] = input[i];
  }
  NN.testNet();
  for (int i = 0; i < NN.outputNum; i++) {
      outputs.push_back(convertDiscrete(NN.Outputs[i]));
  }
  return outputs;
}

float agentStep() {

  float max = -1;
  int action;
  std::vector<float> outputs;


  if (alei.lives() != lastLives) {
     --lastLives;
     alei.act(PLAYER_A_FIRE);
  }


  std::vector<float> v_inputs;

  float reward = 0;

  switch(game_id){
    case TENNIS_ID:
         addTennisData(v_inputs, alei);
         break;
    case FREEWAY_ID:
         addFreewayData(v_inputs, alei);
         break;
    default:
         v_inputs.push_back(0);
         break;
  }


  outputs = testOnInstance(v_inputs);

  for(int i=0; i<outputs.size(); i++){
    if(outputs[i] == 1){
      action = i;
    }
  }

  doAction(action, reward);

  return reward;
}

void useNetOnGame(void) {
  init();

  int step;

  for (step = 0; !alei.game_over() && step < maxSteps; ++step)
  {  alei.act(PLAYER_A_FIRE);
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

    if (m == 1)      trainNet();
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
