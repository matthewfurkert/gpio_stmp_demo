#include "gpio.h"
#include <QDebug>
#include <memory>

// === Backend selection ===
#ifdef USE_REAL_DEV
#include "realgpio.h"
#else
#include "mockgpio.h"
#endif

Gpio::Gpio(QObject *parent)
    : QObject(parent)
{
}

bool Gpio::isReal() const
{
#ifdef USE_REAL_DEV
    return true;
#else
    return false;
#endif
}

void Gpio::createBackend()
{
    if (m_backend) return;

#ifdef USE_REAL_DEV
    auto backend = std::make_unique<RealGpio>();
#else
    auto backend = std::make_unique<MockGpio>();
#endif

    // Sync the full current state to the new backend
    if (!backend->setChipNumber(m_chip)) {
        qWarning() << "Failed to set GPIO chip number: " << m_chip;
        return;
    }
    if (!backend->setPinNumber(m_pin)) {
        qWarning() << "Failed to set GPIO pin number: " << m_pin;
        return;
    }
    if (!backend->write(m_value)) {
        qWarning() << "Failed to write GPIO value: " << m_value;
        return;
    }
    m_backend = std::move(backend);
}


// ============== Getters ==============
int Gpio::chip() const { return m_chip; }
int Gpio::pin() const  { return m_pin; }
bool Gpio::value() const { return m_value; }

// ============== Setters ==============
void Gpio::setChip(int chip)
{
    if (chip == m_chip) return;
    if (chip < 0 || chip > 9) {
        qWarning() << "Invalid GPIO chip (must be 0-9): " << chip;
        return;
    }

    m_chip = chip;

    if (m_pin >= 0 && !m_backend) {
        createBackend();
    }
    if (m_backend) {
        if (m_backend->setChipNumber(chip)) {
            emit chipChanged();
        } else {
            qWarning() << "Failed to set GPIO chip number: " << m_chip;
        }
    }
}

void Gpio::setPin(int pin)
{
    if (pin == m_pin) return;
    if (pin < 0 || pin > 63) {
        qWarning() << "Invalid GPIO pin (must be 0-63):" << pin;
        return;
    }

    m_pin = pin;

    if (m_chip >= 0 && !m_backend) {
        createBackend();
    }
    if (m_backend) {
        if (m_backend->setPinNumber(pin)) {
            emit pinChanged();
        } else {
            qWarning() << "Failed to set GPIO pin number: " << m_pin;
        }
    }
}

void Gpio::setValue(bool value)
{
    if (m_value == value) return;

    m_value = value;

    // Create backend if both chip and pin are already configured
    if (m_chip >= 0 && m_pin >= 0 && !m_backend) {
        createBackend();
    }

    if (m_backend) {
        if (m_backend->write(value)) {
            emit valueChanged();
        } else {
            qWarning() << "Failed to write GPIO value: " << m_value;
        }
    } else {
        qWarning() << "Cannot write value — backend not set up";
    }
}