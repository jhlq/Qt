#ifndef SHARKTRAINER_H
#define SHARKTRAINER_H
#include <string>

#include <shark/Data/Csv.h>
#include <shark/Algorithms/GradientDescent/CG.h>
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>
#include <shark/Models/LinearModel.h>
#include <shark/Models/NeuronLayers.h>
using namespace shark;

class SharkTrainer
{
public:
    SharkTrainer();
    void makeData(int sideLength,std::string inputfile,std::string outputfile);
    RegressionDataset loadData(const std::string& dataFile,const std::string& labelFile);
    void start();
};

#endif // SHARKTRAINER_H
