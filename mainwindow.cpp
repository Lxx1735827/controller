#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QRegularExpression>
#include <QColor>
#include <QMessageBox>

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
    M1->setIcon(QIcon("://image/Rain.png"));
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
            qDebug()<<"发送"<<targetID<<" "<<dx * 50<<" "<<dy * 50;
            m_connector->sendData(gram);
        }else if(ui->stateContent->text()=="running"){
            QMessageBox::warning(this, "未选择目标", "请先在右上方下拉框中选择一个目标无人机！");
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
    }else if(ui->stateContent->text()=="running"){
        QMessageBox::warning(this, "未选择目标", "请先在右上方下拉框中选择一个目标无人机！");
    }

}
void MainWindow::on_down_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 0, 50);
        m_connector->sendData(gram);
    }else if(ui->stateContent->text()=="running"){
        QMessageBox::warning(this, "未选择目标", "请先在右上方下拉框中选择一个目标无人机！");
    }

}
void MainWindow::on_left_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), -50, 0);
        m_connector->sendData(gram);
    }else if(ui->stateContent->text()=="running"){
        QMessageBox::warning(this, "未选择目标", "请先在右上方下拉框中选择一个目标无人机！");
    }

}
void MainWindow::on_right_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 50, 0);
        m_connector->sendData(gram);
    }else if(ui->stateContent->text()=="running"){
        QMessageBox::warning(this, "未选择目标", "请先在右上方下拉框中选择一个目标无人机！");
    }

}
void MainWindow::on_stop_clicked()
{
    if(ui->targetDrone->currentText()!="--"){
        CtrlGram gram = CtrlGram(ui->targetDrone->currentText(), 0, 0);
        m_connector->sendData(gram);
    }else if(ui->stateContent->text()=="running"){
        QMessageBox::warning(this, "未选择目标", "请先在右上方下拉框中选择一个目标无人机！");
    }
}

void MainWindow::setTable(){
    for(int i=0;i<3;i++){
        ui->tableDrone->insertRow(i);
        ui->tableDrone->setItem(i, 0, new QTableWidgetItem("B"+QString::number(i+1)));
        ui->tableDrone->setItem(i, 1, new QTableWidgetItem("B"));
        ui->tableDrone->setItem(i, 2, new QTableWidgetItem(QString::number(100)));
        ui->tableDrone->setItem(i, 3, new QTableWidgetItem("alive"));
        for (int col = 0; col < ui->tableDrone->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableDrone->item(i, col);
            if (item) {
                item->setForeground(QBrush(Qt::blue));
            }
        }
        ui->tableDrone->setRowHeight(i, 20);
    }
    for(int i=3;i<6;i++){
        ui->tableDrone->insertRow(i);
        ui->tableDrone->setItem(i, 0, new QTableWidgetItem("R"+QString::number(i-2)));
        ui->tableDrone->setItem(i, 1, new QTableWidgetItem("R"));
        ui->tableDrone->setItem(i, 2, new QTableWidgetItem(QString::number(100)));
        ui->tableDrone->setItem(i, 3, new QTableWidgetItem("alive"));
        for (int col = 0; col < ui->tableDrone->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableDrone->item(i, col);
            if (item) {
                item->setForeground(QBrush(Qt::red));
            }
        }
        ui->tableDrone->setRowHeight(i, 20);
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
        if(obj._id=="C2"){
            M1->move(obj._x/1280*391-10, obj._y/800*241-10);
            M1->setVisible(true);
        }else if(obj._id=="R1"){
            qDebug()<<obj._x<<obj._y;
            R1->move(obj._x/1280*391-10, obj._y/800*241-10);
            R1->setVisible(true);
        }else{
            C1->move(obj._x/1280*391-10, obj._y/800*241-10);
            C1->setVisible(true);
        }
    }
    if(ui->tableDrone->rowCount() == 0){
         setTable();
    }else{
        for (const auto& drone : gram._drones){
            if(drone._team=="B"){
                int uid = drone._uid[1].toLatin1() - '0';
                uid-=1;
                ui->tableDrone->setItem(uid, 2, new QTableWidgetItem(QString::number(drone._hp)));
                ui->tableDrone->setItem(uid, 3, new QTableWidgetItem(drone._status));
                for (int col = 0; col < ui->tableDrone->columnCount(); ++col) {
                    QTableWidgetItem* item = ui->tableDrone->item(uid, col);
                    if (item) {
                        item->setForeground(QBrush(Qt::blue));
                    }
                }
            }else{
                int uid = drone._uid[1].toLatin1() - '0';
                uid+=2;
                ui->tableDrone->setItem(uid, 2, new QTableWidgetItem(QString::number(drone._hp)));
                ui->tableDrone->setItem(uid, 3, new QTableWidgetItem(drone._status));
                for (int col = 0; col < ui->tableDrone->columnCount(); ++col) {
                    QTableWidgetItem* item = ui->tableDrone->item(uid, col);
                    if (item) {
                        item->setForeground(QBrush(Qt::red));
                    }
                }
                qDebug()<<"修改了第"<<uid<<"行为红色";
            }
        }
    }
    for (const auto& drone : gram._drones) {
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




