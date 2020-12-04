#include "irac_adapter_panasonic.h"

IrAcPanasonic::IrAcPanasonic(int kIrLed)
{
  this->_pAc = new IRPanasonicAc(kIrLed);
}

IrAcPanasonic::~IrAcPanasonic()
{
  delete this->_pAc;
}

void IrAcPanasonic::init()
{
  this->_pAc->begin();
  this->_pAc->setModel(kPanasonicRkr);
  this->_pAc->on();
  this->_pAc->setFan(kPanasonicAcFanAuto);
  this->_pAc->setMode(kPanasonicAcCool);
  this->_pAc->setTemp(25);
  this->_pAc->setSwingVertical(kPanasonicAcSwingVAuto);
  this->_pAc->setSwingHorizontal(kPanasonicAcSwingHAuto);
}

void IrAcPanasonic::setOnOff(bool isOn)
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

void IrAcPanasonic::setTemperature(const uint8_t temp)
{
  uint8_t nTemp = temp;
  if (temp > kPanasonicAcMaxTemp)
    nTemp = kPanasonicAcMaxTemp;
  else if (temp < kPanasonicAcMinTemp)
    nTemp = kPanasonicAcMinTemp;

  this->_pAc->setTemp(nTemp, true);
}

void IrAcPanasonic::setFanSpeed(int fanMode)
{
  switch (fanMode)
  {
  case 1:
    this->_pAc->setFan(kPanasonicAcFanMin);
    break;
  case 2:
    this->_pAc->setFan(kPanasonicAcFanMed);
    break;
  case 3:
    this->_pAc->setFan(kPanasonicAcFanMax);
    break;
  default:
    this->_pAc->setFan(kPanasonicAcFanAuto);
    break;
  }
}

void IrAcPanasonic::sendAc()
{
  this->_pAc->send();
}

String IrAcPanasonic::toString()
{
  return this->_pAc->toString();
}

uint8_t *IrAcPanasonic::getRaw()
{
  return this->_pAc->getRaw();
}