

#ifndef USER_UTILS_H
#define USER_UTILS_H

#include "../src/ale_interface.hpp"

int getTennisPlayerX(ALEInterface &alei);
int getTennisPlayerY(ALEInterface &alei);
int getTennisEnemyX(ALEInterface &alei);
int getTennisEnemyY(ALEInterface &alei);
int getTennisBallX(ALEInterface &alei);
int getTennisBallY(ALEInterface &alei);
int getTennisBallZ(ALEInterface &alei);

float getFreeWayCalle(int param, ALEInterface &alei);
void addTennisInputData(std::vector<float> & v_inputs, ALEInterface &alei);
void addFreewayInputData(std::vector<float> & v_inputs, ALEInterface &alei);
float max(float n1, float n2);
int parseArgs(int argc, char **argv, std::string  &path);
void usage(char* pname);

#endif
