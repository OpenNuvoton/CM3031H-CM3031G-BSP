# emWin_CM3034_NuTFT

CM3034HS8AE / NuMaker-CM3034HSv2 NuTFT emWin adaptation example.

## Distribution and dependencies

This example distributes the prebuilt emWin V6.56 library
`Lib/GUI_CM3034_AC6.lib` and the application / board adaptation sources.
No third-party emWin headers or implementation sources are included in this
example. The vendor library remains subject to its applicable license and
is not relicensed by this example.

Compilation still requires the matching emWin V6.56 headers, supplied
separately by an authorized maintainer. Do not place them in this repository.
Keep the vendor copyright and license notices in the external header package.
The BSP's `ThirdParty/emWin_light` is V6.46 and does not include the FlexColor
header used here; it is not a drop-in substitute for this library's headers.

Before building, arrange the external headers as a sibling of the BSP checkout:

```text
parent/
  CM3031H-CM3031G-BSP/
  emWin_V656_Headers/
    GUI/
      Core/GUI.h
      DisplayDriver/GUIDRV_FlexColor.h
      Widget/...
      WM/...
      ... (remaining matching headers)
```

Preserve the original GUI header directory structure and all required
transitive includes. A `.lib` alone is insufficient for compiling C sources
that call emWin APIs.

## Build

- Keil: open `Keil/emWin_CM3034_NuTFT.uvprojx`, install the referenced
  `Nuvoton.NuMicroCM33_DFP.1.0.2` device pack and select Arm Compiler 6.
  The project records compiler V6.24. Rebuild the application.
- VSCode / CMSIS-Toolbox: open
  `VSCode/emWin_CM3034_NuTFT.csolution.yml` with an AC6 toolchain and the
  Nuvoton NuMicroCM33 device pack installed.
- Both projects use relative paths to the external header package above.
  If installing it elsewhere, update their header search paths accordingly.
- `CMakeLists.txt` integrates with the BSP's parent build environment and
  requires its `create_project` function and `BSP_DIR`. Override the cached
  `EMWIN_GUI_DIR` path if your external GUI headers are installed elsewhere.

The prebuilt library was built for Cortex-M33, Thumb, FPv5-SP-D16,
hard-float ABI, short enums and short wchar. Match these settings in the
application. Library-level emWin configuration changes may require a matching
library supplied by an authorized maintainer. Library rebuild scripts and
intermediate objects are not distributed.

`Config/`, `Board/` and `Application/` contain the configuration, board
adaptation and example sources required to build the application. See
`Board/` for wiring and `docs/` for usage material. Verify the actual board
and display connections before running.

## Validation status

The prior configuration with in-tree headers was built, programmed and tested
on hardware by the contributor. The change to external header search paths
has been checked statically; repeat a clean build and hardware verification
with the external header package before treating this revision as validated.
