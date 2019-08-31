
inline void printLFNChar(uint16_t x)
{
  if (x == 0xFFFF) return;
  if (x > 127) gb.display.print(F("?")); else gb.display.print((char)x);
}
