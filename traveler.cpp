#pragma once
#include "traveler.h"


Traveler::Traveler()
{
    flag.resize(15);
    arriveTime.resize(15);
    leaveTime.resize(15);
    for(int i = 0; i < arriveTime.size(); i++)
    {
        arriveTime[i] = QDateTime::currentDateTime();
        leaveTime[i] = QDateTime::currentDateTime();
    }
}

double Traveler::min_risk()
{
    DFS(start,destination);
    QTime st;
    st.setHMS(startTime.time().hour(),startTime.time().minute(),0);
    //qDebug() << st;
    double min = 99999.9;
    int min_i = total.size();
    for(int i = 0; i < total.size(); i++)
    {
        double temp = 0;
        for(int j = 0; j < total[i].size(); j++)
        {
            //qDebug() << total[i][j].begin.toString("yyyy/MM/dd hh:mm");
            //第一条路径
            int duration = 0;
            if(j == 0)
            {
                if(total[i][j].begin < st)
                {
                    duration = 86400 - total[i][j].begin.secsTo(st);
                }
                else
                    duration = st.secsTo(total[i][j].begin);

            }
            else
            {
                if(total[i][j].begin < total[i][j-1].end)
                {
                    duration = 86400 - total[i][j].begin.secsTo(total[i][j-1].end);
                }
                else
                    duration = total[i][j-1].end.secsTo(total[i][j].begin);
            }
            //qDebug() << duration;
            double s = (double)duration;
            s = s / 3600;
            temp += s * Timetable::risk[total[i][j].from];
        }
        qDebug() << temp;
        if(temp < min)
        {
            min_path.clear();
            min = temp;
            min_i = i;
            for(int k = 0; k < total[i].size(); k++)
                min_path.push_back(total[i][k]);
        }

    }
    //qDebug() << min << " " << min_i;
    compute_arrivelTime();
    totalTime = getTotalTime();

    std::vector<Route> path = min_path;
    for(int i = 0; i < path.size(); i++)
    {
        qDebug() << "path";
        qDebug() << path[i].num;
    }
    return min;

}

double Traveler::min_risk_limit()
{
    DFS(start,destination);
    QTime st;
    st.setHMS(startTime.time().hour(),startTime.time().minute(),0);
    qDebug() << st;

    long long countdown = startTime.secsTo(deadlineTime);
    long long count = 0;
    qDebug() << "倒计时" << countdown;
    double min = 99999.9;
    int min_i = total.size();
    for(int i = 0; i < total.size(); i++)
    {
        count = 0;
        double temp = 0;
        for(int j = 0; j < total[i].size() && count < countdown; j++)
        {
            //qDebug() << total[i][j].begin.toString("yyyy/MM/dd hh:mm");
            //第一条路径
            int duration = 0;
            if(j == 0)
            {
                if(total[i][j].begin < st)
                {
                    duration = 86400 - total[i][j].begin.secsTo(st);
                }
                else
                    duration = st.secsTo(total[i][j].begin);

            }
            else
            {
                if(total[i][j].begin < total[i][j-1].end)
                {
                    duration = 86400 - total[i][j].begin.secsTo(total[i][j-1].end);
                }
                else
                    duration = total[i][j-1].end.secsTo(total[i][j].begin);
            }
            //统计
            count += duration;
            count += total[i][j].spent * 3600;

            qDebug() << duration;
            double s = (double)duration;
            s = s / 3600;
            temp += s * Timetable::risk[total[i][j].from];
        }
        qDebug() << temp;
        if(count > countdown)
            continue;
        if(temp < min)
        {
            min_path.clear();
            min = temp;
            min_i = i;
            for(int k = 0; k < total[i].size(); k++)
                min_path.push_back(total[i][k]);
        }

    }
    if(min_i == total.size())
        qDebug() << "限时时间内无最短路径";
    qDebug() << min << " " << min_i;
    totalTime = getTotalTime();
    return min;
}

void Traveler::DFS(int cur, int des)
{
    flag[cur] = 1;
    if (cur == des)
    {
        qDebug() << "列表：";
        for (int i = 0; i < path.size(); i++)
        {
            qDebug() << Timetable::m[path[i].from] << "->" << Timetable::m[path[i].to] << path[i].num;
        }
        total.push_back(path);
        qDebug() << endl;
    }
    for (int i = 0; i < 14; i++)
    {
        if (Timetable::matrix[cur][i] != 0 && flag[i] != 1)
        {
            for (int j = 0; j < Timetable::matrix[cur][i]; j++)
            {
                multimap<pair<int,int>, Route>::iterator it;
                pair<int, int> p(cur, i);
                it = Timetable::schedule.find(p);
                int n = Timetable::schedule.count(p);
                for (int k = 0; k < j; k++)
                {
                    it++;
                }
                path.push_back(it->second);
                //qDebug() << Timetable::m[it->second.from] <<Timetable::m[it->second.to]<< it->second.begin;
                flag[i] = 1;
                DFS(i, des);
                flag[i] = 0;
                path.pop_back();
            }

        }
    }
}

QDateTime Traveler::getTotalTime()
{
    long long duration = 0;
    long board_time = 0;
    QTime st;
    st.setHMS(startTime.time().hour(),startTime.time().minute(),0);
    for(int i = 0; i < min_path.size(); i++)
    {
        qDebug() << min_path[i].from << "->" << min_path[i].to << " " << min_path[i].num;
        //第一条路径
        if(i == 0)
        {
            if(min_path[i].begin < st)
            {
                duration += 86400 - min_path[i].begin.secsTo(st);
            }
            else
                duration += st.secsTo(min_path[i].begin);

        }
        else
        {
            if(min_path[i].begin < min_path[i-1].end)
            {
                duration += 86400 - min_path[i].begin.secsTo(min_path[i-1].end);
            }
            else
                duration += min_path[i-1].end.secsTo(min_path[i].begin);
        }
        board_time += min_path[i].spent * 60;
        qDebug() << "duration:" << duration << "board:" << board_time;
    }
    int size = min_path.size();
    long long s = startTime.secsTo(leaveTime[min_path[size-1].to]);
    int day = 0;
    int minute = duration / 60 + board_time;
    qDebug() << "分钟" << minute;
    int hour = minute / 60;
    minute = minute % 60;
    while(hour >= 24)
    {
        day++;
        hour -= 24;
    }
    return QDateTime(QDate(1, 1, day+1), QTime(hour, minute, 0));

}

QString Traveler::getInfo()
{
    QString info;
    for(int i = 0; i < min_path.size(); i++)
    {
        info += Timetable::m[min_path[i].from] + "->" + Timetable::m[min_path[i].to] + min_path[i].num + "\n";
    }
    return info;
}

void Traveler::compute_arrivelTime()
{
    arriveTime[start] = startTime;
    QTime st;
    st.setHMS(startTime.time().hour(),startTime.time().minute(),0);
    QTime lastTime = st;
    QDateTime last = startTime;
    int duration = 0;
    for(int i = 0; i < min_path.size(); i++)
    {
        if(min_path[i].begin > lastTime)
            duration = lastTime.secsTo(min_path[i].begin);
        else
            duration = 86400 - min_path[i].begin.secsTo(lastTime);
        leaveTime[min_path[i].from] = last.addSecs(duration);
        //qDebug() << "离开" << Timetable::m[min_path[i].from] << "的时间为" << leaveTime[min_path[i].from];
        arriveTime[min_path[i].to] = leaveTime[min_path[i].from ].addSecs(min_path[i].spent * 3600);
        //qDebug() << "到达" << Timetable::m[min_path[i].to] << "的时间为" << arriveTime[min_path[i].to];
        lastTime = min_path[i].end;
        last = arriveTime[min_path[i].to];
    }
}
