#include "utils.h"

int getTennisPlayerX(ALEInterface &alei) {
   return alei.getRAM().get(26) + ((rand() % 3) - 1);
}
int getTennisPlayerY(ALEInterface &alei) {
   return alei.getRAM().get(24) + ((rand() % 3) - 1);
}
int getTennisEnemyX(ALEInterface &alei) {
   return alei.getRAM().get(27) + ((rand() % 3) - 1);
}
int getTennisEnemyY(ALEInterface &alei) {
   return alei.getRAM().get(53) + ((rand() % 3) - 1);
}
int getTennisBallX(ALEInterface &alei) {
   return alei.getRAM().get(55) + ((rand() % 3) - 1);
}
int getTennisBallY(ALEInterface &alei) {
   return alei.getRAM().get(16) + ((rand() % 3) - 1);
}
int getTennisBallZ(ALEInterface &alei) {
   return alei.getRAM().get(17) + ((rand() % 3) - 1);
}

float getFreeWayCalle(int param, ALEInterface &alei){

    float valor = 0;
     switch(param){
       case 0:
            valor = alei.getRAM().get(43) + ((rand() % 3) - 1);
            break;
       case 1:
            valor = alei.getRAM().get(44) + ((rand() % 3) - 1);
            break;
       case 2:
            valor = alei.getRAM().get(45) + ((rand() % 3) - 1);
            break;
       case 3:
            valor = alei.getRAM().get(46) + ((rand() % 3) - 1);
            break;
       case 4:
            valor = alei.getRAM().get(47) + ((rand() % 3) - 1);
            break;
       case 5:
            valor = alei.getRAM().get(112) + ((rand() % 3) - 1);
            break;
       case 6:
            valor = alei.getRAM().get(111) + ((rand() % 3) - 1);
            break;
       case 7:
            valor = alei.getRAM().get(110) + ((rand() % 3) - 1);
            break;
       case 8:
            valor = alei.getRAM().get(109) + ((rand() % 3) - 1);
            break;
       case 9:
            valor = alei.getRAM().get(108) + ((rand() % 3) - 1);
            break;
       default:
            valor = 0;
            break;
     }

   return valor;
}

void addTennisData(std::vector<float> & v_inputs, ALEInterface &alei)
{

  v_inputs.push_back(getTennisPlayerX(alei));
  v_inputs.push_back(getTennisPlayerY(alei));

  v_inputs.push_back(getTennisEnemyX(alei));
  v_inputs.push_back(getTennisEnemyY(alei));
  v_inputs.push_back(getTennisBallX(alei));
  v_inputs.push_back(getTennisBallY(alei));
  v_inputs.push_back(getTennisBallZ(alei));

}

void addFreewayData(std::vector<float> & v_inputs, ALEInterface &alei)
{
  for(int  i =0; i<10; i++){
    v_inputs.push_back(getFreeWayCalle(i, alei));
  }
}

float max(float n1, float n2){
  if(n1>n2){
    return n1;
  }else{
    return n2;
  }
}

int parseArgs(int argc, char **argv, std::string &path){
  int game_id;

  if (strstr(argv[1], "tennis") != NULL) {
     path.append("tennis/");
     game_id = 1;
  }else if(strstr(argv[1], "freeway") != NULL){
    path.append("freeway/");
    game_id = 2;
  }else if(strstr(argv[1], "Demon") != NULL){
    path.append("demonattack/");
    game_id = 3;
  }else if(strstr(argv[1], "alien") != NULL){
    path.append("alien/");
    game_id = 4;
  }else if(strstr(argv[1], "berzerk") != NULL){
    path.append("berzerk/");
    game_id = 5;
  }else{
    game_id = 0;
  }

  return game_id;
}
void usage(char* pname) {
   std::cerr
      << "\nUSAGE:\n"
      << "   " << pname << " <romfile>\n";
   exit(-1);
}
