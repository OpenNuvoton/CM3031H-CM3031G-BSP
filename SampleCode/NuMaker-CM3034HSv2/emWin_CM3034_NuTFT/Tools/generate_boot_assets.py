#!/usr/bin/env python3
"""Generate compact emWin GIF resources for the 320x240 boot demo."""

from pathlib import Path
import argparse

import cv2
from PIL import Image, ImageChops, ImageDraw, ImageEnhance, ImageFilter, ImageFont


START_LOGICAL_SIZE = (320, 240)
END_LOGICAL_SIZE = (320, 240)
VIDEO_SIZE = (320, 180)
END_SIZE = (240, 80)
HOME_CONTENT_BOTTOM = 181
FPS = 8
TARGET_FRAMES = 40
COLORS = 256
UI_SCALE = 4

UI_BG = "#F5F2EE"
UI_WHITE = "#FFFFFF"
UI_RED = "#CC2924"
UI_RED_DARK = "#A91D19"
UI_TEXT = "#282322"
UI_MUTED = "#756D6A"
UI_BORDER = "#DED6D2"


def _canvas(image: Image.Image, logical_size: tuple[int, int],
            size: tuple[int, int]) -> Image.Image:
    result = Image.new("RGB", logical_size, "white")
    copy = image.copy()
    copy.thumbnail(size, Image.Resampling.LANCZOS)
    result.paste(copy, ((logical_size[0] - copy.width) // 2,
                        (logical_size[1] - copy.height) // 2))
    return result


def _video_frames(path: Path) -> list[Image.Image]:
    capture = cv2.VideoCapture(str(path))
    if not capture.isOpened():
        raise SystemExit(f"cannot open video: {path}")

    source_fps = capture.get(cv2.CAP_PROP_FPS)
    source_frames = capture.get(cv2.CAP_PROP_FRAME_COUNT)
    duration_ms = source_frames * 1000.0 / source_fps
    frames: list[Image.Image] = []
    for index in range(TARGET_FRAMES):
        # Sample the whole source rather than truncating it to five seconds.
        # Some MP4 decoders report a final non-decodable container frame, so
        # stop at frame_count - 2 while still covering 99.4% of the timeline.
        source_index = round(index * (source_frames - 2) /
                             (TARGET_FRAMES - 1))
        capture.set(cv2.CAP_PROP_POS_FRAMES, source_index)
        ok, frame = capture.read()
        if not ok:
            break
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image = _canvas(Image.fromarray(frame), START_LOGICAL_SIZE, VIDEO_SIZE)
        # The LCD shows this image at its native 320x240 resolution. A small
        # amount of contrast and edge recovery counters the softness introduced
        # by video scaling and GIF palette conversion without adding halos.
        image = ImageEnhance.Contrast(image).enhance(1.04)
        image = ImageEnhance.Color(image).enhance(1.03)
        image = image.filter(ImageFilter.UnsharpMask(radius=0.75,
                                                     percent=145,
                                                     threshold=2))
        frames.append(image)
    capture.release()

    if not frames:
        raise SystemExit(f"no frames decoded from: {path}")
    print(f"decoded {len(frames)} frames across the complete "
          f"{duration_ms / 1000.0:.3f}s source ({source_fps:.2f} fps)")
    return frames


def _global_palette(frames: list[Image.Image]) -> Image.Image:
    # A larger palette sample retains more of the fine gradients and small
    # details that are visible on the physical 320x240 panel.
    samples = [frame.resize((160, 120), Image.Resampling.LANCZOS) for frame in frames]
    sheet = Image.new("RGB", (160 * 8, 120 * ((len(samples) + 7) // 8)), "white")
    for index, sample in enumerate(samples):
        sheet.paste(sample, ((index % 8) * 160, (index // 8) * 120))
    return sheet.quantize(colors=COLORS, method=Image.Quantize.MEDIANCUT)


def _save_start_gif(frames: list[Image.Image], output: Path) -> None:
    palette = _global_palette(frames)
    converted = [frame.quantize(palette=palette, dither=Image.Dither.NONE) for frame in frames]
    converted[0].save(
        output,
        save_all=True,
        append_images=converted[1:],
        duration=150,
        loop=0,
        optimize=True,
        disposal=1,
    )


def _font(size: int, bold: bool = False) -> ImageFont.FreeTypeFont:
    filename = "segoeuib.ttf" if bold else "segoeui.ttf"
    path = Path("C:/Windows/Fonts") / filename
    return ImageFont.truetype(str(path), size * UI_SCALE)


def _label(draw: ImageDraw.ImageDraw, xy: tuple[int, int], text: str,
           size: int, color: str, bold: bool = False,
           anchor: str = "mm") -> None:
    draw.text((xy[0] * UI_SCALE, xy[1] * UI_SCALE), text,
              font=_font(size, bold), fill=color, anchor=anchor)


def _rounded_label(draw: ImageDraw.ImageDraw, xy: tuple[int, int],
                   text: str, size: int, color: str) -> None:
    font = ImageFont.truetype("C:/Windows/Fonts/ARLRDBD.TTF",
                              size * UI_SCALE)
    tracking = max(1, UI_SCALE // 2)
    widths = [font.getlength(char) for char in text]
    total = sum(widths) + tracking * max(0, len(text) - 1)
    x = xy[0] * UI_SCALE - total / 2
    y = xy[1] * UI_SCALE
    for char, width in zip(text, widths):
        draw.text((x, y), char, font=font, fill=color, anchor="lm")
        x += width + tracking


def _rounded(draw: ImageDraw.ImageDraw, box: tuple[int, int, int, int],
             radius: int, fill: str, outline: str | None = None,
             width: int = 1) -> None:
    scaled = tuple(value * UI_SCALE for value in box)
    draw.rounded_rectangle(scaled, radius * UI_SCALE, fill=fill,
                           outline=outline, width=width * UI_SCALE)


def _crop_logo(source: Path) -> Image.Image:
    image = Image.open(source).convert("RGB")
    # Remove the large white margins in the supplied image before fitting it.
    difference = Image.eval(image, lambda value: 255 - value)
    box = difference.getbbox()
    if box:
        image = image.crop(box)
    return image


def _draw_bottom_navigation(draw: ImageDraw.ImageDraw) -> None:
    labels = ("CHIP", "FLASH", "SRAM", "WELCOME")
    for index, text in enumerate(labels):
        x0 = index * 80 + 4
        x1 = (index + 1) * 80 - 5
        _rounded(draw, (x0, 184, x1, 216), 8, UI_RED_DARK)
        _rounded(draw, (x0 + 1, 185, x1 - 1, 215), 7, UI_RED)
        _rounded_label(draw, ((x0 + x1) // 2, 200), text, 11, UI_WHITE)
    draw.line((8 * UI_SCALE, 220 * UI_SCALE,
               311 * UI_SCALE, 220 * UI_SCALE), fill=UI_BORDER,
              width=UI_SCALE)
    _rounded_label(draw, (160, 230), "DESIGNED BY NTNJ", 8, UI_MUTED)


def _new_ui_canvas() -> tuple[Image.Image, ImageDraw.ImageDraw]:
    image = Image.new("RGB", (320 * UI_SCALE, 240 * UI_SCALE), UI_BG)
    return image, ImageDraw.Draw(image)


def _finish_ui(image: Image.Image, output: Path) -> None:
    image = image.resize(END_LOGICAL_SIZE, Image.Resampling.LANCZOS)
    image.quantize(colors=COLORS, method=Image.Quantize.MEDIANCUT,
                   dither=Image.Dither.NONE).save(output, optimize=True)


def _finish_control(image: Image.Image, size: tuple[int, int],
                    output: Path) -> None:
    image = image.resize(size, Image.Resampling.LANCZOS)
    image.quantize(colors=COLORS, method=Image.Quantize.MEDIANCUT,
                   dither=Image.Dither.NONE).save(output, optimize=True)


def _finish_info_overlay(image: Image.Image, output: Path) -> None:
    # Store only the card interior. The common rounded card and navigation are
    # drawn once from UI_Info_320x240.gif, saving about 35 KB of MCU flash.
    image = image.crop((16 * UI_SCALE, 14 * UI_SCALE,
                        304 * UI_SCALE, 169 * UI_SCALE))
    image = image.resize((288, 155), Image.Resampling.LANCZOS)
    image.quantize(colors=128, method=Image.Quantize.MEDIANCUT,
                   dither=Image.Dither.NONE).save(output, optimize=True)


def _save_end_gif(source: Path, output: Path) -> None:
    image, draw = _new_ui_canvas()
    logo = _crop_logo(source)

    _rounded(draw, (8, 10, 74, 35), 8, UI_WHITE, UI_RED)
    _rounded_label(draw, (41, 22), "GIF", 10, UI_RED)
    _rounded(draw, (246, 10, 311, 35), 8, UI_WHITE, UI_RED)
    _rounded_label(draw, (278, 22), "PET", 10, UI_RED)
    logo_width = 220 * UI_SCALE
    logo_height = logo.height * logo_width // logo.width
    logo = logo.resize((logo_width, logo_height), Image.Resampling.LANCZOS)
    difference = ImageChops.difference(logo, Image.new("RGB", logo.size, "white"))
    red, green, blue = difference.split()
    mask = ImageChops.lighter(red, ImageChops.lighter(green, blue))
    mask = mask.point(lambda value: min(255, value * 255 // 220))
    image.paste(logo, ((320 * UI_SCALE - logo.width) // 2,
                       58 * UI_SCALE), mask)
    _rounded_label(draw, (160, 119), "CM3034HSB", 13, UI_TEXT)
    _rounded_label(draw, (160, 141), "PRODUCT EXPERIENCE", 8, UI_MUTED)
    draw.line((137 * UI_SCALE, 158 * UI_SCALE,
               183 * UI_SCALE, 158 * UI_SCALE), fill=UI_RED,
              width=2 * UI_SCALE)
    _draw_bottom_navigation(draw)
    _finish_ui(image, output)


def _save_info_background(output: Path) -> None:
    image, draw = _new_ui_canvas()
    _rounded(draw, (10, 8, 309, 175), 14, UI_WHITE, UI_BORDER)
    _rounded(draw, (11, 9, 308, 174), 13, UI_WHITE)
    _draw_bottom_navigation(draw)
    _finish_ui(image, output)


def _info_page_base() -> tuple[Image.Image, ImageDraw.ImageDraw]:
    image, draw = _new_ui_canvas()
    _rounded(draw, (10, 8, 309, 175), 14, UI_WHITE, UI_BORDER)
    _rounded(draw, (11, 9, 308, 174), 13, UI_WHITE)
    _draw_bottom_navigation(draw)
    return image, draw


def _save_info_pages(output_dir: Path) -> list[tuple[Path, str]]:
    pages: list[tuple[Path, str]] = []

    image, draw = _info_page_base()
    draw.rounded_rectangle((24 * UI_SCALE, 23 * UI_SCALE,
                            27 * UI_SCALE, 53 * UI_SCALE),
                           2 * UI_SCALE, fill=UI_RED)
    _rounded_label(draw, (160, 33), "DEVICE PROFILE", 16, UI_TEXT)
    _rounded_label(draw, (160, 53), "CM3034HSB", 9, UI_MUTED)
    for y in (69, 94, 119, 144):
        draw.line((30 * UI_SCALE, y * UI_SCALE,
                   290 * UI_SCALE, y * UI_SCALE), fill=UI_BORDER,
                  width=UI_SCALE)
    _rounded_label(draw, (91, 82), "PROCESSOR", 9, UI_MUTED)
    _rounded_label(draw, (218, 82), "CORTEX-M33 + DSP / FPU", 10, UI_TEXT)
    _rounded_label(draw, (91, 107), "CORE CLOCK", 9, UI_MUTED)
    _rounded_label(draw, (218, 107), "160 MHz", 11, UI_TEXT)
    _rounded_label(draw, (91, 132), "ON-CHIP FLASH", 9, UI_MUTED)
    _rounded_label(draw, (218, 132), "512 KB", 11, UI_TEXT)
    _rounded_label(draw, (91, 157), "ON-CHIP SRAM", 9, UI_MUTED)
    _rounded_label(draw, (218, 157), "320 KB", 11, UI_TEXT)
    path = output_dir / "UI_Info_Chip.gif"
    _finish_info_overlay(image, path)
    pages.append((path, "_UiInfoChip"))

    image, draw = _info_page_base()
    draw.rounded_rectangle((24 * UI_SCALE, 23 * UI_SCALE,
                            27 * UI_SCALE, 53 * UI_SCALE),
                           2 * UI_SCALE, fill=UI_RED)
    _rounded_label(draw, (160, 33), "FLASH MEMORY", 16, UI_TEXT)
    _rounded_label(draw, (160, 53), "INTERNAL STORAGE", 9, UI_MUTED)
    for y in (70, 104, 138):
        draw.line((30 * UI_SCALE, y * UI_SCALE,
                   290 * UI_SCALE, y * UI_SCALE), fill=UI_BORDER,
                  width=UI_SCALE)
    _rounded_label(draw, (75, 87), "USED", 10, UI_MUTED)
    _rounded_label(draw, (75, 121), "AVAILABLE", 10, UI_MUTED)
    _rounded_label(draw, (75, 155), "CAPACITY", 10, UI_MUTED)
    path = output_dir / "UI_Info_Flash.gif"
    _finish_info_overlay(image, path)
    pages.append((path, "_UiInfoFlash"))

    image, draw = _info_page_base()
    draw.rounded_rectangle((24 * UI_SCALE, 23 * UI_SCALE,
                            27 * UI_SCALE, 53 * UI_SCALE),
                           2 * UI_SCALE, fill=UI_RED)
    _rounded_label(draw, (160, 33), "SRAM MEMORY", 16, UI_TEXT)
    _rounded_label(draw, (160, 53), "SYSTEM MEMORY", 9, UI_MUTED)
    for y in (70, 104, 138):
        draw.line((30 * UI_SCALE, y * UI_SCALE,
                   290 * UI_SCALE, y * UI_SCALE), fill=UI_BORDER,
                  width=UI_SCALE)
    _rounded_label(draw, (75, 87), "USED", 10, UI_MUTED)
    _rounded_label(draw, (75, 121), "AVAILABLE", 10, UI_MUTED)
    _rounded_label(draw, (75, 155), "CAPACITY", 10, UI_MUTED)
    path = output_dir / "UI_Info_Sram.gif"
    _finish_info_overlay(image, path)
    pages.append((path, "_UiInfoSram"))

    image, draw = _info_page_base()
    draw.rounded_rectangle((24 * UI_SCALE, 23 * UI_SCALE,
                            27 * UI_SCALE, 53 * UI_SCALE),
                           2 * UI_SCALE, fill=UI_RED)
    _rounded_label(draw, (160, 48), "WELCOME", 20, UI_TEXT)
    _rounded_label(draw, (160, 92), "CM3034HSB", 17, UI_RED)
    _rounded_label(draw, (160, 126), "PRODUCT EXPERIENCE", 10, UI_MUTED)
    draw.line((137 * UI_SCALE, 151 * UI_SCALE,
               183 * UI_SCALE, 151 * UI_SCALE), fill=UI_RED,
              width=2 * UI_SCALE)
    path = output_dir / "UI_Info_Welcome.gif"
    _finish_info_overlay(image, path)
    pages.append((path, "_UiInfoWelcome"))
    return pages


def _save_control_assets(output_dir: Path) -> list[tuple[Path, str]]:
    assets: list[tuple[Path, str]] = []

    exit_path = output_dir / "UI_Exit_Button.gif"
    image = Image.new("RGB", (76 * UI_SCALE, 29 * UI_SCALE), UI_WHITE)
    draw = ImageDraw.Draw(image)
    _rounded(draw, (1, 1, 74, 27), 8, UI_RED_DARK)
    _rounded(draw, (2, 2, 73, 26), 7, UI_RED)
    _rounded_label(draw, (38, 14), "EXIT", 10, UI_WHITE)
    _finish_control(image, (76, 29), exit_path)
    assets.append((exit_path, "_UiExitButton"))

    nav_path = output_dir / "UI_Game_Navigation.gif"
    panel = "#202B35"
    image = Image.new("RGB", (320 * UI_SCALE, 37 * UI_SCALE), panel)
    draw = ImageDraw.Draw(image)
    for index, text in enumerate(("FEED", "PLAY", "CLEAN", "SLEEP")):
        x0 = index * 80 + 4
        x1 = (index + 1) * 80 - 5
        _rounded(draw, (x0, 2, x1, 34), 8, UI_RED_DARK)
        _rounded(draw, (x0 + 1, 3, x1 - 1, 33), 7, UI_RED)
        _rounded_label(draw, ((x0 + x1) // 2, 18), text, 10, UI_WHITE)
    _finish_control(image, (320, 37), nav_path)
    assets.append((nav_path, "_UiGameNavigation"))

    home_path = output_dir / "UI_Home_Button.gif"
    hud = "#223046"
    image = Image.new("RGB", (51 * UI_SCALE, 18 * UI_SCALE), hud)
    draw = ImageDraw.Draw(image)
    _rounded(draw, (0, 0, 50, 17), 6, UI_RED)
    _rounded_label(draw, (25, 8), "HOME", 7, UI_WHITE)
    _finish_control(image, (51, 18), home_path)
    assets.append((home_path, "_UiHomeButton"))

    glyph_path = output_dir / "UI_Capacity_Glyphs.gif"
    glyphs = []
    glyph_font = ImageFont.truetype("C:/Windows/Fonts/ARLRDBD.TTF",
                                    15 * UI_SCALE)
    for char in "0123456789.KB":
        glyph = Image.new("RGB", (12 * UI_SCALE, 20 * UI_SCALE), UI_WHITE)
        glyph_draw = ImageDraw.Draw(glyph)
        if char == ".":
            center_x = 3
        elif char in "KB":
            center_x = 5.5
        else:
            center_x = 5
        glyph_draw.text((center_x * UI_SCALE, 10 * UI_SCALE), char,
                        font=glyph_font, fill=UI_TEXT, anchor="mm")
        glyph = glyph.resize((12, 20), Image.Resampling.LANCZOS)
        glyphs.append(glyph.quantize(colors=32,
                                     method=Image.Quantize.MEDIANCUT,
                                     dither=Image.Dither.NONE))
    glyphs[0].save(glyph_path, save_all=True, append_images=glyphs[1:],
                   duration=100, loop=0, optimize=False, disposal=2)
    assets.append((glyph_path, "_UiCapacityGlyphs"))

    small_path = output_dir / "UI_Small_Glyphs.gif"
    small_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!+:' ."
    small_frames = []
    small_font = ImageFont.truetype("C:/Windows/Fonts/ARLRDBD.TTF",
                                    9 * UI_SCALE)
    for char in small_chars:
        glyph = Image.new("RGB", (8 * UI_SCALE, 12 * UI_SCALE), "#202B35")
        glyph_draw = ImageDraw.Draw(glyph)
        glyph_draw.text((4 * UI_SCALE, 6 * UI_SCALE), char,
                        font=small_font, fill=UI_WHITE, anchor="mm")
        glyph = glyph.resize((8, 12), Image.Resampling.LANCZOS)
        small_frames.append(glyph.quantize(colors=32,
                                           method=Image.Quantize.MEDIANCUT,
                                           dither=Image.Dither.NONE))
    small_frames[0].save(small_path, save_all=True,
                         append_images=small_frames[1:], duration=100,
                         loop=0, optimize=False, disposal=2)
    assets.append((small_path, "_UiSmallGlyphs"))
    return assets


def _write_array(source: Path, output: Path, name: str) -> None:
    data = source.read_bytes()
    lines = [
        "/* Generated by Tools/generate_boot_assets.py. Do not edit manually. */",
        f"static const unsigned char {name}[] = {{",
    ]
    for offset in range(0, len(data), 12):
        chunk = data[offset:offset + 12]
        lines.append("  " + ",".join(f"0x{value:02X}" for value in chunk) + ",")
    lines.extend(["};", ""])
    output.write_text("\n".join(lines), encoding="ascii")
    print(f"generated {output.name}: {len(data)} bytes")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--video", type=Path, required=True)
    parser.add_argument("--end", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()

    args.output_dir.mkdir(parents=True, exist_ok=True)
    start_gif = args.output_dir / "Boot_Start_320x240.gif"
    end_gif = args.output_dir / "Boot_End_320x240.gif"
    info_gif = args.output_dir / "UI_Info_320x240.gif"

    frames = _video_frames(args.video)
    _save_start_gif(frames, start_gif)
    _save_end_gif(args.end, end_gif)
    _save_info_background(info_gif)
    info_pages = _save_info_pages(args.output_dir)
    controls = _save_control_assets(args.output_dir)
    _write_array(start_gif, args.output_dir / "Boot_Start_GIF.inc", "_BootStartGif")
    _write_array(end_gif, args.output_dir / "Boot_End_GIF.inc", "_BootEndGif")
    _write_array(info_gif, args.output_dir / "UI_Info_GIF.inc", "_UiInfoGif")
    for source, name in info_pages:
        _write_array(source, args.output_dir / f"{source.stem}.inc", name)
    for source, name in controls:
        _write_array(source, args.output_dir / f"{source.stem}.inc", name)


if __name__ == "__main__":
    main()
