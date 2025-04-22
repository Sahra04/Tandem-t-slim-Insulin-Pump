#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), device(new InsulinPumpDevice(6.0)), simulatedTime(QTime(12, 0)), timer(new QTimer(this)), alertDialog(new QDialog(this))
{
    ui->setupUi(this);

    //for alert popup
    alertDialog->setWindowTitle("ALERT");
    alertDialog->setFixedSize(300, 150);

    textLabel = new QLabel("", alertDialog);
    okButton = new QPushButton("OK", alertDialog);
    textLabel->setGeometry(50, 50, 200, 30);  // x, y, width, height
    textLabel->setAlignment(Qt::AlignCenter);

    okButton->setGeometry(110, 100, 80, 30);
    connect(okButton, &QPushButton::clicked, alertDialog, &QDialog::close);

    // Seed for randomly generating BG values
    std::srand(std::time(0));

    // set inital graph time period
    currTimePeriod = 0;

     // recharge and refill
     connect(ui->recharge_button, SIGNAL(released()), this, SLOT(rechargeDevice()));
     connect(ui->insulin_refill_button, SIGNAL(released()), this, SLOT(refillCartridge()));

     // For testing only
     connect(ui->logs_screen_button, SIGNAL(released()), this, SLOT(go_to_logs()));
     connect(ui->personal_profiles_screen_button, SIGNAL(released()), this, SLOT(go_to_profiles_list()));
     connect(ui->current_status_button, SIGNAL(released()), this, SLOT(go_to_current_status()));

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
     connect(ui->activateDropdown->view(), &QAbstractItemView::pressed,this, &MainWindow::ActivateProfileClicked);

     // Bolus related signals and slots
     connect(ui->extended_radio_button, SIGNAL(toggled(bool)), this, SLOT(setExtended()));
     connect(ui->bolus_ok_button, SIGNAL(released()), this, SLOT(submitBolusInfo()));

     connect(ui->control_IQ_checkBox,  SIGNAL(toggled(bool)), this, SLOT(setControlIQMode()));
     connect(ui->cgm_checkBox,  SIGNAL(toggled(bool)), this, SLOT(setCgmMode()));

     connect(ui->bolus_carb_intake_textbox,  SIGNAL(textChanged(QString)), this, SLOT(updateInsulinValue()));
     connect(ui->bolus_current_BG_textbox,  SIGNAL(textEdited(QString)), this, SLOT(updateCurrentBGWasEdited()));
     connect(ui->bolus_insulin_dose_textbox,  SIGNAL(textEdited(QString)), this, SLOT(updateInsulinDoseWasEdited())); //sb
     connect(ui->bolus_current_BG_textbox,  SIGNAL(textChanged(QString)), this, SLOT(updateInsulinValue()));

     connect(ui->stop_resume_button, SIGNAL(released()), this, SLOT(stopOrResumeInsulin()));

     // Ensure valid input for manual bolus
     connect(ui->bolus_insulin_duration_textbox,  SIGNAL(textEdited(QString)), this, SLOT(checkInsulinDuration()));

     // Ensure valid input for profiles
     connect(ui->correction_factor_textbox,  SIGNAL(textEdited(QString)), this, SLOT(checkProfileCorrectionFactor()));
     connect(ui->carb_ratio_textbox,  SIGNAL(textEdited(QString)), this, SLOT(checkProfileCarbRatio()));
     connect(ui->basal_rate_textbox,  SIGNAL(textEdited(QString)), this, SLOT(checkProfileBasalRate()));
     connect(ui->insulin_duration_textbox,  SIGNAL(textEdited(QString)), this, SLOT(checkProfileInsulinDuration()));
     connect(ui->target_BG_textbox,  SIGNAL(textEdited(QString)), this, SLOT(checkTargetBG()));
     connect(ui->bolus_insulin_dose_textbox,  SIGNAL(textChanged(QString)), this, SLOT(checkBolusInsulinDose()));

     connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
     timer->start(1000);

     makeGraph();

     connect(ui->BG_ChangeT, SIGNAL(released()), this, SLOT(changeTimeP()));

     //for alert popup
     connect(okButton, &QPushButton::clicked, alertDialog, &QDialog::close);

     connect(ui->quick_bolus_button, SIGNAL(released()), this, SLOT(giveQuickBolus()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTimer(){
    std::cout << "#MainWindow/updateTimer"<<std::endl;
    //add 5 simulated minutes 
    simulatedTime = simulatedTime.addSecs(300);
    minuteCounter += 5;
    std::cout << "#MainWindow/updateTimer point 2"<<std::endl;

    //update UI time
    ui->home_time_label->setText(simulatedTime.toString("hh:mm"));

    std::cout << "#MainWindow/updateTimer point 3" <<std::endl;

    // If in CGM Mode, read in value
    if(device->getCgmMode()){
        std::cout << "#MainWindow/updateTimer point 4" <<std::endl;
        device->readInBGFromCGM();
        std::cout << "#MainWindow/updateTimer point 5" <<std::endl;

        ui->loglist->addItem(QString(ui->home_time_label->text())  + " BEFORE CGM Reading: " + QString::number(device->getCurrentBG()) + " mmol/L");
       

        //If controlIQ detects problem (predictedBG not in 6.3 - 8.9)
        if(device->getUserProfileManager()->getActiveProfile() != nullptr){

            //prediction value for controlIQ
            double randomNumber = (2.0 * rand() / RAND_MAX) - 1.0;
            cout<<"RANDOMNUMBER: "<<randomNumber<<endl;
            double predictedBG = ((device->getCurrentBG()) - ((device->getInsulinOnBoard())*(device->getUserProfileManager()->getActiveProfile()->getCorrectionFactor()))) + randomNumber;
            cout<<"PREDICTEDBG: "<<predictedBG<<endl;


            if(device->getControlIQMode() && (predictedBG < 6.3 || predictedBG > 8.9)) {
                // if predictedBG is high               9.0 - 10.0
                if(predictedBG >= 9.0 && predictedBG <= 10.0) {
                    device->setCurrentBasalRate(device->getCurrentBasalRate() + 0.5);
                    //pump max basal limit
                    if(device->getCurrentBasalRate() > 15.0){
                        device->setCurrentBasalRate(15);
                    }

                    cout<<"UpdateTimer:  Basal Rate Increased: "<<device->getCurrentBasalRate()<< " units"<<endl;
                    ui->loglist->addItem(QString(ui->home_time_label->text())  + " Basal Rate Increased: " + QString::number(device->getCurrentBasalRate()) + " units");
                }
                // else if predictedBG is low           3.9 - 6.2
                else if(predictedBG >= 3.9 && predictedBG <= 6.2) {
                    // decrease basal by 0.5 units
                    device->setCurrentBasalRate(device->getCurrentBasalRate() - 0.5);
                    //can't have negative basal limit
                    if(device->getCurrentBasalRate() < 0){
                        device->setCurrentBasalRate(0);
                    }

                    cout<<"UpdateTimer:  Basal Rate Decreased: "<<device->getCurrentBasalRate()<< " units"<<endl;
                    ui->loglist->addItem(QString(ui->home_time_label->text())  + " Basal Rate Decreased: " + QString::number(device->getCurrentBasalRate()) + " units");
                } 
                // else if predictedBG is veryhigh      10.1 - 15.0
                else if(predictedBG >= 10.1 && predictedBG <= 15.0){
                    // in correction bolus
                    cout<<"UpdateTimer:  Correction Bolus: "<<device->calculatateCorrectionBolus(device->getCurrentBG())<<" units"<<endl;
                    device->setCorrectionBolusInitiated(true);
                }
                // else if predictedBG is veryLow       0.0 - 3.8
                else if(predictedBG >= 0.0 && predictedBG <= 3.8){
                    device->setCurrentBasalRate(0);
                    cout<<"UpdateTimer:  Basal Rate Set to 0: "<<device->getCurrentBasalRate()<< " units"<<endl;
                    ui->loglist->addItem(QString(ui->home_time_label->text())  + " Basal Rate Stopped: " + QString::number(device->getCurrentBasalRate()) + " units");
                }
            }else{
                // normal basal (predictedBG is in 6.3 - 8.9)
                device->setCurrentBasalRate(device->getUserProfileManager()->getActiveProfile()->getBasalRate());
                cout<<"UpdateTimer:  Basal Rate Normal: "<<device->getCurrentBasalRate()<< " units"<<endl;
                ui->loglist->addItem(QString(ui->home_time_label->text())  + " Basal Rate Normal: " + QString::number(device->getCurrentBasalRate()) + " units");
            }
        }


    } else {
        // not in CGM mode
        // setting basal rate to user's profile
        std::cout << "#MainWindow/updateTimer point 6" <<std::endl;
        if(device->getUserProfileManager()->getActiveProfile() != nullptr){
            device->setCurrentBasalRate(device->getUserProfileManager()->getActiveProfile()->getBasalRate());
            cout<<"UpdateTimer:  Basal Rate Normal: "<<device->getCurrentBasalRate()<< " units"<<endl;
            ui->loglist->addItem(QString(ui->home_time_label->text())  + " Basal Rate Normal: " + QString::number(device->getCurrentBasalRate()) + " units");
        }
    }

    // CAN MOVE OUT OF CGM CHECK
    // If insulin is not stopped, basal insulin should affect next CGM reading
    if((device->getIsStopped() == false) && (device->getUserProfileManager()->getActiveProfile() != nullptr)) {
        
        // Will only happen if in ControlIQ mode
        if(device->getCorrectionBolusInitiated() == true){
            //update insulin cartridge & iob
            if(device->getInsulinCartridge()->getInsulinLevel() >= device->calculatateCorrectionBolus(device->getCurrentBG())){
                device->updateInsulinDelivery(minuteCounter, device->calculatateCorrectionBolus(device->getCurrentBG()));
                ui->loglist->addItem(QString(ui->home_time_label->text())  + ": CORRECTION DOSE DELIVERED: " + QString::number(device->calculatateCorrectionBolus(device->getCurrentBG())) + " units");
            } else {
                ui->loglist->addItem(QString(ui->home_time_label->text()) + ": NOT ENOUGH INSULIN IN CARTRIDGE");
                textLabel->setText("NOT ENOUGH INSULIN");
                alertDialog->show();
            }

            device->setCurrentBG(device->getCurrentBG() - device->calculatateCorrectionBolus(device->getCurrentBG()));
            device->setCorrectionBolusInitiated(false);
                
        }else {

            //update insulin cartridge & iob
            if(device->getInsulinCartridge()->getInsulinLevel() >= (device->getCurrentBasalRate() / 12.0)){
                device->updateInsulinDelivery(minuteCounter, (device->getCurrentBasalRate() / 12.0));
                ui->loglist->addItem(QString(ui->home_time_label->text())  + " BASAL DOSE DELIVERED: " + QString::number((device->getCurrentBasalRate() / 12.0)) + " units");
                device->setCurrentBG((device->getCurrentBG()) - ((device->getInsulinOnBoard())*(device->getUserProfileManager()->getActiveProfile()->getCorrectionFactor())));
            } else {
                ui->loglist->addItem(QString(ui->home_time_label->text()) + ": NOT ENOUGH INSULIN IN CARTRIDGE");
                textLabel->setText("NOT ENOUGH INSULIN");
                alertDialog->show();
            }

        }
        
    }   

    if(device->getCurrentBG() < 0){
            device->setCurrentBG(0);
    }


    //check if alert label should expire
    if(alertLabelTimer == 1){
        alertLabelTimer = 2;
    }

    // Only if CGM mode is on
    if(device->getCgmMode()) {
        if(device->getCurrentBG() < 3.9){
            hypoglycemiaAlert();
            alertLabelTimer = 1;

        } else if(device->getCurrentBG() > 10.0){
            hyperglycemiaAlert();
            alertLabelTimer = 1;

        }else if (alertLabelTimer == 2){
            ui->alert_label->clear();
            alertLabelTimer = 0;
        }

        ui->loglist->addItem(QString(ui->home_time_label->text())  + "AFTER CGM Reading: " + QString::number(device->getCurrentBG()) + " mmol/L");

        updateBg(device->getCurrentBG());

        // Updating currentBG on Manual Bolus screen
        if (currentBGWasEdited == false){
            ui->bolus_current_BG_textbox->setText(QString::number(device->getCurrentBG()));
        }

        // Updating currentBG on Home screen
        ui->home_current_BG_label->setText(QString::number(device->getCurrentBG()));

    }
    

    //phase 2 of extended bolus
    if(device->getExtendedBolusPhase() == 2){

        // log: insulin delievered for extended bolus per hour
        if(device->getExtendedBolusTime() == 55){
            // update status page with last bolus and time
            ui->last_bolus_amount_label->setText(QString::number(device->getBolusPerHour()));
            ui->last_bolus_time_label->setText(QString(ui->home_time_label->text()));
            ui->loglist->addItem(QString(ui->home_time_label->text())  + " EXTENDED BOLUS DELIVERED: " + QString::number(device->getBolusPerHour()) + " units");
        }

        device->deliverBolusExtended(minuteCounter);
    }

    //Still showing the updated IOB with or without CGM mode on
    ui->home_iob_label->setText(QString::number(device->getInsulinOnBoard()));

    // Depleting battery by 1%
    device->decreaseBatteryLevel();
    //Alerting user if in critical battery level
    lowBatteryAlert();

    // Updating UI for battery
    ui->battery->setText(QString::number(device->getBattery()->getBatteryLevel()) + "%");

    // Updating UI for insulin cartridge
    ui->insulin_cartidge->setText(QString::number(device->getInsulinCartridge()->getInsulinLevel()) + " u");
 
}


void MainWindow::hypoglycemiaAlert(){
    cout << "#MainWindow/hypoglycemiaAlert "<< endl;

    // Add item to logs list
    ui->loglist->addItem(QString(ui->home_time_label->text()) + ": BLOOD GLUCOSE VERY LOW (HYPOGLYCEMIA)");


    ui->alert_label->setText("ALERT: LOW GLUCOSE");
}

void MainWindow::hyperglycemiaAlert(){
    cout << "#MainWindow/hyperglycemiaAlert "<< endl;

    // Add item to logs list
    ui->loglist->addItem(QString(ui->home_time_label->text()) + ": BLOOD GLUCOSE VERY HIGH (HYPERGLYCEMIA)");

    ui->alert_label->setText("ALERT: HIGH GLUCOSE");
}


void MainWindow::lowBatteryAlert(){

    if (device->getBattery()->getBatteryLevel() == 0){
        ui->power_button->setText("Power Off");

        // Stop reading in BG values
        device->setCgmMode(false);
        ui->cgm_checkBox->setChecked(0);

        // Add item to logs list
        ui->loglist->addItem(QString(ui->home_time_label->text()) + ": BATTERY DEAD");

        alertDialog->close();

        // Power off
        power();

    } else if (device->getBattery()->getBatteryLevel() <= 10){

        //alert
        if (device->getBattery()->getBatteryLevel() == 10){
            textLabel->setText("LOW BATTERY");
            alertDialog->show();
        }


        // Add item to logs list
        ui->loglist->addItem(QString(ui->home_time_label->text()) + ": LOW BATTERY");

    }

}

void MainWindow::updateInsulinDoseWasEdited(){
    currentBGWasEdited = true;

}

// Once the textbox for currentBG was edited, do not overwrite with CGM's reading for currentBG
void MainWindow::updateCurrentBGWasEdited(){
    currentBGWasEdited = true;
    cout << "#MainWindow/updateCurrentBGWasEdited  currentBGWasEdited: " << currentBGWasEdited << endl;
    updateInsulinValue();
}

// On Bolus Screen,  autopopulating the calculated insulin dose recommended
void MainWindow::updateInsulinValue(){

    cout<<"#MAINWINDOW/updateInsulinValue; before calculating"<<endl;
    // To prevent division by 0
   if(ui->bolus_carb_intake_textbox->text().toInt() == 0 || ui->bolus_current_BG_textbox->text().toDouble() == 0){
       return;
   }

    double finalBolus = device->calculateBolus(ui->bolus_carb_intake_textbox->text().toInt(), ui->bolus_current_BG_textbox->text().toDouble(), time);

    cout<<"#MAINWINDOW/updateInsulinValue; before setting"<<endl;

    ui->bolus_insulin_dose_textbox->setText(QString::number(finalBolus));

    cout<<"#MAINWINDOW/updateInsulinValue; after settting"<<endl;



}

// Makes sure insulin duration value is correct for extended bolus
void MainWindow::checkInsulinDuration(){
    if(ui->extended_radio_button->isChecked()){
        if(ui->bolus_insulin_duration_textbox->text().toInt() > 8){
            ui->bolus_insulin_duration_textbox->setText(QString::number(8));
        } 
        if(ui->bolus_insulin_duration_textbox->text().toInt() < 1){
            ui->bolus_insulin_duration_textbox->setText(QString::number(1));
        } 

        if(device->getControlIQMode() && ui->bolus_insulin_duration_textbox->text().toInt() > 2){
            ui->bolus_insulin_duration_textbox->setText(QString::number(2));
        }
    }
    
}

void MainWindow::checkProfileCorrectionFactor(){
    //minimum correction factor is 0.1
    if(ui->correction_factor_textbox->text().toDouble() < 0.1){
        ui->correction_factor_textbox->setText(QString::number(0.1));
    }
    //maximum correction factor is 33.3
    else if (ui->correction_factor_textbox->text().toDouble() > 33.3){
        ui->correction_factor_textbox->setText(QString::number(33.3));
    }
}
void MainWindow::checkProfileCarbRatio(){
    //minimum carb ratio is 1
    if(ui->carb_ratio_textbox->text().toInt() < 1){
        ui->carb_ratio_textbox->setText(QString::number(1));
    }
    //maximum carb ratio is 300
    else if (ui->carb_ratio_textbox->text().toInt() > 300){
        ui->carb_ratio_textbox->setText(QString::number(300));
    }
}
void  MainWindow::checkProfileBasalRate(){
    //minimum basal rate is 0
    if(ui->basal_rate_textbox->text().toDouble() < 0){
        ui->basal_rate_textbox->setText(QString::number(0));
    }
    //maximum basal rate is 15
    else if (ui->basal_rate_textbox->text().toDouble() > 15){
        ui->basal_rate_textbox->setText(QString::number(15));
    }
}

void  MainWindow::checkProfileInsulinDuration(){
    //minimum insulin duration is 2
    if(ui->insulin_duration_textbox->text().toInt() < 2){
        ui->insulin_duration_textbox->setText(QString::number(2));
    }
    //maximum insulin duration is 8
    else if (ui->insulin_duration_textbox->text().toInt() > 8){
        ui->insulin_duration_textbox->setText(QString::number(8));
    }
}


void  MainWindow::checkTargetBG(){
    //mimimum targetBG is 3.9 mmol/L
     if(ui->target_BG_textbox->text().toDouble() < 3.9){
        ui->target_BG_textbox->setText(QString::number(3.9));
    }
    //maximum targetBG is 13.9 mmol/L
    else if(ui->target_BG_textbox->text().toDouble() > 13.9){
        ui->target_BG_textbox->setText(QString::number(13.9));
    }
}

void MainWindow::checkBolusInsulinDose(){
    if(ui->bolus_insulin_dose_textbox->text().toDouble() < 0.05){
        ui->bolus_insulin_dose_textbox->setText(QString::number(0.05));
    }
    else if (ui->bolus_insulin_dose_textbox->text().toDouble() > 25){
         ui->bolus_insulin_dose_textbox->setText(QString::number(25));

    }
}

void MainWindow::power() {

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

        //clear alerts
        ui->alert_label->clear();

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

void MainWindow::rechargeDevice(){
    std::cout << "#MainWindow/rechargeDevice"<<std::endl;

    ui->battery->setText("100%");
    device->getBattery()->rechargeBattery();
    ui->power_button->setText("Power On");
    power();

}

void MainWindow::refillCartridge(){
    std::cout << "#MainWindow/refillInsulin"<<std::endl;
    device->getInsulinCartridge()->refillInsulin();
}


void MainWindow::go_to_logs() {
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

void MainWindow::go_to_profiles_list() {
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
    populateActivateDropdown();
}
void MainWindow::go_to_current_status() {
    ui->Device->setHidden(0);
    ui->log_screen->setHidden(0);
    ui->personal_profiles_list_screen->setHidden(0);
    ui->current_status_screen->setHidden(0);
    ui->options_screen->setHidden(1);
    ui->personal_profile_screen->setHidden(1);
    ui->bolus_screen->setHidden(1);
    ui->home_screen->setHidden(1);

    if(device->getUserProfileManager()->getActiveProfile() != nullptr){
            ui->basal_rate_label->setText(QString::fromStdString(std::to_string(device->getCurrentBasalRate())));
            ui->carb_ratio_label->setText(QString::fromStdString(std::to_string(device->getUserProfileManager()->getActiveProfile()->getCarbRatio())));
            ui->correction_factor_label->setText(QString::fromStdString(std::to_string(device->getUserProfileManager()->getActiveProfile()->getCorrectionFactor())));
            ui->insulin_duration_label->setText(QString::fromStdString(std::to_string(device->getUserProfileManager()->getActiveProfile()->getInsulinDuration())));
            //ui->last_bolus_amount_label->setText(QString::fromStdString(std::to_string(device->getUserProfileManager()->getActiveProfile()->getCarbRatio())));
            //ui->last_bolus_time_label
            ui->profile_name_label->setText(QString::fromStdString(device->getUserProfileManager()->getActiveProfile()->getProfileName()));
            ui->target_BG_label->setText(QString::fromStdString(std::to_string(device->getUserProfileManager()->getActiveProfile()->gettargetBGlevel())));
            std::cout << "CURRENT STATUS BUTTON"<<std::endl;
        }
}



void MainWindow::go_to_profile() {
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

    std::cout << "BOLUS BUTTON"<<std::endl;

    // Only make textboxes editable if an active profile exists
    if (device->getUserProfileManager()->getActiveProfile()){
        ui->bolus_carb_intake_textbox->setReadOnly(false);
        ui->bolus_current_BG_textbox->setReadOnly(false);
        ui->bolus_insulin_dose_textbox->setReadOnly(false);
    }
}
void MainWindow::populateActivateDropdown() {
    ui->activateDropdown->clear(); // Clear existing items

    const std::vector<UserProfile*>& profiles = device->getUserProfileManager()->getAllProfiles();
    QString activeProfileName;

    // Get the active profile if one is set
    UserProfile* activeProfile = device->getUserProfileManager()->getActiveProfile();
    if (activeProfile) {
        activeProfileName = QString::fromStdString(activeProfile->getProfileName());
    }

    // Add all profile names to the dropdown
    for (UserProfile* profile : profiles) {
        QString profileName = QString::fromStdString(profile->getProfileName());
        ui->activateDropdown->addItem(profileName);
    }

    // Set the current index to the active profile, if one was found
    int index = ui->activateDropdown->findText(activeProfileName);
    if (index != -1) {
        ui->activateDropdown->setCurrentIndex(index);
    }
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

    //set insulin duration default to 5
    ui->insulin_duration_textbox->setText(QString::number(5));

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

        if (device->getUserProfileManager()->userProfiles.empty()){
            device->getUserProfileManager()->createProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, true);
            device->setCurrentBasalRate(device->getUserProfileManager()->getActiveProfile()->getBasalRate());
        } else {
            device->getUserProfileManager()->createProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, false);
        }
        

        std::cout << "Profile Name: " << profileName << ", Basal Rate: " << basalRate << ", Carb Rate: " << carbRate << ", Correction Factor: " << correctionFactor << ", Quick Bolus: " << quickBolus << ", Target BG: " << targetBG << std::endl;

        //std::cout << "CREATE BUTTON" << std::endl;

        go_to_profiles_list();
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


        device->getUserProfileManager()->updateProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, device->getUserProfileManager()->getprofile(ui->profile_name_textbox->text().toStdString())->getIsActivated());
        //UserProfileVector.updateProfile(basalRate, carrbRate,, corrrectionFactor, profileeName, quiickBolus,targetBG);
        std::cout << "Profile Name: " << profileName << ", Basal Rate: " << basalRate << ", Carb Rate: " << carbRate << ", Correction Factor: " << correctionFactor << ", Quick Bolus: " << quickBolus << ", Target BG: " << targetBG << ", Insulin Duration: " << insulinDuration << std::endl;

        //std::cout << "SAVE BUTTON" << std::endl;

        go_to_profiles_list();
        // Clear the textboxes after saving profile
        ui->profile_name_textbox->clear();
        ui->basal_rate_textbox->clear();
        ui->carb_ratio_textbox->clear();
        ui->correction_factor_textbox->clear();
        ui->quick_bolus_textbox->clear();
        ui->target_BG_textbox->clear();
        ui->insulin_duration_textbox->clear();

    }
}

void MainWindow::delete_profile(){
    std::string profileName = ui->profile_name_textbox->text().toStdString();
    std::cout << "DELETE BUTTON" << std::endl;
    device->getUserProfileManager()->deleteProfile(profileName);

    go_to_profiles_list();
    // Clear the textboxes after saving profile
    ui->profile_name_textbox->clear();
    ui->basal_rate_textbox->clear();
    ui->carb_ratio_textbox->clear();
    ui->correction_factor_textbox->clear();
    ui->quick_bolus_textbox->clear();
    ui->target_BG_textbox->clear();
    ui->insulin_duration_textbox->clear();

}

void MainWindow::ActivateProfileClicked(const QModelIndex &index) {
    QString profileName = ui->activateDropdown->itemText(index.row());
    device->getUserProfileManager()->setActiveProfile(profileName.toStdString());
    device->setCurrentBasalRate(device->getUserProfileManager()->getActiveProfile()->getBasalRate());
    std::cout << "Activated profile (clicked): " << profileName.toStdString() << std::endl;
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

    //want automatic fill in resumed
    currentBGWasEdited = false;

}

void MainWindow::profile_item_clicked(QListWidgetItem* item) {
    std::string selectedProfileName = item->text().toStdString();

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
        go_to_profile();  // Call any function to refresh UI if needed

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
        ui->bolus_insulin_duration_textbox->setText(QString::number(5));
    } else {
        ui->bolus_insulin_duration_textbox->setReadOnly(true);
        ui->bolus_deliver_now_textbox->setReadOnly(true);
        ui->bolus_insulin_duration_textbox->setText("");
        ui->bolus_deliver_now_textbox->setText("");

    }
}

void MainWindow::submitBolusInfo()  {
    // Go to home screen
    go_to_home();

    // Reset for next bolus
    currentBGWasEdited = false;

    double currentBloodGlucose = ui->bolus_current_BG_textbox->text().toDouble();
    int carbIntake = ui->bolus_carb_intake_textbox->text().toInt();
    double bolusInsulinDose = ui->bolus_insulin_dose_textbox->text().toDouble();

    std::cout << "Current BG: " << currentBloodGlucose << std::endl;
    std::cout << "Carb Intake: " << carbIntake << std::endl;
    std::cout << "Insulin Dose: " << bolusInsulinDose << std::endl;

    if(ui->extended_radio_button->isChecked()){


        device->setInsulinAmountForExtended(ui->bolus_insulin_dose_textbox->text().toDouble());
        device->setImmediateBolusPercentage(ui->bolus_deliver_now_textbox->text().toInt());
        device->setDistributionDuration(ui->bolus_insulin_duration_textbox->text().toInt());
        device->setExtendedBolusPhase(1);

        //if  controlIQ is on the insulin duration cannot be over 2 hours
        if(device->getControlIQMode() && (device->getDistributionDuration() > 2)){
            device->setDistributionDuration(2);
        }

        //phase 1 of extended bolus
        if(device->getInsulinCartridge()->getInsulinLevel() >= bolusInsulinDose){
            device->deliverBolusExtended(minuteCounter);
            //update status page with last bolus amount and time
            ui->last_bolus_amount_label->setText(QString::number(bolusInsulinDose * (double(device->getImmediateBolusPercentage())/double(100))));
            ui->last_bolus_time_label->setText(QString(ui->home_time_label->text()));
            //log: the immediate insulin delievered for extended bolus
            ui->loglist->addItem(QString(ui->home_time_label->text())  + " EXTENDED IMMEDIATE BOLUS DELIVERED: " + QString::number(bolusInsulinDose * (double(device->getImmediateBolusPercentage())/double(100))) + " units");
        }else {
            ui->loglist->addItem(QString(ui->home_time_label->text()) + ": NOT ENOUGH INSULIN IN CARTRIDGE");
            textLabel->setText("NOT ENOUGH INSULIN");
            alertDialog->show();

        }

        std::cout << "Extended selected\n" << std::endl;


    } else {
        std::cout << "Default selected\n" << std::endl;

        //default bolus
        if(device->getInsulinCartridge()->getInsulinLevel() >= bolusInsulinDose){
            device->deliverBolusDefault(minuteCounter, bolusInsulinDose);
            //update status page with last bolus amount and time
            ui->last_bolus_amount_label->setText(QString::number(bolusInsulinDose));
            ui->last_bolus_time_label->setText(QString(ui->home_time_label->text()));
            //log: the insulin delievered for manual bolus
            ui->loglist->addItem(QString(ui->home_time_label->text())  + " MANUAL BOLUS DELIVERED: " + QString::number(bolusInsulinDose) + " units");
        } else {
            ui->loglist->addItem(QString(ui->home_time_label->text()) + ": NOT ENOUGH INSULIN IN CARTRIDGE");
            //QMessageBox::about(this, "ALERT", "NOT ENOUGH INSULIN"); 
            textLabel->setText("NOT ENOUGH INSULIN");
            alertDialog->show();
        }

    }
}

void MainWindow::stopOrResumeInsulin(){
    if(device->getIsStopped()){
        device->setIsStopped(false); // resume basal insulin delivery
        ui->stop_resume_button->setText("StopmanuallyStop = true; Insulin");        
        std::cout << "MainWindow/stopOrResumeInsulin| INSULIN RESUMED \n" << std::endl;
    } else { 
        device->setExtendedBolusPhase(0); // stop extended bolus permanently
        device->setIsStopped(true); // stop basal insulin delivery
        ui->stop_resume_button->setText("Resume Insulin");
        std::cout << "MainWindow/stopOrResumeInsulin| INSULIN STOPPED \n" << std::endl;
    }
} 

void MainWindow::giveQuickBolus(){
    if(device->getUserProfileManager()->getActiveProfile()){

        double quickBolusUnits = device->getUserProfileManager()->getActiveProfile()->getquickBolusUnits();
        if(device->getInsulinCartridge()->getInsulinLevel() >= quickBolusUnits){
            device->deliverBolusDefault(minuteCounter, quickBolusUnits);
            //log: the insulin delievered for quick bolus
            ui->loglist->addItem(QString(ui->home_time_label->text())  + " QUICK BOLUS DELIVERED: " + QString::number(quickBolusUnits) + " units");
        } else {
            ui->loglist->addItem(QString(ui->home_time_label->text()) + ": NOT ENOUGH INSULIN IN CARTRIDGE");
            textLabel->setText("NOT ENOUGH INSULIN");
            alertDialog->show();
        }
    }
}

void MainWindow::setCgmMode(){
    device->setCgmMode(ui->cgm_checkBox->isChecked());

    if(ui->cgm_checkBox->isChecked() == false){
        ui->control_IQ_checkBox->setChecked(0);
    }
}
void MainWindow::setControlIQMode(){
    device->setControlIQMode(ui->control_IQ_checkBox->isChecked());


    if(ui->control_IQ_checkBox->isChecked()){
            ui->cgm_checkBox->setChecked(1);
    }
    device->setExtendedBolusPhase(0); // stop extended bolus permanently
}

void MainWindow::test_bgGraph(){
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->bgGraph->addGraph();
    ui->bgGraph->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->bgGraph->xAxis->setLabel("x");
    ui->bgGraph->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->bgGraph->xAxis->setRange(-1, 1);
    ui->bgGraph->yAxis->setRange(0, 1);
    ui->bgGraph->replot();
}

void MainWindow::makeGraph(){
    QVector<double> bgHighLimit(72,10), bgLowLimit(72,3.9);
    ui->bgGraph->addGraph();
    ui->bgGraph->addGraph();
    ui->bgGraph->addGraph();
    //QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    //timeTicker->setTimeFormat("%h:%m:%s");
    //ui->bgGraph->xAxis->setTicker(timeTicker);
    ui->bgGraph->xAxis->setTicks(false);
    ui->bgGraph->yAxis->setLabel("Blood Glucose mmol/L");
    ui->bgGraph->xAxis->setRange(0, 12 * timePeriod[currTimePeriod]);
    ui->bgGraph->yAxis->setRange(3, 11);
    ui->bgGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,5));
    //ui->bgGraph->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->bgGraph->setBackground(Qt::black);
    ui->bgGraph->yAxis->setTickLabelColor(Qt::white);
    ui->bgGraph->xAxis->setTickLabelColor(Qt::white);

    QPen limits(Qt::red);
    limits.setStyle((Qt::DashLine));
    ui->bgGraph->graph(1)->setPen(limits);
    ui->bgGraph->graph(2)->setPen(limits);
    //ui->bgGraph->graph(1)->setBrush(QBrush(Qt::red,Qt::Dense3Pattern));
    //ui->bgGraph->graph(1)->setChannelFillGraph(ui->bgGraph->graph(2));

    ui->bgGraph->graph(1)->setData(timeRec,bgHighLimit);
    ui->bgGraph->graph(2)->setData(timeRec, bgLowLimit);

    //change graph color to yellow  plot line
    QPen pen;
    pen.setColor(Qt::yellow);
    ui->bgGraph->graph(0)->setPen(pen);

    ui->bgGraph->replot();

}

void MainWindow::updateBg(double newBg){

    bgRec.append(newBg);
    if(bgRec.size() > 72){
        bgRec.pop_front();
    }

    int limit = min(bgRec.size(), timePeriod[currTimePeriod] * 12);
    ui->bgGraph->graph(0)->setData(timeRec.mid(0, limit), bgRec.mid(max(bgRec.size()- (timePeriod[currTimePeriod] * 12),0), limit));
    ui->bgGraph->replot();

}

void MainWindow::changeTimeP(){
    currTimePeriod =  (currTimePeriod + 1) % 3;
    ui->bgGraph->xAxis->setRange(0, 12 * timePeriod[currTimePeriod]);
    int limit = min(bgRec.size(), timePeriod[currTimePeriod] * 12);
    ui->bgGraph->graph(0)->setData(timeRec.mid(0, limit), bgRec.mid(max(bgRec.size()- (timePeriod[currTimePeriod] * 12),0), limit));
    ui->bgGraph->replot();
    ui->BG_ChangeT->setText(QString::fromStdString(std::to_string(timePeriod[currTimePeriod]) + "H"));

}






