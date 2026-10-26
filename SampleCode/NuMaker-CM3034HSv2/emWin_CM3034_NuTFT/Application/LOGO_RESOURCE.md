# NuFun 320x240 Logo resource

The home screen uses `Assets/Nuvoton_Logo_320x98.png` at its native 320x98
resolution. It is centered at LCD coordinate `(0, 42)` in the 320x182 content
area. Transparent and antialiased pixels are composited onto black, then stored
as RGB565.

To regenerate the C resource after replacing the PNG, run:

```powershell
python .\logo2rgb565.py .\Assets\Nuvoton_Logo_320x98.png .\Nuvoton_Logo_RGB565.inc
```

The replacement image must remain RGBA PNG, 320x98. The generated include is
compiled through `Demo_UI.c`; it does not need to be added separately to Keil.
