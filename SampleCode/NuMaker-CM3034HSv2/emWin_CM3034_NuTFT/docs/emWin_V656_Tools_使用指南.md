# emWin V6.56 Tools 使用指南

适用环境：SEGGER emWin V6.56、Windows 开发机、NuMaker-CM3034HSv2 + NuTFT Touch LCM EVB。  
本文中的工具来自本软件包的 `Tool` 目录。它们运行在 Windows 上，用来生成资源、设计界面、调试或远程查看；它们不是烧录到 CM3034 的固件。

## 1. 先看结论：当前移植最需要哪些工具

| 优先级 | 工具 | 当前 CM3034 工程中的建议 |
|---|---|---|
| 必用 | Bitmap Converter（BmpCvt） | 把图标、背景图转换成 emWin 可直接绘制的 C 位图或数据流 |
| 必用（需要中文/自定义字体时） | Font Converter | 只生成实际用到的字符，控制 Flash 占用 |
| 推荐 | AppWizard | 显示驱动、触摸和基本控件稳定后，再用于完整页面开发 |
| 备用 | GUIBuilder | 维护旧式对话框，或快速生成简单固定布局 |
| 自动化 | BmpCvtCL | 批量生成位图资源，适合脚本和持续集成 |
| 辅助 | Bin2C、U2C | 把二进制文件或 UTF-8 文本转换为 C 源码 |
| 调试 | emWinSPY | 查看运行时窗口树、控件、内存、输入和截图；板端要增加通信服务 |
| 联调 | emWinView | 在 PC 上显示/测量远端 emWin 图层并可转发键盘输入 |
| 网络功能 | emVNC | 通过 VNC 远程查看和操作设备；需板端网络协议栈和 VNC Server |
| 动画/视频 | MakeMovie、emWinPlayer | 转换并预览 emWin Movie；首版 CM3034 工程不建议优先加入 |
| 兼容旧流程 | JPEG2Movie | 已被 MakeMovie 取代，只用于旧脚本 |
| PC/Web 演示 | emWin4Web | 把 emWin/AppWizard 应用编译为 WebAssembly，在浏览器运行 |

推荐顺序：先用现有工程确认纯色、文字、触摸；再加入位图和自定义字体；最后考虑 AppWizard、动画、SPY/VNC。

## 2. Tool 目录清单与启动方式

| 文件 | 类型 | 作用 |
|---|---|---|
| `AppWizard_V164_656_Install.exe` | 安装器 | 安装 AppWizard 1.64（与 emWin 6.56 配套） |
| `BmpCvt_V656_Install.exe` | 安装器 | 安装图形版 Bitmap Converter |
| `BmpCvtCL_V656_Install.exe` | 安装器 | 安装命令行版 Bitmap Converter |
| `FontConverter_V656_Install.exe` | 安装器 | 安装 Font Converter |
| `MakeMovie_V656_Install.exe` | 安装器 | 安装 MakeMovie |
| `emWinSPY_V656_Install.exe` | 安装器 | 安装 emWinSPY |
| `emWinView_V656_Install.exe` | 安装器 | 安装 emWinView |
| `emVNC_V656_Install.exe` | 安装器 | 安装 emVNC 客户端 |
| `emWin4Web_V656_Install.exe` | 安装器 | 安装 emWin4Web |
| `GUIBuilder.exe` | 便携图形程序 | 旧式对话框设计器，直接运行 |
| `emWinPlayer.exe` | 便携图形程序 | emWin Movie（EMF）播放器，直接运行 |
| `Bin2C.exe` | 图形/命令行两用 | 任意二进制文件转 C 字节数组 |
| `U2C.exe` | 图形/命令行两用 | UTF-8 文本转 C 字符串文本 |
| `JPEG2Movie.exe` | 图形/命令行两用 | JPEG 序列转旧式 EMF Movie |

带 `_Install.exe` 的文件先安装。建议保持默认安装位置，并避免混用其他 emWin 版本的转换器。便携工具可以双击，也可以在 PowerShell/CMD 中传入文件名。

## 3. Bitmap Converter（BmpCvt）

### 3.1 作用

将 BMP、PNG、JPEG、GIF、SVG 等图像转换成 emWin 能高效绘制的资源。V6.56 支持 SVG 输入，也能生成带 include guard 的头文件。常见输出包括：

- C 源码中的 `GUI_BITMAP` 对象，适合直接编译进 Flash；
- emWin 位图数据流，运行时用 `GUI_DrawStreamedBitmap()` 绘制；
- BMP/PNG 等中间文件；
- 动态 GIF 可转换为位图型 emWin Movie。

它不是普通图片格式转换器：关键价值是选择与目标 LCD 相符的 emWin 像素格式、调色板、透明度和压缩方式。

### 3.2 图形版操作流程

1. 安装并启动 Bitmap Converter。
2. 打开原始图片，先确认宽高没有超过当前屏幕的 480 × 320（或界面实际需要的区域）。
3. 选择目标颜色格式。CM3034 + ILI9341 当前链路最终输出 RGB565，照片/渐变优先尝试 16 bpp；纯色图标可以尝试 4/8 bpp 调色板。
4. 预览颜色、透明边缘和抖动效果。
5. 选择导出为 C 文件，并给对象使用稳定名称，例如 `bmLogo`。
6. 把生成的 `.c`（以及存在时的 `.h`）放入工程的资源目录，在 Keil 中加入编译。

调用示例：

```c
#include "GUI.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmLogo;

void DrawLogo(void) {
  GUI_DrawBitmap(&bmLogo, 20, 20);
}
```

### 3.3 格式选择建议

- 照片/渐变背景：RGB565，显示效果稳定但容量较大。
- 小图标且颜色少：索引色 4 bpp 或 8 bpp，通常明显节省 Flash。
- 透明图标：保留 Alpha 时容量和混合运算都会增加；只有确实需要半透明时才保留完整 Alpha。
- 大背景：如果 Flash 紧张，优先放外部 SPI Flash/文件系统并使用流式解码；不要把多张全屏 RGB565 图全部编译进 MCU Flash。
- RLE：大块同色区域收益高；照片通常收益有限，应比较转换后的实际字节数。

480 × 320 的原始 RGB565 全屏图约为 `480 × 320 × 2 = 307200` 字节。多张全屏图很快会耗尽片内 Flash。

### 3.4 BmpCvtCL

BmpCvtCL 是同一转换功能的命令行版本，适合批量处理和构建脚本。其选项会随版本变化，因此安装后以该版本自带的 `--help`/帮助文档为准，不要直接复制网上旧版参数。

推荐用法是先用图形版完成一张图片并确定像素格式，再把同一套参数固化到脚本。构建系统应把原图视为源文件，把生成的 `.c/.h` 视为构建产物，避免手工转换造成资源版本不一致。

### 3.5 常见问题

- 显示颜色颠倒：检查图片输出格式、LCD 驱动颜色转换和 ILI9341 BGR/RGB 设置，不要只在图片工具里反转颜色。
- 编译后程序暴增：检查是否误用了 32 bpp/完整 Alpha，或把多份同一资源重复编译。
- 链接提示 `bmXxx` 未定义：确认生成的 `.c` 已加入当前 Keil Target，声明名与生成对象名完全一致。
- 边缘出现黑底：通常是 Alpha 未启用、预乘方式不一致，或导出成了不透明格式。

## 4. Font Converter

### 4.1 作用

把 Windows 字体（常见为 TTF/OTF 或已安装字体）转换成 emWin 字体。V6.56 可输出标准 C 字体、SIF、XBF，并支持多级抗锯齿（包括 8 bpp）和 kerning。三种方案的取舍如下：

| 输出 | 特点 | 适用场景 |
|---|---|---|
| C 字体 | 直接编译链接，使用最简单 | 少量固定字符、菜单英文/数字 |
| SIF | 系统无关二进制字体块，可从存储器加载 | 中等规模字符集、资源与程序分离 |
| XBF | 字形按需访问，适合大字符集 | 中文等大字库、外部 Flash/文件系统 |

### 4.2 中文字体推荐流程

1. 先整理产品实际显示的汉字、数字、标点和单位，去重后形成字符清单。
2. 在 Font Converter 中选字体和字号，再导入/启用该字符集合。
3. 首次建议使用 2 bpp 或 4 bpp 抗锯齿；8 bpp 更平滑，但容量和混合开销更大。
4. 只含几十到几百个汉字时，可先导出 C 字体；字符很多时改用 XBF/SIF 并放入外部存储。
5. 将生成的源文件加入 Keil，确认字体对象名称。

C 字体调用示例：

```c
#include "GUI.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontChinese24;

void DrawTitle(void) {
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontChinese24);
  GUI_DispStringAt("系统设置", 12, 12);
}
```

注意：源代码、编译器输入编码与 emWin 字符串编码必须一致。若工程采用 UTF-8，需确认当前 emWin 配置及字符串 API 支持 UTF-8；否则即使字库有汉字，也可能显示乱码或空白。

### 4.3 字库容量估算

字库容量近似与“字符数 × 字号面积 × 每像素位数”成正比。不要一开始导出完整 CJK Unicode 范围。建议先用产品文案生成子集，并在链接 map 文件中核对字体对象实际占用。

## 5. AppWizard

### 5.1 作用

AppWizard 是现代化可视化界面设计工具，用于创建页面、控件、交互、变量、动画和资源，并生成可嵌入 emWin 工程的应用代码。它适合完整产品 UI；Bitmap Converter 和 Font Converter解决的是单个资源，而 AppWizard 解决的是界面结构和交互。

### 5.2 建议工作流

1. 安装与 emWin V6.56 配套的 AppWizard 1.64。
2. 新建项目并设置显示分辨率、方向、颜色格式和资源路径。
3. 创建 Screen，放置 Button、Text、Image、Slider 等控件。
4. 为控件配置 ID、事件、变量绑定和页面跳转。
5. 在 PC 上预览交互。
6. 生成代码，将生成目录整体放入板端工程；不要只复制某一个 `.c` 文件。
7. 将 AppWizard 运行时模块、生成源文件和资源加入 Keil，在系统初始化完成后启动生成的应用入口。

### 5.3 与当前 CM3034 工程的关系

当前工程已经解决 LCD/SPI/触摸和基础 emWin 显示，但为了缩短第一次移植的构建时间，工程生成脚本未默认编译全部 AppWizard 模块。因此，直接把 AppWizard 生成代码加入后可能出现未定义符号。正确做法是：

- 保留现有 `LCDConf.c`、`GUI_X.c`、触摸和底层驱动；
- 加入 AppWizard 生成的应用代码及它引用的 emWin AppWizard 源码目录；
- 根据链接错误补齐模块，而不是替换已经验证通过的 LCD 驱动；
- 先做一个仅含一个 Screen 和一个 Button 的最小工程，再逐步加入字体、图片和动画。

## 6. GUIBuilder

GUIBuilder 是 AppWizard 之前的轻量级对话框设计器。它通常生成 `GUI_WIDGET_CREATE_INFO` 数组、对话框回调和控件 ID，适合固定布局或维护旧项目。

基本流程：

1. 双击 `GUIBuilder.exe`。
2. 新建对话框，设置窗口尺寸为目标区域。
3. 拖入控件，设置位置、尺寸、文本和 ID。
4. 保存/导出 C 文件。
5. 将文件加入工程并调用生成的创建函数。
6. 在回调函数中处理 `WM_INIT_DIALOG`、`WM_NOTIFY_PARENT` 等消息。

典型事件框架：

```c
static void _cbDialog(WM_MESSAGE * pMsg) {
  int Id;
  int NCode;

  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    if ((Id == GUI_ID_BUTTON0) && (NCode == WM_NOTIFICATION_RELEASED)) {
      /* 处理按钮释放事件 */
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}
```

新项目优先使用 AppWizard；只有在界面很简单、希望完全手写消息逻辑，或维护旧式 emWin 对话框时使用 GUIBuilder。

## 7. Bin2C

### 7.1 作用和已核对语法

把任意文件逐字节转换为 C 数组，适合把 SIF、XBF、图片流、Movie、小型配置文件等嵌入 Flash。

```text
Bin2C <FILENAME.EXT>
Bin2C -global <FILENAME.EXT>
```

不带参数运行会打开图形界面。命令行会在源文件旁生成同名 `.c`。默认数组是 `static const unsigned char`；`-global` 用于需要从其他编译单元访问的全局对象。生成数组会在文件原始数据后额外添加一个 `0x00`，因此二进制数据的真实长度应单独保存或使用原始文件长度，不要用含终止字节的数组大小替代协议长度。

推荐步骤：

1. 在 PowerShell 中进入资源文件目录。
2. 执行 `& "<Tool目录>\Bin2C.exe" "logo.bin"`。
3. 重命名生成对象为清晰名称（若工具未提供命名选项），并把 `.c` 加入 Keil。
4. 若对象很大，确认链接到了 Flash 只读段，而不是被复制到 RAM。

## 8. U2C

U2C 将 UTF-8 文本文件转换为可放入 C 源码的字符串文本。已核对命令格式为：

```text
U2C <FILENAME.EXT>
```

输入文件需要被工具识别为 UTF-8；此版本对 UTF-8 BOM（文件头 `EF BB BF`）有要求，缺少 BOM 时会报告 `Source file is not a UTF8 file`。输出为同名 `.c`。它适合批量把多语言文案变成 C 字符串，但不会替你生成对应字形；中文仍需 Font Converter 字库和正确的 emWin 字符编码配置。

## 9. MakeMovie、JPEG2Movie 与 emWinPlayer

### 9.1 MakeMovie

MakeMovie 把视频/动画转换为 emWin Movie 文件。它取代了旧的 JPEG2Movie，优先使用它。基本流程是选择输入视频、设置目标尺寸/帧率/质量、预览容量，再导出 EMF。Movie 播放会持续读取和解码帧，对存储带宽、SPI 刷屏速度和 GUI 内存要求都较高。

对于当前 SPI 接口 ST7796：先把视频缩放到实际显示区域、降低帧率和 JPEG 质量，并实测掉帧。第一版产品界面更适合用少量位图帧、控件动画或局部刷新。

### 9.2 JPEG2Movie

这是旧工具，用一个目录内的 JPEG 序列生成 `JPEG2Movie.emf`。已核对命令格式：

```text
JPEG2Movie <DIRECTORY> <ms>
```

`<ms>` 是帧间隔毫秒数，例如 `40` 约等于 25 帧/秒。仅在复现旧脚本时使用；新流程使用 MakeMovie。

### 9.3 emWinPlayer

PC 端 EMF 播放器，用来在烧录前检查 Movie 文件的尺寸、帧顺序和播放效果。它只能证明资源文件可播放，不能证明 CM3034 的 Flash、文件系统、解码速度和 LCD SPI 带宽足够。

## 10. emWinSPY

emWinSPY 是运行时调试器，可查看：

- emWin 内存使用情况；
- 当前窗口树和控件类型、位置、尺寸、标志等；
- 触摸和键盘输入日志；
- 运行时截图。

PC 工具不能直接从普通 UART 日志推断窗口树。板端必须加入 emWinSPY 服务，并提供可靠的收发通道。源码包中可参考 `Sample/GUI_X/GUI_SPY_X_StartServer.c` 和 Windows 模拟版本；TCP 示例默认使用 2468 端口，另外也可使用 J-Link RTT。核心处理函数是：

```c
GUI_SPY_Process(pfSend, pfRecv, pConnectionInfo);
```

集成建议：

1. 先保证普通 emWin 主循环稳定。
2. 选择 RTT 或 TCP；CM3034 当前工程没有现成网络栈时，优先评估 RTT。
3. 把 SPY 处理放在独立任务/服务循环中，避免阻塞 GUI 线程。
4. 确认 `GUI_SUPPORT_SPY`、相关源码和内存配置。
5. PC 端启动 emWinSPY并连接对应通道。

SPY 会增加代码和 RAM 使用量，建议作为 Debug 配置，Release 产品固件中关闭。

## 11. emWinView

emWinView 用来在 PC 上查看远端 emWin 图层，支持图层/透明效果、区域选择与像素测量、复制图像，并可把键盘输入发送给 emWin 应用。它更偏向“远端画面查看与测量”，而 emWinSPY 更偏向“窗口树、内存和输入事件诊断”。

使用时需要板端/模拟器提供与 emWinView 匹配的连接支持。因为具体连接项取决于安装版本和目标通信实现，按安装后的 Help 建立服务和连接；不要把 emWinView 当成无需板端改动的串口截图工具。

## 12. emVNC

emVNC 是 VNC 客户端。配合 emWin VNC Server，可以从 PC 远程看屏并发送鼠标/键盘/触摸操作。适用于带以太网/Wi-Fi 的设备远程操作或售后诊断。

CM3034 侧通常需要：

- 可用的 TCP/IP 协议栈和网络接口；
- emWin VNC Server 模块；
- VNC 的发送/接收适配；
- 足够的任务栈、GUI 内存和网络缓冲；
- 产品端口和访问控制策略。

当前 LCD 通过 SPI 刷屏并不妨碍 VNC，但网络传输会增加 CPU 和内存负担。没有网络需求时无需加入。

## 13. emWin4Web

emWin4Web 是 PC 命令行工具，可把 emWin/AppWizard 应用编译成 WebAssembly并在浏览器中运行，也可生成独立 HTML。它适合客户评审、网页演示和无需开发板的 UI 验证，不参与 CM3034 固件编译与烧录。

它要求应用代码与 Web 构建环境兼容。直接访问 MCU 寄存器、SPI、文件系统或 RTOS 的部分需要为 Web 环境提供替代实现。正确的组织方式是把页面与业务逻辑和硬件驱动分层，使同一 UI 代码可以连接“浏览器后端”或“CM3034 后端”。具体命令参数以安装后该 V6.56 版本的帮助为准。

## 14. 三组容易混淆的工具

### 14.1 BmpCvt、Bin2C、U2C

- BmpCvt 理解图片和 emWin 像素格式，图片资源优先用它。
- Bin2C 不理解文件内容，只把每个字节变成数组。
- U2C 针对 UTF-8 文本，生成 C 字符串，不生成字体。

### 14.2 AppWizard、GUIBuilder

- AppWizard 面向完整 UI、页面关系、交互和动画，是新项目首选。
- GUIBuilder 面向传统 `WM_DIALOG` 对话框，适合简单界面和旧代码。

### 14.3 emWinView、emWinSPY、emVNC

- emWinView：看图层、测量画面、转发键盘。
- emWinSPY：诊断内存、窗口树、控件和输入。
- emVNC：通过网络远程操作产品界面。

## 15. CM3034 项目的推荐资源目录

建议在应用层采用类似结构：

```text
Application/
  UI/
    Screens/       页面和回调
    Resources/
      Bitmap/      BmpCvt 生成文件
      Font/        Font Converter 生成文件
      Movie/       EMF 或 Movie 访问封装
    UI_Main.c
```

生成资源与手写业务代码分开。资源对象统一使用 `bm...`、`GUI_Font...` 等前缀；原始 PNG/TTF/视频不要加入 Keil 编译，但应进入版本管理并记录转换参数。

## 16. 建议的下一步实操

1. 用 Bitmap Converter 转换一个 64 × 64 PNG，导出 RGB565 C 位图，并在当前主页调用 `GUI_DrawBitmap()`。
2. 用 Font Converter 只生成“中文测试 0123456789”字符，导出 24 像素字体并显示。
3. 查看 Keil map 文件，记录位图、字体和 emWin GUI 内存增量。
4. 两项都稳定后，再建立一个最小 AppWizard 页面接入当前驱动。

如果出现编译或显示问题，请保留：转换器的输出格式截图、生成文件开头的对象定义、Keil 第一条错误、map 文件中的资源大小以及串口启动日志。根据这五项通常可以快速区分“资源生成错误、链接配置错误、编码错误”还是“LCD 驱动错误”。

## 17. 本地参考位置

- 工具目录：`emWin_V656_GUI-00735_8508D313_260223/Tool`
- emWin 主手册：`Doc/emWin_UserGuideAndReferenceManual.html`
- 版本说明：`Doc/Release.html`
- emWinSPY 传输示例：`Sample/GUI_X/GUI_SPY_X_StartServer.c`
- emWinSPY 核心接口：`Start/GUI/Core/GUI_SPY.c`
- 位图绘制接口：`Start/GUI/Core/GUI_DrawBitmap.c`、`Start/GUI/Core/GUI_Stream.c`
- SIF 字体接口：`Start/GUI/Core/GUI_SIF.c`

本文依据当前软件包中的 V6.56 工具文件、Release 说明、头文件和示例源码整理。带安装器工具的菜单名称可能因 Windows 语言和安装补丁略有差异，具体按钮文字以安装后的 Help 为准。
