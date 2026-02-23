// GpioFactory.cpp
#include "gpiofactory.h"
#include "mockgpiopin.h"
#ifdef USE_REAL_GPIO
#include "realgpiopin.h"
#endif

namespace Gpio {

IGpioPin* GpioFactory::createPin(unsigned int offset)
{
#ifdef USE_REAL_GPIO
    return new RealGpioPin(offset);
#else
    return new MockGpioPin(offset);
#endif
}

} // namespace Gpio
