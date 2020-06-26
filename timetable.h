#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <vector>
#include "traveler.h"
#include "route.h"
#include <map>
#include "QFile"
#include "QTextStream"
#include "QDebug"

using namespace std;
class Timetable
{
public:
    Timetable();
    static std::multimap<pair<int,int>, Route> schedule;
    static vector<vector<int> > matrix;
    static QMap<int, QString> m;
    static std::vector<double> risk;
    int CityToNum(QString);
};

#endif // TIMETABLE_H
