#ifndef IGPIO_H
#define IGPIO_H

#include <optional>

class IGpio {
public:
    virtual ~IGpio() = default;
    virtual bool setChipNumber(int chip) = 0;
    virtual bool setPinNumber(int pin) = 0;
    virtual bool write(bool value) = 0;
    virtual std::optional<bool> read() const = 0;

};

#endif // IGPIO_H
