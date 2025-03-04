#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 256

typedef struct {
    int round;
    char userMove;
    char compMove;
    char winner; // 'U' για χρήστη, 'C' για υπολογιστή, 'T' για ισοπαλία
} RoundResult;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char getUserMove() {
    char move;
    printf("Επιλέξτε Rock (R), Paper (P), ή Scissors (S): ");
    scanf(" %c", &move);
    clearInputBuffer();
    // Μετατρέπουμε σε κεφαλαίο
    if(move >= 'a' && move <= 'z')
        move = move - 'a' + 'A';
    return move;
}

char getComputerMove() {
    int r = rand() % 3;
    if (r == 0)
        return 'R';
    else if (r == 1)
        return 'P';
    else
        return 'S';
}

char decideWinner(char user, char comp) {
    if(user == comp)
        return 'T';
    // Ρύθμιση νικητή σύμφωνα με τους κανόνες:
    // Rock (R) νικάει Scissors (S)
    // Scissors (S) νικάει Paper (P)
    // Paper (P) νικάει Rock (R)
    if((user == 'R' && comp == 'S') ||
       (user == 'S' && comp == 'P') ||
       (user == 'P' && comp == 'R'))
       return 'U';
    else
       return 'C';
}

int main() {
    char userName[MAX_NAME], compName[MAX_NAME];
    int userScore = 0, compScore = 0;
    int roundCount = 0;
    int capacity = 10;
    RoundResult *rounds = malloc(capacity * sizeof(RoundResult));
    if(rounds == NULL) {
        printf("Πρόβλημα μνήμης!\n");
        return 1;
    }
    
    srand((unsigned int)time(NULL));
    
    // Χαιρετισμός και είσοδος ονομάτων
    printf("Καλώς ήρθατε στο παιχνίδι Πέτρα-Ψαλίδι-Χαρτί!\n");
    
    printf("Παρακαλώ εισάγετε το όνομά σας (μέχρι %d χαρακτήρες): ", MAX_NAME-1);
    fgets(userName, MAX_NAME, stdin);
    userName[strcspn(userName, "\n")] = '\0';  // Αφαίρεση του newline
    
    printf("Παρακαλώ εισάγετε το επιθυμητό όνομα του αντιπάλου-υπολογιστή (μέχρι %d χαρακτήρες): ", MAX_NAME-1);
    fgets(compName, MAX_NAME, stdin);
    compName[strcspn(compName, "\n")] = '\0';
    
    // Παίρνουμε τις επιλογές μέχρι κάποιος να φτάσει τις 10 νίκες
    while(userScore < 10 && compScore < 10) {
        roundCount++;
        printf("\n--- Στροφή %d ---\n", roundCount);
        printf("Σκορ: %s %d - %d %s\n", userName, userScore, compScore, compName);
        
        char userMove = getUserMove();
        if(userMove != 'R' && userMove != 'P' && userMove != 'S') {
            printf("Μη έγκυρη επιλογή. Παρακαλώ δοκιμάστε ξανά.\n");
            roundCount--; // Μη μετρώνται μη έγκυρες προσπάθειες
            continue;
        }
        
        char compMove = getComputerMove();
        printf("%s επέλεξε: %c\n", compName, compMove);
        
        char winner = decideWinner(userMove, compMove);
        if(winner == 'U') {
            printf("Κερδίσατε αυτή τη στροφή!\n");
            userScore++;
        } else if(winner == 'C') {
            printf("Κέρδισε ο %s σε αυτή τη στροφή!\n", compName);
            compScore++;
        } else {
            printf("Ισοπαλία σε αυτή τη στροφή.\n");
        }
        
        // Αποθήκευση αποτελέσματος στροφής
        if(roundCount > capacity) {
            capacity *= 2;
            rounds = realloc(rounds, capacity * sizeof(RoundResult));
            if(rounds == NULL) {
                printf("Πρόβλημα με την επανακατανομή μνήμης!\n");
                return 1;
            }
        }
        rounds[roundCount-1].round = roundCount;
        rounds[roundCount-1].userMove = userMove;
        rounds[roundCount-1].compMove = compMove;
        rounds[roundCount-1].winner = winner;
    }
    
    // Εμφάνιση τελικού αποτελέσματος
    printf("\n=== Τελικό Σκορ ===\n");
    printf("%s: %d  -  %s: %d\n", userName, userScore, compName, compScore);
    if(userScore > compScore)
        printf("Συγχαρητήρια %s, κερδίσατε το παιχνίδι!\n", userName);
    else
        printf("Ο %s κέρδισε το παιχνίδι. Προσπαθήστε ξανά!\n", compName);
    
    // Εκτύπωση συγκεντρωτικού πίνακα αποτελεσμάτων
    printf("\n=== Συγκεντρωτικός Πίνακας ===\n");
    printf("Στροφή\t%s\t%s\tΝικητής\n", "Επιλογή Χρήστη", "Επιλογή Υπολογιστή");
    for(int i = 0; i < roundCount; i++) {
        printf("%d\t  %c\t    %c\t    ", rounds[i].round, rounds[i].userMove, rounds[i].compMove);
        if(rounds[i].winner == 'U')
            printf("%s\n", userName);
        else if(rounds[i].winner == 'C')
            printf("%s\n", compName);
        else
            printf("Ισοπαλία\n");
    }
    
    free(rounds);
    return 0;
}
