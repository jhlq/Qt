#include "sharktrainer.h"
#include <sstream>
#include <vector>
#include <fstream>
#include "board.h"

#include <boost/algorithm/string.hpp>

SharkTrainer::SharkTrainer()
{

}
#include <iostream>
void SharkTrainer::makeData(std::string inputfile, std::string outputfile){
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
