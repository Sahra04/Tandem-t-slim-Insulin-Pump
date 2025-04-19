#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UserProfile.h"
#include "UserProfileManager.h"
#include <QListWidgetItem>
#include "InsulinPumpDevice.h"
#include <iostream>
#include <cmath>   // for round()
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QDialog>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    InsulinPumpDevice *device;
    int eventCount;
    int minuteCounter = 0;
    // has an InsulinPump object, like how it has pointer to UI

    // the following 2 attributes would probably be stored in InsulinPump instead of here in MainWindow
    // std::vector<double> bgValues; // Store all BG values
    double currentBG = 7.0;
    int time = 0;

    //holds period of time graph shows
    const vector<int> timePeriod = {1,3,6};

    int currTimePeriod;
    int alertLabelTimer = 0;

    QVector<double> bgRec;
    QVector<double> timeRec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71};

    //simuation time attributes
    QTime simulatedTime;
    QTimer *timer;

    //temp
    QDialog *alertDialog;
    QMessageBox *msgBox;
    QLabel *textLabel;
    QPushButton *okButton;

    bool currentBGWasEdited = false;

private slots:
    void rechargeDevice();
    void refillCartridge();

    void edit_button();
    void add_profile();
    void go_to_home();
    void go_to_logs();
    void go_to_profiles_list();
    void go_to_profile();
    void go_to_current_status();
    void test_bgGraph();
    void go_to_bolus();
    void delete_profile();
    void go_to_options();
    void power();
    void profile_item_clicked(QListWidgetItem* item);
    void setExtended();
    void submitBolusInfo();
    void setControlIQMode();
    void setCgmMode();
    void populateActivateDropdown();
    void ActivateProfileClicked(const QModelIndex &index);
    void updateInsulinValue();
    void updateCurrentBGWasEdited();
    void checkInsulinDuration();
    void checkProfileCorrectionFactor();
    void checkProfileCarbRatio();
    void checkProfileBasalRate();
    void checkTargetBG();
    void checkProfileInsulinDuration();
    void stopOrResumeInsulin();
    void giveQuickBolus();

    void updateTimer();
    void lowBatteryAlert();

    void hypoglycemiaAlert();
    void hyperglycemiaAlert();

    void makeGraph();
    void updateBg(double);
    void changeTimeP();



};
#endif // MAINWINDOW_H
