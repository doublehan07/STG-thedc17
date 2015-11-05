#ifndef MAIN_H
#define MAIN_H

uint8_t FindTan(uint16_t s);
int16_t FixedTan(int16_t x, int16_t y);

extern volatile uint8_t ChangeFlag;
extern volatile uint8_t MakeNextMove;
#endif
