#include "sharktrainer.h"
#include <sstream>
#include <vector>
#include <fstream>
#include "board.h"
#include <iostream>

#include <boost/algorithm/string.hpp>

//the model
#include <shark/Models/LinearModel.h>//single dense layer
#include <shark/Models/ConcatenatedModel.h>//for stacking layers, proveides operator>>
//training the  model
#include <shark/ObjectiveFunctions/ErrorFunction.h>//error function, allows for minibatch training
//#include <shark/ObjectiveFunctions/Loss/CrossEntropy.h> // loss used for supervised training
//#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> // loss used for evaluation of performance
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>
#include <shark/Algorithms/GradientDescent/Adam.h> //optimizer: simple gradient descent.
//#include <shark/Data/SparseData.h> //loading the dataset
using namespace shark;

SharkTrainer::SharkTrainer()
{

}
void SharkTrainer::makeData(int sideLength,std::string inputfile, std::string outputfile){
    std::ifstream file;
    file.open (inputfile);
    std::string line;
    if (file.is_open()){
        while (getline(file,line)){
            std::vector<Triangle> moves;
            double target;
            std::vector<std::string> strs;
            boost::split(strs,line,boost::is_any_of(";"));
            for (std::size_t i = 0; i < strs.size(); i++){
                std::cout << strs[i] << std::endl;
            }
            /*std::stringstream ss(line);
            std::string part;
            getline(ss,part,';');
            int sideLength=std::stoi(part);
            std::cout<<"SL "<<sideLength<<std::endl;
            while (getline(ss,part,';')){
                std::cout<<"part "<<part<<std::endl;
                std::string num;
                std::vector<std::string> v;
                while (getline(ss,num,',')){
                    v.push_back(num);
                    std::cout<<"num: "<<num<<std::endl;
                }
                if (v.size()==3){
                    moves.push_back(Triangle(std::stoi(v[0]),std::stoi(v[1]),std::stoi(v[2])));
                } else {
                    target=std::stof(v[0]);
                }
            }*/
            std::cout<<moves.size()<<" "<<target<<std::endl;
        }
    }
}
void SharkTrainer::start(){
    //load data

    //We use a dense linear model with rectifier activations
    typedef LinearModel<RealVector, RectifierNeuron> DenseLayer;

    //build the network
    //DenseLayer layer1(data.inputShape(),hidden1);
    //DenseLayer layer2(layer1.outputShape(),hidden2);
    //LinearModel<RealVector> output(layer2.outputShape(),numClasses);

    DenseLayer layer1(3,5);
    DenseLayer layer2(layer1.outputShape(),7);
    LinearModel<RealVector> output(layer2.outputShape(),1);
    auto network = layer1 >> layer2 >> output;
    RealVector t(3);
    t[1]=1;
    std::cout<<"Testing network: "<<network(t)<<std::endl;

    //create the supervised problem.
    //CrossEntropy loss;
    SquaredLoss<> loss;
    //ErrorFunction error(data, &network, &loss, true);//enable minibatch training

    //optimize the model
    //std::cout<<"training network"<<std::endl;
    initRandomNormal(network,0.001);
    std::cout<<"Testing initialized network: "<<network(t)<<std::endl;
    Adam<> optimizer;
    /*error.init();
    optimizer.init(error);
    for(std::size_t i = 0; i != iterations; ++i){
            optimizer.step(error);
            std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
    }
    network.setParameterVector(optimizer.solution().point);*/

}
