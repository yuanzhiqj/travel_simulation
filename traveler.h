#pragma once
#ifndef TRAVELER_H
#define TRAVELER_H

#include <vector>
#include "QString"
#include "QDateTime"
#include "route.h"
#include <vector>
#include "timetable.h"
class Traveler
{
public:
    Traveler();
    int start;
    int destination;
    int stragety;
    bool isClicked;
    std::vector<int> flag;
    QDateTime startTime, deadlineTime, usedTime;
    QDateTime totalTime;
    QString getInfo();  //获得最短路径信息

    double min_risk();
    double min_risk_limit();
 private:
    void DFS(int,int);          //深度优先搜索最短路径
    QDateTime getTotalTime();        //获得总共需要的时间
    std::vector<Route> path;
    std::vector<std::vector<Route> > total; //遍历到的所有路径
    std::vector<Route> min_path;            //存储最少风险的路径
};

#endif // TRAVELER_H
