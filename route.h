#ifndef ROUTE_H
#define ROUTE_H

#include "QTime"
class Route
{
public:
    Route();
    Route(int, int, QString, QTime, QTime,double);

    //成员变量
    int from;           //出发地
    int to;             //目的地
    QString num;        //编号
    QTime begin, end;   //起止时间
    double spent;       //旅途耗时

};

#endif // ROUTE_H
