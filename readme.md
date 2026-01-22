# Vingt-et-un (Blackjack)

A C++ blackjack game with console and QML GUI interfaces.

## Building

### Console Application

```sh
# Windows
cmake --preset windows
cmake --build --preset windows-release

# Linux
cmake --preset linux
cmake --build --preset linux-release
```

### QML GUI Application

Requires Qt 6.8+ (via vcpkg, system package manager, or Qt installer).

**Install Qt via vcpkg:**
```sh
# Windows
vcpkg install qtbase:x64-windows qtdeclarative:x64-windows

# Linux
vcpkg install qtbase:x64-linux qtdeclarative:x64-linux
```

**Build:**
```sh
# Windows
cmake --preset windows-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset windows-qml-release

# Linux (GCC)
cmake --preset linux-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset linux-qml-release

# Linux (Clang)
cmake --preset linux-clang-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset linux-clang-qml-release
```

**Run (Windows):**
```sh
# Deploy Qt DLLs first
windeployqt --qmldir app/qml build-win-qml/Release/blackjack-qml.exe

# Run
./build-win-qml/Release/blackjack-qml.exe
```

## Testing

```sh
ctest --preset windows-release   # or linux-release
```

## Project Structure

```
src/           # Core game library (cardgames) and console app
app/qml/       # QML GUI application
test/          # Catch2 unit tests
```

## Download

Pre-built binaries are available at [Releases](https://github.com/j-brent/vingt-et-un/releases/).

**Windows:**
1. Download `blackjack-qml-<version>-windows-x64.zip`
2. Extract and run `blackjack-qml.exe`

**Linux:**
1. Download `blackjack-qml-<version>-linux-x86_64.AppImage`
2. Run:
   ```sh
   chmod +x blackjack-qml-*-linux-x86_64.AppImage
   ./blackjack-qml-*-linux-x86_64.AppImage
   ```
