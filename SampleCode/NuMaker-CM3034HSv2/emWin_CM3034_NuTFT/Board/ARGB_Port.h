#ifndef ARGB_PORT_H
#define ARGB_PORT_H

#include <stdint.h>

void ARGBPORT_Init(void);
void ARGBPORT_Exec(uint32_t now_ms);
void ARGBPORT_Off(void);

#endif
