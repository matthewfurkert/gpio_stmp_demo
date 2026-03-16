#ifndef MOCKGPIO_H
#define MOCKGPIO_H

#include "igpio.h"

class MockGpio : public IGpio {
public:
    bool setChipNumber(int chip) override { m_chip = chip; return true;}
    bool setPinNumber(int pin) override { m_pin = pin; return true;}
    bool write(bool value) override { m_value = value; return true;}
    std::optional<bool> read() const override { return m_value; }


private:
    int m_chip = 0;
    int m_pin = 0;
    bool m_value = false;

};

#endif // MOCKGPIO_H
