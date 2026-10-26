#include <stdio.h>
#include "GUI.h"
#include "LCD_Port.h"
#include "Boot_Screen.h"

/*
 * Both resources are stored at the native 320x240 display size.  The animation
 * uses an optimized 256-color palette for the best quality GIF can provide.
 */
#include "Boot_Start_GIF.inc"
#include "Boot_End_GIF.inc"
#include "UI_Info_GIF.inc"
#include "UI_Info_Chip.inc"
#include "UI_Info_Flash.inc"
#include "UI_Info_Sram.inc"
#include "UI_Info_Welcome.inc"
#include "UI_Exit_Button.inc"
#include "UI_Game_Navigation.inc"
#include "UI_Home_Button.inc"
#include "UI_Capacity_Glyphs.inc"
#include "UI_Small_Glyphs.inc"

#define BOOT_FRAME_DELAY_MS 150

static int _DrawFrame(const unsigned char *pData, U32 NumBytes, int Frame) {
  return GUI_GIF_DrawSub(pData, NumBytes, 0, 0, Frame);
}

int BOOTSCREEN_DrawStartFrame(int Frame) {
  return _DrawFrame(_BootStartGif, sizeof(_BootStartGif), Frame);
}

int BOOTSCREEN_GetStartFrameCount(void) {
  GUI_GIF_INFO Info;

  if (GUI_GIF_GetInfo(_BootStartGif, sizeof(_BootStartGif), &Info) != 0) {
    return 0;
  }
  return Info.NumImages;
}

int BOOTSCREEN_GetStartFrameDelay(int Frame) {
  GUI_USE_PARA(Frame);
  return BOOT_FRAME_DELAY_MS;
}

void BOOTSCREEN_Show(void) {
  GUI_GIF_INFO Info;
  GUI_TIMER_TIME FrameStart;
  GUI_TIMER_TIME Elapsed;
  int Delay;
  int Frame;

  if (GUI_GIF_GetInfo(_BootStartGif, sizeof(_BootStartGif), &Info) != 0) {
    printf("[BOOT GIF] invalid start GIF\n");
    return;
  }

  LCDPORT_SetBacklight(0);
  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();
  printf("[BOOT GIF] %dx%d, frames=%d, bytes=%u\n",
         Info.xSize, Info.ySize, Info.NumImages,
         (unsigned)sizeof(_BootStartGif));

  for (Frame = 0; Frame < Info.NumImages; Frame++) {
    FrameStart = GUI_GetTime();
    if (BOOTSCREEN_DrawStartFrame(Frame) != 0) {
      printf("[BOOT GIF] draw failed at frame %d\n", Frame);
      break;
    }
    if (Frame == 0) {
      /* Reveal the display only after frame zero is fully transferred. */
      LCDPORT_SetBacklight(1);
      printf("[BOOT GIF] first frame ready, backlight on\n");
    }
    Delay = BOOTSCREEN_GetStartFrameDelay(Frame);
    Elapsed = GUI_GetTime() - FrameStart;
    if (Elapsed < Delay) {
      GUI_Delay(Delay - Elapsed);
    }
  }
}

void BOOTSCREEN_DrawEndImage(void) {
  if (GUI_GIF_Draw(_BootEndGif, sizeof(_BootEndGif), 0, 0) != 0) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    printf("[BOOT GIF] invalid end image\n");
  }
}

void BOOTSCREEN_DrawInfoPage(int Page) {
  const unsigned char *pOverlay;
  U32 OverlaySize;

  if (GUI_GIF_Draw(_UiInfoGif, sizeof(_UiInfoGif), 0, 0) != 0) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    printf("[UI] invalid NTNJ information background\n");
    return;
  }
  switch (Page) {
  case 1:
    pOverlay = _UiInfoChip;
    OverlaySize = sizeof(_UiInfoChip);
    break;
  case 2:
    pOverlay = _UiInfoFlash;
    OverlaySize = sizeof(_UiInfoFlash);
    break;
  case 3:
    pOverlay = _UiInfoSram;
    OverlaySize = sizeof(_UiInfoSram);
    break;
  default:
    pOverlay = _UiInfoWelcome;
    OverlaySize = sizeof(_UiInfoWelcome);
    break;
  }
  (void)GUI_GIF_Draw(pOverlay, OverlaySize, 16, 14);
  if (Page == 4) {
    /* The welcome page intentionally has no design credit. Cover the common
       footer credit while keeping the navigation buttons unchanged. */
    GUI_SetColor(GUI_MAKE_COLOR(0x00EEF2F5u));
    GUI_FillRect(80, 220, 239, 239);
  }
}

void BOOTSCREEN_DrawExitButton(int x, int y) {
  (void)GUI_GIF_Draw(_UiExitButton, sizeof(_UiExitButton), x, y);
}

void BOOTSCREEN_DrawGameNavigation(int x, int y) {
  (void)GUI_GIF_Draw(_UiGameNavigation, sizeof(_UiGameNavigation), x, y);
}

void BOOTSCREEN_DrawHomeButton(int x, int y) {
  (void)GUI_GIF_Draw(_UiHomeButton, sizeof(_UiHomeButton), x, y);
}

void BOOTSCREEN_DrawCapacityGlyph(char Character, int x, int y) {
  int Index;

  if ((Character >= '0') && (Character <= '9')) {
    Index = Character - '0';
  } else if (Character == '.') {
    Index = 10;
  } else if (Character == 'K') {
    Index = 11;
  } else if (Character == 'B') {
    Index = 12;
  } else {
    return;
  }
  (void)GUI_GIF_DrawSub(_UiCapacityGlyphs, sizeof(_UiCapacityGlyphs),
                        x, y, Index);
}

void BOOTSCREEN_DrawSmallGlyph(char Character, int x, int y) {
  static const char Glyphs[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!+:' .";
  int Index;

  if ((Character >= 'a') && (Character <= 'z')) {
    Character = (char)(Character - 'a' + 'A');
  }
  for (Index = 0; Glyphs[Index]; Index++) {
    if (Glyphs[Index] == Character) {
      (void)GUI_GIF_DrawSub(_UiSmallGlyphs, sizeof(_UiSmallGlyphs),
                            x, y, Index);
      return;
    }
  }
}
