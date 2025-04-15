#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), device(new InsulinPumpDevice(6.0)), simulatedTime(QTime(12, 0)), timer(new QTimer(this))
{
    ui->setupUi(this);

    // Set to how many BG values to be generated
    eventCount = 24;

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
     connect(ui->extended_radio_button, SIGNAL(toggled(bool)), this, SLOT(setExtended()));
     connect(ui->bolus_ok_button, SIGNAL(released()), this, SLOT(submitBolusInfo()));
     //connect(ui->bolus_current_BG_textbox);
     ui->bolus_current_BG_textbox->text();

     connect(ui->control_IQ_checkBox,  SIGNAL(toggled(bool)), this, SLOT(setControlIQMode()));
     connect(ui->cgm_checkBox,  SIGNAL(toggled(bool)), this, SLOT(setCgmMode()));

     connect(ui->bolus_carb_intake_textbox,  SIGNAL(textChanged(QString)), this, SLOT(updateInsulinValue()));
     connect(ui->bolus_current_BG_textbox,  SIGNAL(textEdited(QString)), this, SLOT(updateCurrentBGWasEdited()));
     connect(ui->bolus_current_BG_textbox,  SIGNAL(textChanged(QString)), this, SLOT(updateInsulinValue()));

     connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
     timer->start(1000);

     makeGraph();

     connect(ui->BG_ChangeT, SIGNAL(released()), this, SLOT(changeTimeP()));
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

    //update UI time
    ui->home_time_label->setText(simulatedTime.toString("hh:mm"));


    // If in CGM Mode, read in  value
    if(device->getCgmMode()){
        device->readInBGFromCGM();

        if(device->getCurrentBG() < 3.9){
            hypoglycemiaAlert();
        } else if(device->getCurrentBG() > 10.0){
            hyperglycemiaAlert();
        }

        updateBg(device->getCurrentBG());

        cout<<"currentBG: "<<device->getCurrentBG()<<endl;
        cout<<"currentBGWasEdited: "<< currentBGWasEdited << endl;

        if (currentBGWasEdited == false){
            ui->bolus_current_BG_textbox->setText(QString::number(device->getCurrentBG()));
        }
        ui->home_current_BG_label->setText(QString::number(device->getCurrentBG()));
//        device->calculateInsulinOnBoard(minuteCounter);
//        ui->home_iob_label->setText(QString::number(device->getInsulinOnBoard()));
    }

    // Depleting battery by 1%
    device->decreaseBatteryLevel();

    if (device->getBattery()->getBatteryLevel() <= 10){
        lowBatteryAlert();
    }

    // Updating UI for battery
    ui->battery->setText(QString::number(device->getBattery()->getBatteryLevel()) + "%");

    // Updating UI for insulin cartridge
    ui->insulin_cartidge->setText(QString::number(device->getInsulinCartridge()->getInsulinLevel()) + " u");
 
}

void MainWindow::hypoglycemiaAlert(){
    // Add item to logs list
    ui->loglist->addItem(QString(ui->home_time_label->text()) + ": BLOOD GLUCOSE VERY LOW (HYPOGLYCEMIA)");

    // Go to logs screen
    go_to_logs();
}

void MainWindow::hyperglycemiaAlert(){
    // Add item to logs list
    ui->loglist->addItem(QString(ui->home_time_label->text()) + ": BLOOD GLUCOSE VERY HIGH (HYPERGLYCEMIA)");

    // Go to logs screen
    go_to_logs();
}


void MainWindow::lowBatteryAlert(){
    if (device->getBattery()->getBatteryLevel() == 0){
        ui->power_button->setText("Power Off");

        // Stop reading in BG values
        device->setCgmMode(false);

        // Add item to logs list
        ui->loglist->addItem(QString(ui->home_time_label->text()) + ": BATTERY DEAD");

        // Go to logs screen
        go_to_logs();

        // Power off
        power();

    } else if (device->getBattery()->getBatteryLevel() <= 10){
        // Add item to logs list
        ui->loglist->addItem(QString(ui->home_time_label->text()) + ": LOW BATTERY");

        // Go to logs screen
        go_to_logs();
    }
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

    // Set CGM Mode back on
    device->setCgmMode(true);

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
    std::cout << "CURRENT STATUS BUTTON"<<std::endl;
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
    ui->activateDropdown->clear(); // Clear existing items if any

    const std::vector<UserProfile*>& profiles = device->getUserProfileManager()->getAllProfiles();
    for (UserProfile* profile : profiles) {
        ui->activateDropdown->addItem(QString::fromStdString(profile->getProfileName()));
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
        //device->getUserProfileManager()->setActiveProfile(profileName);

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


        device->getUserProfileManager()->updateProfile(profileName, basalRate, carbRate, correctionFactor, targetBG, quickBolus, insulinDuration, false);
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

        int deliverNow = ui->bolus_deliver_now_textbox->text().toInt();
        int distributionDuration = ui->bolus_insulin_duration_textbox->text().toInt();

        std::cout << "Extended selected\n" << std::endl;
        std::cout << "Deliver now: " << deliverNow << std::endl;
        std::cout << "Bolus Distribution Duration: " << distributionDuration << std::endl;

        device->deliverBolusExtended(time, bolusInsulinDose, deliverNow, distributionDuration);

    } else {
        std::cout << "Default selected\n" << std::endl;

        if(device->getInsulinCartridge()->getInsulinLevel() >= bolusInsulinDose){
            device->deliverBolusDefault(minuteCounter, bolusInsulinDose);
        } else {
            ui->loglist->addItem(QString(ui->home_time_label->text()) + ": NOT ENOUGH INSULIN IN CARTRIDGE");
            go_to_logs();
        }

    }
}

void MainWindow::setCgmMode(){
    device->setCgmMode(ui->cgm_checkBox->isChecked());
}
void MainWindow::setControlIQMode(){
    device->setControlIQMode(ui->control_IQ_checkBox->isChecked());
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
    ui->bgGraph->replot();
    ui->BG_ChangeT->setText(QString::fromStdString(std::to_string(timePeriod[currTimePeriod]) + "H"));

}


