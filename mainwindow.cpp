#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cmath>   // for round()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), device(new InsulinPumpDevice(6.0))
{
    ui->setupUi(this);

    // Set to how many BG values to be generated
    eventCount = 24;

    // Seed for randomly generating BG values
    std::srand(std::time(0));


     connect(ui->recharge_button, SIGNAL(released()), this, SLOT(test1()));

     connect(ui->insulin_refill_button, SIGNAL(released()), this, SLOT(test2()));

     // For testing only
     connect(ui->test_log_button, SIGNAL(released()), this, SLOT(test_log()));
     connect(ui->test_profile_list_button, SIGNAL(released()), this, SLOT(test_profiles_list()));
     connect(ui->test_current_status_button, SIGNAL(released()), this, SLOT(test_current_status()));
     connect(ui->test_options_button, SIGNAL(released()), this, SLOT(test_options()));
     connect(ui->test_profile_button, SIGNAL(released()), this, SLOT(test_profile()));
     connect(ui->test_bolus_button, SIGNAL(released()), this, SLOT(go_to_bolus()));

    // Home related signals and slots
     connect(ui->home_button, SIGNAL(released()), this, SLOT(go_to_home()));
     connect(ui->home_options_button, SIGNAL(released()), this, SLOT(go_to_options()));
     connect(ui->home_bolus_button, SIGNAL(released()), this, SLOT(go_to_bolus()));
     connect(ui->power_button, SIGNAL(released()), this, SLOT(power()));

     // Profile related signals and slots
     connect(ui->personal_profiles_list_add_button, SIGNAL(released()), this, SLOT(add_profile()));
     connect(ui->edit_profile_button, SIGNAL(released()), this, SLOT(edit_button()));
     connect(ui->delete_profile_button, SIGNAL(released()), this, SLOT(delete_profile()));
     connect(ui->personal_profiles_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(profile_item_clicked(QListWidgetItem*)));

     // Bolus related signals and slots
     connect(ui->extended_radio_button, SIGNAL(toggled(bool)), this, SLOT(setExtended()));
     connect(ui->extended_radio_button, SIGNAL(toggled(bool)), this, SLOT(setExtended()));
     connect(ui->bolus_ok_button, SIGNAL(released()), this, SLOT(submitBolusInfo()));
     //connect(ui->bolus_current_BG_textbox);
     ui->bolus_current_BG_textbox->text();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generateEvents(){

}

double MainWindow::generateVeryLow() {
    double newBG = std::round((3.8 * std::rand() / RAND_MAX) * 10) / 10; // 0.0 - 3.8
    //return std::round((3.8 * std::rand() / RAND_MAX) * 10) / 10; // 0.0 - 3.8
    device->setCurrentBG(newBG);
    return newBG;
}

double MainWindow::generateLow() {
    double newBG = std::round((2.3 * std::rand() / RAND_MAX + 3.9) * 10) / 10; // 3.9 - 6.2
    //return std::round((2.3 * std::rand() / RAND_MAX + 3.9) * 10) / 10; // 3.9 - 6.2
    device->setCurrentBG(newBG);
    return newBG;
}

double MainWindow::generateNormal() {
    double newBG = std::round((2.6 * std::rand() / RAND_MAX + 6.3) * 10) / 10; // 6.3 - 8.9
    //return std::round((2.6 * std::rand() / RAND_MAX + 6.3) * 10) / 10; // 6.3 - 8.9
    device->setCurrentBG(newBG);
    return newBG;
}

double MainWindow::generateHigh() {
    double newBG = std::round((1.1 * std::rand() / RAND_MAX + 8.9) * 10) / 10; // 9.0 - 10.0
    //return std::round((1.1 * std::rand() / RAND_MAX + 8.9) * 10) / 10; // 9.0 - 10.0
    device->setCurrentBG(newBG);
    return newBG;
}

double MainWindow::generateVeryHigh() {
    // 15.0 was set as an upper bound, design choice

    double newBG = std::round((5.0 * std::rand() / RAND_MAX + 10.0) * 10) / 10; // 10.1 - 15.0
    //return std::round((5.0 * std::rand() / RAND_MAX + 10.0) * 10) / 10; // 10.1 - 15.0
    device->setCurrentBG(newBG);
    return newBG;
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
        ui->log_screen->setHidden(1);
        ui->personal_profiles_list_screen->setHidden(1);
        ui->current_status_screen->setHidden(1);
        ui->options_screen->setHidden(1);
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
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
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


void MainWindow::test_log() {
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(1);
    ui->current_status_screen->setHidden(1);
    ui->options_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "LOG BUTTON"<<std::endl;
}

void MainWindow::test_profiles_list() {
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(1);
    ui->options_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "PERSONAL PROFILES LIST BUTTON"<<std::endl;

    ui->personal_profiles_list->clear();

    //for (UserProfile* profile : profileManager.getAllProfiles()) {
    for (UserProfile* profile : device->getUserProfileManager()->getAllProfiles()) {
        QString profileName = QString::fromStdString(profile->getProfileName());
        ui->personal_profiles_list->addItem(profileName);
    }
}
void MainWindow::test_current_status() {
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "CURRENT STATUS BUTTON"<<std::endl;
}

void MainWindow::test_options() {
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    std::cout << "OPTIONS BUTTON"<<std::endl;
}


void MainWindow::test_profile() {
    ui->profile_name_textbox->setReadOnly(true);
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    ui->edit_profile_button->setText("Save");
    std::cout << "PERSONAL PROFILE BUTTON"<<std::endl;
}

void MainWindow::go_to_bolus()  {
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);
    ui->bolus_screen->setHidden(0);

    ui->home_screen->setHidden(1);
    // Sets current BG
    ui->bolus_current_BG_textbox->setText(QString::number(device->getCurrentBG()));
    std::cout << "BOLUS BUTTON"<<std::endl;
}

void MainWindow::add_profile(){
    std::cout << "mainwindow: add_profile" << std::endl;
    ui->profile_name_textbox->setReadOnly(false);
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);
    ui->edit_profile_button->setText("Create");
    std::cout << "ADD PROFILE BUTTON"<<std::endl;

    ui->personal_profiles_list->clear();

    //for (UserProfile* profile : profileManager.getAllProfiles()) {
    for (UserProfile* profile : device->getUserProfileManager()->getAllProfiles()) {
        QString profileName = QString::fromStdString(profile->getProfileName());
        ui->personal_profiles_list->addItem(profileName);
    }
}

void MainWindow::edit_button(){
    std::cout << "mainwindow: edit_button" << std::endl;
    if (ui->edit_profile_button->text() == "Create") {
        std::cout << "mainwindow: add_profile: CREATE" << std::endl;
        ui->profile_name_textbox->setReadOnly(false);
        int basalRate = ui->basal_rate_textbox->text().toInt();
        int carbRate = ui->carb_ratio_textbox->text().toInt();
        int correctionFactor = ui->correction_factor_textbox->text().toInt();
        std::string profileName = ui->profile_name_textbox->text().toStdString();
        double quickBolus = ui->quick_bolus_textbox->text().toDouble();
        double targetBG = ui->target_BG_textbox->text().toDouble();
        int insulinDuration = ui->insulin_duration_textbox->text().toInt();

        //profileManager.createProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, true);
        device->getUserProfileManager()->createProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, true);
        //profileManager.setActiveProfile(profileName);
        device->getUserProfileManager()->setActiveProfile(profileName);

        std::cout << "Profile Name: " << profileName << ", Basal Rate: " << basalRate << ", Carb Rate: " << carbRate << ", Correction Factor: " << correctionFactor << ", Quick Bolus: " << quickBolus << ", Target BG: " << targetBG << std::endl;

        //std::cout << "CREATE BUTTON" << std::endl;

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
        std::cout << "mainwindow: add_profile: SAVE" << std::endl;
        ui->profile_name_textbox->setReadOnly(true);
        int basalRate = ui->basal_rate_textbox->text().toInt();
        int carbRate = ui->carb_ratio_textbox->text().toInt();
        int correctionFactor = ui->correction_factor_textbox->text().toInt();
        std::string profileName = ui->profile_name_textbox->text().toStdString();
        double quickBolus = ui->quick_bolus_textbox->text().toDouble();
        double targetBG = ui->target_BG_textbox->text().toDouble();
        int insulinDuration = ui->insulin_duration_textbox->text().toInt();


        device->getUserProfileManager()->updateProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, false);
        //UserProfileVector.updateProfile(basalRate, carrbRate,, corrrectionFactor, profileeName, quiickBolus,targetBG);
        std::cout << "Profile Name: " << profileName << ", Basal Rate: " << basalRate << ", Carb Rate: " << carbRate << ", Correction Factor: " << correctionFactor << ", Quick Bolus: " << quickBolus << ", Target BG: " << targetBG << ", Insulin Duration: " << insulinDuration << std::endl;

        //std::cout << "SAVE BUTTON" << std::endl;

        test_profiles_list();
        // Clear the textboxes after saving profile
        ui->profile_name_textbox->clear();
        ui->basal_rate_textbox->clear();
        ui->carb_ratio_textbox->clear();
        ui->correction_factor_textbox->clear();
        ui->quick_bolus_textbox->clear();
        ui->target_BG_textbox->clear();
        ui->insulin_duration_textbox->clear();

    }
    //ui->edit_profile_button->setText("Save");
}

void MainWindow::delete_profile(){
    std::string profileName = ui->profile_name_textbox->text().toStdString();
    //UserProfileVector.deleteProfile(profileeName);
    std::cout << "DELETE BUTTON" << std::endl;
}


void MainWindow::go_to_home(){
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(0);
    ui->personal_profile_screen->setHidden(0);
    ui->bolus_screen->setHidden(0);
    ui->home_screen->setHidden(0);

    std::cout << "HOME BUTTON"<<std::endl;

}

void MainWindow::profile_item_clicked(QListWidgetItem* item) {
    std::string selectedProfileName = item->text().toStdString();

    //UserProfile* selectedProfile = profileManager.getprofile(selectedProfileName);
    UserProfile* selectedProfile = device->getUserProfileManager()->getprofile(selectedProfileName);

    if (selectedProfile) {
        ui->profile_name_textbox->setText(QString::fromStdString(selectedProfile->getProfileName()));
        ui->basal_rate_textbox->setText(QString::number(selectedProfile->getBasalRate()));
        ui->carb_ratio_textbox->setText(QString::number(selectedProfile->getCarbRatio()));
        ui->correction_factor_textbox->setText(QString::number(selectedProfile->getCorrectionFactor()));
        ui->quick_bolus_textbox->setText(QString::number(selectedProfile->getquickBolusUnits()));
        ui->target_BG_textbox->setText(QString::number(selectedProfile->gettargetBGlevel()));
        ui->insulin_duration_textbox->setText(QString::number(selectedProfile->getInsulinDuration()));

        ui->edit_profile_button->setText("Save");
        ui->profile_name_textbox->setReadOnly(true);
        test_profile();  // Call any function to refresh UI if needed

        std::cout << "Loaded profile: " << selectedProfileName << std::endl;
    } else {
        std::cerr << "Profile not found!" << std::endl;
    }
}

// Allows user to enter the input for extended bolus only if radio button is checked
void MainWindow::setExtended() {
    if (ui->extended_radio_button->isChecked()) {
        ui->bolus_insulin_duration_textbox->setReadOnly(false);
        ui->bolus_deliver_now_textbox->setReadOnly(false);
    } else {
        ui->bolus_insulin_duration_textbox->setReadOnly(true);
        ui->bolus_deliver_now_textbox->setReadOnly(true);
        ui->bolus_insulin_duration_textbox->setText("");
        ui->bolus_deliver_now_textbox->setText("");

    }
}

void MainWindow::submitBolusInfo()  {
    double currentBloodGlucose = ui->bolus_current_BG_textbox->text().toDouble();
    int carbIntake = ui->bolus_carb_intake_textbox->text().toDouble();
    double insulinDose = ui->bolus_insulin_dose_textbox->text().toDouble();

    std::cout << "Current BG: " << currentBloodGlucose << std::endl;
    std::cout << "Carb Intake: " << carbIntake << std::endl;
    std::cout << "Insulin Dose: " << insulinDose << std::endl;

    if(ui->extended_radio_button->isChecked()){
        std::cout << "Extended selected\n" << std::endl;
        int deliverNow = ui->bolus_deliver_now_textbox->text().toInt();
        int duration = ui->bolus_insulin_duration_textbox->text().toInt();
        std::cout << "Deliver now: " << deliverNow << std::endl;
        std::cout << "Insulin Duration: " << duration << std::endl;

    } else {
        std::cout << "Default selected\n" << std::endl;
    }
}
