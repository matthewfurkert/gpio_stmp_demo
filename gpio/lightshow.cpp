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

void LightShow::setLight1(Gpio* gpio)
{
    if (m_light1 == gpio) return;
    m_light1 = gpio;
    emit light1Changed();
}

void LightShow::setLight2(Gpio* gpio)
{
    if (m_light2 == gpio) return;
    m_light2 = gpio;
    emit light2Changed();
}

void LightShow::setLight3(Gpio* gpio)
{
    if (m_light3 == gpio) return;
    m_light3 = gpio;
    emit light3Changed();
}

void LightShow::start()
{
    if (!m_light1 || !m_light2 || !m_light3) {
        qWarning() << "LightShow::start() failed — all three GPIOs (light1/light2/light3) must be assigned!";
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
    if (m_light1) m_light1->setValue(false);
    if (m_light2) m_light2->setValue(false);
    if (m_light3) m_light3->setValue(false);
}

void LightShow::nextStep()
{
    if (!m_light1 || !m_light2 || !m_light3 || m_pattern.empty()) return;

    const Step& step = m_pattern[m_currentStep];

    m_light1->setValue(step.firstLight);
    m_light2->setValue(step.secondLight);
    m_light3->setValue(step.thirdLight);

    m_timer.start(step.durationMs);
    m_currentStep = (m_currentStep + 1) % m_pattern.size();
}