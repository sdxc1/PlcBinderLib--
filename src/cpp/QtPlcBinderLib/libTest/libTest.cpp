#include "libTest.h"

libTest::libTest(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_plc.init("127.0.0.1", 502,1);
    auto binder = new qtBinde(&m_plc, this);
    binder->toggleM(ui.m100, 100);
    
    binder->toggleM(ui.m2, 2);


    binder->toggleM(ui.m1, 1);
}

libTest::~libTest()
{}

