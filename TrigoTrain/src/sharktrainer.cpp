#include "sharktrainer.h"
#include <sstream>
#include <vector>
#include <fstream>
#include "board.h"
#include <iostream>
//#include <filesystem> //requires standard 17
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <boost/algorithm/string.hpp>

//the model
//#include <shark/Models/LinearModel.h>//single dense layer
#include <shark/Models/ConcatenatedModel.h>//for stacking layers, proveides operator>>
//training the  model
//#include <shark/ObjectiveFunctions/ErrorFunction.h>//error function, allows for minibatch training
//#include <shark/ObjectiveFunctions/Loss/CrossEntropy.h> // loss used for supervised training
//#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> // loss used for evaluation of performance
//#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>
//#include <shark/Algorithms/GradientDescent/Adam.h> //optimizer: simple gradient descent. //not included in old lib?
//#include <shark/Data/SparseData.h> //loading the dataset
using namespace shark;

SharkTrainer::SharkTrainer()
{

}
void SharkTrainer::makeData(int sideLength,std::string inputfile, std::string outputfile){
    std::ifstream file;
    file.open (inputfile);
    std::string line;
    int inputlength=sideLength*sideLength*6+2;
    //std::vector< int( * )[inputlength] > arrays;
    //std::vector< int( * ) > arrays;
    std::vector< std::vector<int> > arrays;
    std::vector<double> labels;
    if (file.is_open()){
        while (getline(file,line)){
            std::vector<Triangle> moves;
            double target;
            std::vector<std::string> strs;
            boost::split(strs,line,boost::is_any_of(";"));
            //std::cout<<"line: "<<line<<" nstrs: "<<strs.size()<<std::endl;
            //std::cout<<"strend: "<<strs[strs.size()-2]<<std::endl;
            if (sideLength==std::stoi(strs[0])){
                target=std::stof(strs[strs.size()-2]);
                labels.push_back(target);
                for (std::size_t i = 1; i < strs.size()-2; i++){
                    //std::cout << strs[i] << std::endl;
                    std::vector<std::string> strs2;
                    boost::split(strs2,strs[i],boost::is_any_of(","));
                    Triangle t=Triangle(std::stoi(strs2[0]),std::stoi(strs2[1]),std::stoi(strs2[2]));
                    moves.push_back(t);
                }
                //std::cout<<"moves: "<<moves.size()<<" target: "<<target<<std::endl;
                Board board(sideLength);
                board.moves=moves;
                Triangle markedmove=board.moves[board.moves.size()-1];
                board.player=board.otherPlayer(markedmove.player);
                board.placeMoves();
                //int input [inputlength]={};
                //for (int il=0;il<inputlength;il++){
                //    input[il]=0;
                //}
                std::vector<int> input(inputlength,0);
                int n=0;
                for (int yi=0;yi<board.tg.triangles.size();yi++){
                    for (int xi=0;xi<board.tg.triangles[yi].size();xi++){
                        Triangle t=board.tg.get(xi,yi);
                        if (t==markedmove){
                            input[n]=1;
                        } else if (t.player>0){
                            input[n+t.player]=1;
                        } else {
                            int imt=board.invalidMoveType(t.x,t.y,board.player);
                            if (imt>1){
                                input[n+imt+1]=1;
                            }
                        }
                        n+=6;
                    }
                }
                if (markedmove.isPass()){
                    input[n]=1;
                }
                if (board.player==2){
                    input[n+1]=1;
                }
                arrays.push_back(input);
            }
        }
    }
    file.close();
    if (labels.size()>0){
        std::ofstream inputsfile;
        std::ofstream labelsfile;
        inputsfile.open("inputs.csv");//,std::ios_base::app);
        labelsfile.open("labels.csv");//,std::ios_base::app);
        labelsfile<<labels[0];
        for (int i=1;i<labels.size();i++){
            labelsfile<<"\n"<<labels[i];
        }
        //int inp [inputlength]=*(arrays[0]);
        inputsfile<<arrays[0][0];
        for (int n=1;n<inputlength;n++){
            inputsfile<<","<<arrays[0][n];
        }
        for (int ai=1;ai<arrays.size();ai++){
            inputsfile<<"\n"<<arrays[ai][0];
            for (int n=1;n<inputlength;n++){
                inputsfile<<","<<arrays[ai][n];
            }
        }
        inputsfile.close();
        labelsfile.close();
    }

    //datafile << s;
    //datafile<<s<<target<<";\n";
    //datafile.close();
}
RegressionDataset SharkTrainer::loadData(const std::string& dataFile,const std::string& labelFile){
        //we first load two separate data files for the training inputs and the labels of the data point
        Data<RealVector> inputs;
        Data<RealVector> labels;
        try {
                importCSV(inputs, dataFile);
                importCSV(labels, labelFile);
        } catch (Exception exc) {
            char buff[FILENAME_MAX];
            GetCurrentDir( buff, FILENAME_MAX );
            std::string current_working_dir(buff);
                std::cerr << "Unable to open file " <<  dataFile << " and/or " << labelFile <<
                             ". Check paths! Current dir is " << current_working_dir << " Exception: " <<exc.what()<< std::endl;
                exit(EXIT_FAILURE);
        }
        //now we create a complete dataset which represents pairs of inputs and labels
        RegressionDataset data(inputs, labels);
        return data;
}
void SharkTrainer::start(){
    //load data
    RegressionDataset data = loadData("inputs.csv","labels.csv");
    //We use a dense linear model with rectifier activations
    typedef LinearModel<RealVector, RectifierNeuron> DenseLayer;

    //build the network
    int hidden1=300;
    int hidden2=500;
    DenseLayer layer1(data.inputShape(),hidden1);
    DenseLayer layer2(layer1.outputShape(),hidden2);
    LinearModel<RealVector> output(layer2.outputShape(),1);

    //DenseLayer layer1(3,5);
    //DenseLayer layer2(layer1.outputShape(),7);
    //LinearModel<RealVector> output(layer2.outputShape(),1);
    auto network = layer1 >> layer2 >> output;
    //RealVector t(3);
    //t[1]=1;
    //std::cout<<"Testing network: "<<network(t)<<std::endl;

    //create the supervised problem.
    //CrossEntropy loss;
    SquaredLoss<> loss;
    ErrorFunction<> errorFunction(data, &network, &loss, true);//enable minibatch training

    //optimize the model
    //std::cout<<"training network"<<std::endl;
    initRandomNormal(network,0.001);
    //std::cout<<"Testing initialized network: "<<network(t)<<std::endl;
    //Adam<> optimizer;
    /*error.init();
    optimizer.init(error);
    for(std::size_t i = 0; i != iterations; ++i){
            optimizer.step(error);
            std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
    }
    network.setParameterVector(optimizer.solution().point);*/

    CG<> optimizer;
    errorFunction.init();
    optimizer.init(errorFunction);
    for(int i = 0; i != 100; ++i)
    {
            optimizer.step(errorFunction);
            std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
    }
    //copy solution parameters into model
    network.setParameterVector(optimizer.solution().point);

}
