# FlatOut 2 FPS Unlocker

Plugin to remove the framerate cap from FlatOut 2 without breaking sounds

While this technically won't make the entirety of the game run at a higher framerate, extra frames above 100FPS repeat the physics state, but update the HUD, camera and particle effects. The menus also run at the highest framerate possible.

## Installation

- Make sure you have v1.2 of the game, as this is the only version this plugin is compatible with. (exe size of 2990080 bytes)
- Plop the files into your game folder, edit `FlatOut2FPSUnlocker_gcp.toml` to change the options to your liking.
- Enjoy, nya~ :3

## Building

Building is done on an Arch Linux system with CLion and vcpkg being used for the build process. 

Before you begin, clone [nya-common](https://github.com/gaycoderprincess/nya-common) to a folder next to this one, so it can be found.

Required packages: `mingw-w64-gcc vcpkg`

To install all dependencies, use:
```console
vcpkg install tomlplusplus:x86-mingw-static
```

Once installed, copy files from `~/.vcpkg/vcpkg/installed/x86-mingw-static/`:

- `include` dir to `nya-common/3rdparty`
- `lib` dir to `nya-common/lib32`

You should be able to build the project now in CLion.
