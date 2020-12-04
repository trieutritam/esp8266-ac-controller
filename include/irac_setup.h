#include <map>
#include <IRrecv.h>
#include <IRremoteESP8266.h>

#include <irac_adapter.h>

const uint16_t kRecvPin = D5;
const uint16_t kCaptureBufferSize = 1024;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%

// Send AC
const u_int64_t kIrLed = D2;// 4;

#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kTimeout = 50;
#else   // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
#endif  // DECODE_AC

const uint8_t USE_AC_SHARP = 0;
const uint8_t USE_AC_PANASONIC = 1;
const uint8_t USE_AC_HITACHI = 2;

class IRACSetup 
{
private:
  decode_results results;
  IRrecv *_pIrrecv;

  IrAcAdapter *_pIrAc;

public:
  IRACSetup();
  ~IRACSetup();
  void init(const uint8_t acType);
  void debugIRReceived();
  void printState();

  void setOnOff(bool isOn);
  void setTemperature(const uint8_t temp);

  /**
   * 1 = MIN, 2 = MED, 3 = MAX, othewise = AUTO 
   */
  void setFanSpeed(int fanMode);
  void sendAc();
};
