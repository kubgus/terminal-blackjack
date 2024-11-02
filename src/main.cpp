#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

// Simple terminal blackjack game

enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    RESET  // Used to reset to default terminal color
};

// Function to convert Color enum to corresponding ANSI code
std::string color(Color color) {
    switch (color) {
        case RED:     return "\033[31m";
        case GREEN:   return "\033[32m";
        case BLUE:    return "\033[34m";
        case YELLOW:  return "\033[33m";
        case CYAN:    return "\033[36m";
        case MAGENTA: return "\033[35m";
        case RESET:   return "\033[0m";
        default:      return "\033[0m";
    }
}

const int WINNING_HAND_VALUE = 21;

enum Suit {
    Clubs, Diamonds, Hearts, Spades
};

const int SUIT_COUNT = 4;
const Suit SUITS[SUIT_COUNT] = {
    Clubs, Diamonds, Hearts, Spades
};

std::string suit_to_string(Suit suit) {
    switch (suit) {
        case Clubs:
            return color(CYAN) + "♣" + color(RESET);
        case Diamonds:
            return color(RED) + "♦" + color(RESET);
        case Hearts:
            return color(RED) + "♥" + color(RESET);
        case Spades:
            return color(CYAN) + "♠" + color(RESET);
        default:
            return "?";
    }
}

enum Rank {
    _2, _3, _4, _5, _6, _7, _8, _9, _10, J, Q, K, A
};

const int RANK_COUNT = 13;
const Rank RANKS[RANK_COUNT] = {
    _2, _3, _4, _5, _6, _7, _8, _9, _10, J, Q, K, A
};

std::string rank_to_string(Rank rank) {
    switch (rank) {
        case _2:
            return "2";
        case _3:
            return "3";
        case _4:
            return "4";
        case _5:
            return "5";
        case _6:
            return "6";
        case _7:
            return "7";
        case _8:
            return "8";
        case _9:
            return "9";
        case _10:
            return "10";
        case J:
            return "J";
        case Q:
            return "Q";
        case K:
            return "K";
        case A:
            return "A";
        default:
            return "?";
    }
}

int rank_to_value(Rank rank) {
    switch (rank) {
        case _2:
            return 2;
        case _3:
            return 3;
        case _4:
            return 4;
        case _5:
            return 5;
        case _6:
            return 6;
        case _7:
            return 7;
        case _8:
            return 8;
        case _9:
            return 9;
        case _10:
        case J:
        case Q:
        case K:
            return 10;
        case A:
            return 11;
        default:
            return 0;
    }
}

struct Card {
    Suit suit;
    Rank rank;

    std::string to_string() const {
        return suit_to_string(suit) + rank_to_string(rank);
    }
};
using Hand = std::vector<Card>;


std::string hand_to_string(Hand hand, int limit = -1) {
    std::string buffer;
    for (
            int i = 0;
            i < hand.size() && (limit < 0 || i < limit);
            i++
        ) {
        Card card = hand[i];
        if (i > 0) buffer += ", ";
        buffer += card.to_string();
    }
    return buffer;
}

int hand_to_value(Hand hand) {
    int value = 0;
    for (Card card : hand) {
        int card_value = rank_to_value(card.rank);
        if (card.rank == A && value + card_value > WINNING_HAND_VALUE) card_value = 1;
        value += card_value;
    }
    return value;
}

void print_hand(Hand hand) {
        std::cout
            << hand_to_string(hand)
            << " = "
            << hand_to_value(hand)
            << std::endl;
}

const int DECK_SIZE = SUIT_COUNT * RANK_COUNT;
class Deck {
public:
    Card cards[DECK_SIZE];
public:
    Deck() {
        for (int i = 0; i < DECK_SIZE; i++) {
            int suit_index = i % SUIT_COUNT;
            int rank_index = i % RANK_COUNT;
            cards[i] = {
                SUITS[suit_index],
                RANKS[rank_index]
            };
        }
    }

    void shuffle() {
        for (int i = 0; i < DECK_SIZE; i++) {
            std::srand(std::time(nullptr));
            int j = i + std::rand() % (DECK_SIZE - i);
            Card temp = cards[i];
            cards[i] = cards[j];
            cards[j] = temp;
        }
    }

    Card draw() {
        if (_draw_index >= DECK_SIZE) {
            _draw_index = 0;
            std::cout << std::endl;
            std::cout << color(CYAN) << "Shuffling cards..." << color(RESET) << std::endl;
            std::cout << std::endl;
        }

        Card card = cards[_draw_index];
        _draw_index++;
        return card;
    }
private:
    int _draw_index = 0;
};

void start_round(int& balance, Deck& deck) {
    std::cout << std::endl;
    std::cout << color(YELLOW) << "Current balance: " << color(RESET) << balance << std::endl;

    int bet;
    std::cout << color(MAGENTA) +  "Place bet: " + color(RESET);
    std::cin >> bet;

    if (bet <= 0) {
        std::cout << "Don't even try... 😒" << std::endl;
        return;
    }

    if (bet > balance) {
        std::cout << "Insufficient balance!" << std::endl;
        return;
    }

    Hand dealer_hand = {
        deck.draw(), deck.draw()
    };

    Hand player_hand = {
        deck.draw(), deck.draw()
    };

    bool running = true;
    while (running) {
        std::cout << std::endl;

        std::cout
            << "Dealer's hand: "
            << std::endl
            << dealer_hand[0].to_string()
            << ", ??"
            << std::endl;

        std::cout << "Your hand: " << std::endl;
        print_hand(player_hand);

        if (hand_to_value(player_hand) == WINNING_HAND_VALUE) {
            running = false;
            continue;
        }

        std::cout
            << color(MAGENTA)
            << "What will you do? (h/s): "
            << color(RESET);
        char action;
        std::cin >> action;

        std::cout << std::endl;

        switch (tolower(action)) {
            case 'h':
                {
                    Card new_card = deck.draw();
                    std::cout << "You drew a " << new_card.to_string() << std::endl;
                    player_hand.push_back(new_card);
                    if (hand_to_value(player_hand) >= WINNING_HAND_VALUE) {
                        running = false;
                    }
                    continue;
                }
            case 's':
                std::cout << "You chose to stand on " << hand_to_value(player_hand) << std::endl;
                running = false;
                continue;
            default:
                std::cout << "Invalid choice!" << std::endl;
                continue;
        }
    }

    std::cout << std::endl;

    int player_hand_value = hand_to_value(player_hand);
    int player_win_diff = WINNING_HAND_VALUE - player_hand_value;

    if (player_win_diff <= 0) {
        std::cout << "Dealer's hand: " << std::endl;
        print_hand(dealer_hand);

        std::cout << "Your hand: " << std::endl;
        print_hand(player_hand);

        if (player_win_diff < 0) {
            std::cout << color(RED) << "You busted!" << color(RESET) << std::endl;
            balance -= bet;
            return;
        }

        if (player_win_diff == 0) {
            std::cout << color(GREEN) << "You got a blackjack!" << color(RESET) << std::endl;
            balance += bet;
            return;
        }
    }

    while (hand_to_value(dealer_hand) < 17) {
        dealer_hand.push_back(deck.draw());
    }

    std::cout << "Dealer's hand: " << std::endl;
    print_hand(dealer_hand);

    std::cout << "Your hand: " << std::endl;
    print_hand(player_hand);

    int dealer_hand_value = hand_to_value(dealer_hand);
    int dealer_win_diff = WINNING_HAND_VALUE - dealer_hand_value;

    if (dealer_win_diff < 0) {
        std::cout << color(GREEN) << "Dealer busted! You win!" << color(RESET) << std::endl;
        balance += bet;
        return;
    }

    if (player_win_diff < dealer_win_diff) {
        std::cout << color(GREEN) << "You win!" << color(RESET) << std::endl;
        balance += bet;
        return;
    }

    if (player_win_diff == dealer_win_diff) {
        std::cout << color(MAGENTA) << "It's a tie!" << color(RESET) << std::endl;
        return;
    }

    std::cout << color(RED) << "You lose!" << color(RESET) << std::endl;
    balance -= bet;
}

int main() {
    int balance;
    std::cout << color(YELLOW) << "Starting balance: " << color(RESET);
    std::cin >> balance;

    Deck deck;
    deck.shuffle();

    bool running = true;
    while (running) {
        start_round(balance, deck);

        std::cout << std::endl;
        std::cout << color(BLUE) << "Play again? (y/n): " << color(RESET);
        char action;
        std::cin >> action;

        switch (tolower(action)) {
            case'y':
            default:
                break;
            case 'n':
                running = false;
        }

    }
}
