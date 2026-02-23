#pragma once
#include <QObject>

namespace Gpio {

class IGpioPin : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int offset READ offset CONSTANT)
    Q_PROPERTY(bool value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool isReal READ isReal CONSTANT)

public:
    explicit IGpioPin(unsigned int offset, QObject* parent = nullptr)
        : QObject(parent), m_offset(offset) {}

    virtual ~IGpioPin() = default;

    int offset() const { return m_offset; }
    virtual bool value() const = 0;
    virtual void setValue(bool value) = 0;
    virtual bool isReal() const = 0;

    Q_INVOKABLE void toggle() { setValue(!value()); }

signals:
    void valueChanged(bool newValue);

private:
    unsigned int m_offset;
};

} // namespace Gpio
