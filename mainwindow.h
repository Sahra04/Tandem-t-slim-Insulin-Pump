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

    //simuation time attributes
    QTime simulatedTime;
    QTimer *timer;

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
    void makeGraph();
    void changeTimeP();
    void updateInsulinValue();
    void updateCurrentBGWasEdited();

    void updateTimer();
    void lowBatteryAlert();

    void hypoglycemiaAlert();
    void hyperglycemiaAlert();


};
#endif // MAINWINDOW_H
