# RiveQtQuickPlugin2

This is the second pass at a Rive Qt Quick Plugin.

It plays Rive's `.riv` files in Qt Quick by using the official Rive runtime and
renderer. The project is called `RiveQtQuickPlugin2`, but the QML import stays
`RiveQtQuick`. 

The target here is to build a highperformance plugin, utilizing the official implementation in a qtquickscene.
All Rive drawing share a context, meaning they render extremly fast.

```
import RiveQtQuick

RiveItem {
    source: "qrc:/animations/example.riv"
}
```
## The Icon Grid demo

This is a table view with 100x100 buttons. Each button has a RiveItem on it. You find the code in the examples.

https://github.com/user-attachments/assets/077cfbf9-a3d2-4a5f-93f5-b1051273a8dc

## What you get

- built for Qt `6.6.0` and newer
- uses the official Rive runtime
- Qt Quick / RHI based rendering
- a couple of small example apps to test things quickly
- shared context rendering between all rive elements
- caching of riv files so the plugin does not load them more than once
- Android Support 
- Vulkan
- Direct3d11 and 12
- Metal Support for Mac OS
- IOS Support

-> no opengl support

-> no software render support

Right now the project is mainly exercised on Windows. Release builds are the
ones that really matter for performance, debug builds are okay, but can be slow on complex animations.

## Project layout

- `src/RiveQtQuick` is the plugin itself
- `examples` has the small demo apps
- `tests` has unit and QML coverage
- `scripts` has the helper scripts

## Examples

- `riveqtquick_minimal` is the smallest viewer
- `riveqtquick_marketplace_demo` is the marketplace browser / inspector demo loading elements live from the official rive marketplace
- `riveqtquick_icon_grid` shows a big grid of Qt Quick buttons with live Rive
  icons inside

## Third-party setup

The repo expects the `3rdparty/` folder to be present.

If you want to fetch or refresh it:

```
python .\scripts\bootstrap.py
```

## Build

Typical Windows build with MSVC 2022:

```
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=.....<qtpath>.....
cmake --build build --config Release --target RiveQtQuick
```

If you want the demo apps too:

```
cmake --build build --config Release --target riveqtquick_minimal
cmake --build build --config Release --target riveqtquick_marketplace_demo
cmake --build build --config Release --target riveqtquick_icon_grid
```

## Tests

```
ctest --test-dir build -C Debug --output-on-failure
```

## License

MIT
