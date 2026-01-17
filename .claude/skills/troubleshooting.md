# QML/Qt Troubleshooting Guide

Lessons learned from implementing the QML visual redesign with shared CSS styling.

## Problem 1: QML Does Not Support CSS

**Symptom:** Assumed CSS stylesheets could be applied directly to QML components.

**Root Cause:** QML has its own styling system. Qt explicitly does not plan to support widget stylesheets in Qt Quick.

**Solution:** Use **Design Tokens** pattern:
- Store values in `tokens.json`
- Generate `Theme.qml` (QML singleton) and `theme.css` (CSS custom properties)
- Both platforms read from the same source of truth

**Resources:**
- [Qt Wiki: QML Styling](https://wiki.qt.io/Qml_Styling)
- [aqt-stylesheets](https://github.com/Ableton/aqt-stylesheets) - evaluated but adds Boost dependency

---

## Problem 2: FontMetrics Inside QtObject Fails

**Symptom:**
```
qrc:/blackjack/Theme.qml:12,5: Cannot assign to non-existent default property
```

**Root Cause:** `QtObject` cannot have child items. `FontMetrics` is a visual element that requires a visual parent with a default property.

**Bad Code:**
```qml
pragma Singleton
import QtQuick

QtObject {
    FontMetrics {  // ERROR: QtObject has no default property
        id: fontMetrics
        font.pixelSize: 16
    }
    readonly property real em: fontMetrics.height
}
```

**Solution:** Use a constant for the em base value (16px is the browser standard):
```qml
pragma Singleton
import QtQuick

QtObject {
    readonly property real em: 16  // Standard browser default
    readonly property real cardWidth: em * 8
}
```

**Alternative:** Use `Item` as root instead of `QtObject` if you need FontMetrics, but this creates a visual element which is less ideal for a pure data singleton.

---

## Problem 3: QML Tests Fail Silently (Exit Code 4)

**Symptom:** Tests return exit code 4 with no visible output:
```
$ ./Release/blackjack-qml-tests.exe
Exit code: 4
```

**Root Cause:** Qt Quick Test doesn't output to stdout/stderr by default when there are compilation errors.

**Solution:** Write output to a file:
```bash
./Release/blackjack-qml-tests.exe -o test_output.txt
cat test_output.txt
```

This reveals the actual error messages:
```
QWARN: Type TestCase unavailable
qrc:/qt-project.org/imports/QtTest/TestCase.qml:5,1: module "QtQuick.Window" is not installed
```

---

## Problem 4: Qt QML Modules Not Found in Tests

**Symptom:**
```
module "QtQuick.Window" is not installed
module "QtQuick.Controls" is not installed
```

**Root Cause:** When running tests manually, the Qt QML import paths aren't set. CMake sets these via `set_tests_properties(... ENVIRONMENT ...)` but this only applies when running through ctest.

**Solution:** Set environment variables manually:
```bash
export QML2_IMPORT_PATH="C:/vcpkg/installed/x64-windows/Qt6/qml"
export QT_PLUGIN_PATH="C:/vcpkg/installed/x64-windows/Qt6/plugins"
./Release/blackjack-qml-tests.exe -input "path/to/tests"
```

**To find the correct paths:**
```bash
ls $VCPKG_ROOT/installed/x64-windows/Qt6/
# Should show: plugins/ qml/
```

---

## Problem 5: Tests Fail After Refactoring to Theme Values

**Symptom:** Tests that previously passed now fail with comparison errors.

**Root Cause:** Tests had hardcoded expectations:
```qml
function test_defaultDimensions() {
    compare(card.width, 50, "Card width should be 50")  // Hardcoded!
}
```

**Solution:** Update tests to use Theme values:
```qml
function test_dimensionsMatchTheme() {
    compare(card.width, Theme.cardWidth, "Card width should match Theme")
}
```

**Affected tests in this project:**
- `tst_cardview.qml`: dimensions, suit colors, card face/back colors
- `tst_handview.qml`: spacing value

---

## Problem 6: Edit Tool Fails with "File Unexpectedly Modified"

**Symptom:** Repeated attempts to edit a file fail with:
```
File has been unexpectedly modified. Read it again before attempting to write it.
```

**Root Cause:** File system timing issues, possibly related to Windows or IDE file watchers.

**Solution:** Use Bash with heredoc to write the entire file:
```bash
cat > "path/to/file.txt" << 'EOF'
file contents here
EOF
```

**Note:** This is a workaround. The Edit tool should be preferred when it works, as it provides better diff tracking.

---

## Problem 7: Registering QML Singletons in CMake

**Symptom:** Singleton QML file not recognized or "Type X unavailable" errors.

**Solution:** Use `set_source_files_properties` before `qt_add_qml_module`:
```cmake
set_source_files_properties(Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(mylib
  URI mymodule
  QML_FILES
    Main.qml
    Theme.qml  # Listed with other QML files
)
```

The `pragma Singleton` in the QML file is also required:
```qml
pragma Singleton
import QtQuick
QtObject { ... }
```

---

## Quick Reference: Qt/QML Test Commands

```bash
# Run tests via ctest (uses CMake-configured environment)
ctest --preset windows-qml-release --output-on-failure

# Run tests manually with output capture
./Release/blackjack-qml-tests.exe -o output.txt
cat output.txt

# Run with explicit Qt paths
export QML2_IMPORT_PATH="$VCPKG_ROOT/installed/x64-windows/Qt6/qml"
export QT_PLUGIN_PATH="$VCPKG_ROOT/installed/x64-windows/Qt6/plugins"
./Release/blackjack-qml-tests.exe -input "path/to/tests"

# List available test functions
./Release/blackjack-qml-tests.exe -functions

# Run with verbose output
./Release/blackjack-qml-tests.exe -v1 -o output.txt
```
