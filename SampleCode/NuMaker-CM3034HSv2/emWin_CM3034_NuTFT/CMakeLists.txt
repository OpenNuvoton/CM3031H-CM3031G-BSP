set(EMWIN_DEMO_DIR ${CMAKE_CURRENT_LIST_DIR})
set(EMWIN_GUI_DIR  ${EMWIN_DEMO_DIR}/ThirdParty/emWin_V656/GUI)

add_library(emwin_cm3034_ac6 STATIC IMPORTED)
set_target_properties(emwin_cm3034_ac6 PROPERTIES
    IMPORTED_LOCATION ${EMWIN_DEMO_DIR}/Lib/GUI_CM3034_AC6.lib
)
target_include_directories(emwin_cm3034_ac6 INTERFACE
    ${EMWIN_DEMO_DIR}/Config
    ${EMWIN_GUI_DIR}/Core
    ${EMWIN_GUI_DIR}/DisplayDriver
    ${EMWIN_GUI_DIR}/Widget
    ${EMWIN_GUI_DIR}/WM
    ${EMWIN_GUI_DIR}/MemDev
)

create_project(emWin_CM3034_NuTFT
    ${EMWIN_DEMO_DIR}/main.c
    ${EMWIN_DEMO_DIR}/Application/Demo_UI.c
    ${EMWIN_DEMO_DIR}/Application/Boot_Screen.c
    ${EMWIN_DEMO_DIR}/Board/LCD_Port.c
    ${EMWIN_DEMO_DIR}/Board/Touch_Port.c
    ${EMWIN_DEMO_DIR}/Board/Keys_Port.c
    ${EMWIN_DEMO_DIR}/Board/Joystick_Port.c
    ${EMWIN_DEMO_DIR}/Board/ARGB_Port.c
    ${EMWIN_DEMO_DIR}/Config/GUIConf.c
    ${EMWIN_DEMO_DIR}/Config/GUI_X.c
    ${EMWIN_DEMO_DIR}/Config/LCDConf.c
    ${BSP_DIR}/Library/StdDriver/src/clk.c
    ${BSP_DIR}/Library/StdDriver/src/eadc.c
    ${BSP_DIR}/Library/StdDriver/src/fmc.c
    ${BSP_DIR}/Library/StdDriver/src/gpio.c
    ${BSP_DIR}/Library/StdDriver/src/i2c.c
    ${BSP_DIR}/Library/StdDriver/src/llsi.c
    ${BSP_DIR}/Library/StdDriver/src/retarget.c
    ${BSP_DIR}/Library/StdDriver/src/spi.c
    ${BSP_DIR}/Library/StdDriver/src/sys.c
    ${BSP_DIR}/Library/StdDriver/src/timer.c
    ${BSP_DIR}/Library/StdDriver/src/uart.c
)
target_link_libraries(emWin_CM3034_NuTFT emwin_cm3034_ac6)
target_include_directories(emWin_CM3034_NuTFT PRIVATE
    ${EMWIN_DEMO_DIR}
    ${EMWIN_DEMO_DIR}/Application
    ${EMWIN_DEMO_DIR}/Board
    ${EMWIN_DEMO_DIR}/Config
)
