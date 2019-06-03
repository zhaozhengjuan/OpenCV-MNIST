#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "training.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_ButtonTrain_clicked();

    void on_ButtonTest_clicked();

private:
    Ui::MainWindow ui;
    Training *train;
    //计时器
    double cost_time_;
    clock_t start_time_;
    clock_t end_time_;
};

#endif // MAINWINDOW_H
