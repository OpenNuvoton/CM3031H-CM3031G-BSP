#include <stdio.h>
#include <stdint.h>
#include "NuMicro.h"
#include "GUI.h"
#include "Keys_Port.h"
#include "Joystick_Port.h"
#include "Demo_UI.h"
#include "Boot_Screen.h"

#include "Pet_Walk_3x64.inc"

#define SCREEN_W 320
#define CONTENT_Y1 181
#define BUTTON_Y0 182
#define BUTTON_Y1 218
#define FOOTER_Y0 219
#define INFO_TIMEOUT_MS 4000u
#define PAGE_HOME 0
#define PAGE_GAME 1
#define PAGE_GIF  2
#define HOME_GIF_X0 8
#define HOME_GIF_Y0 10
#define HOME_GIF_X1 74
#define HOME_GIF_Y1 35
#define HOME_PET_X0 246
#define HOME_PET_Y0 10
#define HOME_PET_X1 311
#define HOME_PET_Y1 35
#define GIF_EXIT_X0 244
#define GIF_EXIT_Y0 211
#define GIF_EXIT_X1 319
#define GIF_EXIT_Y1 239
#define FIELD_Y0 32
#define PET_MIN_X 0
#define PET_MAX_X (SCREEN_W - PET_SPRITE_WIDTH)
#define PET_MIN_Y 38
#define PET_MAX_Y (CONTENT_Y1 + 1 - PET_SPRITE_HEIGHT)
#define PET_MOVE_MS 40u
#define PET_STEP 4
#define PET_WALK_FRAME_DIVIDER 2u
#define PET_SLEEP_MS 8000u
#define PET_SLEEP_ANIM_MS 360u
#define GAME_DECAY_MS 5000u
#define ITEM_RESPAWN_MS 6000u
#define FLASH_TOTAL (512u * 1024u)
#define SRAM_TOTAL  (320u * 1024u)
#define COLOR_SKY       GUI_MAKE_COLOR(0x00F4D7A8u)
#define COLOR_SKY_DARK  GUI_MAKE_COLOR(0x00E9BE78u)
#define COLOR_GRASS     GUI_MAKE_COLOR(0x0079D893u)
#define COLOR_GRASS_DK  GUI_MAKE_COLOR(0x0053A45Cu)
#define COLOR_HUD       GUI_MAKE_COLOR(0x00352B20u)
#define COLOR_PANEL     GUI_MAKE_COLOR(0x00352B20u)
#define COLOR_NIGHT     GUI_MAKE_COLOR(0x0042180Bu)
#define COLOR_NIGHT_HI  GUI_MAKE_COLOR(0x00783A18u)
#define COLOR_NIGHT_GND GUI_MAKE_COLOR(0x00332A19u)
#define COLOR_PET_FACE  GUI_MAKE_COLOR(0x00FAFAFAu)
#define COLOR_PET_LINE  GUI_MAKE_COLOR(0x001A123Eu)

/* Product-page palette (GUI_MAKE_COLOR uses emWin's 0x00BBGGRR order). */
#define COLOR_UI_BG       GUI_MAKE_COLOR(0x00EEF2F5u)

extern uint32_t Image$$ER_ROM$$Limit;
extern uint32_t Image$$RW_RAM$$ZI$$Limit;

static int _Page;
static int _InfoPage;
static GUI_TIMER_TIME _InfoUntil;
static int _TouchWasPressed;
static int _TouchButton;
static int _PressedButton;
static GUI_MEMDEV_Handle _hGifMem;
static int _GifFrame;
static int _GifFrameCount;
static GUI_TIMER_TIME _NextGifFrame;
static int _PetX = (SCREEN_W - PET_SPRITE_WIDTH) / 2;
static int _PetY = 94;
static int _PetTargetX = (SCREEN_W - PET_SPRITE_WIDTH) / 2;
static int _PetTargetY = 94;
static int _TouchMoving;
static GUI_TIMER_TIME _NextMove;
static uint8_t _LastJoy;
static uint8_t _WalkFrame = 1u;
static uint8_t _WalkDivider;
static uint8_t _NightMode;
static uint8_t _SleepFrame;
static uint8_t _WakeInputLatch;
static GUI_TIMER_TIME _NextSleepAnim;

static int _Hunger = 78;
static int _Happy = 74;
static int _Clean = 86;
static int _Energy = 82;
static unsigned _Score;
static unsigned _AgeSeconds;
static GUI_TIMER_TIME _LastDecay;
static GUI_TIMER_TIME _SleepUntil;
static GUI_TIMER_TIME _MessageUntil;
static GUI_TIMER_TIME _NextItem;
static const char *_Message = "Explore and have fun!";
static int _ItemVisible = 1;
static int _ItemX = 46;
static int _ItemY = 79;
static uint32_t _RandomState = 0x3034A55Au;

static void _RenderHome(void);
static void _RenderGame(GUI_TIMER_TIME now);

static int _Clamp(int value, int minimum, int maximum) {
  if (value < minimum) return minimum;
  if (value > maximum) return maximum;
  return value;
}

static int _Approach(int value, int target) {
  if (value < target) {
    value += PET_STEP;
    if (value > target) value = target;
  } else if (value > target) {
    value -= PET_STEP;
    if (value < target) value = target;
  }
  return value;
}

static int _HitBottomButton(int x, int y) {
  if ((y < BUTTON_Y0) || (y > BUTTON_Y1) || (x < 0) || (x >= SCREEN_W)) return 0;
  return x / 80 + 1;
}

static GUI_MEMDEV_Handle _BeginBufferedDraw(void) {
  GUI_MEMDEV_Handle hMem;

  hMem = GUI_MEMDEV_CreateFixed(0, 0, 320, 240, GUI_MEMDEV_NOTRANS,
                                GUI_MEMDEV_APILIST_16, GUICC_M565);
  if (hMem) {
    GUI_MEMDEV_Select(hMem);
  }
  return hMem;
}

static void _EndBufferedDraw(GUI_MEMDEV_Handle hMem) {
  if (hMem) {
    GUI_MEMDEV_Select(0);
    GUI_MEMDEV_CopyToLCD(hMem);
    GUI_MEMDEV_Delete(hMem);
  }
}

static void _DrawGifExitButton(void) {
  BOOTSCREEN_DrawExitButton(GIF_EXIT_X0, GIF_EXIT_Y0);
}

static void _ShowHome(void) {
  _Page = PAGE_HOME;
  _InfoPage = 0;
  _TouchMoving = 0;
  _PressedButton = 0;
  _RenderHome();
}

static void _FormatCapacity(char *pText, unsigned TextSize, uint32_t Bytes) {
  uint32_t Whole;
  uint32_t Fraction;

  Whole = Bytes / 1024u;
  Fraction = ((Bytes % 1024u) * 100u + 512u) / 1024u;
  if (Fraction >= 100u) {
    Whole++;
    Fraction = 0u;
  }
  snprintf(pText, TextSize, "%u.%02u KB",
           (unsigned)Whole, (unsigned)Fraction);
}

static void _DrawCapacityText(const char *pText, int xCenter, int y) {
  const char *p;
  int Width = 0;
  int x;

  for (p = pText; *p; p++) {
    if (*p == ' ') Width += 5;
    else if (*p == '.') Width += 6;
    else if ((*p == 'K') || (*p == 'B')) Width += 11;
    else Width += 10;
  }
  x = xCenter - Width / 2;
  for (p = pText; *p; p++) {
    if (*p == ' ') {
      x += 5;
    } else {
      BOOTSCREEN_DrawCapacityGlyph(*p, x, y);
      if (*p == '.') x += 6;
      else if ((*p == 'K') || (*p == 'B')) x += 11;
      else x += 10;
    }
  }
}

static int _SmallTextWidth(const char *pText) {
  int Width = 0;

  while (*pText++) {
    Width += 8;
  }
  return Width;
}

static void _DrawSmallTextAt(const char *pText, int x, int y) {
  while (*pText) {
    BOOTSCREEN_DrawSmallGlyph(*pText++, x, y);
    x += 8;
  }
}

static void _DrawSmallTextCentered(const char *pText, int xCenter, int y) {
  _DrawSmallTextAt(pText, xCenter - _SmallTextWidth(pText) / 2, y);
}

static void _DrawUsageValues(void) {
  char Text[24];
  uint32_t Used;
  uint32_t Available;
  uint32_t Total;

  if (_InfoPage == 2) {
    Used = (uint32_t)&Image$$ER_ROM$$Limit;
    Total = FLASH_TOTAL;
  } else if (_InfoPage == 3) {
    Used = (uint32_t)&Image$$RW_RAM$$ZI$$Limit - 0x20000000u;
    Total = SRAM_TOTAL;
  } else {
    return;
  }
  Available = (Used < Total) ? (Total - Used) : 0u;
  _FormatCapacity(Text, sizeof(Text), Used);
  _DrawCapacityText(Text, 225, 77);
  _FormatCapacity(Text, sizeof(Text), Available);
  _DrawCapacityText(Text, 225, 111);
  _FormatCapacity(Text, sizeof(Text), Total);
  _DrawCapacityText(Text, 225, 145);
}

static void _RenderHome(void) {
  GUI_MEMDEV_Handle hMem;

  hMem = _BeginBufferedDraw();
  GUI_SetBkColor(COLOR_UI_BG);
  GUI_Clear();
  if (_InfoPage != 0) {
    BOOTSCREEN_DrawInfoPage(_InfoPage);
    _DrawUsageValues();
  } else {
    BOOTSCREEN_DrawEndImage();
  }
  _EndBufferedDraw(hMem);
}

static void _PresentGifFrame(int Frame) {
  if (_hGifMem) {
    GUI_MEMDEV_Select(_hGifMem);
  }
  if (BOOTSCREEN_DrawStartFrame(Frame) != 0) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
  }
  _DrawGifExitButton();
  if (_hGifMem) {
    GUI_MEMDEV_Select(0);
    GUI_MEMDEV_CopyToLCD(_hGifMem);
  }
}

static void _ShowGifLoop(GUI_TIMER_TIME now) {
  int Delay;

  _Page = PAGE_GIF;
  _InfoPage = 0;
  _TouchMoving = 0;
  _PressedButton = 0;
  _GifFrameCount = BOOTSCREEN_GetStartFrameCount();
  if (_GifFrameCount <= 0) {
    _ShowHome();
    return;
  }
  _hGifMem = _BeginBufferedDraw();
  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();
  if (_hGifMem) {
    GUI_MEMDEV_Select(0);
  }
  _GifFrame = 0;
  _PresentGifFrame(_GifFrame);
  Delay = BOOTSCREEN_GetStartFrameDelay(_GifFrame);
  _GifFrame = (_GifFrame + 1) % _GifFrameCount;
  _NextGifFrame = now + Delay;
  printf("[UI] looping GIF page entered, frames=%d\n", _GifFrameCount);
}

static void _GifLoopTick(GUI_TIMER_TIME now) {
  int Delay;

  if ((int32_t)(now - _NextGifFrame) < 0) {
    return;
  }
  _PresentGifFrame(_GifFrame);
  Delay = BOOTSCREEN_GetStartFrameDelay(_GifFrame);
  _GifFrame = (_GifFrame + 1) % _GifFrameCount;
  _NextGifFrame = now + Delay;
}

static void _ExitGifLoop(void) {
  if (_hGifMem) {
    GUI_MEMDEV_Delete(_hGifMem);
    _hGifMem = 0;
  }
  _ShowHome();
  printf("[UI] looping GIF page exited\n");
}

static void _ActivateInfo(int page, GUI_TIMER_TIME now) {
  if ((page < 1) || (page > 4)) return;
  _InfoPage = page;
  _InfoUntil = now + INFO_TIMEOUT_MS;
  printf("[UI] action %d selected\n", page);
  _RenderHome();
}

static void _DrawHeart(int x, int y) {
  GUI_POINT points[3];
  GUI_SetColor(GUI_RED);
  GUI_FillCircle(x + 4, y + 4, 4);
  GUI_FillCircle(x + 10, y + 4, 4);
  points[0].x = 0; points[0].y = 4;
  points[1].x = 14; points[1].y = 4;
  points[2].x = 7; points[2].y = 14;
  GUI_FillPolygon(points, 3, x, y);
  GUI_SetColor(GUI_WHITE);
  GUI_DrawPixel(x + 4, y + 2);
}

static void _DrawGameBase(void) {
  if (_NightMode) {
    int twinkle = (int)(_SleepFrame & 1u);
    GUI_SetColor(COLOR_NIGHT);
    GUI_FillRect(0, FIELD_Y0, 319, 130);
    GUI_SetColor(COLOR_NIGHT_HI);
    GUI_DrawHLine(FIELD_Y0, 0, 319);
    GUI_SetColor(GUI_LIGHTYELLOW);
    GUI_FillCircle(274, 57, 14);
    GUI_SetColor(COLOR_NIGHT);
    GUI_FillCircle(280, 52, 13);
    GUI_SetColor(GUI_WHITE);
    GUI_DrawPixel(22, 49 + twinkle);
    GUI_DrawPixel(72, 67);
    GUI_DrawPixel(118, 45 + twinkle);
    GUI_DrawPixel(169, 61);
    GUI_DrawPixel(218, 42 + twinkle);
    GUI_DrawPixel(303, 83);
    GUI_SetColor(GUI_LIGHTBLUE);
    GUI_DrawPixel(48, 91);
    GUI_DrawPixel(145, 82 + twinkle);
    GUI_DrawPixel(250, 96);
    GUI_SetColor(COLOR_NIGHT_GND);
    GUI_FillRect(0, 131, 319, CONTENT_Y1);
    GUI_SetColor(COLOR_NIGHT_HI);
    GUI_DrawHLine(131, 0, 319);
    GUI_DrawHLine(180, 0, 319);
    return;
  }
  GUI_SetColor(COLOR_SKY);
  GUI_FillRect(0, FIELD_Y0, 319, 130);
  GUI_SetColor(COLOR_SKY_DARK);
  GUI_DrawHLine(FIELD_Y0, 0, 319);
  GUI_SetColor(GUI_YELLOW);
  GUI_FillCircle(28, 57, 14);
  GUI_SetColor(GUI_WHITE);
  GUI_FillCircle(242, 59, 9);
  GUI_FillCircle(254, 54, 12);
  GUI_FillCircle(269, 60, 8);
  GUI_FillRect(242, 59, 269, 68);
  GUI_SetColor(COLOR_GRASS);
  GUI_FillRect(0, 131, 319, CONTENT_Y1);
  GUI_SetColor(COLOR_GRASS_DK);
  GUI_DrawHLine(131, 0, 319);
  GUI_DrawHLine(180, 0, 319);
  GUI_DrawLine(10, 173, 14, 164);
  GUI_DrawLine(14, 173, 19, 166);
  GUI_DrawLine(298, 172, 302, 163);
  GUI_DrawLine(302, 172, 307, 165);
}

static void _DrawCollectible(void) {
  if (_ItemVisible) _DrawHeart(_ItemX, _ItemY);
}

static const char *_GetMood(GUI_TIMER_TIME now) {
  int average = (_Hunger + _Happy + _Clean + _Energy) / 4;
  if ((int32_t)(now - _SleepUntil) < 0) return "SLEEPING";
  if (average >= 80) return "SUPER HAPPY";
  if (average >= 60) return "HAPPY";
  if (average >= 40) return "NEEDS CARE";
  return "SAD";
}

static void _DrawStatBar(int x, const char *name, int value, GUI_COLOR color) {
  int width = value * 43 / 100;
  _DrawSmallTextAt(name, x, 2);
  GUI_SetColor(GUI_DARKGRAY);
  GUI_FillRect(x + 11, 5, x + 55, 10);
  GUI_SetColor(color);
  if (width > 0) GUI_FillRect(x + 12, 6, x + 11 + width, 9);
}

static void _DrawHUD(GUI_TIMER_TIME now) {
  char text[48];
  GUI_SetColor(COLOR_HUD);
  GUI_FillRect(0, 0, 319, FIELD_Y0 - 1);
  _DrawStatBar(3, "H", _Hunger, GUI_ORANGE);
  _DrawStatBar(65, "F", _Happy, GUI_MAGENTA);
  _DrawStatBar(127, "C", _Clean, GUI_CYAN);
  _DrawStatBar(189, "E", _Energy, GUI_GREEN);
  snprintf(text, sizeof(text), "%s  AGE:%us", _GetMood(now), _AgeSeconds);
  _DrawSmallTextAt(text, 5, 17);
  BOOTSCREEN_DrawHomeButton(267, 13);
}

static void _DrawGameFooter(void) {
  char text[64];
  GUI_SetColor(COLOR_PANEL);
  GUI_FillRect(0, FOOTER_Y0, 319, 239);
  snprintf(text, sizeof(text), "%s   SCORE:%u", _Message, _Score);
  _DrawSmallTextCentered(text, 160, 225);
}

static void _DrawGameButtons(void) {
  BOOTSCREEN_DrawGameNavigation(0, BUTTON_Y0);
}

static void _DrawPet(void) {
  int y = _PetY;
  if (_NightMode) {
    int phase = (int)(_SleepFrame % 3u);
    int pet_y = y - 5 + (int)(_SleepFrame & 1u);

    /* Bed frame, mattress and pillow are drawn behind the character. */
    GUI_SetColor(GUI_DARKGRAY);
    GUI_FillRoundedRect(_PetX - 5, y + 34, _PetX + 69, y + 62, 5);
    GUI_SetColor(GUI_MAKE_COLOR(0x0023446Bu));
    GUI_FillRoundedRect(_PetX - 4, y + 32, _PetX + 68, y + 59, 5);
    GUI_SetColor(GUI_LIGHTBLUE);
    GUI_FillRoundedRect(_PetX - 1, y + 34, _PetX + 65, y + 55, 6);
    GUI_SetColor(GUI_WHITE);
    GUI_FillRoundedRect(_PetX + 3, y + 33, _PetX + 30, y + 44, 5);

    /* Only the head and upper body remain visible above the quilt. */
    GUI_DrawBitmap(&_PetWalkBitmaps[1], _PetX, pet_y);

    /* Replace the open eyes with small curved sleeping eyes. */
    GUI_SetColor(COLOR_PET_FACE);
    GUI_FillRoundedRect(_PetX + 17, pet_y + 21, _PetX + 27, pet_y + 28, 3);
    GUI_FillRoundedRect(_PetX + 37, pet_y + 21, _PetX + 47, pet_y + 28, 3);
    GUI_SetColor(COLOR_PET_LINE);
    GUI_DrawHLine(pet_y + 25, _PetX + 19, _PetX + 25);
    GUI_DrawHLine(pet_y + 25, _PetX + 39, _PetX + 45);
    GUI_DrawPixel(_PetX + 18, pet_y + 24);
    GUI_DrawPixel(_PetX + 46, pet_y + 24);

    /* Rounded blue quilt covers the body and gently moves with breathing. */
    GUI_SetColor(GUI_MAKE_COLOR(0x00DFA45Bu));
    GUI_FillRoundedRect(_PetX + 6, y + 39, _PetX + 64,
                        y + 57 + (int)(_SleepFrame & 1u), 7);
    GUI_SetColor(GUI_LIGHTCYAN);
    GUI_DrawRoundedRect(_PetX + 6, y + 39, _PetX + 64,
                        y + 57 + (int)(_SleepFrame & 1u), 7);
    GUI_DrawLine(_PetX + 18, y + 42, _PetX + 22, y + 55);
    GUI_DrawLine(_PetX + 34, y + 41, _PetX + 38, y + 56);
    GUI_DrawLine(_PetX + 50, y + 42, _PetX + 54, y + 55);
    GUI_SetColor(GUI_MAKE_COLOR(0x0023446Bu));
    GUI_FillRect(_PetX - 4, y + 57, _PetX + 68, y + 62);
    GUI_FillRect(_PetX - 3, y + 60, _PetX + 1, y + 64);
    GUI_FillRect(_PetX + 64, y + 60, _PetX + 68, y + 64);

    /* A tiny bedside cup makes the sleep scene feel less empty. */
    GUI_SetColor(GUI_LIGHTYELLOW);
    GUI_FillRect(_PetX + 69, y + 48, _PetX + 74, y + 56);
    GUI_DrawHLine(y + 47, _PetX + 68, _PetX + 75);
    GUI_DrawPixel(_PetX + 75, y + 51);
    GUI_DrawPixel(_PetX + 75, y + 52);

    /* Three bubbles rise from the sleeping pet without introducing a second
       font style into the interface. */
    GUI_SetColor(GUI_LIGHTCYAN);
    GUI_FillCircle(_PetX + 50, pet_y + 19 - phase * 2, 3);
    GUI_FillCircle(_PetX + 57, pet_y + 11 - ((phase + 1) % 3) * 2, 2);
    GUI_FillCircle(_PetX + 62, pet_y + 5 - ((phase + 2) % 3) * 2, 1);
  } else {
    GUI_DrawBitmap(&_PetWalkBitmaps[_WalkFrame], _PetX, y);
  }
}

static void _DrawGameField(void) {
  _DrawGameBase();
  _DrawCollectible();
  _DrawPet();
}

static void _RenderGame(GUI_TIMER_TIME now) {
  GUI_MEMDEV_Handle hMem;

  hMem = _BeginBufferedDraw();
  GUI_SetBkColor(COLOR_PANEL);
  GUI_Clear();
  _DrawHUD(now);
  _DrawGameField();
  _DrawGameButtons();
  _DrawGameFooter();
  _EndBufferedDraw(hMem);
}

static void _ShowGame(GUI_TIMER_TIME now) {
  _Page = PAGE_GAME;
  _InfoPage = 0;
  _TouchMoving = 0;
  _PressedButton = 0;
  _WalkFrame = 1u;
  _WalkDivider = 0u;
  _NightMode = ((int32_t)(now - _SleepUntil) < 0) ? 1u : 0u;
  _SleepFrame = 0u;
  _WakeInputLatch = 0u;
  _NextSleepAnim = now;
  _LastDecay = now;
  _RenderGame(now);
  printf("[PET] Tamagotchi page entered, touch/joystick movement active\n");
}

static void _RedrawPetMove(int old_x, int old_y) {
  GUI_RECT clip;
  clip.x0 = (old_x < _PetX ? old_x : _PetX) - 6;
  clip.y0 = (old_y < _PetY ? old_y : _PetY) - 10;
  clip.x1 = (old_x > _PetX ? old_x : _PetX) + PET_SPRITE_WIDTH + 11;
  clip.y1 = (old_y > _PetY ? old_y : _PetY) + PET_SPRITE_HEIGHT + 1;
  clip.x0 = _Clamp(clip.x0, 0, 319);
  clip.y0 = _Clamp(clip.y0, FIELD_Y0, CONTENT_Y1);
  clip.x1 = _Clamp(clip.x1, 0, 319);
  clip.y1 = _Clamp(clip.y1, FIELD_Y0, CONTENT_Y1);
  GUI_SetClipRect(&clip);
  _DrawGameBase();
  _DrawCollectible();
  _DrawPet();
  GUI_SetClipRect(NULL);
}

static void _SetMessage(const char *message, GUI_TIMER_TIME now) {
  _Message = message;
  _MessageUntil = now + 2200u;
}

static void _WakeUp(GUI_TIMER_TIME now, const char *message) {
  if (!_NightMode) return;
  _SleepUntil = now;
  _NightMode = 0u;
  _SleepFrame = 0u;
  _WalkFrame = 1u;
  _TouchMoving = 0;
  _Message = message;
  _MessageUntil = now + 2200u;
  _RenderGame(now);
}

static int _AddStat(int value, int delta) {
  return _Clamp(value + delta, 0, 100);
}

static void _GameAction(int action, GUI_TIMER_TIME now) {
  if (action == 1) {
    _Hunger = _AddStat(_Hunger, 22);
    _Happy = _AddStat(_Happy, 3);
    _SetMessage("Yummy!", now);
  } else if (action == 2) {
    _Happy = _AddStat(_Happy, 18);
    _Energy = _AddStat(_Energy, -7);
    _Score += 2u;
    _SetMessage("That was fun!", now);
  } else if (action == 3) {
    _Clean = _AddStat(_Clean, 26);
    _Happy = _AddStat(_Happy, 2);
    _SetMessage("Sparkling clean!", now);
  } else if (action == 4) {
    _SleepUntil = now + PET_SLEEP_MS;
    _TouchMoving = 0;
    _WalkFrame = 1u;
    _NightMode = 1u;
    _SleepFrame = 0u;
    _NextSleepAnim = now + PET_SLEEP_ANIM_MS;
    _Message = "Sweet dreams... Zzz";
    _MessageUntil = _SleepUntil;
    printf("[PET] sleep animation started: night scene, breathing and Zzz\n");
  } else {
    return;
  }
  printf("[PET] action=%d H/F/C/E=%d/%d/%d/%d score=%u\n",
         action, _Hunger, _Happy, _Clean, _Energy, _Score);
  _RenderGame(now);
}

static void _SpawnItem(GUI_TIMER_TIME now) {
  _RandomState = _RandomState * 1664525u + 1013904223u;
  _ItemX = 12 + (int)((_RandomState >> 8) % 282u);
  _RandomState = _RandomState * 1664525u + 1013904223u;
  _ItemY = 48 + (int)((_RandomState >> 8) % 61u);
  _ItemVisible = 1;
  _NextItem = now + ITEM_RESPAWN_MS;
  _DrawGameField();
}

static void _CheckCollectible(GUI_TIMER_TIME now) {
  int pet_cx;
  int pet_cy;
  if (!_ItemVisible) return;
  pet_cx = _PetX + PET_SPRITE_WIDTH / 2;
  pet_cy = _PetY + PET_SPRITE_HEIGHT / 2;
  if ((pet_cx >= _ItemX - 12) && (pet_cx <= _ItemX + 26) &&
      (pet_cy >= _ItemY - 12) && (pet_cy <= _ItemY + 26)) {
    _ItemVisible = 0;
    _Score += 10u;
    _Happy = _AddStat(_Happy, 6);
    _Hunger = _AddStat(_Hunger, 3);
    _NextItem = now + 2500u;
    _SetMessage("Heart found! +10", now);
    _DrawHUD(now);
    _DrawGameFooter();
    printf("[PET] collectible found, score=%u\n", _Score);
  }
}

static void _MovePet(GUI_TIMER_TIME now, uint8_t joy) {
  int old_x;
  int old_y;
  int dx = 0;
  int dy = 0;
  int moving;
  int frame_changed = 0;
  if ((int32_t)(now - _NextMove) < 0) return;
  if ((int32_t)(now - _SleepUntil) < 0) return;
  if (joy & JOY_LEFT) dx -= PET_STEP;
  if (joy & JOY_RIGHT) dx += PET_STEP;
  if (joy & JOY_UP) dy -= PET_STEP;
  if (joy & JOY_DOWN) dy += PET_STEP;
  old_x = _PetX;
  old_y = _PetY;
  if ((dx != 0) || (dy != 0)) {
    _PetX = _Clamp(_PetX + dx, PET_MIN_X, PET_MAX_X);
    _PetY = _Clamp(_PetY + dy, PET_MIN_Y, PET_MAX_Y);
  } else if (_TouchMoving) {
    _PetX = _Approach(_PetX, _PetTargetX);
    _PetY = _Approach(_PetY, _PetTargetY);
  }
  moving = (_PetX != old_x) || (_PetY != old_y);
  if (moving) {
    _WalkDivider++;
    if (_WalkDivider >= PET_WALK_FRAME_DIVIDER) {
      _WalkDivider = 0u;
      _WalkFrame = (_WalkFrame == 0u) ? 2u : 0u;
      frame_changed = 1;
    }
  } else if (_WalkFrame != 1u) {
    _WalkFrame = 1u;
    _WalkDivider = 0u;
    frame_changed = 1;
  }
  if (moving || frame_changed) {
    _RedrawPetMove(old_x, old_y);
    if (moving) _CheckCollectible(now);
  }
  _NextMove = now + PET_MOVE_MS;
}

static void _GameTick(GUI_TIMER_TIME now) {
  int changed = 0;
  int sleeping = ((int32_t)(now - _SleepUntil) < 0);
  while ((uint32_t)(now - _LastDecay) >= GAME_DECAY_MS) {
    _LastDecay += GAME_DECAY_MS;
    _AgeSeconds += GAME_DECAY_MS / 1000u;
    _Hunger = _AddStat(_Hunger, -2);
    _Happy = _AddStat(_Happy, -1);
    _Clean = _AddStat(_Clean, -1);
    if ((int32_t)(now - _SleepUntil) < 0) _Energy = _AddStat(_Energy, 8);
    else _Energy = _AddStat(_Energy, -1);
    changed = 1;
  }
  if (_NightMode) {
    if (sleeping) {
      if ((int32_t)(now - _NextSleepAnim) >= 0) {
        _SleepFrame = (uint8_t)((_SleepFrame + 1u) % 6u);
        _NextSleepAnim = now + PET_SLEEP_ANIM_MS;
        _RedrawPetMove(_PetX, _PetY);
      }
    } else {
      _WakeUp(now, "Good morning!");
      printf("[PET] sleep finished: daytime restored\n");
    }
  }
  if ((_ItemVisible == 0) && ((int32_t)(now - _NextItem) >= 0)) _SpawnItem(now);
  if (!_NightMode && (_MessageUntil != 0u) &&
      ((int32_t)(now - _MessageUntil) >= 0)) {
    _MessageUntil = 0u;
    _Message = "Touch or joystick to move";
    _DrawGameFooter();
  }
  if (changed) _DrawHUD(now);
}

void DEMOUI_Init(void) {
  _ShowHome();
  printf("[UI] page 1 end image shown; PET entry button enabled\n");
  printf("[UI] emWin heap free=%u bytes\n", (unsigned)GUI_ALLOC_GetNumFreeBytes());
  printf("[PET] 3-frame 64x64 walk cycle; 25 FPS clipped local redraw enabled\n");
}

void DEMOUI_Exec(GUI_TIMER_TIME now, int touch_x, int touch_y, int touch_pressed) {
  int key_action = KEYPORT_Exec();
  int released_button;
  int woke_now = 0;
  uint8_t joy = JOYPORT_GetState();
  uint8_t joy_pressed = (uint8_t)(joy & (uint8_t)~_LastJoy);

  if ((_Page == PAGE_GAME) && _NightMode &&
      ((key_action != 0) || (joy_pressed != 0u) ||
       (touch_pressed && !_TouchWasPressed))) {
    _WakeUp(now, "I'm awake!");
    printf("[PET] sleep interrupted by user: daytime restored\n");
    _WakeInputLatch = 1u;
    woke_now = 1;
  }
  if (_WakeInputLatch && (joy == 0u) && !touch_pressed) {
    _WakeInputLatch = 0u;
  }

  if (_Page == PAGE_HOME) {
    if (key_action != 0) _ActivateInfo(key_action, now);
    /* Any joystick contact enters the game.  If a direction is held, the pet
       starts moving on the following 5 ms loop, so the control feels direct. */
    if (joy_pressed != 0u) _ShowGame(now);
    if (touch_pressed && !_TouchWasPressed) {
      if ((_InfoPage == 0) &&
          (touch_x >= HOME_GIF_X0) && (touch_x <= HOME_GIF_X1) &&
          (touch_y >= HOME_GIF_Y0) && (touch_y <= HOME_GIF_Y1)) {
        _ShowGifLoop(now);
      } else if ((_InfoPage == 0) &&
          (touch_x >= HOME_PET_X0) && (touch_x <= HOME_PET_X1) &&
          (touch_y >= HOME_PET_Y0) && (touch_y <= HOME_PET_Y1)) {
        _ShowGame(now);
      } else {
        _TouchButton = _HitBottomButton(touch_x, touch_y);
        _PressedButton = _TouchButton;
        if (_TouchButton != 0) _ActivateInfo(_TouchButton, now);
      }
    } else if (!touch_pressed && _TouchWasPressed) {
      released_button = _TouchButton;
      _TouchButton = 0;
      _PressedButton = 0;
      if (released_button != 0) _RenderHome();
    }
    if ((_InfoPage != 0) && ((int32_t)(now - _InfoUntil) >= 0)) _ShowHome();
  } else if (_Page == PAGE_GIF) {
    if (touch_pressed && !_TouchWasPressed &&
        (touch_x >= GIF_EXIT_X0) && (touch_x <= GIF_EXIT_X1) &&
        (touch_y >= GIF_EXIT_Y0) && (touch_y <= GIF_EXIT_Y1)) {
      _ExitGifLoop();
    } else {
      _GifLoopTick(now);
    }
  } else {
    if (!woke_now && !_WakeInputLatch && (key_action != 0)) {
      _GameAction(key_action, now);
    }
    if (!woke_now && !_WakeInputLatch && (joy_pressed & JOY_CENTER)) {
      _GameAction(2, now);
    }
    if (!woke_now && !_WakeInputLatch && touch_pressed && !_TouchWasPressed) {
      if ((touch_x >= 267) && (touch_y <= 31)) {
        _ShowHome();
      } else {
        _TouchButton = _HitBottomButton(touch_x, touch_y);
        _PressedButton = _TouchButton;
        if (_TouchButton != 0) {
          _GameAction(_TouchButton, now);
        } else if ((touch_y >= FIELD_Y0) && (touch_y <= CONTENT_Y1)) {
          _PetTargetX = _Clamp(touch_x - PET_SPRITE_WIDTH / 2, PET_MIN_X, PET_MAX_X);
          _PetTargetY = _Clamp(touch_y - PET_SPRITE_HEIGHT / 2, PET_MIN_Y, PET_MAX_Y);
          _TouchMoving = 1;
        }
      }
    } else if (!woke_now && !_WakeInputLatch && touch_pressed &&
               (touch_y >= FIELD_Y0) && (touch_y <= CONTENT_Y1)) {
      _PetTargetX = _Clamp(touch_x - PET_SPRITE_WIDTH / 2, PET_MIN_X, PET_MAX_X);
      _PetTargetY = _Clamp(touch_y - PET_SPRITE_HEIGHT / 2, PET_MIN_Y, PET_MAX_Y);
      _TouchMoving = 1;
    } else if (!woke_now && !_WakeInputLatch && !touch_pressed && _TouchWasPressed) {
      _TouchMoving = 0;
      released_button = _TouchButton;
      _TouchButton = 0;
      _PressedButton = 0;
      if (released_button != 0) _RenderGame(now);
    }
    _MovePet(now, (woke_now || _WakeInputLatch) ? 0u : joy);
    _GameTick(now);
  }
  _TouchWasPressed = touch_pressed;
  _LastJoy = joy;
}
