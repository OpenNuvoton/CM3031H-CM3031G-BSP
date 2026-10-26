#ifndef BOOT_SCREEN_H
#define BOOT_SCREEN_H

void BOOTSCREEN_Show(void);
void BOOTSCREEN_DrawEndImage(void);
void BOOTSCREEN_DrawInfoPage(int Page);
void BOOTSCREEN_DrawExitButton(int x, int y);
void BOOTSCREEN_DrawGameNavigation(int x, int y);
void BOOTSCREEN_DrawHomeButton(int x, int y);
void BOOTSCREEN_DrawCapacityGlyph(char Character, int x, int y);
void BOOTSCREEN_DrawSmallGlyph(char Character, int x, int y);
int  BOOTSCREEN_DrawStartFrame(int Frame);
int  BOOTSCREEN_GetStartFrameCount(void);
int  BOOTSCREEN_GetStartFrameDelay(int Frame);

#endif
