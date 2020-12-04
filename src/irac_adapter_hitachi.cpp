#include "irac_adapter_hitachi.h"

IrAcHitachi::IrAcHitachi(int kIrLed)
{
  this->_pAc = new IRHitachiAc(kIrLed);
}

IrAcHitachi::~IrAcHitachi()
{
  delete this->_pAc;
}

void IrAcHitachi::init()
{
  this->_pAc->begin();
  this->_pAc->on();
  this->_pAc->setFan(kHitachiAcFanAuto);
  this->_pAc->setMode(kHitachiAcCool);
  this->_pAc->setTemp(25);
  this->_pAc->setSwingVertical(kHitachiAc344SwingHAuto);
  this->_pAc->setSwingHorizontal(kHitachiAc344SwingHAuto);
}

void IrAcHitachi::setOnOff(bool isOn)
{
  if (isOn)
  {
    this->_pAc->on();
  }
  else
  {
    this->_pAc->off();
  }
}

void IrAcHitachi::setTemperature(const uint8_t temp)
{
  uint8_t nTemp = temp;
  if (temp > kHitachiAcMaxTemp)
    nTemp = kHitachiAcMaxTemp;
  else if (temp < kHitachiAcMinTemp)
    nTemp = kHitachiAcMinTemp;

  this->_pAc->setTemp(nTemp);
}

void IrAcHitachi::setFanSpeed(int fanMode)
{
  switch (fanMode)
  {
  case 1:
    this->_pAc->setFan(kHitachiAcFanLow);
    break;
  case 2:
    this->_pAc->setFan(kHitachiAcFanMed);
    break;
  case 3:
    this->_pAc->setFan(kHitachiAcFanHigh);
    break;
  default:
    this->_pAc->setFan(kHitachiAcFanAuto);
    break;
  }
}

void IrAcHitachi::sendAc()
{
  this->_pAc->send();
}

String IrAcHitachi::toString()
{
  return this->_pAc->toString();
}

uint8_t *IrAcHitachi::getRaw()
{
  return this->_pAc->getRaw();
}