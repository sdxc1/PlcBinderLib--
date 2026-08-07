#pragma once

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include "InovanceModbusTcp.h"

class qtBinde : public QObject
{
    Q_OBJECT
public:
    explicit qtBinde(InovanceModbusTcp* plc, QObject* parent = nullptr);

    //M区 - 功能码01读/05写
    //写固定值
    void writeM(QPushButton* btn, int addr, bool value);
    //切换-读反写
    void toggleM(QPushButton* btn, int addr);   
    //读取显示ON/OFF
    void readM(QPushButton* btn, QLineEdit* edit, int addr);     

    //整数D区 - 功能码03读/06写
    //从输入框写入D
    void writeD(QPushButton* btn, QLineEdit* edit, int addr);
    //读取D显示
    void readD(QPushButton* btn, QLineEdit* edit, int addr);

    //实数D区 - 功能码03读/10写
    //从输入框写入D实数
    void writeDF(QPushButton* btn, QLineEdit* edit, int addr);
    //读取D实数显示
    void readDF(QPushButton* btn, QLineEdit* edit, int addr);

private:
    InovanceModbusTcp* m_plc;
};