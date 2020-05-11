#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_pu_1_clicked();
    void on_pu_2_clicked();
    void on_pu_3_clicked();
    void on_pu_4_clicked();
    void on_pu_5_clicked();

    void on_cx_currentIndexChanged(int index);
    void on_dh_begin_activated(int index);
    void on_dh_end_activated(int index);
};


#endif // MAINWINDOW_H
