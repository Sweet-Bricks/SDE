# SDE

## Development setup

### Compile via Zig drop-in llvm

Install zvm for zig versions control
```sh
# macOS
brew tap hendriknielaender/zvm && brew install zvm

# Linux
curl -fsSL https://raw.githubusercontent.com/hendriknielaender/zvm/main/install.sh | bash

# Windows
irm https://raw.githubusercontent.com/hendriknielaender/zvm/master/install.ps1 | iex
```

Install and use zig version.
```sh
zvm install 0.17.0
zvm use 0.17.0
```

### macOS

```sh
xcode-select --install
brew install meson ninja
```

### Linux

Meson from distro repos is often too old; use `pipx install meson`.

```sh
sudo apt install ninja-build pkg-config libudev-dev libdbus-1-dev libpipewire-0.3-dev \
    libwayland-dev libxkbcommon-dev libegl-dev libgl-dev
```

### Windows

```powershell
winget install mesonbuild.meson
winget install Ninja-build.Ninja
```

For `clang-cl`:

```powershell
winget install LLVM.LLVM
winget install Microsoft.VisualStudio.BuildTools --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

## Build

With Zig (any platform):

```sh
meson setup _build/debug --native-file build/zig.ini --reconfigure --prefix="$PWD/_build/debug/install"
meson install -C _build/debug --tags runtime
```

With `clang-cl` (Windows):

```sh
meson setup _build/debug --native-file build/clang-cl.ini --reconfigure --prefix="$PWD/_build/debug/install"
meson install -C _build/debug --tags runtime
```

For release, use a separate build dir with `--buildtype=release`:

```sh
meson setup _build/release --native-file build/zig.ini --buildtype=release --prefix="$PWD/_build/release/install"
```

## Tests

```sh
meson test -C _build/debug

# With debugger
meson test -C _build/debug --wrapper=raddbg -t 0 unittest
```

Full log: `_build/debug/meson-logs/testlog.txt`.

## IDE (clangd)

`meson setup` writes `_build/debug/compile_commands.json`; `.clangd` points clangd at it.

## Lint / format

```sh
ninja -C _build/debug clang-format
ninja -C _build/debug clang-format-check
ninja -C _build/debug clang-tidy
ninja -C _build/debug clang-tidy-fix
```
