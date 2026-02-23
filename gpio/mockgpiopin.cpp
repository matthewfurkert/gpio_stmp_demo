// MockGpioPin.cpp
#include "mockgpiopin.h"
#include <QDebug>

namespace Gpio {

MockGpioPin::MockGpioPin(unsigned int offset, QObject* parent)
    : IGpioPin(offset, parent)
{
    qDebug() << "Mock GPIO" << offset << "created";
}

bool MockGpioPin::value() const { return m_value; }

void MockGpioPin::setValue(bool value)
{
    if (m_value != value) {
        m_value = value;
        qDebug() << "Mock GPIO" << offset() << "set to" << (value ? "HIGH" : "LOW");
        emit valueChanged(value);
    }
}

} // namespace Gpio
