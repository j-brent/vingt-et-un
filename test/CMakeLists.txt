
add_executable(deck-tests
  deck_tests.cpp
)
target_link_libraries(deck-tests
  PRIVATE
    Catch2::Catch2WithMain
    cardgames
)
include(Catch)
catch_discover_tests(deck-tests)
