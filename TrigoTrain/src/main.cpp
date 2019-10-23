#include <QApplication>
#include "mainwindow.h"

#include "sharktrainer.h"

//using namespace std;

int main(int argc, char *argv[]){
	
    SharkTrainer st;
    st.makeData(9,"trainingData.txt","");
    //SharkTrainer::makeData(9,"trainingData.txt","");
    st.start();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}

