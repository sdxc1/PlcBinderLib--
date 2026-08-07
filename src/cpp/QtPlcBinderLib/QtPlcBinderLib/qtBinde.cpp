#include "qtBinde.h"

qtBinde::qtBinde(InovanceModbusTcp* plc, QObject* parent)
    : QObject(parent), m_plc(plc)
{
}

//=====写M=====
void qtBinde::writeM(QPushButton* btn, int addr, bool value)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        m_plc->writeM(addr, value);
        });
}

//=====取反M=====
void qtBinde::toggleM(QPushButton* btn, int addr)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        m_plc->writeM(addr, !m_plc->readM(addr));
        });
}

//=====读M=====
void qtBinde::readM(QPushButton* btn, QLineEdit* edit, int addr)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        edit->setText(m_plc->readM(addr) ? "ON" : "OFF");
        });
}

//=====写整数D=====
void qtBinde::writeD(QPushButton* btn, QLineEdit* edit, int addr)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        bool ok;
        int val = edit->text().toInt(&ok);
        if (ok)
            m_plc->writeD(addr, static_cast<uint16_t>(val));
        });
}

//=====读整数D=====
void qtBinde::readD(QPushButton* btn, QLineEdit* edit, int addr)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        edit->setText(QString::number(m_plc->readD(addr)));
        });
}

//=====写实数D=====
void qtBinde::writeDF(QPushButton* btn, QLineEdit* edit, int addr)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        bool ok;
        float val = edit->text().toFloat(&ok);
        if (ok)
            m_plc->writeDFloat(addr, val);
        });
}

//=====读实数D=====
void qtBinde::readDF(QPushButton* btn, QLineEdit* edit, int addr)
{
    connect(btn, &QPushButton::clicked, this, [=]() {
        edit->setText(QString::number(m_plc->readDFloat(addr), 'f', 2));
        });
}