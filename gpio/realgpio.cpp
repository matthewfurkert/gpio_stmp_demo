#include "realgpio.h"
#include <QDebug>
#include <QString>

RealGpio::RealGpio()
{
}

RealGpio::~RealGpio()
{
#ifdef USE_REAL_DEV
    if (m_request) {
        gpiod_line_request_release(m_request);
        m_request = nullptr;
    }
    if (m_gpiodChip) {
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
    }
#endif
}

bool RealGpio::setChipNumber(int chip)
{
    if (m_chipNumber == chip) return true;
    m_chipNumber = chip;

#ifdef USE_REAL_DEV
    if (m_pinNumber >= 0 && !m_request) {
        openAndRequestLine();
        if (!m_request) return false;
    }
#endif
    return true;
}

bool RealGpio::setPinNumber(int pin)
{
    if (pin < 0) return false;
    unsigned int newPin = static_cast<unsigned int>(pin);
    if (newPin == m_pinNumber) return true;

    m_pinNumber = newPin;

#ifdef USE_REAL_DEV
    if (m_chipNumber >= 0 && !m_request) {
        openAndRequestLine();
        if (!m_request) return false;
    }
#endif
    return true;
}

bool RealGpio::write(bool value)
{
    m_currentValue = value;
#ifdef USE_REAL_DEV
    if (!m_request) {
        if (m_chipNumber >= 0 && m_pinNumber >= 0) {
            openAndRequestLine();
        }
        if (!m_request) {
            qWarning() << "RealGpio: Cannot write - chip and pin not ready yet";
            return false;
        }
    }
    enum gpiod_line_value val = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    int ret = gpiod_line_request_set_value(m_request, m_pinNumber, val);
    if (ret < 0) {
        qWarning() << "Failed to set GPIO" << m_chipNumber << "/" << m_pinNumber;
        return false;
    }
#endif
    return true;
}

std::optional<bool> RealGpio::read() const
{
#ifdef USE_REAL_DEV
    if (m_request) {
        int val = gpiod_line_request_get_value(m_request, m_pinNumber);
        if (val < 0) {
            qWarning() << "Failed to read GPIO" << m_chipNumber << "/" << m_pinNumber;
            return {};
        }
        return val == GPIOD_LINE_VALUE_ACTIVE;
    } else return {};
}
#endif

void RealGpio::openAndRequestLine()
{
#ifdef USE_REAL_DEV
    if (m_request || m_chipNumber < 0)
        return;

    QString chipPath = QString("/dev/gpiochip%1").arg(m_chipNumber);
    m_gpiodChip = gpiod_chip_open(chipPath.toStdString().c_str());

    if (!m_gpiodChip) {
        qWarning() << "Failed to open" << chipPath;
        return;
    }

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (req_cfg)
        gpiod_request_config_set_consumer(req_cfg, "qt_gpio_hal");

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        qWarning() << "Failed to create line settings";
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
        return;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, m_currentValue ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        qWarning() << "Failed to create line config";
        gpiod_line_settings_free(settings);
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
        return;
    }

    if (gpiod_line_config_add_line_settings(line_cfg, &m_pinNumber, 1, settings) < 0) {
        qWarning() << "Failed to add line settings for pin" << m_pinNumber;
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
        return;
    }

    m_request = gpiod_chip_request_lines(m_gpiodChip, req_cfg, line_cfg);

    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    if (req_cfg)
        gpiod_request_config_free(req_cfg);

    if (m_request) {
        qDebug() << "Real GPIO requested successfully:" << m_chipNumber << "/" << m_pinNumber;
    } else {
        qWarning() << "Failed to request GPIO line" << m_chipNumber << "/" << m_pinNumber;
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
    }
#endif
}