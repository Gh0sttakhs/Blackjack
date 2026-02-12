#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- ΠΡΟΣΘΗΚΗ ΒΙΒΛΙΟΘΗΚΩΝ ΓΙΑ SLEEP & CLEAR ---
#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(x) Sleep(x)
#define CLEAR_SCREEN system("cls")
#else
#include <unistd.h>
#define SLEEP_MS(x) usleep((x)*1000)
#define CLEAR_SCREEN system("clear")
#endif

// --- ΟΡΙΣΜΟΣ ΧΡΩΜΑΤΩΝ (ANSI CODES) ---
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define BOLD    "\033[1m"

typedef struct {
    char suit[10];
    char rank[3];
    int value;
} Card;

const char *SUITS[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char *RANKS[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
const int VALUES[]  = { 2,   3,   4,   5,   6,   7,   8,   9,   10,   10,  10,  10,  11};

void init_card(Card deck[]);
void shuffle_deck(Card deck[]);
void print_hand(Card hand[], int num_cards, char *owner);
int calculate_score(Card hand[], int num_cards);

int main() {
    //INITIALIZE AND SHUFFLE
    Card deck[52];
    Card playerHand[10];
    Card dealerHand[10];
    init_card(deck);
    
    char answer;
    
    do {
        CLEAR_SCREEN; // Καθαρισμός οθόνης στην αρχή
        printf(CYAN BOLD "\n=== WELCOME TO BLACKJACK ===\n" RESET);
        printf(YELLOW "\nShuffling the deck...\n" RESET);
        SLEEP_MS(1200); // Animation ανακατέματος
        shuffle_deck(deck);

        printf(GREEN "\n=== NEW GAME ===\n" RESET);
        SLEEP_MS(500);

        int pcount = 0 , dcount = 0, card = 0;

        // Animation μοιράσματος (ένα-ένα τα χαρτιά)
        printf("Dealing cards...\n");
        playerHand[pcount++] = deck[card++]; SLEEP_MS(400);
        dealerHand[dcount++] = deck[card++]; SLEEP_MS(400);
        playerHand[pcount++] = deck[card++]; SLEEP_MS(400);
        dealerHand[dcount++] = deck[card++]; SLEEP_MS(400);

        int pscore = 0, playerBusted = 0;
        char choice;
        while(1) {
            CLEAR_SCREEN; // Καθαρίζουμε για να φαίνεται καθαρό το τραπέζι
            printf(CYAN BOLD "\n=== BLACKJACK TABLE ===\n" RESET);
            
            pscore = calculate_score(playerHand, pcount);

            printf("\nDealer shows: " YELLOW "%s of %s" RESET " [Hidden Card]\n", dealerHand[0].rank, dealerHand[0].suit);
            print_hand(playerHand,pcount,"YOUR HAND");
            printf("Your score: " BOLD "%d" RESET "\n", pscore);

            //lost
            if (pscore > 21) {
                printf(RED BOLD "\n--> BUSTED! You went over 21.\n" RESET);
                playerBusted = 1;
                break;
            }
            if (pscore == 21) {
                printf(GREEN BOLD "\n--> BLACKJACK / 21! Great!\n" RESET);
                break;
            }

            printf(CYAN "\nHit (h) or Stand (s)? " RESET);
            scanf(" %c", &choice);

            if (choice == 'h') {
                printf(YELLOW "--> You draw a card...\n" RESET);
                SLEEP_MS(600); // Αγωνία στο τράβηγμα
                playerHand[pcount++] = deck[card++];
            } else if (choice == 's') {
                printf(BLUE "--> You stand.\n" RESET);
                SLEEP_MS(500);
                break;
            }
        }

        int dscore = calculate_score(dealerHand, dcount);

        if (!playerBusted) {
            printf(YELLOW "\n--- DEALER'S TURN ---\n" RESET);
            SLEEP_MS(800);
            print_hand(dealerHand, dcount, "DEALER HAND");
            printf("Dealer Score: %d\n", dscore);
            SLEEP_MS(1000);

            while (dscore < 17) {
                printf(YELLOW "--> Dealer draws...\n" RESET);
                SLEEP_MS(1200); // Αγωνία όταν τραβάει ο Dealer
                dealerHand[dcount++] = deck[card++];
                dscore = calculate_score(dealerHand, dcount);
            
                print_hand(dealerHand, dcount, "DEALER HAND");
                printf("Dealer Score: %d\n", dscore);
            }
        }   

        printf(CYAN BOLD "\n=== FINAL RESULT ===\n" RESET);
        printf("You: %d | Dealer: %d\n", pscore, dscore);

        if (playerBusted) {
            printf(RED BOLD "YOU LOSE! (Busted)\n" RESET);
        } else if (dscore > 21) {
            printf(GREEN BOLD "YOU WIN! (Dealer Busted)\n" RESET);
        } else if (pscore > dscore) {
            printf(GREEN BOLD "YOU WIN!\n" RESET);
        } else if (pscore < dscore) {
            printf(RED BOLD "YOU LOSE!\n" RESET);
        } else {
            printf(YELLOW BOLD "PUSH! (It's a tie)\n" RESET);
        }

        printf("\nPlay again? Yes (y) or No (n): ");
        scanf(" %c",&answer);
    } while (answer != 'n');
    
    printf(CYAN "\nThanks for playing! Goodbye.\n" RESET);
    return 0;
}

void init_card(Card deck[]) {
    int k = 0;
    for (int i = 0 ; i < 4 ; i++) {
        for (int j = 0 ; j < 13 ; j++) {
            strcpy(deck[k].suit , SUITS[i]);
            strcpy(deck[k].rank , RANKS[j]);
            deck[k].value = VALUES[j];
            k++;
        }
    }
}

void shuffle_deck(Card deck[]) {
    srand(time(NULL));
    for (int i = 0 ; i < 52 ; i++) {
        int r = rand() % 52;
        Card temp = deck[i];
        deck[i] = deck[r];
        deck[r] = temp;
    }
}

void print_hand(Card hand[], int num_cards, char *owner) {
    printf("%s: ",owner);
    for (int i = 0 ; i < num_cards ; i++) {
        // Προσθήκη χρώματος στα χαρτιά
        printf(BOLD "[%s %s] " RESET, hand[i].rank, hand[i].suit);
    }
    printf("\n");
}

int calculate_score(Card hand[], int num_cards) {
    int sum = 0;
    int aces = 0;
    for (int i = 0 ; i < num_cards ; i++) {
        sum += hand[i].value;
        if (strcmp(hand[i].rank,"A") == 0) {
            aces++;
        }
    }

    while (sum > 21 && aces > 0) {
        sum -= 10;
        aces--;
    }

    return sum;
}