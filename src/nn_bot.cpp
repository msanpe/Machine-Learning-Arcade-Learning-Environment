#include <iostream>
#include <cmath>
#include "../include/nn.h"
#include "../include/utils.h"

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
NeuralNetwork NN;

int input = 3;
int hidden = 10;
int out = 2;
double alfa = 0.1f;
double learn = 0.5f;

double OBi[] = {5.335289, -3.453761};

double HBi[] = {-45.999525,37.993412,43.771578,13.377170,95.645671,11.262282,5.205646,6.245962,6.334917,6.337150};

double inputW[] = {51.185716,-45.102032,-30.954749,-42.671695,-108.496581,-37.194303,-17.389026,-15.229254,-15.525554,-15.524927,34.306819,-63.677966,-26.290950 ,3.435675,-39.796685,11.595467,-4.287363,-26.270758,-26.267267,-26.275897,-1.824937,18.567471,-38.301395,-35.564518,-70.818409,-30.121436,-11.036599,3.942000,3.807325,3.808696}

double hiddenW[] = {-7.010166,-4.854041,-16.651222,6.329404,-9.945141,9.130411,24.685573,-9.951565,8.457381,-19.347575,20.706092,-6.759163,8.895047,-2.970118,2.974869,-7.278487,2.650856,-7.773801 ,.660687,-7.7714,}

double DWeigH[] = {0.000001,-0.000000}

double HDWeigh[] = {-0.005445,-0.275051,-0.000000,0.000000,0.000000,0.000000,0.000000,0.000003,0.000003,0.000003}


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

void init(){

  alei.setInt("random_seed", 0);
  alei.setFloat("repeat_action_probability", 0);
  alei.setBool("display_screen", true);
  alei.setBool("sound", true);
 //printf("Phone Number: %s\n", game_name);
  alei.loadROM(game_name);

  srand(time(NULL));
  lastLives = alei.lives();
  totalReward = .0f;
  botInit();
}

void botInit() {
 NN.loadBot(int input, int hidden, int out, double alfa, double learnR,
                             double OBi, double HBi,
                             double inputW, double hiddenW,
                             double DWeigH, double HDWeigh);
}

void play(){

  float arr[2][4] = {
                                  {-15.2762,17.0575,-0.448558,-2.23137},
                                  {12.8465,-16.9949,1.43864,0.192643}
                                 };

  std::vector< std::vector<float> > biases;

  for(int i=0; i<2; i++){
    std::vector<float> v;
    for(int j = 0; j<4; j++){
      v.push_back(arr[i][j]);
    }
    biases.push_back(v);
  }


  logistic.setInputSize(3);
  logistic.setOutputSize(2);

  logistic.setBotInfo(8, 142, biases);

  init();

  int step;

  for (step = 0; !alei.game_over() && step < maxSteps; ++step)
  {
     alei.act(PLAYER_A_FIRE);
     totalReward += agentStep();
  }
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
   play();

   return 0;
}