#include <iostream>
#include <cmath>
#include "../include/LogisticRegression.h"
#include "../include/utils.h"

#define NONE_ID 0
#define TENNIS_ID 1
#define FREEWAY_ID 2

// Global vars
std::string sfilename;
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

  int n;
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


         addTennisInputData(v_inputs, alei);

         break;
    case FREEWAY_ID:
         addFreewayInputData(v_inputs, alei);
         break;
    default:
         v_inputs.push_back(0);
         break;
  }

  logistic.normalizeInstance(v_inputs);
  outputs = logistic.testOnInstance(v_inputs);
  for(int i=0; i<outputs.size(); i++){
    if(outputs[i] == 1){
      action = i+3;
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

  std::ofstream save_file;

  int n1, n2;
  int epochs = 10;
  std::string filename;

  std::cout << "Introduce el nombre del fichero (archivo txt sin extension)" << std::endl;
  //filename = "datos_tennis_bueno";
  std::cin >> filename;
  std::cout << "Numero de entradas:" << std::endl;
  //n1 = 3;
  std::cin >> n1;
  std::cout << "Numero de saldas:" << std::endl;
  //n2 = 2;
  std::cin >> n2;

  std::cout << "Introduce el nombre del fichero para salvar los bias" << std::endl;
  //filename = "datos_tennis_bueno";
  std::cin >> sfilename;

  path.append(filename);
  path.append(".txt");
  logistic.setInputSize(n1);
  logistic.setOutputSize(n2);
  logistic.openFile(path);
  logistic.normalizeData();
  std::cout << "Introduce el numero de epocas" << std::endl;
  std::cin >> epochs;
  for(int i = 0; i<epochs; i++){
    std::cout << "-----EPOCA: " << i << "-----" << std::endl;
    logistic.trainModel();
  }

  std::cout << "BIAS FINALES: " << logistic.getStringBias() << std::endl;
  save_file.open(sfilename.c_str(), std::ios::out | std::ios::trunc);
  if(save_file.is_open()){
    save_file << n1 << std::endl;
    save_file << n2 << std::endl;
    save_file << logistic.getMinFromData() << std::endl;
    save_file << logistic.getMaxFromData() << std::endl;
    save_file << logistic.getStringBias();
  }
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


void useLogReg(void) {
    float n;
    std::vector<float> inps;
    std::vector<float> n_inps;
    std::vector<float> outps;

    std::cout << "Ingresar los valores de entrada a la red:" << std::endl;
    for (int i = 0; i < logistic.getInputSize(); i++) {
        std::cout << "Input " << (i + 1) << std::endl;
        std::cin >> n;
        inps.push_back(n);
    }
    logistic.normalizeInstance(inps);
    outps = logistic.testOnInstance(inps);
    std::cout << "Output:" << std::endl;
    for (int i = 0; i < logistic.getOutputSize(); i++) {
        std::cout << outps[i] << std::endl;
    }
}

void loadLogReg(){
    std::string filename;
    std::cout << "Ingresa el nombre del fichero de bias:" << std::endl;
    std::cin >> filename;
    logistic.loadBiases(filename);
    std::cout << logistic.getStringBias() << std::endl;
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
  std::cout << "4. Test on single instance" << std::endl;
  std::cout << "5. Load BIAS file" << std::endl;
  std::cout << "6. Exit" << std::endl;
  std::cin >> m;
  std::cout << std::endl;

  switch (m) {
    case 1:
      trainLogReg();
      break;
    case 2:
      toggleDiscrete();
      break;
    case 3:
      testLogReg();
      break;
    case 4:
      useLogReg();
      break;
    case 5:
      loadLogReg();
      break;
    case 6:
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
