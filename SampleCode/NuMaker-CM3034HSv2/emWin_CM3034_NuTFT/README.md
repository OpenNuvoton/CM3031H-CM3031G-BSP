# emWin_CM3034_NuTFT

CM3034HS8AE / NuMaker-CM3034HSv2 NuTFT emWin adaptation example.

## Distribution

This example links the prebuilt emWin V6.56 library
`Lib/GUI_CM3034_AC6.lib`. `ThirdParty/emWin_V656/GUI/` contains headers
only; proprietary emWin implementation sources and library rebuild scripts
are not distributed. Keep the original vendor copyright and license notices.
The vendor library and headers remain subject to their applicable license;
they are not relicensed by this example.

`Config/`, `Board/` and `Application/` contain the configuration, board
adaptation and example sources required to build the application.

## Build

- Keil: open `Keil/emWin_CM3034_NuTFT.uvprojx`, install the referenced
  `Nuvoton.NuMicroCM33_DFP.1.0.2` device pack and select Arm Compiler 6.
  The existing project records compiler V6.24. Rebuild the application.
- VSCode / CMSIS-Toolbox: open
  `VSCode/emWin_CM3034_NuTFT.csolution.yml` with an AC6 toolchain and the
  Nuvoton NuMicroCM33 device pack installed.
- `CMakeLists.txt` integrates with the BSP's parent build environment;
  it requires the parent `create_project` function and `BSP_DIR`.

The prebuilt library was built for Cortex-M33, Thumb, FPv5-SP-D16,
hard-float ABI, short enums and short wchar. Match these settings in the
application. Do not attempt to regenerate the library from this public
example. Changes to library-level emWin configuration may require a matching
library supplied by an authorized maintainer.

The board display port uses SPI1 and the current UI uses 320 x 240 assets.
See `Board/` for wiring and `docs/` for usage material. Verify the actual
board and display connections before running.

## Validation status

The distribution cleanup has been checked statically. Compilation and
on-board testing must be performed with the installed device pack and AC6
toolchain before submitting the pull request.
