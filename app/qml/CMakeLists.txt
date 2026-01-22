# Create a library for the QML types and logic to be shared between app and tests
qt_add_library(blackjack-qml-lib STATIC
  GameController.h
  GameController.cpp
)

set_source_files_properties(Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(blackjack-qml-lib
  URI blackjack
  VERSION 1.0
  QML_FILES
    Main.qml
    CardView.qml
    HandView.qml
    Theme.qml
  RESOURCE_PREFIX /
)

target_link_libraries(blackjack-qml-lib
  PUBLIC
    cardgames
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)

target_include_directories(blackjack-qml-lib
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}/generated"
)

# Main executable
qt_add_executable(blackjack-qml
  main.cpp
)

target_link_libraries(blackjack-qml
  PRIVATE
    blackjack-qml-lib
    blackjack-qml-libplugin
    Qt6::QuickControls2
)

# Install configuration for deployment
install(TARGETS blackjack-qml
  RUNTIME DESTINATION bin
  BUNDLE DESTINATION .
)

# QML Tests
if(ENABLE_TESTING)
  find_package(Qt6 REQUIRED COMPONENTS QuickTest)

  qt_add_executable(blackjack-qml-tests
    tests/tst_main.cpp
  )

  target_link_libraries(blackjack-qml-tests
    PRIVATE
      blackjack-qml-lib
      blackjack-qml-libplugin
      Qt6::QuickTest
  )

  target_compile_definitions(blackjack-qml-tests
    PRIVATE
      QUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests"
  )

  add_test(
    NAME blackjack-qml-tests
    COMMAND blackjack-qml-tests
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  )

  # Set environment for QML module discovery
  # QT_PLUGIN_PATH: where platform plugins (qwindows.dll) live - in the main binary output dir
  # QML2_IMPORT_PATH: where QML modules live - in Qt's qml folder (relative to Qt6Core lib)
  get_target_property(_qt6core_location Qt6::Core LOCATION)
  get_filename_component(_qt6_lib_dir "${_qt6core_location}" DIRECTORY)
  get_filename_component(_qt6_root "${_qt6_lib_dir}" DIRECTORY)
  set(_qml_import_path "${_qt6_root}/Qt6/qml")

  set_tests_properties(blackjack-qml-tests PROPERTIES
    ENVIRONMENT "QT_PLUGIN_PATH=${CMAKE_BINARY_DIR}/$<CONFIG>;QML2_IMPORT_PATH=${_qml_import_path}"
  )
endif()
