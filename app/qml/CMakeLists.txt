qt_add_executable(blackjack-qml
  main.cpp
  GameController.h
  GameController.cpp
)

qt_add_qml_module(blackjack-qml
  URI blackjack
  VERSION 1.0
  QML_FILES
    Main.qml
    CardView.qml
    HandView.qml
)

target_link_libraries(blackjack-qml
  PRIVATE
    cardgames
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)

target_include_directories(blackjack-qml
  PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)
