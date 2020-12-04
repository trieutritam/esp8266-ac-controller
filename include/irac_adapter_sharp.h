#include <ir_Sharp.h>

#include "irac_adapter.h"

class IrAcSharp : public IrAcAdapter
{
private:
  IRSharpAc *_pSharpAc;

public:
  IrAcSharp(int kIrLed);
  ~IrAcSharp();

  void init();
  void setOnOff(bool isOn);
  void setTemperature(const uint8_t temp);
  void setFanSpeed(int fanMode);
  void sendAc();

  String toString();
  uint8_t * getRaw();
};