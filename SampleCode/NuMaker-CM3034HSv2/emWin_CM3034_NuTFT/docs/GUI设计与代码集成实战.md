# CM3034 + emWin：GUI 设计与业务代码集成实战

适用工程：`emWin_CM3034_NuTFT`  
显示：ST7796，480 × 320 横屏，RGB565  
输入：NuTFT 电容触摸

## 1. 工程应分成三层

1. **板级驱动层**：`LCD_Port.c`、`Touch_Port.c`、SPI、I2C、定时器。
2. **emWin 配置层**：`LCDConf.c`、`GUI_X.c`、`GUIConf.h`。
3. **应用界面层**：页面、控件、图片、字体、事件和业务状态。

现在前两层已经跑通。以后使用 GUIBuilder 或 AppWizard 时，只增加或替换第三层，
不要让工具生成的 LCD、触摸或时间基准代码覆盖当前板级移植。

## 2. 工具如何选择

| 目标 | 工具 |
|---|---|
| 学习窗口、按钮和回调代码 | GUIBuilder |
| 设计完整多页面产品界面 | AppWizard |
| 把 PNG/JPG/图标变成 emWin 位图 | Bitmap Converter |
| 生成中文或品牌字体 | Font Converter |
| 查看窗口树、内存和消息 | emWinSPY |

推荐学习顺序：GUIBuilder → Bitmap Converter → Font Converter → AppWizard。

## 3. 第一个 GUIBuilder 界面

运行源码包 `Tool/GUIBuilder.exe`，创建一个 480 × 320 的对话框。建议第一个界面放：

- 一个 `WINDOW` 作为全屏根窗口，坐标 `(0, 0)`，尺寸 `480 × 320`；
- 一个 `TEXT`，ID 为 `GUI_ID_TEXT0`；
- 一个 `BUTTON`，ID 为 `GUI_ID_BUTTON0`，文字为 `Start`；
- 一个 `SLIDER`，ID 为 `GUI_ID_SLIDER0`。

根控件优先用 `WINDOW`。如果使用 `FRAMEWIN`，标题栏和边框会占用客户区，看起来会像
界面尺寸不足。

保存生成文件到工程的 `UI` 目录，例如：

```text
UI/
  UI_Main.c
  UI_Main.h
```

在 Keil 中新建 `UI` Group，把两个文件加入工程，并将 `..\UI` 加入 Include Paths。

## 4. GUIBuilder 生成代码的结构

生成代码的核心是控件表和窗口回调：

```c
#include "DIALOG.h"

#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_0  (GUI_ID_USER + 0x02)
#define ID_SLIDER_0  (GUI_ID_USER + 0x03)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Main",  ID_WINDOW_0, 0,   0, 480, 320, 0, 0, 0 },
  { TEXT_CreateIndirect,   "Ready", ID_TEXT_0,   20, 20, 220, 30, 0, 0, 0 },
  { BUTTON_CreateIndirect, "Start", ID_BUTTON_0, 20, 70, 120, 45, 0, 0, 0 },
  { SLIDER_CreateIndirect, 0,       ID_SLIDER_0, 20, 140, 300, 35, 0, 0, 0 },
};
```

控件 ID 是代码与设计界面的连接点。业务代码不要依赖控件坐标，应通过 ID 找控件。

## 5. 在回调中连接业务代码

```c
static int _RunEnabled;

static void _cbDialog(WM_MESSAGE *pMsg) {
  WM_HWIN hItem;
  int id;
  int notification;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "System ready");
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
    SLIDER_SetRange(hItem, 0, 100);
    SLIDER_SetValue(hItem, 50);
    break;

  case WM_NOTIFY_PARENT:
    id = WM_GetId(pMsg->hWinSrc);
    notification = pMsg->Data.v;

    if ((id == ID_BUTTON_0) &&
        (notification == WM_NOTIFICATION_RELEASED)) {
      _RunEnabled = !_RunEnabled;
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetText(hItem, _RunEnabled ? "Running" : "Stopped");
    }

    if ((id == ID_SLIDER_0) &&
        (notification == WM_NOTIFICATION_VALUE_CHANGED)) {
      int value = SLIDER_GetValue(pMsg->hWinSrc);
      /* 把 value 交给业务模块；不要在 GUI 回调中长时间等待硬件。 */
      (void)value;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

WM_HWIN UI_Main_Create(void) {
  return GUI_CreateDialogBox(_aDialogCreate,
                             GUI_COUNTOF(_aDialogCreate),
                             _cbDialog,
                             WM_HBKWIN,
                             0,
                             0);
}
```

常用通知：

- `WM_NOTIFICATION_CLICKED`：手指按下；
- `WM_NOTIFICATION_RELEASED`：手指释放，按钮动作通常在这里执行；
- `WM_NOTIFICATION_VALUE_CHANGED`：Slider、Edit 等数值发生变化；
- `WM_NOTIFICATION_SEL_CHANGED`：列表选择改变。

## 6. 当前 main.c 如何接入 GUI 页面

保留系统、LCD、定时器和触摸初始化，把当前红绿蓝测试画面与小球测试替换为页面创建：

```c
#include "UI_Main.h"

int main(void) {
  int touch_x;
  int touch_y;

  /* 保留当前工程已有的 SYS、UART 和 TIMER0 初始化。 */
  GUI_Init();
  LCDPORT_SetLandscape();
  TOUCHPORT_Init();

  UI_Main_Create();

  while (1) {
    TOUCHPORT_Exec(&touch_x, &touch_y);
    GUI_Exec();
    GUI_Delay(10);
  }
}
```

`TOUCHPORT_Exec()` 已把触摸状态送给 `GUI_PID_StoreState()`，因此 Button、Slider 等控件
会自动收到触摸消息。切换到正式 GUI 后，不再调用测试用的 `DrawTouchBall()`。

## 7. 正确组织业务逻辑

GUI 回调应快速返回。不要在按钮事件里执行长延时、阻塞串口接收或大段 Flash 擦写。
推荐结构：

```c
typedef enum {
  APP_CMD_NONE,
  APP_CMD_START,
  APP_CMD_STOP
} APP_COMMAND;

static volatile APP_COMMAND _PendingCommand;
```

按钮回调只设置 `_PendingCommand`；主循环或独立任务完成硬件操作，再调用界面更新函数：

```c
void UI_Main_SetTemperature(int value) {
  char text[24];
  WM_HWIN hItem;

  GUI_snprintf(text, sizeof(text), "%d C", value);
  hItem = WM_GetDialogItem(_hMain, ID_TEXT_0);
  TEXT_SetText(hItem, text);
}
```

不要从中断服务函数直接调用 emWin 绘图 API。中断只更新变量或置标志，GUI 更新放在主循环。

## 8. 图片与图标

用 Bitmap Converter 打开 PNG，按用途选择格式：

- 照片和渐变：RGB565；
- 少量颜色的图标：4 bpp 或 8 bpp 索引色；
- 需要透明边缘的图标：选择支持 Alpha 的输出格式。

导出为 C 文件，例如生成 `bmLogo`，加入 Keil 后使用：

```c
extern GUI_CONST_STORAGE GUI_BITMAP bmLogo;
GUI_DrawBitmap(&bmLogo, 20, 20);
```

480 × 320 的无压缩 RGB565 全屏图约占 307200 字节，因此不要大量使用全屏 C 位图。

## 9. 中文字体

用 Font Converter 只选择产品实际需要的汉字、数字和符号。少量字符可导出 C 字体：

```c
extern GUI_CONST_STORAGE GUI_FONT GUI_FontChinese24;

GUI_SetTextMode(GUI_TM_TRANS);
GUI_SetFont(&GUI_FontChinese24);
GUI_DispStringAt("设备状态", 20, 20);
```

完整中文字库应使用 XBF/SIF 并放入外部 Flash；否则会很快耗尽 MCU Flash。

## 10. 多页面设计

每个页面提供统一接口：

```c
WM_HWIN UI_Home_Create(void);
WM_HWIN UI_Settings_Create(void);
void UI_Home_Delete(void);
```

简单产品可以删除旧页面后创建新页面：

```c
WM_DeleteWindow(_hCurrentPage);
_hCurrentPage = UI_Settings_Create();
```

频繁切换且内存允许时，也可以预先创建多个页面，用 `WM_ShowWindow()` 和
`WM_HideWindow()` 切换。不要反复创建而忘记删除，否则 GUI 内存会逐渐耗尽。

## 11. AppWizard 的使用时机

当已经理解 `WM_INIT_DIALOG`、`WM_NOTIFY_PARENT`、控件 ID 和页面生命周期后，再安装
`AppWizard_V164_656_Install.exe`：

1. 新建项目，Display 设置为 480 × 320、横屏；
2. 先只创建一个 Screen、一个 Text 和一个 Button；
3. 在 PC 中运行模拟，确认布局与事件；
4. 使用 AppWizard 的 Generate/Export 生成完整代码目录；
5. 保留当前 `LCDConf.c`、`GUI_X.c`、`LCD_Port.c`、`Touch_Port.c`；
6. 将生成文件、资源和它依赖的 AppWizard runtime 加入 Keil；
7. 调用生成工程提供的入口，不要凭名称猜测入口函数；
8. 主循环继续执行触摸采集、`GUI_Exec()`，生成项目要求时再调用 `APPW_Exec()`。

当前构建脚本只编译了基础 emWin 模块，没有默认加入 `GUI/AppWizard`。接入 AppWizard
时必须把该目录所需源码加入 emWin 库构建列表，否则会出现 `APPW_*` 未定义链接错误。

## 12. 第一个练习的验收标准

1. 页面四边覆盖完整 480 × 320；
2. Button 按下和释放皮肤状态正确；
3. 点击 Button 后 Text 在 Running/Stopped 之间切换；
4. Slider 可从 0 拖到 100；
5. 连续切换页面 100 次，`GUI_ALLOC_GetNumFreeBytes()` 不持续下降；
6. GUI 回调中没有阻塞延时；
7. 触摸坐标与控件位置一致。

