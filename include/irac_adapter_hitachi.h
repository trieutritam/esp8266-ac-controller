#include <ir_Hitachi.h>

#include "irac_adapter.h"

class IrAcHitachi : public IrAcAdapter
{
private:
  IRHitachiAc *_pAc;

public:
  IrAcHitachi(int kIrLed);
  ~IrAcHitachi();

  void init();
  void setOnOff(bool isOn);
  void setTemperature(const uint8_t temp);
  void setFanSpeed(int fanMode);
  void sendAc();

  String toString();
  uint8_t * getRaw();
};