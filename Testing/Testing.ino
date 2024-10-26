#include <MCP4661_asukiaaa.hpp>

MCP4661_asukiaaa::Core potentio(&Wire, MCP4661_asukiaaa::DeviceAddress::A0H_A1H_A2H);

#define R_VALUE_MAX 256

int RrA_value = 0;
int RrB_value = 0;

#define R_62K  162 // 145 ~ 179
#define R_47K  121 // 114 ~ 132
#define R_39K  102 //  95 ~ 108
#define R_30K   78 //  74 ~  82
#define R_24K   63 //  60 ~  65
#define R_20K   52 //  50 ~  54
#define R_16K   42 //  39 ~  44
#define R_12K   31 //  29 ~  32
#define R_10K   26 //  25 ~  27
#define R_8_2K  21 //  20 ~  22
#define R_6_8K  17 //  17 ~  18
#define R_5_6K  14 //  14 ~  15


void printLogAboutWrite(Stream* serial, uint8_t channel, uint16_t valWrote, int result) {
  serial->print("write " + String(valWrote) + " for wiper" + String(channel) +
                " ");
  serial->println(result == 0 ? "succeeded"
                              : "failed by error " + String(result));
}

void printVal(Stream* serial, String target, uint8_t channel, int result,
              uint16_t val) {
  serial->print("read " + target + " of wiper" + String(channel) + " ");
  if (result != 0) {
    serial->println("failed error: " + String(result));
  } else {
    serial->println("got " + String(val));
  }
}

void setup(void)
{

  Wire.end();
  Wire.begin(6, 7);

  Serial.begin(115200);
  int result_a, result_b;
  result_a = potentio.writeWiperRAM(0, R_62K);
  printLogAboutWrite(&Serial, 0, R_62K, result_a);
  result_b = potentio.writeWiperRAM(1, R_62K);
  printLogAboutWrite(&Serial, 1, R_62K, result_b);
}

void loop(void)
{
  uint16_t val;
  int result;
  for (uint8_t targetChannel = 0; targetChannel < 2; ++targetChannel) {
    result = potentio.readWiperEEPROM(targetChannel, &val);
    printVal(&Serial, "EEPROM", targetChannel, result, val);
    result = potentio.readWiperRAM(targetChannel, &val);
    printVal(&Serial, "RAM", targetChannel, result, val);
  }

  MCP4661_asukiaaa::TCON tcon;
  potentio.readTCON(&tcon);
  Serial.print("read TCON ");
  if (tcon.readResult == 0) {
    tcon.println(&Serial);
  } else {
    Serial.println("failed error: " + String(tcon.readResult));
  }

  MCP4661_asukiaaa::Status statusPotentio;
  potentio.readStatus(&statusPotentio);
  Serial.print("read status ");
  if (statusPotentio.readResult == 0) {
    statusPotentio.println(&Serial);
  } else {
    Serial.println("failed error: " + String(statusPotentio.readResult));
  }

  Serial.println("at " + String(millis()));
  delay(1000);
}