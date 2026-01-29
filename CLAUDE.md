# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Console Application

```sh
# Windows
cmake --preset windows
cmake --build --preset windows-release   # or windows-debug

# Linux
cmake --preset linux
cmake --build --preset linux-release     # or linux-debug
```

### QML GUI Application

Requires Qt 6.8+ installed via vcpkg or system package manager.

```sh
# Windows (with vcpkg)
cmake --preset windows-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset windows-qml-release

# Linux GCC (with vcpkg)
cmake --preset linux-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset linux-qml-release

# Linux Clang (with vcpkg)
cmake --preset linux-clang-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset linux-clang-qml-release
```

**Installing Qt via vcpkg:**
```sh
# Windows
vcpkg install qtbase:x64-windows qtdeclarative:x64-windows

# Linux
vcpkg install qtbase:x64-linux qtdeclarative:x64-linux
```

**Windows deployment** (copy Qt DLLs to build directory):
```sh
windeployqt --qmldir app/qml build-win-qml/Release/blackjack-qml.exe
```

## Testing

### Console Application Tests

C++ tests use Catch2 (fetched automatically via CMake).

```sh
# Run all tests
ctest --preset windows-release   # or linux-release

# Run single test by name
ctest --preset windows-release -R "Game states"

# Full workflow (configure + build + test)
cmake --workflow --preset windows-release
```

### QML UI Tests

QML tests use Qt Quick Test. Requires Qt 6.8+ with QuickTest component.

```sh
# Full workflow (configure + build + test)
cmake --workflow --preset windows-qml-release

# Run tests only (after build)
ctest --preset windows-qml-release

# Run only QML tests
ctest --preset windows-qml-release -R "blackjack-qml"

# Run only C++ tests in QML build
ctest --preset windows-qml-release -R "Scenario"
```

**QML test files** (`app/qml/tests/`):
- `tst_gamecontroller.qml` - GameController state machine and signals
- `tst_cardview.qml` - Card display properties (suit colors, symbols, face-down)
- `tst_handview.qml` - Hand population and card hiding
- `tst_mainwindow.qml` - UI integration (button states, status messages, game flow)

## Test Documentation

When any `test/*.cpp` file is added or modified, update `test/README.md` to reflect the changes. The README documents the test strategy per type with links to test files and line numbers. Ensure new scenarios are listed in the appropriate table, and line numbers stay accurate after edits.

## Git Commits

Follow the Conventional Commits 1.0.0 specification when creating commit messages. See `.claude/skills/conventional-commit-msg.md` for format, types, and examples.

## Architecture

C++ blackjack game with a `cardgames` static library, `blackjack` console executable, and optional `blackjack-qml` GUI.

**Core types** (`src/`):
- `Card` - Suit/Rank value type with comparison operators
- `Deck` - Card container with shuffle/deal operations
- `CardGames::BlackJack::Game` - State machine managing game flow via `GameNode` enum (Ready → PlayersRound → DealersRound → GameOver*)
- `GameState` - Immutable snapshot holding hands, deck, and current node
- `add_em_up(hand)` - Calculates blackjack hand value (Ace=11, face cards=10)

**QML UI** (`app/qml/`):
- `GameController` - Q_OBJECT wrapper exposing Game to QML
- `Main.qml` - Main window with dealer/player sections and action buttons
- `CardView.qml` - Single card display (rank, suit, face-down state)
- `HandView.qml` - Row of cards using Repeater

**Design patterns**:
- All core types use `static_assert(std::regular<T>)` to enforce regular type semantics (copyable, comparable)
- Game uses history-based state tracking (vector of GameState)
- Player actions: `Deal`, `Hit`, `Stay`
- Blackjack (21 on initial deal) immediately wins/loses - no PlayersRound

## Reading Large PDFs

When a PDF is too large to read (error about file size), split it into smaller chunks:

1. Install pypdf: `pip install pypdf --quiet`
2. Split the PDF into 40-page chunks using this Python script:
   ```python
   from pathlib import Path
   from pypdf import PdfReader, PdfWriter

   input_file = Path(r"path/to/large.pdf")
   pages_per_chunk = 40

   reader = PdfReader(input_file)
   for start in range(0, len(reader.pages), pages_per_chunk):
       end = min(start + pages_per_chunk, len(reader.pages))
       writer = PdfWriter()
       for page_num in range(start, end):
           writer.add_page(reader.pages[page_num])
       chunk_num = (start // pages_per_chunk) + 1
       writer.write(input_file.parent / f"{input_file.stem}_part{chunk_num:02d}.pdf")
   ```
3. Read the split files individually

This approach is documented in `.claude/skills/split-pdf.md` and should be used automatically when PDF reading fails
