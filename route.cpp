#include "route.h"

Route::Route()
{
    this->from = -1;
}

Route::Route(int from, int to, QString num, QTime begin, QTime end, double spent)
{
    this->from = from;
    this->to = to;
    this->num = num;
    this->begin = begin;
    this->end = end;
    this->spent = spent;
}

