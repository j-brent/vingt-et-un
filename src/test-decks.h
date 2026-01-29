#pragma once

#include "deck.h"

#include <optional>
#include <string>
#include <vector>

namespace CardGames::BlackJack
{

	/// Returns a named test deck, or nullopt if name not found
	std::optional<Deck> get_test_deck(const std::string& name);

	/// Returns list of available test deck names
	std::vector<std::string> get_test_deck_names();

} // namespace CardGames::BlackJack
