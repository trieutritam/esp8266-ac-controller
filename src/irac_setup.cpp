#include <IRtext.h>
#include <assert.h>
#include <IRutils.h>
#include <IRac.h>

#include "irac_adapter_sharp.h"
#include "irac_adapter_panasonic.h"
#include "irac_adapter_hitachi.h"
#include "irac_setup.h"

IRACSetup::IRACSetup()
{
  #ifdef ENABLE_RECEIVER
  this->_pIrrecv = new IRrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
  #endif
}

IRACSetup::~IRACSetup()
{
  #ifdef ENABLE_RECEIVER
  delete this->_pIrrecv;
  #endif

  // delete this->_pPanasonicAc;
  delete this->_pIrAc;
  // delete this->_pHitachiAc;

}

void IRACSetup::init(const uint8_t acType)
{
  assert(irutils::lowLevelSanityCheck() == 0);

#ifdef ENABLE_RECEIVER

#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  this->_pIrrecv->setUnknownThreshold(kMinUnknownSize);
#endif                                                // DECODE_HASH

  this->_pIrrecv->setTolerance(kTolerancePercentage); // Override the default tolerance.
  this->_pIrrecv->enableIRIn();                       // Start the receiver
#endif

  switch (acType)
  {
  case USE_AC_SHARP:
    Serial.println("Init AC Sharp Controller");
    this->_pIrAc = new IrAcSharp(kIrLed);
    break;
  case USE_AC_PANASONIC:
    Serial.println("Init AC Panasonic Controller");
    this->_pIrAc = new IrAcPanasonic(kIrLed);
    break;
  case USE_AC_HITACHI:
    Serial.println("Init AC Hitachi Controller");
    this->_pIrAc = new IrAcHitachi(kIrLed);
    break;
  default:
    break;
  }
  
  this->_pIrAc->init();
}

void IRACSetup::debugIRReceived()
{
  #ifdef ENABLE_RECEIVER

  if (this->_pIrrecv->decode(&results))
  {
    // if (results.decode_type == decode_type_t::UNKNOWN)
      // return;

    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    // Display the tolerance percentage if it has been change from the default.
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length())
      Serial.println(D_STR_MESGDESC ": " + description);
    yield(); // Feed the WDT as the text output can take a while to print.
#if LEGACY_TIMING_INFO
    // Output legacy RAW timing info of the result.
    Serial.println(resultToTimingInfo(&results));
    yield(); // Feed the WDT (again)
#endif       // LEGACY_TIMING_INFO
    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println(); // Blank line between entries
    yield();          // Feed the WDT (again)
  }
  #endif
}

void IRACSetup::setOnOff(bool isOn)
{
  Serial.printf("Set AC power mode to: %s\n", isOn ? "ON": "OFF");
  this->_pIrAc->setOnOff(isOn);
}

void IRACSetup::setTemperature(const uint8_t temp)
{
  Serial.printf("Set AC temperature to: %d\n", temp);

  this->_pIrAc->setTemperature(temp);
}

void IRACSetup::sendAc()
{
  Serial.println("Send AC setting");

  this->_pIrAc->sendAc();
}

void IRACSetup::setFanSpeed(int fanMode)
{
  Serial.printf("Set AC Fan mode to: %d\n", fanMode);
  this->_pIrAc->setFanSpeed(fanMode);
}

void IRACSetup::printState()
{
  // Display the settings.
  Serial.println("A/C remote is in the following state:");
  Serial.printf("  %s\n", this->_pIrAc->toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = this->_pIrAc->getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kSharpAcStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}
