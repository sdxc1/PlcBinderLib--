#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_libTest.h"

#include "InovanceModbusTcp.h"
#include "qtBinde.h"

class libTest : public QMainWindow
{
    Q_OBJECT

public:
    libTest(QWidget *parent = nullptr);
    ~libTest();

    InovanceModbusTcp m_plc;

private:
    Ui::libTestClass ui;
};

