#ifndef SHARKTRAINER_H
#define SHARKTRAINER_H
#include <string>

class SharkTrainer
{
public:
    SharkTrainer();
    void makeData(int sideLength,std::string inputfile,std::string outputfile);
    void start();
};

#endif // SHARKTRAINER_H
