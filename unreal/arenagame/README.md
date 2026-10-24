# Arenagame

A small, C++-first Unreal Engine **5.8** foundation for a first-person psychological horror game. The player wakes in an abandoned coastal research station during a storm, scavenges fuel and a keycard, restores power, avoids a stalking entity, and reaches the boat.

## Requirements

- Unreal Engine 5.8 installed through the Epic Games Launcher or a source build.
- A C++ toolchain supported by your UE 5.8 installation (Visual Studio 2022 on Windows; Xcode on macOS; clang toolchain on Linux).

## Open and build

1. Open `Arenagame.uproject` in Unreal Editor 5.8. If prompted, allow the editor to generate project files/build the C++ module.
2. For IDE project files, right-click the `.uproject` and choose **Generate project files**, or use the UE 5.8 `GenerateProjectFiles` script. Open `Arenagame.sln` / the generated workspace and build the **ArenagameEditor** target in Development Editor configuration.
3. Alternatively, build from a terminal, passing the project file explicitly. On Linux run `/path/to/UE_5.8/Engine/Build/BatchFiles/Linux/Build.sh ArenagameEditor Linux Development "$PWD/Arenagame.uproject" -WaitMutex`; on Windows run `"C:\\Program Files\\Epic Games\\UE_5.8\\Engine\\Build\\BatchFiles\\Build.bat" ArenagameEditor Win64 Development "%CD%\\Arenagame.uproject" -WaitMutex`. Adjust the engine path/platform for your installation.
4. Open the project in Unreal Editor 5.8 and press **Play**. The configured game mode spawns `AArenagameCharacter`; basic input is installed from C++ at runtime, so no Input Action or Mapping Context assets are needed to test movement.

This repository intentionally contains no `.uasset` or `.umap` files. The empty project opens without a playable level; create a level and set it as the Editor Startup Map / Game Default Map (Project Settings → Maps & Modes) to build out the experience. A temporary blank level can still be used to verify compilation and the pawn, but add a floor and lighting to play comfortably.

## Controls

- **W/A/S/D** move, **Mouse** look
- **Space** jump, **Left Shift** sprint, **Left Ctrl** crouch
- **F** toggle flashlight, **E** interact

Enhanced Input is enabled in the project descriptor and is the only input system used. The runtime mapping context is generated in `AArenagameCharacter`.

## C++ layout

- `Player/` first-person pawn and survival stats
- `Horror/` stalker AI and fear/sanity response manager
- `Interaction/` interaction interface, reusable interactables and pickups
- `Objectives/` objective state actor
- `Atmosphere/` storm lighting, light flicker and ambient audio hooks

See [ARENAGAME_DESIGN.md](ARENAGAME_DESIGN.md), [TESTING.md](TESTING.md), and [TODO.md](TODO.md) for the design brief, Editor setup and next steps.
