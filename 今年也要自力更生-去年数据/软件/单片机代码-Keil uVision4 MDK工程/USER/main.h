#ifndef MAIN_H
#define MAIN_H

uint8_t FindTan(uint16_t s);
int16_t FixedTan(int16_t x, int16_t y);
void SetNextHole(uint8_t mode);
uint8_t IsAHole(uint8_t x, uint8_t y);
uint8_t IsTargetAvailable(uint8_t available);
void TurnToEmy(void);

extern volatile uint8_t ChangeFlag;
extern volatile uint8_t MakeNextMove;
#endif
