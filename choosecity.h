#ifndef CHOOSECITY_H
#define CHOOSECITY_H

#include <QDialog>
#include <vector>
#include "mainwindow.h"
#include "traveler.h"
namespace Ui {
class ChooseCity;
}

class ChooseCity : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCity(int cur_traveler, std::vector<Traveler*>& traveler_list, QWidget *parent = 0);
    std::vector<Traveler*> temp_list;
    int traveler;
    ~ChooseCity();

private:
    Ui::ChooseCity *ui;
    void initui();
private slots:
    void getCity();
};

#endif // CHOOSECITY_H
