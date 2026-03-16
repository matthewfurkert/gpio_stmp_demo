#ifndef IGPIO_H
#define IGPIO_H

class IGpio {
public:
    virtual ~IGpio() = default;
    virtual void setChipNumber(int chip) = 0;
    virtual void setPinNumber(int pin) = 0;
    virtual void write(bool value) = 0;
    virtual bool read() const = 0;

};

#endif // IGPIO_H
