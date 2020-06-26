#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "choosecity.h"
#include <vector>
#include "traveler.h"
#include "route.h"
#include "timetable.h"
#include "QTimer"
#include "QThread"
#include <qmap.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int travelers = 0;
    int cur_travel = -1;
    std::vector<Traveler*> traveler_list;
    QMap<int, QString> map;
    QTimer *mstimer;
    QThread *timethread;

    QDateTime getSpentTime();
    void showInfo();
private:
    Ui::MainWindow *ui;
    void init();    //初始化函数
private slots:
    void addTraveler();         //添加乘客
    void chooseCity();          //选择城市
    void changeTraveler();      //更改乘客
    void getStart();            //获得起始地点
    void getDestination();      //获得终点
    void getStrategy();         //获得当前策略
    void test();                //
    void updateUi();            //更新UI界面
    void showTime();            //显示当前已用时间
    void timeStart();           //计时器开始工作
    //void getStartTime();        //获得开始时间
    void getPredictTime();      //获得预计时间

signals:
    void DoStartTimer();//开始计时的信号
};

#endif // MAINWINDOW_H
