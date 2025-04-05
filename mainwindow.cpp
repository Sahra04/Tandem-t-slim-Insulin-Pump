#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cmath>   // for round()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set to how many BG values to be generated
    eventCount = 24;

    // Seed for randomly generating BG values
    std::srand(std::time(0));


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
     connect(ui->home_button, SIGNAL(released()), this, SLOT(go_to_home()));
     connect(ui->home_options_button, SIGNAL(released()), this, SLOT(go_to_options()));
     connect(ui->power_button, SIGNAL(released()), this, SLOT(power()));
     connect(ui->personal_profiles_list_add_button, SIGNAL(released()), this, SLOT(add_profile()));
     connect(ui->edit_profile_button, SIGNAL(released()), this, SLOT(edit_button()));
     connect(ui->delete_profile_button, SIGNAL(released()), this, SLOT(delete_profile()));
     connect(ui->activate_profile_button, SIGNAL(released()), this, SLOT(activate_profile()));
     connect(ui->personal_profiles_list, &QListWidget::itemClicked,this, &MainWindow::on_profile_item_clicked);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generateEvents(){

}

double MainWindow::generateVeryLow() {
    return std::round((3.8 * std::rand() / RAND_MAX) * 10) / 10; // 0.0 - 3.8
}

double MainWindow::generateLow() {
    return std::round((2.3 * std::rand() / RAND_MAX + 3.9) * 10) / 10; // 3.9 - 6.2
}

double MainWindow::generateNormal() {
    return std::round((2.6 * std::rand() / RAND_MAX + 6.3) * 10) / 10; // 6.3 - 8.9
}

double MainWindow::generateHigh() {
    return std::round((1.1 * std::rand() / RAND_MAX + 8.9) * 10) / 10; // 9.0 - 10.0
}

double MainWindow::generateVeryHigh() {
    return std::round((5.0 * std::rand() / RAND_MAX + 10.0) * 10) / 10; // 10.1 - 15.0
    // 15.0 was set as an upper bound, design choice
}

void MainWindow::power() {
    double test = generateVeryLow();
    std::cout << "generateVeryLow: " << test << std::endl;
    test = generateLow();
    std::cout << "generateLow: " << test << std::endl;
    test = generateNormal();
    std::cout << "generateNormal: " << test << std::endl;
    test = generateHigh();
    std::cout << "generateHigh: " << test << std::endl;
    test = generateVeryHigh();
    std::cout << "generateVeryHigh: " << test << std::endl;

    if (ui->power_button->text() == "Power Off") {
        ui->power_button->setText("Power On");
        ui->Device->setHidden(0);
        ui->lock_screen->setHidden(1);
        ui->log_screen->setHidden(1);
        ui->personal_profiles_list_screen->setHidden(1);
        ui->current_status_screen->setHidden(1);
        ui->options_screen->setHidden(1);
        ui->setup_pin_screen->setHidden(1);
        ui->personal_profile_screen->setHidden(1);
        ui->bolus_screen->setHidden(1);
        ui->home_screen->setHidden(1);
        ui->battery->setHidden(1);
        ui->insulin_cartidge->setHidden(1);
        ui->current_status_button->setHidden(1);

        // Design choice to hide home  button too
        ui->home_button->setHidden(1);
    } else {
        ui->power_button->setText("Power Off");
        ui->battery->setHidden(0);
        ui->insulin_cartidge->setHidden(0);
        ui->current_status_button->setHidden(0);

        ui->home_button->setHidden(0);
        go_to_home();
    }
    std::cout << "POWER BUTTON"<<std::endl;

}

void MainWindow::go_to_options(){
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

    ui->personal_profiles_list->clear();

    for (UserProfile* profile : profileManager.getAllProfiles()) {
        QString profileName = QString::fromStdString(profile->getProfileName());
        ui->personal_profiles_list->addItem(profileName);
    }
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
    ui->profile_name_textbox->setReadOnly(true);
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
    ui->edit_profile_button->setText("Save");
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

void MainWindow::add_profile(){
    ui->profile_name_textbox->setReadOnly(false);
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
    ui->edit_profile_button->setText("Create");
    std::cout << "ADD PROFILE BUTTON"<<std::endl;

    ui->personal_profiles_list->clear();

    for (UserProfile* profile : profileManager.getAllProfiles()) {
        QString profileName = QString::fromStdString(profile->getProfileName());
        ui->personal_profiles_list->addItem(profileName);
    }
}

void MainWindow::edit_button(){
    if (ui->edit_profile_button->text() == "Create") {
        ui->profile_name_textbox->setReadOnly(false);
        int basalRate = ui->basal_rate_textbox->text().toInt();
        int carbRate = ui->basal_rate_textbox->text().toInt();
        int correctionFactor = ui->correction_factor_textbox->text().toInt();
        std::string profileName = ui->profile_name_textbox->text().toStdString();
        double quickBolus = ui->quick_bolus_textbox->text().toDouble();
        double targetBG = ui->target_BG_textbox->text().toDouble();
        int insulinDuration = ui->insulin_duration_textbox->text().toInt();

        profileManager.createProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, true);
        profileManager.setActiveProfile(profileName);

        std::cout << "Profile Name: " << profileName << ", Basal Rate: " << basalRate << ", Carb Rate: " << carbRate << ", Correction Factor: " << correctionFactor << ", Quick Bolus: " << quickBolus << ", Target BG: " << targetBG << std::endl;

        std::cout << "CREATE BUTTON" << std::endl;

        test_profiles_list();
        // Clear the textboxes after profile creation
        ui->profile_name_textbox->clear();
        ui->basal_rate_textbox->clear();
        ui->carb_ratio_textbox->clear();
        ui->correction_factor_textbox->clear();
        ui->quick_bolus_textbox->clear();
        ui->target_BG_textbox->clear();
        ui->insulin_duration_textbox->clear();
    } else{
        ui->profile_name_textbox->setReadOnly(true);
        int basalRate = ui->basal_rate_textbox->text().toInt();
        int carbRate = ui->basal_rate_textbox->text().toInt();
        int correctionFactor = ui->correction_factor_textbox->text().toInt();
        std::string profileName = ui->profile_name_textbox->text().toStdString();
        double quickBolus = ui->quick_bolus_textbox->text().toDouble();
        double targetBG = ui->target_BG_textbox->text().toDouble();
        int insulinDuration = ui->insulin_duration_textbox->text().toInt();

        //UserProfileVector.updateProfile(basalRate, carrbRate,, corrrectionFactor, profileeName, quiickBolus,targetBG);
        std::cout << "Profile Name: " << profileName << ", Basal Rate: " << basalRate << ", Carb Rate: " << carbRate << ", Correction Factor: " << correctionFactor << ", Quick Bolus: " << quickBolus << ", Target BG: " << targetBG << std::endl;

        std::cout << "SAVE BUTTON" << std::endl;

    }
    ui->edit_profile_button->setText("Save");
}

void MainWindow::delete_profile(){
    std::string profileName = ui->profile_name_textbox->text().toStdString();
    //UserProfileVector.deleteProfile(profileeName);
    std::cout << "DELETE BUTTON" << std::endl;
}

void MainWindow::activate_profile(){
    std::string profileName = ui->profile_name_textbox->text().toStdString();
    //UserProfileVector.activate_profile(profileeName);
    std::cout << "ACTIVATE BUTTON" << std::endl;
}

void MainWindow::go_to_home(){
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
