#include <iostream>
#include <cmath>
#include <ncurses.h>
#include <iterator>
#include "src/ale_interface.hpp"

// Global vars
const int maxSteps = 7500;
int lastLives;
float totalReward;
ALEInterface alei;
ALERAM ram;
ALERAM pram;
char c = 'x';
char nc = 'x';
int  action_typo = 2;
int  prev_action_typo = 2;
char filename[80];
int game_id = 0;


std::ofstream myfile("datos.csv", std::ofstream::out | std::ios_base::app);

int al[16][8] = {
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
            if(al[i][j] == 0){

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
                /*if( (i ==14  && j==1) || (i ==14  && j==3) || (i ==14  && j==2) || (i ==14  && j==4)

                  || (i ==15  && j==2)
                  || (i ==4  && j==1) || (i ==4  && j==5) || (i ==4  && j==2) || (i ==4  && j==7)
                  || (i ==5  && j==8) || (i ==5  && j==4) || (i ==5  && j==6)
                  || (i ==1  && j==4) || (i ==1  && j==8)
                  || (i ==6  && j==6) || (i ==6  && j==5) || (i ==6  && j==7)
                  || (i ==8  && j==5) || (i ==8  && j==1) || (i ==8  && j==7)
                  || (i ==12  && j==3)
                  || (i ==2 && j== 2) || (i ==2  && j==7) || (i ==2  && j==4)
                  || (i ==10  && j==2) || (i ==10  && j==3) || (i ==10  && j==4)
                  || (i ==7  && j==2) || (i ==7  && j==6) || (i ==7  && j==4) || (i ==7  && j==8)
                  || (i ==9  && j==4)
                  || (i ==4  && j==1) || (i ==4  && j==1) || (i ==4  && j==1))
                {
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*j));
                }else{*/
                  al[i][j] = 1;
                        /* Start color 			*/
                  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
                  attron(COLOR_PAIR(2));
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
                  attroff(COLOR_PAIR(2));
                //}
              }else{
                /*
                if( (i ==14  && j==1) || (i ==14  && j==3) || (i ==14  && j==2) || (i ==14  && j==4)

                  || (i ==15  && j==2)
                  || (i ==4  && j==1) || (i ==4  && j==5) || (i ==4  && j==2) || (i ==4  && j==7)
                  || (i ==5  && j==8) || (i ==5  && j==4) || (i ==5  && j==6)
                  || (i ==1  && j==4) || (i ==1  && j==8)
                  || (i ==6  && j==6) || (i ==6  && j==5) || (i ==6  && j==7)
                  || (i ==8  && j==5) || (i ==8  && j==1) || (i ==8  && j==7)
                  || (i ==12  && j==3)
                  || (i ==2 && j== 2) || (i ==2  && j==7) || (i ==2  && j==4)
                  || (i ==10  && j==2) || (i ==10  && j==3) || (i ==10  && j==4)
                  || (i ==7  && j==2) || (i ==7  && j==6) || (i ==7  && j==4) || (i ==7  && j==8)
                  || (i ==9  && j==4)
                  || (i ==4  && j==1) || (i ==4  && j==1) || (i ==4  && j==1))
                {
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*j));
                }else{*/
                  al[i][j] = 2;
                        /* Start color 			*/
                  init_pair(1, COLOR_RED, COLOR_BLACK);
                  attron(COLOR_PAIR(1));
                  mvprintw(i+4, j*5 + desp, "%2X", nram.get(i*8 + j));
                  attroff(COLOR_PAIR(1));
                //}
              }
            }else{
              al[i][j] = 0;
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
        std::ostream_iterator<int>(oss, ","));

    // Now add the last element with no delimiter
    oss << vec.back();
  }

  return oss.str();
}

int doAction(char c, float &reward){

  if(c == 'a' || c == 'A'){
    reward += alei.act(PLAYER_A_LEFT);
    return 1;
  }else if(c == 'd'|| c == 'D'){
    reward += alei.act(PLAYER_A_RIGHT);
    return 2;
  }else if(c == 'w' || c == 'W'){
    reward += alei.act(PLAYER_A_UP);
    return 3;
  }else if(c == 's' || c == 'S'){
    reward += alei.act(PLAYER_A_DOWN);
    return 4;
  }else if(c == 'v' || c == 'V'){
    reward += alei.act(PLAYER_A_FIRE);
    return 5;
  }
  else{
    reward += alei.act(PLAYER_A_NOOP);
    return 0;
  }

  return 1;
}

///////////////////////////////////////////////////////////////////////////////
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
float agentStep() {

   std::vector<int> v_datos;
   static int wide = 9;
   float reward = 0;




   char mesg[]="Enter move: ";
   char prmesg[]="Prev. RAM: ";
   char nrmesg[]="New RAM: ";
   char str[80];
   int row,col;


   getmaxyx(stdscr,row,col);

   mvprintw(3, 0,"%s",prmesg);
   mvprintw(3, 60,"%s",nrmesg);
   mvprintw(0, 0,"%s",mesg);

   printPRam(pram, 0, 1, action_typo);

   c = nc;

   action_typo = doAction(c, reward);

   ram = alei.getRAM();

   myfile.open(filename, std::ios::out | std::ios::app);
   switch(game_id){
     case 1:
          v_datos.push_back(getTennisPlayerX());
          v_datos.push_back(getTennisPlayerY());
          v_datos.push_back(getTennisEnemyX());
          v_datos.push_back(getTennisEnemyY());
          v_datos.push_back(getTennisBallX());
          v_datos.push_back(getTennisBallY());
          v_datos.push_back(getTennisBallZ());
          v_datos.push_back(action_typo);

          break;
     case 2:
          for(int  i =0; i<10; i++){
            v_datos.push_back(getFreeWayCalle(i));
          }
          v_datos.push_back(action_typo);
          break;
     default:
          break;
   }

   myfile << convertVectorToString(v_datos) << std::endl;
   myfile.close();

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

   // Create alei object.
   alei.setInt("random_seed", 0);
   alei.setFloat("repeat_action_probability", 0);
   alei.setBool("display_screen", true);
   alei.setBool("sound", true);
   alei.loadROM(argv[1]);

   strcpy(filename, "datos_");
   if (strstr(argv[1], "tennis") != NULL) {
      strcat(filename, "tennis");
      game_id = 1;
   }else if(strstr(argv[1], "freeway") != NULL){
      strcat(filename, "freeway");
      game_id = 2;
   }else{
     strcat(filename, "error");
     game_id = 0;
   }
   strcat(filename, ".txt");


   // Init
   srand(time(NULL));
   lastLives = alei.lives();
   totalReward = .0f;

   // Main loop
   alei.act(PLAYER_A_FIRE);
   int step;


   for (step = 0;
        !alei.game_over() && step < maxSteps;
        ++step)
   {
     initscr();


     pram = alei.getRAM();
     totalReward += agentStep();
     endwin();
   }

   std::cout << "Steps: " << step << std::endl;
   std::cout << "Reward: " << totalReward << std::endl;



   return 0;
}
