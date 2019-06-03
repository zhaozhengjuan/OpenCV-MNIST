#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    train = new Training();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_ButtonTrain_clicked()
{
    train->knnTrain();
    train->testMnist();
    qDebug()<<"All done";
}

void MainWindow::on_ButtonTest_clicked()
{
    //--------------------- 1. Set up training data ---------------------------------------
    Mat trainData;
    Mat labels;
    trainData = read_mnist_image(trainImage);
    labels = read_mnist_label(trainLabel);

    cout << trainData.rows << " " << trainData.cols << endl;
    cout << labels.rows << " " << labels.cols << endl;

    //------------------------ 2. Set up the support vector machines parameters --------------------
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::RBF);
    //svm->setDegree(10.0);
    svm->setGamma(0.01);
    //svm->setCoef0(1.0);
    svm->setC(10.0);
    //svm->setNu(0.5);
    //svm->setP(0.1);
    svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON));

    //------------------------ 3. Train the svm ----------------------------------------------------
    cout << "Starting training process" << endl;
    start_time_ = clock();
    svm->train(trainData, ROW_SAMPLE, labels);
    end_time_ = clock();
    cost_time_ = (end_time_ - start_time_) / CLOCKS_PER_SEC;
    cout << "Finished training process...cost " << cost_time_ << " seconds..." << endl;

    //------------------------ 4. save the svm ----------------------------------------------------
    svm->save("mnist_dataset/mnist_svm.xml");
    cout << "save as /mnist_dataset/mnist_svm.xml" << endl;


    //------------------------ 5. load the svm ----------------------------------------------------
    cout << "开始导入SVM文件...\n";
    Ptr<SVM> svm1 = StatModel::load<SVM>("mnist_dataset/mnist_svm.xml");
    cout << "成功导入SVM文件...\n";


    //------------------------ 6. read the test dataset -------------------------------------------
    cout << "开始导入测试数据...\n";
    Mat testData;
    Mat tLabel;
    testData = read_mnist_image(testImage);
    tLabel = read_mnist_label(testLabel);
    cout << "成功导入测试数据！！！\n";


    float count = 0;
    for (int i = 0; i < testData.rows; i++) {
        Mat sample = testData.row(i);
        float res = svm1->predict(sample);
        res = std::abs(res - tLabel.at<unsigned int>(i, 0)) <= FLT_EPSILON ? 1.f : 0.f;
        count += res;
    }
    cout << "正确的识别个数 count = " << count << endl;
    cout << "错误率为..." << (10000 - count + 0.0) / 10000 * 100.0 << "%....\n";

}
