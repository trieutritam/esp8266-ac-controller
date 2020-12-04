#ifndef __IRAC_ADAPTER__
#define __IRAC_ADAPTER__

#include <string>
#include <stdint.h>

class IrAcAdapter {
public:
  IrAcAdapter() {};
  virtual ~IrAcAdapter() {};

  virtual void init() = 0;
  virtual void setOnOff(bool isOn) = 0;
  virtual void setTemperature(const uint8_t temp) = 0;
  virtual void setFanSpeed(int fanMode) = 0;
  virtual void sendAc() = 0;

  virtual String toString() = 0;
  virtual uint8_t * getRaw() = 0;
};

#endif