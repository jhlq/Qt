#include <QApplication>
#include "mainwindow.h"

#include <shark/Data/Csv.h>
#include <shark/Algorithms/Trainers/LDA.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
using namespace shark;

#include <iostream>
using namespace std;

#include "trianglegrid.h"

int main(int argc, char *argv[]){

    Triangle t1=Triangle(1,2);
    Triangle t2=Triangle(3,2);
    t1.player=1;
    t2.player=2;
    cout << t1.sameAs(&t2) << endl;
    cout << t1.markedDead << endl;
    bool e=t1.player==t2.player || ((t1.markedDead||t1.player==0)&&(t2.markedDead||t1.player==0));
    cout << e << endl;
    TriangleGrid tg(7);
    cout<<tg.sideLength<<endl;
    cout<<tg.nTriangles()<<endl;

    string filename="/home/unikt/SharkML/Shark/examples/Supervised/data/quickstartData.csv";
    ClassificationDataset data;
        try {
            importCSV(data, filename, LAST_COLUMN, ' ');
        }
        catch (...) {
            cerr << "unable to read data from file " <<  filename << endl;
            exit(EXIT_FAILURE);
        }
        //###end<load_data>

        //create a test and training partition of the data
        //###begin<split_data>
        ClassificationDataset test = splitAtElement(data,static_cast<std::size_t>(0.8*data.numberOfElements()));
        //###end<split_data>

        //###begin<objects>
        //create a classifier for the problem
        LinearClassifier<> classifier;
        //create the lda trainer
        LDA lda;
        //###end<objects>
        //train the classifier using the training portion of the Data
        //###begin<train>
        lda.train(classifier,data);
        //###end<train>

        //###begin<eval>
        ZeroOneLoss<> loss;
        Data<unsigned int> predictions = classifier(test.inputs());
        double error = loss(test.labels(),predictions);
        //###end<eval>

        //print results
        //###begin<outputs>
        cout << "RESULTS: " << endl;
        cout << "========\n" << endl;
        cout << "test data size: " << test.numberOfElements() << endl;
        cout << "error rate: " << error << endl;
        //###end<outputs>
	
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}

