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

    train->testMnist();
    qDebug()<<"All done";
}
