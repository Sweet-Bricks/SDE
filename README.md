# SDE

## Development setup

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

```sh
winget install mesonbuild.meson
winget install Ninja-build.Ninja
winget install LLVM.LLVM
winget install Microsoft.VisualStudio.BuildTools --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

## Build

For Mac/Linux with `clang`:

```sh
meson setup _build/debug --native-file build/clang.ini --reconfigure --prefix="$PWD/_build/debug/install"
meson install -C _build/debug --tags runtime
```

For Windows with `clang-cl`:

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
