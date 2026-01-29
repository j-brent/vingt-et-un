add_library(cardgames
  blackjack-game.h
  blackjack-game.cpp
  card.h
  deck.h
  deck.cpp
  streaming.h
  streaming.cpp
  test-decks.h
  test-decks.cpp
)
target_include_directories(cardgames
  PUBLIC
    "${CURRENT_SOURCE_DIR}"
)

add_executable(blackjack
  main.cpp
)

target_link_libraries(blackjack
  PRIVATE
    cardgames
)
