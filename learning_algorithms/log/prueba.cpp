
#include "LogisticRegression.h"


int main(int argc, char *argv[])
{
    char const *filename = "datos.txt";
    LogisticRegression logistic;

    std::cout << logistic.openFile(filename) << "chupamelaaaaaaaa" << std::endl;
    int epochs = 10;
    float prediction1, prediction2, prediction3;

    for (int i=0; i<epochs; i++){
      std::cout << "-----EPOCA" << i << "-----" << std::endl;
      logistic.trainModel();
    }

    std::cout << "BIAS FINALES: " << logistic.getStringBias() << std::endl;

    logistic.testOnFile();
    logistic.toggleDiscrete();
    logistic.testOnFile();


    return 0;
}
