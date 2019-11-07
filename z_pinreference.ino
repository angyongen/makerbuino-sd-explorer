
const char sp00[] PROGMEM = "rxPin   ";
const char sp01[] PROGMEM = "txPin   ";
const char sp02[] PROGMEM = "BBtn    ";
const char sp03[] PROGMEM = "SPEAKER ";
const char sp04[] PROGMEM = "ABtn    ";
const char sp05[] PROGMEM = "GB_BackL";
const char sp06[] PROGMEM = "DownBtn ";
const char sp07[] PROGMEM = "RightBtn";
const char sp08[] PROGMEM = "LeftBtn ";
const char sp09[] PROGMEM = "UpBtn   ";
const char sp10[] PROGMEM = "SD_CS   ";
const char sp11[] PROGMEM = "SPI_MOSI";
const char sp12[] PROGMEM = "SPI_MISO";
const char sp13[] PROGMEM = "SPI_CLK ";
const char sp14[] PROGMEM = "SCR_RST "; //a0
const char sp15[] PROGMEM = "SCR_CS  "; //a1
const char sp16[] PROGMEM = "SCR_DC  "; //a2
const char sp17[] PROGMEM = "BTN_C   "; //a3
const char sp18[] PROGMEM = "I2C_SDA "; //a4
const char sp19[] PROGMEM = "I2C_SCL "; //a5
const char* const pinInfo[20] = {sp00, sp01, sp02, sp03, sp04, sp05, sp06, sp07, sp08, sp09, sp10, sp11, sp12, sp13, sp14, sp15, sp16, sp17, sp18, sp19};
inline void print_pinInfo(uint8_t pin) {
  if (pin > 19) return;
  gb.display.print((const __FlashStringHelper*)pinInfo[pin]);
}
