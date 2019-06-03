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
};

#endif // MAINWINDOW_H
