#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UserProfile.h"
#include "UserProfileManager.h"
#include <QListWidgetItem>
#include "InsulinPumpDevice.h"

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
    // has an InsulinPump object, like how it has pointer to UI

    // the following 2 attributes would probably be stored in InsulinPump instead of here in MainWindow
    // std::vector<double> bgValues; // Store all BG values
    double currentBG = 7.0;

    // Functions

    void generateEvents();

    double generateVeryLow();
    double generateLow();
    double generateNormal();
    double generateHigh();
    double generateVeryHigh();
    //UserProfileManager profileManager;

private slots:
    void test1();
    void test2();

    void edit_button();
    void add_profile();
    void go_to_home();
    void test_log();
    void test_profiles_list();
    void test_profile();
    void test_options();
    void test_current_status();
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


};
#endif // MAINWINDOW_H
