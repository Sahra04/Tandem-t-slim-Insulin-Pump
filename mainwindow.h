#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void test1();
    void test2();

    void edit_button();
    void add_profile();
    void go_to_home();
    void test_lock();
    void test_log();
    void test_profiles_list();
    void test_profile();
    void test_options();
    void test_setup_pin();
    void test_current_status();
    void test_bolus();
    void delete_profile();
    void go_to_options();
    void power();
    void activate_profile();


};
#endif // MAINWINDOW_H
