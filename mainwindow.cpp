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


     connect(ui->test_lock_button, SIGNAL(released()), this, SLOT(test_lock()));
     connect(ui->test_log_button, SIGNAL(released()), this, SLOT(test_log()));
     connect(ui->test_profile_list_button, SIGNAL(released()), this, SLOT(test_profiles_list()));
     connect(ui->test_current_status_button, SIGNAL(released()), this, SLOT(test_current_status()));
     connect(ui->test_options_button, SIGNAL(released()), this, SLOT(test_options()));
     connect(ui->test_setup_pin_button, SIGNAL(released()), this, SLOT(test_setup_pin()));
     connect(ui->test_profile_button, SIGNAL(released()), this, SLOT(test_profile()));
     connect(ui->test_bolus_button, SIGNAL(released()), this, SLOT(test_bolus()));
     connect(ui->home_button, SIGNAL(released()), this, SLOT(test_home()));


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


void MainWindow::test_lock() {
        ui->Device->setHidden(0);
        ui->lock_screen->setHidden(0);

        ui->log_screen->setHidden(1);
        ui->personal_profiles_list_screen->setHidden(1);
        ui->current_status_screen->setHidden(1);
        ui->options_screen->setHidden(1);
        ui->setup_pin_screen->setHidden(1);
        ui->personal_profile_screen->setHidden(1);
        ui->bolus_screen->setHidden(1);
        ui->home_screen->setHidden(1);

        std::cout << "LOCK BUTTON"<<std::endl;

}
void MainWindow::test_log() {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);

    ui->personal_profiles_list_screen->setHidden(1);
    ui->current_status_screen->setHidden(1);
    ui->options_screen->setHidden(1);
    ui->setup_pin_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "LOG BUTTON"<<std::endl;
}

void MainWindow::test_profiles_list() {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);

    ui->current_status_screen->setHidden(1);
    ui->options_screen->setHidden(1);
    ui->setup_pin_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "PERSONAL PROFILES LIST BUTTON"<<std::endl;
}
void MainWindow::test_current_status() {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);

    ui->options_screen->setHidden(1);
    ui->setup_pin_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "CURRENT STATUS BUTTON"<<std::endl;
}

void MainWindow::test_options() {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);

    ui->setup_pin_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "OPTIONS BUTTON"<<std::endl;
}

void MainWindow::test_setup_pin() {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->setup_pin_screen->setHidden(0);

    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "SETUP PIN BUTTON"<<std::endl;
}

void MainWindow::test_profile() {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->setup_pin_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);

    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "PERSONAL PROFILE BUTTON"<<std::endl;
}

void MainWindow::test_bolus()  {
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->setup_pin_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);
    ui->bolus_screen->setHidden(0);

    ui->home_screen->setHidden(1);
    std::cout << "BOLUS BUTTON"<<std::endl;
}

void MainWindow::test_home(){
    ui->Device->setHidden(0);
    ui->lock_screen->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->setup_pin_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);
    ui->bolus_screen->setHidden(0);
    ui->home_screen->setHidden(0);

    std::cout << "HOME BUTTON"<<std::endl;

}

