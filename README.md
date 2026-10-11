# SDE

## Setup (Windows)

Builds with LLVM `clang-cl`; STL, CRT and Windows SDK come from Visual Studio Build Tools.

```powershell
winget install mesonbuild.meson
winget install Ninja-build.Ninja
winget install LLVM.LLVM
winget install Microsoft.VisualStudio.BuildTools --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

## Build

```powershell
meson setup _build/debug --native-file build-logic/clang-cl.ini
meson compile -C _build/debug
```

For release:
```powershell
meson setup _build/release --native-file build-logic/clang-cl.ini --buildtype=release
```

## IDE (clangd)

`meson setup` writes `_build/debug/compile_commands.json`; `.clangd` points clangd at it.

## Lint / format

Meson creates these targets from `.clang-format` / `.clang-tidy` (third-party is skipped). Run them via `ninja`, `meson compile` doesn't see them:

```powershell
ninja -C _build/debug clang-format        # reformat in place
ninja -C _build/debug clang-format-check  # check only
ninja -C _build/debug clang-tidy
ninja -C _build/debug clang-tidy-fix      # apply fix-its
```
