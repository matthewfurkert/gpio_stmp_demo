#ifndef GPIO_H
#define GPIO_H
#include <QObject>
#include <qqmlintegration.h>
#include <QtQml/qqmlregistration.h>
#include <memory>
#include "igpio.h"

class Gpio : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isReal READ isReal CONSTANT FINAL)
    Q_PROPERTY(int chip READ chip WRITE setChip NOTIFY chipChanged FINAL)
    Q_PROPERTY(int pin READ pin WRITE setPin NOTIFY pinChanged FINAL)
    Q_PROPERTY(bool value READ value WRITE setValue NOTIFY valueChanged FINAL)

public:
    explicit Gpio(QObject *parent = nullptr);
    ~Gpio() override = default;

    bool isReal() const;
    int chip() const;
    int pin() const;
    bool value() const;

    Q_INVOKABLE void toggle() { setValue(!value()); }

public slots:
    void setChip(int chip);
    void setPin(int pin);
    void setValue(bool value);

signals:
    void chipChanged();
    void pinChanged();
    void valueChanged();

private:
    std::unique_ptr<IGpio> m_backend;
    int m_chip = -1;
    int m_pin = -1;
    bool m_value = false;
    void createBackend();
};

#endif // GPIOPIN_H