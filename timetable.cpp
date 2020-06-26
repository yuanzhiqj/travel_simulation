#include "timetable.h"

std::multimap<pair<int,int>, Route> Timetable::schedule;
vector<vector<int> > Timetable::matrix;
QMap<int, QString> Timetable::m;
std::vector<double> Timetable::risk;
Timetable::Timetable()
{
    //建立一个数字跟城市对应的哈希表
    m.insert(0,"北京");
    m.insert(1,"上海");
    m.insert(2,"深圳");
    m.insert(3,"杭州");
    m.insert(4,"西安");
    m.insert(5,"哈尔滨");
    m.insert(6,"昆明");
    m.insert(7,"成都");
    m.insert(8,"桂林");
    m.insert(9,"拉萨");
    m.insert(10,"乌鲁木齐");
    m.insert(11,"台北");
    m.insert(12,"香港");
    m.insert(13,"澳门");
    matrix.resize(14);

    risk.resize(14);
    risk = {0.9, 0.9, 0.9, 0.5, 0.2, 0.5, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.9, 0.5};
    for(int i = 0; i < 14; i++)
    {
        qDebug() << risk[i];
        matrix[i].resize(14);
    }
    for(int i = 0; i < 14; i++)
    {
        for(int j = 0; j < 14; j++)
            matrix[i][j] = 0;
    }
    QFile file(":/sche.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open the file by reading";
        return;
    }

    QTextStream in(&file);
    QString from, to, number, begin, end, spent; //分别为出发地、目的地、班次、开始时间、结束时间、费用、交通方式

    //将时刻表信息读入database数据结构
    while(!in.atEnd())
    {
        in >> from >> to >> number >> begin >> end >> spent >>endl;
        if(from != "")
        {
            //qDebug() << from << to << number << begin;
            Route line(CityToNum(from), CityToNum(to), number, QTime::fromString(begin),
                           QTime::fromString(end), spent.toDouble());
            qDebug() << m[line.from] << m[line.to] << line.num << line.begin << line.end << line.spent;
            matrix[CityToNum(from)][CityToNum(to)]++;
            pair<int,int> p(CityToNum(from),CityToNum(to));
            schedule.insert(std::make_pair(p, line));
            //qDebug() << schedule[CityToNum(from)].cost;
        }
    }
    qDebug() << "input success...";
}

//根据字符串转换为相应的序号
int Timetable::CityToNum(QString city)
{
    int num = -1;

    if(city == "Beijing")
        num = 0;
    else if(city == "Shanghai")
        num = 1;
    else if(city == "Shenzheng")
        num = 2;
    else if(city == "Hangzhou")
        num = 3;
    else if(city == "Xian")
        num = 4;
    else if(city == "Haerbin")
        num = 5;
    else if(city == "Kunming")
        num = 6;
    else if(city == "Chengdu")
        num = 7;
    else if(city == "Guilin")
        num = 8;
    else if(city == "Lasa")
        num = 9;
    else if(city == "Wulumuqi")
        num = 10;
    else if(city == "Taibei")
        num = 11;
    else if(city == "HK")
        num = 12;
    else if(city == "MK")
        num = 13;

    return num;
}
