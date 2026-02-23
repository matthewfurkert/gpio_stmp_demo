// MockGpioPin.h
#pragma once
#include "igpiopin.h"

namespace Gpio {

class MockGpioPin : public IGpioPin
{
    Q_OBJECT
public:
    explicit MockGpioPin(unsigned int offset, QObject* parent = nullptr);

    bool value() const override;
    void setValue(bool value) override;
    bool isReal() const override { return false; }

private:
    bool m_value = false;
};
}
