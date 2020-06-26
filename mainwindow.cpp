#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include "QDebug"
#include "QWidget"
#include "QImage"
#include "QTimer"
#include "QLayout"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Timetable* timetable = new Timetable();
    init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    //this->setFixedSize(1280, 720);
    this->setWindowTitle("模拟旅行系统");

    //建立一个数字跟城市对应的哈希表
    map.insert(0,"北京");
    map.insert(1,"上海");
    map.insert(2,"深圳");
    map.insert(3,"杭州");
    map.insert(4,"西安");
    map.insert(5,"哈尔滨");
    map.insert(6,"昆明");
    map.insert(7,"成都");
    map.insert(8,"桂林");
    map.insert(9,"拉萨");
    map.insert(10,"乌鲁木齐");
    map.insert(11,"台北");
    map.insert(12,"香港");
    map.insert(13,"澳门");
    //选项初始化
    ui->strategy_comboBox->addItem("最少策略风险");
    ui->strategy_comboBox->addItem("限时最少策略风险");
    travelers = 0;

    //QTime
    mstimer = new QTimer;
    mstimer->setInterval(1);

    timethread = new QThread();
    timethread->start();

    mstimer->moveToThread(timethread);

    //定义槽函数
    //按下添加乘客按扭后，执行操作
    connect(ui->add_traveler_button,&QPushButton::clicked,this,&MainWindow::addTraveler);
    connect(ui->add_traveler_button,&QPushButton::clicked,this,&MainWindow::updateUi);
    connect(ui->choosecity_button,&QPushButton::clicked,this,&MainWindow::chooseCity);

    connect(ui->current_traveler_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeTraveler()));
    connect(ui->test_button,&QPushButton::clicked,this,&MainWindow::test);

    connect(ui->start_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getStart()));
    connect(ui->destination_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDestination()));
    connect(ui->strategy_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeStrategy()));

    //
    connect(ui->test_button,&QPushButton::clicked,this,&MainWindow::getPredictTime);
    //计时器相关
    connect(mstimer,SIGNAL(timeout()),this,SLOT(showTime()));
    connect(ui->test_button, SIGNAL(clicked()), this, SLOT(timeStart()));
    connect(this, SIGNAL(DoStartTimer()), mstimer, SLOT(start()));
}

void MainWindow::addTraveler()
{
    travelers++;
    Traveler* temp_traveler = new Traveler();
    traveler_list.push_back(temp_traveler);

    ui->current_traveler_comboBox->addItem(QString::number(travelers));
    //ui->current_traveler_comboBox->setCurrentText(QString::number(travelers));
    cur_travel = ui->current_traveler_comboBox->currentIndex();


    //ui->start_comboBox->setCurrentIndex(0);
    //ui->destination_comboBox->setCurrentIndex(1);

    traveler_list[travelers-1]->start = 0;
    traveler_list[travelers-1]->destination = 1;
    traveler_list[travelers-1]->stragety = 0;
    traveler_list[travelers-1]->isClicked = false;
    traveler_list[travelers-1]->usedTime = QDateTime(QDate(1, 1, 1), QTime(0, 0, 0, 0));
    traveler_list[travelers-1]->startTime = QDateTime::currentDateTime();
    traveler_list[travelers-1]->deadlineTime = QDateTime::currentDateTime();
}

void MainWindow::chooseCity()
{
    ChooseCity* choose_city_dia = new ChooseCity(cur_travel, traveler_list,this);
    choose_city_dia->show();
    //QWidget* choosecity_widget = new QWidget;

}

//改变当前旅客
void MainWindow::changeTraveler()
{
    cur_travel = ui->current_traveler_comboBox->currentIndex();

    //恢复旅客ui的状态
    ui->start_comboBox->setCurrentIndex(traveler_list[cur_travel]->start);
    ui->destination_comboBox->setCurrentIndex(traveler_list[cur_travel]->destination);
    ui->strategy_comboBox->setCurrentIndex(traveler_list[cur_travel]->stragety);
    if(traveler_list[cur_travel]->isClicked == true)
    {
        ui->start_comboBox->setEnabled(false);
        ui->destination_comboBox->setEnabled(false);
        ui->strategy_comboBox->setEnabled(false);
        ui->test_button->setEnabled(false);
        ui->start_time->setDateTime(traveler_list[cur_travel]->startTime);
        ui->end_time->setDateTime(traveler_list[cur_travel]->deadlineTime);
        showTime();
        getPredictTime();
        showInfo();
    }
    else
    {
        ui->start_comboBox->setEnabled(true);
        ui->destination_comboBox->setEnabled(true);
        ui->strategy_comboBox->setEnabled(true);
        ui->test_button->setEnabled(true);
        ui->start_time->setDateTime(traveler_list[cur_travel]->startTime);
        ui->end_time->setDateTime(traveler_list[cur_travel]->deadlineTime);
        showTime();
        getPredictTime();
        showInfo();
    }
}

void MainWindow::getStart()
{
    traveler_list[cur_travel]->start = ui->start_comboBox->currentIndex();
}

void MainWindow::getDestination()
{
    traveler_list[cur_travel]->destination = ui->destination_comboBox->currentIndex();
}

//获取策略
void MainWindow::changeStrategy()
{
    traveler_list[cur_travel]->stragety = ui->strategy_comboBox->currentIndex();
    if(traveler_list[cur_travel]->stragety == 0)
        ui->end_time->setEnabled(false);
    else
        ui->end_time->setEnabled(true);
}


void MainWindow::updateUi()
{
    //恢复旅客ui的状态
    ui->start_comboBox->setCurrentIndex(traveler_list[cur_travel]->start);
    ui->destination_comboBox->setCurrentIndex(traveler_list[cur_travel]->destination);
    ui->strategy_comboBox->setCurrentIndex(traveler_list[cur_travel]->stragety);
    if(traveler_list[cur_travel]->isClicked == true)
    {
        ui->start_comboBox->setEnabled(false);
        ui->destination_comboBox->setEnabled(false);
        ui->strategy_comboBox->setEnabled(false);
        ui->test_button->setEnabled(false);
        ui->start_time->setDateTime(traveler_list[cur_travel]->startTime);
        ui->end_time->setDateTime(traveler_list[cur_travel]->deadlineTime);
        showTime();
        getPredictTime();
        showInfo();
    }
    else
    {
        ui->start_comboBox->setEnabled(true);
        ui->destination_comboBox->setEnabled(true);
        ui->strategy_comboBox->setEnabled(true);
        ui->test_button->setEnabled(true);
        ui->start_time->setDateTime(traveler_list[cur_travel]->startTime);
        ui->end_time->setDateTime(traveler_list[cur_travel]->deadlineTime);

        showTime();
        getPredictTime();
        showInfo();
    }
}

void MainWindow::timeStart()
{
    qDebug() << "mstimer timeout, emit SIGNAL DoStartTimer()";
    if (mstimer->isActive())
        return;
    emit DoStartTimer();
}

QDateTime MainWindow::getSpentTime()
{
    QDateTime usedDateTime = traveler_list[cur_travel]->usedTime;

    int durday = usedDateTime.date().day();
    int durhour = usedDateTime.time().hour();
    int durmin = usedDateTime.time().minute();
    int dursec = usedDateTime.time().second();
    int durmsec = usedDateTime.time().msec();

    durmsec += 360;

    dursec += durmsec / 1000;
    durmsec = durmsec % 1000;
    durmin += dursec / 60;
    dursec = dursec % 60;
    durhour += durmin / 60;
    durmin = durmin % 60;
    durday += durhour /24;
    durhour = durhour % 24;
    durday = durday % 30;

   traveler_list[cur_travel]->usedTime = QDateTime(QDate(1, 1, durday), QTime(durhour, durmin, dursec, durmsec));
   return traveler_list[cur_travel]->usedTime;
}
void MainWindow::showTime()
{
    if(traveler_list[cur_travel]->isClicked)
    {
        QDateTime spentTime = getSpentTime();
        QDateTime predictTime = traveler_list[cur_travel]->totalTime;
        if(spentTime < predictTime)
        {
            ui->usedtime_lineEdit->setText(QString::number(spentTime.date().day()-1) + QString::fromWCharArray(L"天 ")
                                        + QString::number(spentTime.time().hour()) + QString::fromWCharArray(L"小时 ")
                                       + QString::number(spentTime.time().minute()) + QString::fromWCharArray(L"分钟"));
        }
        else
        {
            ui->usedtime_lineEdit->setText(QString::number(predictTime.date().day()-1) + QString::fromWCharArray(L"天 ")
                                        + QString::number(predictTime.time().hour()) + QString::fromWCharArray(L"小时 ")
                                       + QString::number(predictTime.time().minute()) + QString::fromWCharArray(L"分钟"));
        }
    }
    else
        ui->usedtime_lineEdit->clear();
}

//获得预计时间
void MainWindow::getPredictTime()
{
    if(traveler_list[cur_travel]->isClicked)
    {
        QDateTime predictTime = traveler_list[cur_travel]->totalTime;
        ui->predictTime_lineEdit->setText(QString::number(predictTime.date().day()-1) + QString::fromWCharArray(L"天 ")
                                        + QString::number(predictTime.time().hour()) + QString::fromWCharArray(L"小时 ")
                                       + QString::number(predictTime.time().minute()) + QString::fromWCharArray(L"分钟"));
    }
    else
        ui->predictTime_lineEdit->clear();

}

void MainWindow::showInfo()
{
    QString info = traveler_list[cur_travel]->getInfo();
    QLabel* textLabel = new QLabel;
    textLabel->setText(info);
    qDebug() << info;
    ui->label_11->setText(info);

}
void MainWindow::test()
{
    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->start_time->setDateTime(curDateTime);
    traveler_list[cur_travel]->startTime = curDateTime;
    //qDebug() << Timetable::m[1];
    //点击开始之后，将选项框设置为不可选状态

    traveler_list[cur_travel]->isClicked = true;
    ui->start_comboBox->setEnabled(false);
    ui->destination_comboBox->setEnabled(false);
    ui->strategy_comboBox->setEnabled(false);
    ui->test_button->setEnabled(false);
    qDebug() << "当前旅客: " << cur_travel+1 << " 出发地: " << map[traveler_list[cur_travel]->start] <<
              " 目的地: " << map[traveler_list[cur_travel]->destination] << endl;

    /*
    for(int i = 0; i < 14; i++)
    {
        for(int j = 0; j < 14; j++)
            cout << Timetable::matrix[i][j] << " ";
        cout << endl;
    }*/
//    qDebug() << "途径城市:\n";
//    for(int i = 0; i < 14; i++)
//    {
//        if(traveler_list[cur_travel]->flag[i] == 1)
//            qDebug() << map[i] << " ";
//    }
    if(traveler_list[cur_travel]->stragety == 0)
    {
        qDebug() << "当前策略：最小风险";
        double min = traveler_list[cur_travel]->min_risk();
        QString str = QString("%1").arg(min);
        ui->minRisk_label->setText(str);
        showInfo();
    }
    else
    {
        traveler_list[cur_travel]->deadlineTime = ui->end_time->dateTime();
        qDebug() << "当前策略：限时最小风险" << " 截止时间" << traveler_list[cur_travel]->deadlineTime;
        double min = traveler_list[cur_travel]->min_risk_limit();
        QString str = QString("%1").arg(min);
        ui->minRisk_label->setText(str);
        showInfo();
    }

}
