#include <iostream>
#include <cmath>
#include <ncurses.h>
#include <iterator>
#include "src/ale_interface.hpp"

/*
// Define actions
enum Action {
    PLAYER_A_NOOP           = 0,
    PLAYER_A_FIRE           = 1,
    PLAYER_A_UP             = 2,
    PLAYER_A_RIGHT          = 3,
    PLAYER_A_LEFT           = 4,
    PLAYER_A_DOWN           = 5,
    PLAYER_A_UPRIGHT        = 6,
    PLAYER_A_UPLEFT         = 7,
    PLAYER_A_DOWNRIGHT      = 8,
    PLAYER_A_DOWNLEFT       = 9,
    PLAYER_A_UPFIRE         = 10,
    PLAYER_A_RIGHTFIRE      = 11,
    PLAYER_A_LEFTFIRE       = 12,
    PLAYER_A_DOWNFIRE       = 13,
    PLAYER_A_UPRIGHTFIRE    = 14,
    PLAYER_A_UPLEFTFIRE     = 15,
    PLAYER_A_DOWNRIGHTFIRE  = 16,
    PLAYER_A_DOWNLEFTFIRE   = 17,
    PLAYER_B_NOOP           = 18,
    PLAYER_B_FIRE           = 19,
    PLAYER_B_UP             = 20,
    PLAYER_B_RIGHT          = 21,
    PLAYER_B_LEFT           = 22,
    PLAYER_B_DOWN           = 23,
    PLAYER_B_UPRIGHT        = 24,
    PLAYER_B_UPLEFT         = 25,
    PLAYER_B_DOWNRIGHT      = 26,
    PLAYER_B_DOWNLEFT       = 27,
    PLAYER_B_UPFIRE         = 28,
    PLAYER_B_RIGHTFIRE      = 29,
    PLAYER_B_LEFTFIRE       = 30,
    PLAYER_B_DOWNFIRE       = 31,
    PLAYER_B_UPRIGHTFIRE    = 32,
    PLAYER_B_UPLEFTFIRE     = 33,
    PLAYER_B_DOWNRIGHTFIRE  = 34,
    PLAYER_B_DOWNLEFTFIRE   = 35,
    RESET                   = 40, // MGB: Use SYSTEM_RESET to reset the environment.
    UNDEFINED               = 41,
    RANDOM                  = 42,
    SAVE_STATE              = 43,
    LOAD_STATE              = 44,
    SYSTEM_RESET            = 45,
    LAST_ACTION_INDEX       = 50
};
*/

#define NONE_ID 0
#define TENNIS_ID 1
#define FREEWAY_ID 2

// Global vars
const int maxSteps = 7500;
const char mesg[]="Enter move: ";
const char prmesg[]="Prev. RAM: ";
const char nrmesg[]="New RAM: ";
const char *path="datos/";

ALEInterface alei;
ALERAM ram;
ALERAM pram;

int lastLives;
int  action_typo = PLAYER_A_UP;

float totalReward;

char c = 'x';
char nc = 'x';

char dfile_name[80];
char cfile_name[80];

int game_id = NONE_ID;
int count = 0;

std::ofstream datafile;
std::ofstream countfile;

int al_colores[16][8] = {
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0}
};

///////////////////////////////////////////////////////////////////////////////
/// Get info from RAM
///////////////////////////////////////////////////////////////////////////////

int getTennisPlayerX() {
   return ram.get(26) + ((rand() % 3) - 1);
}
int getTennisPlayerY() {
   return ram.get(24) + ((rand() % 3) - 1);
}
int getTennisEnemyX() {
   return ram.get(27) + ((rand() % 3) - 1);
}
int getTennisEnemyY() {
   return ram.get(53) + ((rand() % 3) - 1);
}
int getTennisBallX() {
   return ram.get(55) + ((rand() % 3) - 1);
}
int getTennisBallY() {
   return ram.get(16) + ((rand() % 3) - 1);
}
int getTennisBallZ() {
   return ram.get(17) + ((rand() % 3) - 1);
}

int getFreeWayCalle(int param){

    int valor = 0;
     switch(param){
       case 0:
            valor = ram.get(43) + ((rand() % 3) - 1);
            break;
       case 1:
            valor = ram.get(44) + ((rand() % 3) - 1);
            break;
       case 2:
            valor = ram.get(45) + ((rand() % 3) - 1);
            break;
       case 3:
            valor = ram.get(46) + ((rand() % 3) - 1);
            break;
       case 4:
            valor = ram.get(47) + ((rand() % 3) - 1);
            break;
       case 5:
            valor = ram.get(112) + ((rand() % 3) - 1);
            break;
       case 6:
            valor = ram.get(111) + ((rand() % 3) - 1);
            break;
       case 7:
            valor = ram.get(110) + ((rand() % 3) - 1);
            break;
       case 8:
            valor = ram.get(109) + ((rand() % 3) - 1);
            break;
       case 9:
            valor = ram.get(108) + ((rand() % 3) - 1);
            break;
       default:
            valor = 0;
            break;
     }

   return valor;
}

void printPRam(ALERAM nram, int desp, bool color, int ac_typo){

  mvprintw(3, 0,"%s",prmesg);
  mvprintw(3, 60,"%s",nrmesg);
  mvprintw(0, 0,"%s",mesg);

  start_color();
  for (int i = 0; i<17; i++){
    for(int j = 0; j<9; j++){
      if(i==16){
        if(j!=0){
          mvprintw(i+4, j*5 + desp, "|%i|", j);
        }
      }else{
        if(j == 8){
          mvprintw(i+4, j*5 + desp, "|%i|", i);
        }else{
          if(color){
            if(al_colores[i][j] == 0){

              mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));

            }else if (ac_typo != 0){
                    /* Start color 			*/
              init_pair(2, COLOR_YELLOW, COLOR_BLACK);

              attron(COLOR_PAIR(2));
              mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
              attroff(COLOR_PAIR(2));
            }else{
                    /* Start color 			*/
              init_pair(1, COLOR_RED, COLOR_BLACK);

              attron(COLOR_PAIR(1));
              mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
              attroff(COLOR_PAIR(1));
            }
          }else{
            mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
          }
        }
      }
      if(j!=8){
        mvprintw(i+4, j*5 + 3 + desp, "%s", "- ");
      }
    }
    //printf("\n");
  }
}

void printNRam(ALERAM nram, int desp, bool color, int ac_typo){
  start_color();
  for (int i = 0; i<17; i++){
    for(int j = 0; j<9; j++){
      if(i==16){
        if(j!=8){
          mvprintw(i+4, j*5 + desp, "|%i|", j);
        }
      }else{
        if(j == 8){
          mvprintw(i+4, j*5 + desp - 45, "|%i|", i);
        }else{

          if(color){
            if(nram.get(i*8 + j)!=pram.get(i*8 + j)){
              if(ac_typo != 0){
                //Usar este trozo para descartar posiciones
                /*if(
                     (i ==14  && j==1) || (i ==14  && j==3) || (i ==14  && j==2) || (i ==14  && j==4)
                  || (i ==15  && j==2)
                  || (i ==4  && j==1) || (i ==4  && j==5) || (i ==4  && j==2) || (i ==4  && j==7)
                  || (i ==5  && j==8) || (i ==5  && j==4) || (i ==5  && j==6)
                  || (i ==1  && j==4) || (i ==1  && j==8)
                  || (i ==6  && j==6) || (i ==6  && j==5) || (i ==6  && j==7)
                  || (i ==8  && j==5) || (i ==8  && j==1) || (i ==8  && j==7)
                  )
                {
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*j));
                }else{*/
                  al_colores[i][j] = 1;
                        /* Start color 			*/
                  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
                  attron(COLOR_PAIR(2));
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
                  attroff(COLOR_PAIR(2));
                //}
              }else{
                //Usar este trozo para descartar posiciones
                /*
                if(
                     (i ==14  && j==1) || (i ==14  && j==3) || (i ==14  && j==2) || (i ==14  && j==4)
                  || (i ==15  && j==2)
                  || (i ==4  && j==1) || (i ==4  && j==5) || (i ==4  && j==2) || (i ==4  && j==7)
                  || (i ==5  && j==8) || (i ==5  && j==4) || (i ==5  && j==6)
                )
                {
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*j));
                }else{*/
                  al_colores[i][j] = 2;
                        /* Start color 			*/
                  init_pair(1, COLOR_RED, COLOR_BLACK);
                  attron(COLOR_PAIR(1));
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
                  attroff(COLOR_PAIR(1));
                //}
              }
            }else{
              al_colores[i][j] = 0;
              mvprintw(i+4, j*5 +desp, "%2X", nram.get(i*8 + j));
            }
          }

          else{
            mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
          }
        }
      }

      if(j!=8){
        mvprintw(i+4, j*5 + 3 + desp, "%s", "- ");
      }
    }
    //printf("\n");
  }
}

std::string convertVectorToString(std::vector<int> vec){
  std::ostringstream oss;

  if (!vec.empty())
  {
    // Convert all but the last element to avoid a trailing ","
    std::copy(vec.begin(), vec.end()-1,
        std::ostream_iterator<int>(oss, " "));

    // Now add the last element with no delimiter
    oss << vec.back();
  }

  return oss.str();
}

int doAction(char c, float &reward){

  if(c == 'a' || c == 'A'){
    reward += alei.act(PLAYER_A_LEFT);
    return PLAYER_A_LEFT;
  }else if(c == 'd'|| c == 'D'){
    reward += alei.act(PLAYER_A_RIGHT);
    return PLAYER_A_RIGHT;
  }else if(c == 'w' || c == 'W'){
    reward += alei.act(PLAYER_A_UP);
    return PLAYER_A_UP;
  }else if(c == 's' || c == 'S'){
    reward += alei.act(PLAYER_A_DOWN);
    return PLAYER_A_DOWN;
  }else if(c == 'v' || c == 'V'){
    reward += alei.act(PLAYER_A_FIRE);
    return PLAYER_A_FIRE;
  }
  else{
    reward += alei.act(PLAYER_A_NOOP);
    return PLAYER_A_NOOP;
  }

  return PLAYER_A_NOOP;
}

void addTennisData(std::vector<int> & v_inputs, std::vector<int> & v_outputs, int action_typo)
{

  v_inputs.push_back(getTennisPlayerX());
  v_inputs.push_back(getTennisPlayerY());
  v_inputs.push_back(getTennisEnemyX());
  v_inputs.push_back(getTennisEnemyY());
  v_inputs.push_back(getTennisBallX());
  v_inputs.push_back(getTennisBallY());
  v_inputs.push_back(getTennisBallZ());

  for(int j=0; j<5; j++){
    if(j==0 && action_typo == PLAYER_A_FIRE){
      v_outputs.push_back(1);
    }else if(j==1 && action_typo == PLAYER_A_UP){
      v_outputs.push_back(1);
    }else if(j==2 && action_typo == PLAYER_A_RIGHT){
      v_outputs.push_back(1);
    }else if(j==3 && action_typo == PLAYER_A_LEFT){
      v_outputs.push_back(1);
    }else if(j==4 && action_typo == PLAYER_A_DOWN){
      v_outputs.push_back(1);
    }else{
      v_outputs.push_back(0);
    }
  }
}

void addFreewayData(std::vector<int> & v_inputs, std::vector<int> & v_outputs, int action_typo)
{

  for(int  i =0; i<10; i++){
    v_inputs.push_back(getFreeWayCalle(i));
  }

  for(int j=0; j<2; j++){
    if(j==0 && action_typo == PLAYER_A_UP){
      v_outputs.push_back(1);
    }else if(j==1 && action_typo == PLAYER_A_DOWN){
      v_outputs.push_back(1);
    }else{
      v_outputs.push_back(0);
    }
  }
}

void concatenateFiles(){

  std::string command;

  command = "cat ";
  command.append(cfile_name);
  command.append(" ");
  command.append(dfile_name);
  command.append(" > ");
  command.append(dfile_name);
  command.append(".txt");

  const char * c = command.c_str();

  system(c);
}
///////////////////////////////////////////////////////////////////////////////
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
float agentStep() {
   std::vector<int> v_inputs;
   std::vector<int> v_outputs;

   float reward = 0;

   int row,col;

   printPRam(pram, 0, 1, action_typo);

   c = nc;

   action_typo = doAction(c, reward);

   ram = alei.getRAM();

   datafile.open(dfile_name, std::ios::out | std::ios_base::app);
   countfile.open(cfile_name, std::ios::out | std::ios_base::trunc);


   switch(game_id){
     case TENNIS_ID:
          addTennisData(v_inputs, v_outputs, action_typo);
          //v_outputs.push_back(action_typo);
          break;
     case FREEWAY_ID:

          addFreewayData(v_inputs, v_outputs, action_typo);
          //v_outputs.push_back(action_typo);
          break;
     default:
          v_inputs.push_back(0);
          v_outputs.push_back(0);
          break;
   }

   datafile << convertVectorToString(v_inputs) << std::endl;
   datafile << convertVectorToString(v_outputs) << std::endl;

   count++;
   countfile << count << std::endl;

   datafile.close();
   countfile.close();

   concatenateFiles();

   printNRam(ram, 60, 1, action_typo);
   pram = ram;

   nc = getch();

   return reward;
}

///////////////////////////////////////////////////////////////////////////////
/// Print usage and exit
///////////////////////////////////////////////////////////////////////////////
void usage(char* pname) {
   std::cerr
      << "\nUSAGE:\n"
      << "   " << pname << " <romfile>\n";
   exit(-1);
}

///////////////////////////////////////////////////////////////////////////////
/// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {


   // Check input parameter
   if (argc != 2)
      usage(argv[0]);

   strcpy(dfile_name, path);

   if (strstr(argv[1], "tennis") != NULL) {
      strcat(dfile_name, "tennis/datos_tennis");
      game_id = 1;
   }else if(strstr(argv[1], "freeway") != NULL){
      strcat(dfile_name, "freeway/datos_freeway");
      game_id = 2;
   }else if(strstr(argv[1], "Demon") != NULL){
      strcat(dfile_name, "demonattack/datos_demonattack");
      game_id = 3;
   }else if(strstr(argv[1], "alien") != NULL){
      strcat(dfile_name, "alien/datos_alien");
      game_id = 4;
   }else if(strstr(argv[1], "berzerk") != NULL){
      strcat(dfile_name, "berzerk/datos_berzerk");
      game_id = 5;
   }else{
     strcat(dfile_name, "datos_error");
     game_id = 0;
   }

   strcpy(cfile_name, dfile_name);
   strcat(cfile_name, "_count");

   // Create alei object.
   alei.setInt("random_seed", 0);
   alei.setFloat("repeat_action_probability", 0);
   alei.setBool("display_screen", true);
   alei.setBool("sound", true);
   alei.loadROM(argv[1]);

   //borra el archivo previo
   datafile.open(dfile_name, std::ios::out | std::ios::trunc );
   countfile.open(cfile_name, std::ios::out | std::ios::trunc );
   datafile.close();
   countfile.close();

   // Init
   srand(time(NULL));
   lastLives = alei.lives();
   totalReward = .0f;

   // Main loop
   int step;

   for (step = 0; !alei.game_over() && step < maxSteps; ++step){
     initscr();
     pram = alei.getRAM();
     totalReward += agentStep();
     endwin();
   }

   std::cout << "Steps: " << step << std::endl;
   std::cout << "Reward: " << totalReward << std::endl;

   return 0;
}
