#ifndef JOYSTICK_PORT_H
#define JOYSTICK_PORT_H

#include <stdint.h>

#define JOY_UP       0x01u
#define JOY_DOWN     0x02u
#define JOY_LEFT     0x04u
#define JOY_RIGHT    0x08u
#define JOY_CENTER   0x10u

void JOYPORT_Init(void);
uint8_t JOYPORT_GetState(void);

#endif
