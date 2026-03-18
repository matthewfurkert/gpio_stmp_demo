#include "lightshow.h"
#include "gpio.h"
#include <QDebug>

LightShow::LightShow(QObject* parent)
    : QObject(parent),
    m_pattern({
        // Fun rhythmic light-show pattern (loops forever)
        {true,  false, false, 180},
        {true,  true,  false, 120},
        {false, true,  false, 180},
        {false, true,  true,  120},
        {false, false, true,  250},
        {true,  false, true,  80},
        {true,  true,  true,  220},
        {false, false, false, 90},

        {true,  false, false, 90},
        {false, true,  false, 90},
        {false, false, true,  90},
        {true,  true,  true,  150},
        {false, false, false, 80}
    })
{
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &LightShow::nextStep);
    m_running = false;
}

LightShow::~LightShow()
{
    stop();
}

bool LightShow::isRunning() const
{
    return m_running;
}

void LightShow::setRedGpio(Gpio* gpio)
{
    if (m_red == gpio) return;
    m_red = gpio;
    emit redGpioChanged();
}

void LightShow::setOrangeGpio(Gpio* gpio)
{
    if (m_orange == gpio) return;
    m_orange = gpio;
    emit orangeGpioChanged();
}

void LightShow::setGreenGpio(Gpio* gpio)
{
    if (m_green == gpio) return;
    m_green = gpio;
    emit greenGpioChanged();
}

void LightShow::start()
{
    if (!m_red || !m_orange || !m_green) {
        qWarning() << "LightShow::start() failed — all three GPIOs (red/orange/green) must be assigned!";
        return;
    }
    m_currentStep = 0;
    m_running = true;
    emit runningChanged();
    nextStep();   // start immediately
}

void LightShow::stop()
{
    m_timer.stop();
    m_running = false;
    emit runningChanged();
    if (m_red)    m_red->setValue(false);
    if (m_orange) m_orange->setValue(false);
    if (m_green)  m_green->setValue(false);
}

void LightShow::nextStep()
{
    if (!m_red || !m_orange || !m_green || m_pattern.empty()) return;

    const Step& step = m_pattern[m_currentStep];

    m_red->setValue(step.red);
    m_orange->setValue(step.orange);
    m_green->setValue(step.green);

    m_timer.start(step.durationMs);
    m_currentStep = (m_currentStep + 1) % m_pattern.size();
}