#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QRegularExpression>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //表格
    ui->tableDrone->setColumnCount(4);
    QStringList headers;
    headers << "UID" << "TEAM" << "HP"<< "Status";
    ui->tableDrone->setHorizontalHeaderLabels(headers);
    ui->tableDrone->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //图像框
    for (int i = 0; i < 3; ++i) {
        QPushButton* button = new QPushButton(ui->widget); // 创建QLabel并设置父控件为frame
        button->resize(20, 20);
        button->setFlat(true);
        button->setStyleSheet("QPushButton{border:none;background:transparent;}");
        button->setIcon(QIcon("://image/DroneB.png"));
        button->setVisible(false);
        buttonB.append(button);
    }
    for (int i = 0; i < 3; ++i) {
        QPushButton* button = new QPushButton(ui->widget); // 创建QLabel并设置父控件为frame
        button->resize(20, 20);
        button->setFlat(true);
        button->setStyleSheet("QPushButton{border:none;background:transparent;}");
        button->setIcon(QIcon("://image/DroneR.png"));
        button->setVisible(false);
        buttonR.append(button);
    }
    M1 = new QPushButton(ui->widget);
    M1->resize(20, 20);
    M1->setFlat(true);
    M1->setStyleSheet("QPushButton{border:none;background:transparent;}");
    M1->setIcon(QIcon("://image/Mountain.png"));
    M1->setVisible(false);
    R1 = new QPushButton(ui->widget);
    R1->resize(20, 20);
    R1->setFlat(true);
    R1->setStyleSheet("QPushButton{border:none;background:transparent;}");
    R1->setIcon(QIcon("://image/Ladar.png"));
    R1->setVisible(false);
    C1 = new QPushButton(ui->widget);
    C1->resize(20, 20);
    C1->setFlat(true);
    C1->setStyleSheet("QPushButton{border:none;background:transparent;}");
    C1->setIcon(QIcon("://image/Rain.png"));
    C1->setVisible(false);

    m_connector = new Connector(this);
    m_connector->setGameMessageHandler([this](const GameGram& gram){
        this->setContent(gram);
    });
    m_connector->enableReceive();
    ui->targetDrone->setEnabled(true);

    joystick = new JoystickWidget(ui->widgetControl);
    joystick->setGeometry(230, 30, 120, 120); // 设置位置与大小，可根据 UI 调整
    connect(joystick, &JoystickWidget::directionChanged,
            this, [=](double dx, double dy){
        QString targetID = ui->targetDrone->currentText();
        if(targetID!="--"){
            CtrlGram gram(targetID, dx * 50, dy * 50);  // 可以调节乘数控制灵敏度
            m_connector->sendData(gram);
        }

    });

    hpBar = new QProgressBar(ui->scrollArea);
    hpBar->setGeometry(40, 365, 200, 13);
    hpBar->setRange(0, 100);
    hpBar->setValue(0);
    hpBar->setTextVisible(true);
    hpBar->setStyleSheet(R"(
        QProgressBar {
            border: 1px solid gray;
            text-align: center;
            color: black;
        }
        QProgressBar::chunk {
            background-color: blue;
        }
    )");

    connect(ui->up, &QPushButton::clicked, this, &MainWindow::on_up_clicked);
    connect(ui->down, &QPushButton::clicked, this, &MainWindow::on_down_clicked);
    connect(ui->right, &QPushButton::clicked, this, &MainWindow::on_right_clicked);
    connect(ui->left, &QPushButton::clicked, this, &MainWindow::on_left_clicked);
    connect(ui->stopb, &QPushButton::clicked, this, &MainWindow::on_stop_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_up_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 0, -50);
        m_connector->sendData(gram);
    }

}
void MainWindow::on_down_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 0, 50);
        m_connector->sendData(gram);
    }

}
void MainWindow::on_left_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), -50, 0);
        m_connector->sendData(gram);
    }

}
void MainWindow::on_right_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 50, 0);
        m_connector->sendData(gram);
    }

}
void MainWindow::on_stop_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 0, 0);
        m_connector->sendData(gram);
    }
}
void MainWindow::setContent(const GameGram& gram) {
    QString stateContent = QString("%1").arg(gram._stage);
    QString leftTime = QString("%1").arg(gram._left_time);
    ui->stateContent->setText(stateContent);
    ui->leftTime->setText(leftTime);
    if(stateContent=="finish"){
        ui->uid->setText("--");
        ui->team->setText("--");
        ui->status->setText("--");
        ui->xl->setText("--");
        ui->yl->setText("--");
        ui->vx->setText("--");
        ui->vy->setText("--");
        hpBar->setValue(0);
    }

    ui->tableDrone->setRowCount(0);

    int row = 0;
    for (int i=0; i<3; i++){
         buttonR[i]->setVisible(false);
    }
    for (int i=0; i<3; i++){
         buttonB[i]->setVisible(false);
    }
    M1->setVisible(false);
    R1->setVisible(false);
    C1->setVisible(false);
    for (const auto& obj: gram._obstacles){
        if(obj._id=="M1"){
            M1->move(obj._x/1280*391-10, obj._y/800*241-10);
            M1->setVisible(true);
        }else if(obj._id=="R1"){
            R1->move(obj._x/1280*391-10, obj._y/800*241-10);
            R1->setVisible(true);
        }else{
            C1->move(obj._x/1280*391-10, obj._y/800*241-10);
            C1->setVisible(true);
        }
    }
    for (const auto& drone : gram._drones) {
        ui->tableDrone->insertRow(row);
        ui->tableDrone->setItem(row, 0, new QTableWidgetItem(drone._uid));
        ui->tableDrone->setItem(row, 1, new QTableWidgetItem(drone._team));
        ui->tableDrone->setItem(row, 2, new QTableWidgetItem(QString::number(drone._hp)));
        ui->tableDrone->setItem(row, 3, new QTableWidgetItem(drone._status));
        for (int col = 0; col < ui->tableDrone->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableDrone->item(row, col);
            if (item) {
                if(drone._team=="B"){
                    item->setForeground(QBrush(Qt::blue));
                }else{
                    item->setForeground(QBrush(Qt::red));
                }
            }
        }
        if(drone._uid==ui->targetDrone->currentText()){
            ui->uid->setText(drone._uid);
            ui->team->setText(drone._team);
            ui->status->setText(drone._status);
            ui->xl->setText(QString::number(drone._x, 'f', 8).left(8));
            ui->yl->setText(QString::number(drone._x, 'f', 8).left(8));
            if(drone._status=="down"){
                ui->vx->setText("0");
                ui->vy->setText("0");
            }else{
                ui->vx->setText(QString::number(drone._vx, 'f', 8).left(8));
                ui->vy->setText(QString::number(drone._vy, 'f', 8).left(8));
            }
            hpBar->setValue(drone._hp);
        }
        double x = drone._x/1280*391-10;
        double y = drone._y/800*241-10;
        int uid = 0;
        QString numberStr = drone._uid.mid(1);
        uid = numberStr.toInt();
        if (drone._team == "B") {
            buttonB[uid-1]->move(x, y);
            buttonB[uid-1]->setVisible(true);
            if(drone._status=="down"){
                buttonB[uid-1]->setIcon(QIcon("://image/DroneB2.png"));
            }else{
                buttonB[uid-1]->setIcon(QIcon("://image/DroneB.png"));
            }
        }else{
            buttonR[uid-1]->move(x, y);
            buttonR[uid-1]->setVisible(true);
            if(drone._status=="down"){
                buttonR[uid-1]->setIcon(QIcon("://image/DroneR2.png"));
            }else{
                buttonR[uid-1]->setIcon(QIcon("://image/DroneR.png"));
            }
        }
        row++;
    }
}




