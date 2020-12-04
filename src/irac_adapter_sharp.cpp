#include "irac_adapter_sharp.h"

IrAcSharp::IrAcSharp(int kIrLed)
{
  this->_pSharpAc = new IRSharpAc(kIrLed);
}

IrAcSharp::~IrAcSharp()
{
  delete this->_pSharpAc;
}

void IrAcSharp::init()
{
  this->_pSharpAc->begin();
  this->_pSharpAc->setModel(sharp_ac_remote_model_t::A907);
  this->_pSharpAc->on();
  this->_pSharpAc->setFan(kSharpAcFanAuto);
  this->_pSharpAc->setMode(kSharpAcCool);
  this->_pSharpAc->setTemp(26);
  this->_pSharpAc->setSwingToggle(true);
}

void IrAcSharp::setOnOff(bool isOn)
{
  if (isOn)
  {
    this->_pSharpAc->on();
  }
  else
  {
    this->_pSharpAc->off();
  }
}

void IrAcSharp::setTemperature(const uint8_t temp)
{
  uint8_t nTemp = temp;
  if (temp > kSharpAcMaxTemp)
    nTemp = kSharpAcMaxTemp;
  else if (temp < kSharpAcMinTemp)
    nTemp = kSharpAcMinTemp;

  this->_pSharpAc->setTemp(nTemp, true);
}

void IrAcSharp::setFanSpeed(int fanMode)
{
  switch (fanMode)
  {
  case 1:
    this->_pSharpAc->setFan(kSharpAcFanMin);
    break;
  case 2:
    this->_pSharpAc->setFan(kSharpAcFanMed);
    break;
  case 3:
    this->_pSharpAc->setFan(kSharpAcFanHigh);
    break;
  case 4:
    this->_pSharpAc->setFan(kSharpAcFanMax);
    break;
  default:
    this->_pSharpAc->setFan(kSharpAcFanAuto);
    break;
  }
}

void IrAcSharp::sendAc()
{
  this->_pSharpAc->send();
}

String IrAcSharp::toString()
{
  return this->_pSharpAc->toString();
}

uint8_t *IrAcSharp::getRaw()
{
  return this->_pSharpAc->getRaw();
}