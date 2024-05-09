#include "streaming.h"

#ifdef _WIN32
#	include <fcntl.h>
#	include <io.h>
#endif

namespace
{
	template<typename Function>
	void repeat(size_t num_times, Function&& f)
	{
		for (auto i = 0; i < num_times; ++i) {
			f();
		}
	}
} // namespace

std::ostream& operator<<(std::ostream& os, const Rank& value)
{
	switch (value) {
		case Rank::Two: os << "2"; break;
		case Rank::Three: os << "3"; break;
		case Rank::Four: os << "4"; break;
		case Rank::Five: os << "5"; break;
		case Rank::Six: os << "6"; break;
		case Rank::Seven: os << "7"; break;
		case Rank::Eight: os << "8"; break;
		case Rank::Nine: os << "9"; break;
		case Rank::Ten: os << "10"; break;
		case Rank::Jack: os << "Jack"; break;
		case Rank::Queen: os << "Queen"; break;
		case Rank::King: os << "King"; break;
		case Rank::Ace: os << "Ace"; break;
		default: os << "Uknown Rank";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Suit& suit)
{
	switch (suit) {
		case Suit::Clubs: os << "Clubs"; break;
		case Suit::Diamonds: os << "Diamonds"; break;
		case Suit::Hearts: os << "Hearts"; break;
		case Suit::Spades: os << "Spades"; break;
		default: os << "Uknown Suit";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << card.rank << " of " << card.suit;
	return os;
}

void print(const Deck& deck)
{
	const auto& cards = deck.cards();
	for (const auto& card : cards) {
		std::cout << card << std::endl;
	}
	std::cout << std::endl;
}

// unicode printing

std::wostream& operator<<(std::wostream& os, const Rank& value)
{
	switch (value) {
		case Rank::Two: os << L" 2"; break;
		case Rank::Three: os << L" 3"; break;
		case Rank::Four: os << L" 4"; break;
		case Rank::Five: os << L" 5"; break;
		case Rank::Six: os << L" 6"; break;
		case Rank::Seven: os << L" 7"; break;
		case Rank::Eight: os << L" 8"; break;
		case Rank::Nine: os << L" 9"; break;
		case Rank::Ten: os << L"10"; break;
		case Rank::Jack: os << L" J"; break;
		case Rank::Queen: os << L" Q"; break;
		case Rank::King: os << L" K"; break;
		case Rank::Ace: os << L" A"; break;
		default: os << L"<Unknown Rank>";
	}
	return os;
}

#ifdef _WIN32
std::wostream& operator<<(std::wostream& os, const Suit& suit)
{
	switch (suit) {
		case Suit::Clubs: os << L"♣"; break;
		case Suit::Diamonds: os << L"♦"; break;
		case Suit::Hearts: os << L"♥"; break;
		case Suit::Spades: os << L"♠"; break;
		default: os << L"<Unknown Suit>";
	}
	return os;
}
#else
std::wostream& operator<<(std::wostream& os, const Suit& suit)
{
	switch (suit) {
		case Suit::Clubs: os << L"C"; break;
		case Suit::Diamonds: os << L"D"; break;
		case Suit::Hearts: os << L"H"; break;
		case Suit::Spades: os << L"S"; break;
		default: os << L"<Unknown Suit>";
	}
	return os;
}
#endif

std::wostream& operator<<(std::wostream& os, const Card& card)
{
	os << card.rank << card.suit;
	return os;
}

#ifdef _WIN32
void print_terse(const Deck& deck)
{
	// see example at https://msdn.microsoft.com/en-us/library/tw4k6df8.aspx
	auto old_mode = _setmode(_fileno(stdout), _O_U16TEXT);
	// std::wcout << L"Testing unicode -- English -- Ελληνικά -- Español." <<
	// std::endl;  std::wcout << L"♠ ♣ ♥ ♦" << std::endl;
	for (const auto& card : deck.cards()) {
		std::wcout << card << std::endl;
	}
	_setmode(_fileno(stdout), old_mode);
	std::cout << std::endl;
	// std::cout << std::hex << old_mode << std::endl;
	// std::cout << std::hex << _O_U16TEXT << std::endl;

	// ♠ 0x2660
	// ♣ 0x2663
	// ♥ 0x2665
	// ♦ 0x2666
}
#endif

void print_hand(const std::vector<Card>& hand)
{
#ifdef _WIN32
	// see example at https://msdn.microsoft.com/en-us/library/tw4k6df8.aspx
	auto old_mode = _setmode(_fileno(stdout), _O_U16TEXT);
#endif
	for (const auto& card : hand) {
		std::wcout << card << L"  ";
	}
#ifdef _WIN32
	_setmode(_fileno(stdout), old_mode);
#endif
	std::cout << std::endl;
}

void print_hidden_hand(size_t num_cards)
{
	repeat(num_cards, []() { std::cout << " **  "; });
	std::cout << std::endl;
}

void print_hand_hide_some(const std::vector<Card>& hand, size_t num_to_hide)
{
	repeat(num_to_hide, []() { std::cout << " **  "; });
#ifdef _WIN32
	auto old_mode = _setmode(_fileno(stdout), _O_U16TEXT);
#endif
	for (auto i = num_to_hide; i < hand.size(); ++i) {
		const auto& card = hand.at(i);
		std::wcout << card << L"  ";
	}
#ifdef _WIN32
	_setmode(_fileno(stdout), old_mode);
#endif
	std::cout << std::endl;
}
