#include <QApplication>
#include "mainwindow.h"

#include "sharktrainer.h"

//using namespace std;

int main(int argc, char *argv[]){
	
    SharkTrainer st;
    st.makeData("trainingData.txt","");

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}

