#pragma once
#include "igpiopin.h"
#ifdef USE_REAL_GPIO
#include <gpiod.h>
#endif

namespace Gpio {
class RealGpioPin : public IGpioPin
{
    Q_OBJECT
public:
    explicit RealGpioPin(unsigned int offset, QObject* parent = nullptr);
    ~RealGpioPin() override;
    bool value() const override;
    void setValue(bool value) override;
    bool isReal() const override { return true; }
private:
    struct gpiod_chip* m_chip = nullptr;
    struct gpiod_line_request* m_request = nullptr;
    unsigned int m_offset;
};
} // namespace Gpio
