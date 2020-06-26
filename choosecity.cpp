#include "choosecity.h"
#include "ui_choosecity.h"
#include "traveler.h"
ChooseCity::ChooseCity(int cur_traveler, std::vector<Traveler*>& traveler_list, QWidget *parent) :
    temp_list(traveler_list),
    QDialog(parent),
    ui(new Ui::ChooseCity)
{
    ui->setupUi(this);
    traveler = cur_traveler;

    initui();

    connect(ui->ok_pushButton,&QPushButton::clicked,this,&ChooseCity::getCity);
    connect(ui->cancel_pushButton,&QPushButton::clicked,[=](){
        this->close();
    });
}

ChooseCity::~ChooseCity()
{
    delete ui;
}

void ChooseCity::initui()
{
    //屏蔽选择的出发地和目的地
    if(temp_list[traveler]->start == 0 || temp_list[traveler]->destination == 0)
        ui->checkBox_1->setEnabled(false);
    if(temp_list[traveler]->start == 1 || temp_list[traveler]->destination == 1)
        ui->checkBox_2->setEnabled(false);
    if(temp_list[traveler]->start == 2 || temp_list[traveler]->destination == 2)
        ui->checkBox_3->setEnabled(false);
    if(temp_list[traveler]->start == 3 || temp_list[traveler]->destination == 3)
        ui->checkBox_4->setEnabled(false);
    if(temp_list[traveler]->start == 4 || temp_list[traveler]->destination == 4)
        ui->checkBox_5->setEnabled(false);
    if(temp_list[traveler]->start == 5 || temp_list[traveler]->destination == 5)
        ui->checkBox_6->setEnabled(false);
    if(temp_list[traveler]->start == 6 || temp_list[traveler]->destination == 6)
        ui->checkBox_7->setEnabled(false);
    if(temp_list[traveler]->start == 7 || temp_list[traveler]->destination == 7)
        ui->checkBox_8->setEnabled(false);
    if(temp_list[traveler]->start == 8 || temp_list[traveler]->destination == 8)
        ui->checkBox_9->setEnabled(false);
    if(temp_list[traveler]->start == 9 || temp_list[traveler]->destination == 9)
        ui->checkBox_10->setEnabled(false);
    if(temp_list[traveler]->start == 10 || temp_list[traveler]->destination == 10)
        ui->checkBox_11->setEnabled(false);
    if(temp_list[traveler]->start == 11 || temp_list[traveler]->destination == 11)
        ui->checkBox_12->setEnabled(false);
    if(temp_list[traveler]->start == 12 || temp_list[traveler]->destination == 12)
        ui->checkBox_13->setEnabled(false);
    if(temp_list[traveler]->start == 13 || temp_list[traveler]->destination == 13)
        ui->checkBox_14->setEnabled(false);

    //之前的勾选框
    /*if(temp_list[traveler]->flag[0] == 1)
        ui->checkBox_1->setChecked(2);
   /* if(temp_list[traveler]->flag[1] == 1)
        ui->checkBox_2->isChecked() = true;
    if(temp_list[traveler]->flag[2] == 1)
        ui->checkBox_3->isChecked() = true;
    if(temp_list[traveler]->flag[3] == 1)
        ui->checkBox_4->isChecked() = true;
    if(temp_list[traveler]->flag[4] == 1)
        ui->checkBox_5->isChecked() = true;
    if(temp_list[traveler]->flag[5] == 1)
        ui->checkBox_6->isChecked() = true;
    if(temp_list[traveler]->flag[6] == 1)
        ui->checkBox_7->isChecked() = true;
    if(temp_list[traveler]->flag[7] == 1)
        ui->checkBox_8->isChecked() = true;
    if(temp_list[traveler]->flag[8] == 1)
        ui->checkBox_9->isChecked() = true;
    if(temp_list[traveler]->flag[9] == 1)
        ui->checkBox_10->isChecked() = true;
    if(temp_list[traveler]->flag[10] == 1)
        ui->checkBox_11->isChecked() = true;
    if(temp_list[traveler]->flag[11] == 1)
        ui->checkBox_12->isChecked() = true;
    if(temp_list[traveler]->flag[12] == 1)
        ui->checkBox_13->isChecked() = true;
    if(temp_list[traveler]->flag[13] == 1)
        ui->checkBox_14->isChecked() = true;
    if(temp_list[traveler]->flag[0] == 1)
        ui->checkBox_1->isChecked() = true;*/
}
void ChooseCity::getCity()
{
    if(ui->checkBox_1->isChecked() == true)
        temp_list[traveler]->flag[0] = 1;
    if(ui->checkBox_2->isChecked() == true)
        temp_list[traveler]->flag[1] = 1;
    if(ui->checkBox_3->isChecked() == true)
        temp_list[traveler]->flag[2] = 1;
    if(ui->checkBox_4->isChecked() == true)
        temp_list[traveler]->flag[3] = 1;
    if(ui->checkBox_5->isChecked() == true)
        temp_list[traveler]->flag[4] = 1;
    if(ui->checkBox_6->isChecked() == true)
        temp_list[traveler]->flag[5] = 1;
    if(ui->checkBox_7->isChecked() == true)
        temp_list[traveler]->flag[6] = 1;
    if(ui->checkBox_8->isChecked() == true)
        temp_list[traveler]->flag[7] = 1;
    if(ui->checkBox_9->isChecked() == true)
        temp_list[traveler]->flag[8] = 1;
    if(ui->checkBox_10->isChecked() == true)
        temp_list[traveler]->flag[9] = 1;
    if(ui->checkBox_11->isChecked() == true)
        temp_list[traveler]->flag[10] = 1;
    if(ui->checkBox_12->isChecked() == true)
        temp_list[traveler]->flag[11] = 1;
    if(ui->checkBox_13->isChecked() == true)
        temp_list[traveler]->flag[12] = 1;
    if(ui->checkBox_14->isChecked() == true)
        temp_list[traveler]->flag[13] = 1;
    this->close();
}
