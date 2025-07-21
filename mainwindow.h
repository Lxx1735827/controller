#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QList>
#include <QIcon>
#include <QProgressBar>
#include "connector.h"
#include "gamegram.h"
#include "ctrlgram.h"
#include "JoystickWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setContent(const GameGram& gram);
    ~MainWindow();

private slots:
    void on_up_clicked();
    void on_down_clicked();
    void on_left_clicked();
    void on_right_clicked();
    void on_stop_clicked();
    void setTable();

private:
    Ui::MainWindow *ui;
    QList<QPushButton*> buttonB;
    QList<QPushButton*> buttonR;
    QPushButton* M1;
    QPushButton* R1;
    QPushButton* C1;
    Connector* m_connector;
    JoystickWidget* joystick;
    QProgressBar* hpBar;
};
#endif // MAINWINDOW_H
