#include "sharktrainer.h"
#include <sstream>
#include <vector>
#include <fstream>
#include "board.h"
#include <iostream>
//#include <filesystem> //requires standard 17
// #define WINDOWS  /* uncomment this line to use it for windows.*/
/*#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif */
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <clocale>  // std::setlocate

//the model
//#include <shark/Models/LinearModel.h>//single dense layer
//#include <shark/Models/ConcatenatedModel.h>//for stacking layers, proveides operator>>
//training the  model
//#include <shark/ObjectiveFunctions/ErrorFunction.h>//error function, allows for minibatch training
//#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>
#include <shark/Algorithms/GradientDescent/Adam.h> //optimizer: simple gradient descent.
using namespace shark;

SharkTrainer::SharkTrainer()
{

}
/*RealVector SharkTrainer::makeEvalVector_old(Board board,Triangle move){
    int inputlength=board.tg.sideLength*board.tg.sideLength*6+3;
    RealVector input(inputlength,0);
    int n=0;
    bool hasKO=false;
    for (int yi=0;yi<board.tg.triangles.size();yi++){
        for (int xi=0;xi<board.tg.triangles[yi].size();xi++){
            Triangle t=board.tg.get(xi,yi);
            if (t==move){
                input[n]=1;
            } else if (t.player>0){
                if (t.player==move.player){
                    input[n+1]=1;
                } else {
                    input[n+2]=1;
                }
                //input[n+t.player]=1;
            } else {
                int imt=board.invalidMoveType(t.x,t.y,move.player);
                if (imt>1){
                    input[n+imt+1]=1;
                    if (imt==3){
                        hasKO=true;
                    }
                }
            }
            n+=6;
        }
    }
    if (hasKO){
        input[n]=1;
    }
    if (move.player==2){
        input[n+1]=1;
    }
    if (move.isPass()){
        input[n+2]=1;
    }
    return input;
}*/
RealVector SharkTrainer::makeEvalVector(Board board,Triangle move){
    std::vector<Triangle> inds=board.tg.adjacentIndsSpread(move,2);
    int inputlength=inds.size()*5;
    RealVector input(inputlength,0);
    int n=0;
    bool hasKO=false; //board.hasKO()?
    for (Triangle i:inds){
        if (board.tg.has(i)){
            Triangle t=board.tg.get(i.x,i.y);
            if (t.player>0){
                if (t.player==move.player){
                    input[n]=1;
                } else {
                    input[n+1]=1;
                }
            } else {
                int imt=board.invalidMoveType(t.x,t.y,move.player); //add imt for other player?
                if (imt>1){
                    input[n+imt]=1;
                    if (imt==3){
                        hasKO=true;
                    }
                }
            }
        } else {
            input[n+4]=1;
        }
        n+=5;
    }
    /*
    if (hasKO){
        input[n]=1;
    }
    if (move.player==2){
        input[n+1]=1;
    }
    if (move.isPass()){
        input[n+2]=1;
    }*/
    return input;
}
void SharkTrainer::makeData(int sideLength,std::string inputfile){
    std::ifstream file;
    file.open (inputfile);
    std::string line;
    //int inputlength=sideLength*sideLength*6+3;
    //std::vector< int( * )[inputlength] > arrays;
    //std::vector< int( * ) > arrays;
    //std::vector< std::vector<int> > arrays;
    std::vector< RealVector > arrays;
    std::vector<double> labels;
    std::setlocale(LC_ALL, "C"); // C uses "." as decimal-point separator
    if (file.is_open()){
        while (getline(file,line)){
            std::vector<Triangle> moves;
            double target;
            std::vector<std::string> strs;
            boost::split(strs,line,boost::is_any_of(";"));
            //std::cout<<"line: "<<line<<" nstrs: "<<strs.size()<<std::endl;
            //std::cout<<"strend: "<<strs[strs.size()-2]<<std::endl;
            if (sideLength==std::stoi(strs[0])){
                target=std::stod(strs[strs.size()-2]);
                labels.push_back(target);
                for (std::size_t i = 1; i < strs.size()-2; i++){
                    //std::cout << strs[i] << std::endl;
                    std::vector<std::string> strs2;
                    boost::split(strs2,strs[i],boost::is_any_of(","));
                    Triangle t=Triangle(std::stoi(strs2[0]),std::stoi(strs2[1]),std::stoi(strs2[2]));
                    moves.push_back(t);
                }
                Triangle markedmove=moves.back();
                moves.pop_back();
                Board board(sideLength);
                board.moves=moves;
                //Triangle markedmove=board.moves[board.moves.size()-1];
                board.player=markedmove.player;//board.otherPlayer(markedmove.player);
                board.placeMoves();
                arrays.push_back(makeEvalVector(board,markedmove));
                /*
                std::vector<int> input(inputlength,0); //replace with RealVector and use makeEvalVector?
                int n=0;
                for (int yi=0;yi<board.tg.triangles.size();yi++){
                    for (int xi=0;xi<board.tg.triangles[yi].size();xi++){
                        Triangle t=board.tg.get(xi,yi);
                        if (t==markedmove){
                            input[n]=1;
                        } else if (t.player>0){
                            if (t.player==markedmove.player){
                                input[n+1]=1;
                            } else {
                                input[n+2]=1;
                            }
                            //input[n+t.player]=1;
                        } else {
                            int imt=board.invalidMoveType(t.x,t.y,markedmove.player);
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
                if (markedmove.player==2){
                    input[n+1]=1;
                }
                arrays.push_back(input); */
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
        int inputlength=arrays[0].size();
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
            //char buff[FILENAME_MAX];
            //GetCurrentDir( buff, FILENAME_MAX );
            //std::string current_working_dir(buff);
            boost::filesystem::path current_working_dir=boost::filesystem::current_path();
            std::cerr << "Unable to open file " <<  dataFile << " and/or " << labelFile <<
                             ". Check paths! Current dir is " << current_working_dir << " Exception: " <<exc.what()<< std::endl;
            exit(EXIT_FAILURE);
        }
        //now we create a complete dataset which represents pairs of inputs and labels
        //RegressionDataset data(inputs, labels);
        //dataset=data;
        //std::cout<<"l0: "<<labels[0]<<std::endl;
        dataset=RegressionDataset(inputs,labels);
        return dataset;
}
void SharkTrainer::makeModel(){
    //typedef LinearModel<RealVector, RectifierNeuron> DenseLayer;

    //build the network
    int hidden1=700;
    int hidden2=500;
    int hidden3=300;
    //DenseLayer layer1(dataset.inputShape(),hidden1);
    //DenseLayer layer2(layer1.outputShape(),hidden2);
    //LinearModel<RealVector> output(layer2.outputShape(),1);
    auto l1=std::make_shared<DenseLayer>(dataset.inputShape(),hidden1,true);
    auto l2=std::make_shared<DenseLayer>(l1->outputShape(),hidden2);
    auto l3=std::make_shared<DenseLayer>(l2->outputShape(),hidden3);
    auto o=std::make_shared<LinearModel<RealVector>>(l3->outputShape(),1);
    layers.push_back(l1);
    layers.push_back(l2);
    layers.push_back(l3);
    layers.push_back(o);
    model=*l1>>*l2>>*l3>>*o;
    initRandomNormal(model,0.001);
}
void SharkTrainer::trainModel(){
    SquaredLoss<> loss;
    ErrorFunction<> errorFunction(dataset, &model, &loss);//, true);//enable minibatch training
    //CG<> optimizer;
    Adam<> optimizer;
    optimizer.setEta(0.001);//learning rate of the algorithm
    errorFunction.init();
    optimizer.init(errorFunction);
    for(int i = 0; i < 100; ++i)
    {
            optimizer.step(errorFunction);
            std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
    }
    //copy solution parameters into model
    model.setParameterVector(optimizer.solution().point);
}
void SharkTrainer::init(){
    makeData(9,"trainingData.txt");
    loadData("inputs.csv","labels.csv");
    makeModel();
    trainModel();
}
void SharkTrainer::start(){
    //load data
    //RegressionDataset data = loadData("inputs.csv","labels.csv");
    //We use a dense linear model with rectifier activations
    /*typedef LinearModel<RealVector, RectifierNeuron> DenseLayer;

    //build the network
    int hidden1=300;
    int hidden2=500;
    DenseLayer layer1(data.inputShape(),hidden1);
    DenseLayer layer2(layer1.outputShape(),hidden2);
    LinearModel<RealVector> output(layer2.outputShape(),1); */

    //DenseLayer layer1(3,5);
    //DenseLayer layer2(layer1.outputShape(),7);
    //LinearModel<RealVector> output(layer2.outputShape(),1);
    //auto network = layer1 >> layer2 >> output;
    //model=network;
    //RealVector t(3);
    //t[1]=1;
    //std::cout<<"Testing network: "<<network(t)<<std::endl;

    //create the supervised problem.
    //CrossEntropy loss;
    //SquaredLoss<> loss;
    //ErrorFunction<> errorFunction(data, &network, &loss, true);//enable minibatch training

    //optimize the model
    //std::cout<<"training network"<<std::endl;
    //initRandomNormal(network,0.001);
    //std::cout<<"Testing initialized network: "<<network(t)<<std::endl;
    //Adam<> optimizer;
    /*error.init();
    optimizer.init(error);
    for(std::size_t i = 0; i != iterations; ++i){
            optimizer.step(error);
            std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
    }
    network.setParameterVector(optimizer.solution().point);*/
    /*
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
    */
    //std::vector<int> v={1,2,3};
    //RealVector rv({1,2,3});
    //rv.push_back(4.5);
    //std::cout<<rv<<std::endl;
    SharkTrainer st;
    st.loadData("inputs.csv","labels.csv");
    std::cout<<"Loaded data."<<std::endl;
    st.makeModel();
    std::cout<<"Made model."<<std::endl;
    RealVector initpass({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1});
    RealVector eval1=st.model(initpass);
    std::cout<<"Model eval: "<<eval1<<std::endl;
    //std::cout<<"Data: "<<st.dataset<<std::endl;
    st.trainModel();
    std::cout<<"Trained."<<std::endl;
    //RealVector initpass({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1});
    //RealVector eval=st.model(initpass);
    RealVector eval=st.model(initpass);
    std::cout<<"Pass first? "<<eval<<std::endl;
    Board b(9);
    b.placeMove(3,3);
    RealVector p2=makeEvalVector(b,Triangle(-1,-1,2));
    eval=st.model(p2);
    std::cout<<"Pass second? "<<eval<<std::endl;
}
double SharkTrainer::evaluateMove(Board b,Triangle move){
    RealVector rv=makeEvalVector(b,move);
    return model(rv)[0];
}
