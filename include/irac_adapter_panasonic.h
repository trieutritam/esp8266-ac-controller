#include <ir_Panasonic.h>

#include "irac_adapter.h"

class IrAcPanasonic : public IrAcAdapter
{
private:
  IRPanasonicAc *_pAc;

public:
  IrAcPanasonic(int kIrLed);
  ~IrAcPanasonic();

  void init();
  void setOnOff(bool isOn);
  void setTemperature(const uint8_t temp);
  void setFanSpeed(int fanMode);
  void sendAc();

  String toString();
  uint8_t * getRaw();
};