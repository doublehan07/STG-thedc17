#ifndef _PUBLIC_H
#define _PUBLIC_H

#include "stm32f10x.h"

void Delay_mS(uint16_t delay);
void Delay_Simple(uint32_t delay);
void Array16_SmallToBig(uint16_t *array, uint8_t size);
void FigToString(uint32_t fig, uint8_t fignum, uint8_t figdot, uint8_t *s);
void FigToString(uint32_t fig, uint8_t fignum, uint8_t figdot, uint8_t *s);
void FigToStringAuto(uint32_t fig, uint8_t fignum, uint8_t figdot, uint8_t *s);
void FigToStringHex(uint32_t fig, uint8_t num, uint8_t *s);
uint8_t StringAdd(uint8_t *dirs, uint8_t *dirsadd, uint8_t size);
uint8_t CompareString(uint8_t *s1,uint8_t *s2);
uint8_t CopyString(uint8_t *s, uint8_t *s1, uint8_t s1size);
void DoubleFigToStringAuto(double fig, uint8_t fignum, uint8_t figdot, uint8_t *s);

#endif

