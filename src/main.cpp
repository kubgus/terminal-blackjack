#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Simple terminal blackjack game

const int WINNING_HAND_VALUE = 21;

std::string player_name = "Player";

enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    RESET
};

inline std::string ansi_color(Color color) {
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

inline std::string colorize(std::string text, Color color) {
    return ansi_color(color) + text + ansi_color(RESET);
}

template <typename T>
T in(
    std::string prompt,
    std::function<bool(T)> validate = [](T){ return true; },
    std::string error_msg = "Unexpected input!"
) {
    T variable;

    while (true) {
        std::cout << prompt << " ";
        std::cin >> variable;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << colorize("Invalid input! Try again!", RED) << std::endl;
        } else if (!validate(variable)) {
            std::cout << colorize(error_msg + " Try again!", RED) << std::endl;
        } else break;
    }

    return variable;
}

enum Suit {
    Spades, Clubs,
    Diamonds, Hearts
};

const int SUIT_COUNT = 4;
const Suit SUITS[SUIT_COUNT] = {
    Spades, Clubs,
    Diamonds, Hearts
};

std::string suit_to_string(Suit suit) {
    switch (suit) {
        case Spades:
            return colorize("♠", CYAN);
        case Clubs:
            return colorize("♣", CYAN);
        case Diamonds:
            return colorize("♦", RED);
        case Hearts:
            return colorize("♥", RED);
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

struct Hand {
    std::string owner_name = "Player";
    std::vector<Card> cards;

    inline void push(Card card) { cards.push_back(card); }
    inline void push(std::vector<Card> card_vec) {
        for (Card card : card_vec) cards.push_back(card);
    }

    std::string to_string(int limit = -1) const {
        std::string buffer;
        for (int i = 0; i < cards.size(); i++) {
            Card card = cards[i];
            if (i > 0) buffer += ", ";
            if (limit < 0 || i < limit) buffer += card.to_string();
            else buffer += "??";
        }
        return buffer;
    }

    int to_value() const {
        int value = 0;
        int ace_count = 0;
        for (Card card : cards) {
            int card_value = rank_to_value(card.rank);
            if (card.rank == A) ace_count++;
            else value += card_value;
        }
        for (int i = 0; i < ace_count; i++) {
            if (value + 11 <= WINNING_HAND_VALUE) value += 11;
            else value += 1;
        }
        return value;
    }

    inline void print(int limit = -1) const {
        std::cout
            << owner_name << "'s hand: " << std::endl
            << to_string(limit);
        if (limit < 0) std::cout << " = " << to_value();
        std::cout << std::endl;
    }

    inline bool is_blackjack() const {
        return to_value() == WINNING_HAND_VALUE;
    }

    inline bool is_busted() const {
        return to_value() > WINNING_HAND_VALUE;
    }

    inline bool should_end() const {
        return is_blackjack() || is_busted();
    }

    inline int win_diff() const {
        return WINNING_HAND_VALUE - to_value();
    }
};

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
            std::cout << colorize("Shuffling cards...", CYAN) << std::endl;
            std::cout << std::endl;
        }

        Card card = cards[_draw_index];
        _draw_index++;
        return card;
    }

    std::vector<Card> draw(int count) {
        std::vector<Card> result;
        for (int i = 0; i < count; i++) {
            result.push_back(draw());
        }
        return result;
    }
private:
    int _draw_index = 0;
};

// returns transtaction completed state
bool transaction(int& balance, int amount, double rate = 1) {
    int final_amount = ceil(amount * rate);
    if (balance + final_amount < 0) return false;

    balance += final_amount;
    std::cout << colorize((final_amount < 0 ? "" : "+") + std::to_string(final_amount), YELLOW) << std::endl;
    return true;
}

void start_round(int& balance, Deck& deck) {
    static const int starting_balance = balance;

    int gain = balance - starting_balance;
    bool loss = gain < 0;
    std::cout << std::endl;
    std::cout
        << colorize("Current balance: ", YELLOW) << balance << " "
        << "("
        << colorize(
            loss ? std::to_string(gain) : "+" + std::to_string(gain),
            loss ? RED : GREEN)
        << ")"
        << std::endl;

    int bet = in<int>(
        colorize("Place bet:", MAGENTA),
        [balance](int b) {
            return b > 0 && b <= balance;
        },
        "Invalid bet!"
    );

    transaction(balance, -bet);

    Hand dealer_hand = { "Dealer", deck.draw(2) };
    Hand player_hand = { player_name, deck.draw(2) };

    std::cout << std::endl;
    dealer_hand.print(1);
    player_hand.print();

    bool running = true;

    bool purchased_insurance = false;
    bool payout_insurance = false;
    if (dealer_hand.cards[0].rank == A) {
        std::cout << std::endl;
        char action = in<char>(
            colorize("Would you like to purchase insurance? (y/n):", MAGENTA),
            [&balance, bet](char act) {
                if (act == 'y') return transaction(balance, -bet * 0.5);
                return act == 'n';
            },
            "You must choose a valid option with sufficient balance!"
        );

        if (action == 'y') {
            purchased_insurance = true;
            if (rank_to_value(dealer_hand.cards[1].rank) == 10)
                payout_insurance = true;
        }
    }

    bool natural_21 = false;
    if (player_hand.is_blackjack()) {
        running = false;
        natural_21 = true;
    }

    while (running) {
        if (player_hand.should_end()) {
            running = false;
            break;
        }

        std::cout << std::endl;

        char action = in<char>(
            colorize("What will you do? (h/s/d):", MAGENTA),
            [balance, bet](char act) {
                if (act == 'd') return balance >= bet * 2;
                return act == 'h' || act == 's';
            },
            "You must either hit, stand, or double down with sufficient balance!"
        );

        switch (tolower(action)) {
            case 'h':
                {
                    Card new_card = deck.draw();
                    player_hand.push(new_card);
                    std::cout << std::endl;
                    std::cout << "You drew a " << new_card.to_string() << std::endl;
                    break;
                }
            case 's':
                std::cout << std::endl;
                std::cout << "You chose to stand on " << player_hand.to_value() << std::endl;
                running = false;
                break;
            case 'd':
                {
                    transaction(balance, -bet);
                    bet *= 2;
                    Card new_card = deck.draw();
                    player_hand.push(new_card);
                    std::cout << std::endl;
                    std::cout << "You doubled down and drew a " << new_card.to_string() << std::endl;
                    running = false;
                    break;
                }
        }

       std::cout << std::endl;
       dealer_hand.print(running ? 1 : -1);
       player_hand.print();
    }

    if (natural_21) {
        std::cout << std::endl;
        if (dealer_hand.is_blackjack()) {
            dealer_hand.print();
            player_hand.print();
            std::cout << colorize("Both players got a blackjack! It's a tie!", CYAN) << std::endl;
            transaction(balance, bet);
        } else {
            dealer_hand.print(1);
            player_hand.print();
            std::cout << colorize("You got a blackjack! (3:2)", GREEN) << std::endl;
            transaction(balance, bet, 2.5); // 1 + 3/2
        }
    } else if (player_hand.is_busted()) {
        std::cout << colorize("You busted!", RED) << std::endl;
    } else {
        while (dealer_hand.to_value() < 17) {
            dealer_hand.push(deck.draw());
        }

        std::cout << std::endl;
        dealer_hand.print();
        player_hand.print();

        int dealer_win_diff = dealer_hand.win_diff();
        int player_win_diff = player_hand.win_diff();

        if (dealer_hand.is_busted()) {
            std::cout << colorize("Dealer busted! You win!", GREEN) << std::endl;
            transaction(balance, bet, 2);
        } else if (dealer_win_diff < player_win_diff) {
            std::cout << colorize("You lose!", RED) << std::endl;
        } else if (dealer_win_diff == player_win_diff) {
            std::cout << colorize("It's a tie!", MAGENTA) << std::endl;
            transaction(balance, bet);
        } else {
            std::cout << colorize("You win!", GREEN) << std::endl;
            transaction(balance, bet, 2);
        }
    }

    if (purchased_insurance) {
        if (payout_insurance) {
            std::cout << colorize("Dealer got a blackjack! You won the insurance! (2:1)", GREEN) << std::endl;
            transaction(balance, bet * 0.5, 3); // 1 + 2/1
        } else {
            std::cout << colorize("You lost the insurance!", RED) << std::endl;
        }
    }

}

int main() {
    // std::cout << colorize("Player name: ", MAGENTA);
    // std::cin >> player_name;
    player_name = getenv("USER");
    player_name[0] = toupper(player_name[0]); // capitalize first letter

    int balance = in<int>(
        colorize("Starting balance:", YELLOW),
        [](int bal) {
            return bal > 0;
        },
        "Starting balance must be greater than 0!"
    );

    Deck deck;
    deck.shuffle();

    bool running = true;
    while (running) {
        start_round(balance, deck);
        std::cout
            << std::endl
            << colorize("Hint: Press Ctrl+C to exit anytime.", CYAN) << std::endl;
    }
}
