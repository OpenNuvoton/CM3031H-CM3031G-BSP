#include <stdio.h>
#include "GUI.h"

/* One 320x240 RGB565 memory device plus emWin/GIF working memory. */
#define GUI_NUMBYTES (192u * 1024u)

void GUI_X_Config(void) {
  static U32 aMemory[GUI_NUMBYTES / 4u];

  printf("[GUI 01] GUI_X_Config enter, memory=%u bytes\n",
         (unsigned int)sizeof(aMemory));
  GUI_ALLOC_AssignMemory(aMemory, sizeof(aMemory));
  printf("[GUI 02] GUI memory assigned\n");
  GUI_SetDefaultFont(GUI_FONT_6X8);
  printf("[GUI 03] Default font configured\n");
}
