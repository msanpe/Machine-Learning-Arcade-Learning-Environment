#include <iostream>
#include <cmath>
#include "learning_algorithms/log/LogisticRegression.h"
#include "utils/utils.h"

#define NONE_ID 0
#define TENNIS_ID 1
#define FREEWAY_ID 2

// Global vars
std::string path="datos/";
const int maxSteps = 7500;
char const * game_name;
int game_id;
int lastLives;
float totalReward;
ALEInterface alei;
LogisticRegression logistic;

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

///////////////////////////////////////////////////////////////////////////////
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
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

  outputs = logistic.testOnInstance(v_inputs);
  for(int i=0; i<outputs.size(); i++){
    if(outputs[i] > max){
      max = outputs[i];
      action = i;
    }
  }

  doAction(action, reward);

  return reward;
}

void toggleDiscrete(void) {
    logistic.toggleDiscrete();
}

///////////////////////////////////////////////////////////////////////////////
/// Print usage and exit
///////////////////////////////////////////////////////////////////////////////

void trainLogReg(){
  int epochs = 10;
  std::string filename;

  std::cout << "Introduce el nombre del fichero (archivo txt sin extension)" << std::endl;
  std::cin >> filename;

  path.append(filename);
  path.append(".txt");
  logistic.openFile(path);
  std::cout << "Introduce el numero de epocas" << std::endl;
  std::cin >> epochs;
  for(int i = 0; i<epochs; i++){
    std::cout << "-----EPOCA: " << i << "-----" << std::endl;
    logistic.trainModel();
  }

  std::cout << "BIAS FINALES: " << logistic.getStringBias() << std::endl;
  std::cout << "Regresion entrenada OK" << std::endl;
}

void init(){

  alei.setInt("random_seed", 0);
  alei.setFloat("repeat_action_probability", 0);
  alei.setBool("display_screen", true);
  alei.setBool("sound", true);
    std::cout << path << std::endl;
 //printf("Phone Number: %s\n", game_name);
  alei.loadROM(game_name);

  srand(time(NULL));
  lastLives = alei.lives();
  totalReward = .0f;

}

void testLogReg(){

  init();

  int step;

  for (step = 0; !alei.game_over() && step < maxSteps; ++step)
  {

     alei.act(PLAYER_A_FIRE);
     totalReward += agentStep();
  }

}

int menu(){
  int num = 0;
  int m;

  std::cout << std::endl;
  std::cout << "Menu" << std::endl;
  std::cout << "1. Train on file" << std::endl;
  std::cout << "2. Toggle Discrete" << std::endl;
  std::cout << "3. Test on game" << std::endl;
  std::cout << "4. Exit" << std::endl;
  std::cin >> m;
  std::cout << std::endl;

  switch (m) {
    case 1:
      trainLogReg();
    case 2:
      toggleDiscrete();
      break;
    case 3:
      testLogReg();
      break;
    case 4:
      return 1;
    default:
      return 0;
  }
  return num;
}

///////////////////////////////////////////////////////////////////////////////
/// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  int c = 0;
   // Check input parameter
   if (argc != 2)
      usage(argv[0]);
   game_name = argv[1];
   game_id=parseArgs(argc, argv, path);
   while (c != 1) {
       c = menu();
   }

   return 0;
}
