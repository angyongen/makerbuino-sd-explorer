
inline void printLFNChar(uint16_t x)
{
  if (x == 0xFFFF) return;
  gb.display.print((char)x);
}
