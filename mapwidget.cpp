#include "mapwidget.h"

#include <QApplication>
#include <QStateMachine>
#include <QPushButton>
#include <QSignalTransition>
#include <QPropertyAnimation>
#include <QPainter>
#include <QState>
#include <QLabel>

//添加新的timer，使得绘图准确
MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent), state(-1)
{
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/res/map.jpg")));

    this->setPalette(palette);

    paintmstimer = new QTimer;
    paintmstimer->start(1000/60);
    QObject::connect(paintmstimer, SIGNAL(timeout()), this, SLOT(update()));
}

//绘图实践，绘制旅行过程
void MapWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    MainWindow * Ptr = MainWindow::getMainWindow();
    int currentTraveler = Ptr->getCurTraveler();
    QPixmap p;
    p=QPixmap(":/res/train.svg");
    if(currentTraveler != -1)
    {
        painter.drawPixmap((setPointPos()), setPointGraph());
    }
}


//根据当前状态、交通方式决定图标
QPixmap MapWidget::setPointGraph()
{
    QPixmap pointGraph;
    switch(state)
    {
    case -2://arrived destination
        pointGraph = QPixmap(":/res/arrive.ico");
        break;
    case -1://pause waiting
        pointGraph = QPixmap(":/res/pause.ico");
        break;
    case 0:
        pointGraph = QPixmap(":/res/Bus.svg");
        break;
    case 1:
        pointGraph = QPixmap(":/res/train.svg");
        break;
    case 2:
        pointGraph = QPixmap(":/res/fly.ico");
        break;
    }

    return pointGraph;
}

//设置当前图标所处位置
QPointF MapWidget::setPointPos()
{
    MainWindow * fatherPtr = MainWindow::getMainWindow();
    static QPointF pointPos;
    std::vector<Route> path = fatherPtr->traveler_list[fatherPtr->cur_travel]->min_path;
    QDateTime spenttime = fatherPtr->traveler_list[fatherPtr->cur_travel]->usedTime;
    QDateTime starttime = fatherPtr->traveler_list[fatherPtr->cur_travel]->startTime;

    //已用时间不小于总时间，当前位置为目的地
    if(spenttime >= fatherPtr->traveler_list[fatherPtr->cur_travel]->totalTime)
    {
         qDebug() << "到达";
         /*if(spenttime == fatherPtr->traveler_list[fatherPtr->cur_travel]->totalTime)
         {
              qDebug() << "Arriving at destination" << path[path.size()-1].to;
         }*/
         pointPos = getCityCor(path[path.size()-1].to);
         state = -2;
    }
    else
        for (std::vector<Route>::size_type index = 0;
            index != path.size(); index++)
        {
            QDateTime departuredatetime = fatherPtr->traveler_list[fatherPtr->cur_travel]->leaveTime[path[index].from];
            QDateTime cityarrivaltime = fatherPtr->traveler_list[fatherPtr->cur_travel]->arriveTime[path[index].to];
            QDateTime start2Begin = getSplitTime(starttime, departuredatetime);
            QDateTime start2End = getSplitTime(starttime, cityarrivaltime);
            //已用时间不超过一段路径发车时间，状态为等待
            if (spenttime <= start2Begin)
            {
                pointPos = getCityCor(path[index].from);
                state = -1;
                break;
            }
            //已用时间不超过一段路径的到站时间，状态为运动中
            else if (spenttime <= start2End)
            {
                pointPos = getCityCor(path[index].from);
                pointPos += getMoveDistance(spenttime, start2Begin, start2End, path[index].from, path[index].to);
                QString vehicle = path[index].num;
                if(vehicle[0] == 'F')
                    state = 2;
                else if(vehicle[0] == 'G')
                    state = 1;
                else if(vehicle[0] == 'C')
                    state = 0;
                if (spenttime == start2End)
                {
                    qDebug() << "Arriving at " << path[index].to;
                }
                break;
            }
        }
    return pointPos;
}


//获得两时间点之间的时间差，判断当前所处的状态
QDateTime MapWidget::getSplitTime(QDateTime former, QDateTime later)
{
    int durationSec = (later.time().second() - former.time().second());
    int durationMin = (later.time().minute() - former.time().minute() - (int)((durationSec >= 0)?0:1));
    int durationHour = (later.time().hour() - former.time().hour() - (int)((durationMin >= 0)?0:1));
    int durationDay = (later.date().day() - former.date().day() - (int)((durationHour >= 0)?0:1) + former.date().daysInMonth())
            % former.date().daysInMonth();
    durationSec = (durationSec + 60) % 60;
    durationMin = (durationMin + 60) % 60;
    durationHour = (durationHour + 24) % 24;

    return QDateTime(QDate(1, 1, durationDay+1), QTime(durationHour, durationMin, durationSec, 999));
}

//获得图形中各个城市的位置
QPointF MapWidget::getCityCor(int city)
{
    int x, y;
    switch (city)
    {
    case 0:
        x = 512;
        y = 210;
        break;
    case 1:
        x = 580;
        y = 340;
        break;
    case 2:
        x = 458;
        y = 468;
        break;
    case 3:
        x = 560;
        y = 360;
        break;
    case 4:
        x = 435;
        y = 280;
        break;
    case 5:
        x = 600;
        y = 100;
        break;
    case 6:
        x = 340;
        y = 430;
        break;
    case 7:
        x = 357;
        y = 340;
        break;
    case 8:
        x = 439;
        y = 439;
        break;
    case 9:
        x = 165;
        y = 300;
        break;
    case 10:
        x = 130;
        y = 170;
        break;
    case 11:
        x = 590;
        y = 430;
        break;
    case 12:
        x = 455;
        y = 490;
        break;
    case 13:
        x = 435;
        y = 490;
    }

    return QPointF(x, y);
}

///获得两个时间段的时间差，用于计算坐标增量
double MapWidget::getTimeDifference(QDateTime former, QDateTime later)
{

    int durationSec = (later.time().second() - former.time().second());
    int durationMin = (later.time().minute() - former.time().minute() - (int)((durationSec >= 0)?0:1));
    int durationHour = (later.time().hour() - former.time().hour() - (int)((durationMin >= 0)?0:1));
    int durationDay = (later.date().day() - former.date().day() - (int)((durationHour >= 0)?0:1) + former.date().daysInMonth())
            % former.date().daysInMonth();
    durationSec = (durationSec + 60) % 60;
    durationMin = (durationMin + 60) % 60;
    durationHour = (durationHour + 24) % 24;

    return (double)(durationDay * 86400 + durationHour * 3600 + durationMin * 60 + durationSec);
}

//计算坐标增量
QPointF MapWidget::getMoveDistance(QDateTime spentTime, QDateTime start2Begin, QDateTime start2End,
                                   int from, int to)
{
    double increaseRatio = getTimeDifference(start2Begin, spentTime)/getTimeDifference(start2Begin, start2End);
    double xIncrease = (getCityCor(to) - getCityCor(from)).x() * increaseRatio;
    double yIncrease = (getCityCor(to) - getCityCor(from)).y() * increaseRatio;

    return QPointF(xIncrease, yIncrease);
}
