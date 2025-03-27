#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->textbox1->setReadOnly(1);


     connect(ui->recharge_button, SIGNAL(released()), this, SLOT(test1()));

     connect(ui->insulin_refill_button, SIGNAL(released()), this, SLOT(test2()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test1(){
    std::cout << "Test 1 is being called "<<std::endl;

}

void MainWindow::test2(){
    std::cout << "Test 2 is being called "<<std::endl;



}
