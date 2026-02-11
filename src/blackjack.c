#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
    printf("\n=== WELCOME TO BLACKJACK ===");
    do {
        printf("\n\n\n=== NEW GAME ===\n");
        shuffle_deck(deck);

        int pcount = 0 , dcount = 0, card = 0;

        playerHand[pcount++] = deck[card++];
        dealerHand[dcount++] = deck[card++];
        playerHand[pcount++] = deck[card++];
        dealerHand[dcount++] = deck[card++];

        int pscore = 0, playerBusted = 0;
        char choice;
        while(1) {
            pscore = calculate_score(playerHand, pcount);

            printf("\nDealer shows: %s of %s [Hidden Card]\n", dealerHand[0].rank, dealerHand[0].suit);
            print_hand(playerHand,pcount,"YOUR HAND");
            printf("Your score: %d\n",pscore);

            //lost
            if (pscore > 21) {
                printf("--> BUSTED! You went over 21.\n");
                playerBusted = 1;
                break;
            }
            if (pscore == 21) {
                printf("--> BLACKJACK / 21! Great!\n");
                break;
            }

            printf("Hit (h) or Stand (s)? ");
            scanf(" %c", &choice);

            if (choice == 'h') {
                printf("--> You draw a card...\n");
                playerHand[pcount++] = deck[card++];
            } else if (choice == 's') {
                printf("--> You stand.\n");
                break;
            }
        }

        int dscore = calculate_score(dealerHand, dcount);

        if (!playerBusted) {
            printf("\n--- DEALER'S TURN ---\n");
            print_hand(dealerHand, dcount, "DEALER HAND");
            printf("Dealer Score: %d\n", dscore);

            while (dscore < 17) {
                printf("--> Dealer draws...\n");
                dealerHand[dcount++] = deck[card++];
                dscore = calculate_score(dealerHand, dcount);
            
                print_hand(dealerHand, dcount, "DEALER HAND");
                printf("Dealer Score: %d\n", dscore);
            }
        }   

        printf("\n=== FINAL RESULT ===\n");
        printf("You: %d | Dealer: %d\n", pscore, dscore);

        if (playerBusted) {
            printf("YOU LOSE! (Busted)\n");
        } else if (dscore > 21) {
            printf("YOU WIN! (Dealer Busted)\n");
        } else if (pscore > dscore) {
            printf("YOU WIN!\n");
        } else if (pscore < dscore) {
            printf("YOU LOSE!\n");
        } else {
            printf("PUSH! (It's a tie)\n");
        }

        printf("Play again? Yes (y) or No (n): ");
        scanf(" %c",&answer);
    } while (answer != 'n');
    
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
        printf("[%s %s] ", hand[i].rank, hand[i].suit);
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