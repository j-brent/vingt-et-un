add_executable(blackjack-tests
  card_tests.cpp
  deck_tests.cpp
  game_tests.cpp
  streaming_tests.cpp
)

target_link_libraries(blackjack-tests
  PRIVATE
    Catch2::Catch2WithMain
    cardgames
)

include(Catch)
catch_discover_tests(blackjack-tests)
