#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct card
{
    char suit;
    char num;
} card;

typedef struct node
{
    card card;
    struct node *next;
} node;

typedef struct column
{
    char name[3];
    node *head;
} column;

char findSuit(int i)
{
    if (i == 1)
    {
        return 'C';
    }
    if (i == 2)
    {
        return 'D';
    }
    if (i == 3)
    {
        return 'H';
    }
    if (i == 4)
    {
        return 'S';
    }

    // Used to check for errors
    return -1;
}

char findCardNo(int i)
{
    if (i == 1)
    {
        return 'A';
    }
    if (i == 10)
    {
        return 'T';
    }
    if (i == 11)
    {
        return 'J';
    }
    if (i == 12)
    {
        return 'Q';
    }
    if (i == 13)
    {
        return 'K';
    }
    if (i < 14)
    {
        return '0' + i;
    }

    // Used to check for errors
    return -1;
}

// Initialize a new deck of unshuffled cards
node *initializeCards()
{
    node *head = NULL;
    node *last = NULL;

    int i;
    int j;
    for (i = 1; i <= 4; i++)
    {
        char suit = findSuit(i);

        for (j = 1; j <= 13; j++)
        {
            char res[2];
            char num = findCardNo(j);

            if (head == NULL)
            {
                // Allocate memory with malloc to ensure that head is not overwritten
                head = (struct node *)malloc(sizeof(struct node));
                head->card.num = num;
                head->card.suit = suit;
                head->next = NULL;
                last = head;
            }
            else
            {
                // Allocate memory
                last->next = (struct node *)malloc(sizeof(struct node));
                last->next->card.num = num;
                last->next->card.suit = suit;
                last->next->next = NULL;
                last = last->next;
            }
        }
    }
    return head;
}

// Check if loaded cards contains dublicates
int checkDuplicates(node *head)
{
    int outerLine = 1;
    while (head->next != NULL)
    {
        node *next = head->next;
        int line = outerLine + 1;
        while (next->next != NULL)
        {
            card headCard = head->card;
            card nextCard = next->card;
            if (nextCard.num == headCard.num && nextCard.suit == headCard.suit)
            {
                return line; // Error
            }
            line++;
            next = next->next;
        }
        head = head->next;
        outerLine++;
    }
    return 0;
}

// Check if there is a missing card in the deck
card *checkForMissingCards(node *head)
{
    int line = 1;
    int i;
    int j;

    // Checking for each card if it exists in file
    for (i = 1; i <= 4; i++)
    {
        for (j = 1; j <= 13; j++)
        {
            node *node = head;

            char num = findCardNo(j);
            char suit = findSuit(i);
            int found = 0; // false
            while (node != NULL)
            {
                if (node->card.num == num && node->card.suit == suit)
                {
                    found = 1;
                    break; // Found specific card, break to movo onto next card
                }
                node = node->next;
            }

            // Card is not found in file
            if (found == 0)
            {
                card *missing = (struct node *)malloc(sizeof(struct node));
                missing->num = num;
                missing->suit = suit;
                return missing;
            }
        }
    }

    // All cards are found
    return NULL;
}

// Check if all cards have valid number and suit
int checkValidityOfCards(node *head)
{
    node *node = head;
    int line = 1;
    while (node != NULL)
    {
        for (int i = 1; i <= 4; i++)
        {
            int found = 0;
            char suit = findSuit(i);
            for (int j = 1; j <= 13; j++)
            {
                char num = findCardNo(j);
                if (node->card.num == num && node->card.suit == suit)
                {
                    goto found;
                }
            }
        }

        // Returns line of first occuring error
        return line;

    found:
        line++;
        node = node->next;
    }
    return 0;
}

node *findLastCard(node *head)
{
    node *tmp = head;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    return tmp;
}

// Helper function to shuffle
node *findSecondLast(node *head)
{
    node *tmp = head;
    while (tmp->next->next != NULL)
    {
        tmp = tmp->next;
    }
    return tmp;
}

// Reads a deck of cards from the specified file
node *readFromFile(FILE *fptr)
{
    node *head = NULL;
    node *current = NULL;
    // Load content into line
    char line[500]; // Need to contain full file

    while ((fgets(line, sizeof(2), fptr)))
    {

        if (head == NULL)
        {
            head = (struct node *)malloc(sizeof(struct node));
            head->card.num = line[0];
            head->card.suit = line[1];
            head->next = NULL;
            current = head;
        }
        else
        {
            // Allocate memory
            current->next = (struct node *)malloc(sizeof(struct node));
            current->next->card.num = line[0];
            current->next->card.suit = line[1];
            current->next->next = NULL;
            current = current->next;
        }
    }
    return head;
}

// Get cards from specified source
node *getCards(char *filename, char *message)
{
    // If no filname
    if (strcmp(filename, "") == 0)
    {
        // Create new deck of cards
        return initializeCards();
    }

    FILE *fptr = fopen(filename, "r");
    node *head;
    if (fptr != NULL)
    {
        head = readFromFile(fptr);
        fclose(fptr);
    }
    else
    {
        return NULL;
    }
    int dub = checkDuplicates(head);
    if (dub != 0)
    {
        sprintf(message, "%s%d%s%s", "Duplicate on line: ", dub, " in file ", filename);
        goto error;
    }

    card *mis = checkForMissingCards(head);
    if (mis != NULL)
    {
        sprintf(message, "%s%c%c%s%s", "Following card is missing: ", mis->num, mis->suit, " in file: ", filename);
        free(mis);
        goto error;
    }

    int valid = checkValidityOfCards(head);
    if (valid != 0)
    {
        printf("%s%d", "Invalid card on line: ", valid);
        goto error;
    }

    sprintf(message, "OK");
    return head;
// Return line with error if there is an error
error:
    return NULL;
}

void printEmptyTable()
{
    // HEADER
    int i = 0;
    int j = 0;
    int k = 1;
    for (int i = 0; i < 50; i++)
    {
        if (i % 7 == 0)
        {
            j++;
            if (j % 2 == 0)
            {
                printf("\t%s\t%c%d", "[]", 'F', k);
                k++;
            }
            printf("\n");
        }
        printf(" \t");
    }
}

void printHeader()
{
    printf("\n\n");
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7");
    printf("\n");
}

void printFooter(char *lastInput, char *message)
{
    printf("\n\n");
    printf("LAST Command: %s\n", lastInput);
    printf("Message: %s\n", message);
    printf("INPUT > ");
}

/// @brief
/// @param head
/// @param showCards 0 false, 1 true
void printDeck(node *head, int showCards)
{
    // BOARD
    int i = 0;
    int j = 0;
    int k = 1;
    node *current = head;
    while (current != NULL)
    {
        if (i % 7 == 0)
        {
            j++;
            if (j % 2 == 0)
            {
                printf("\t%s\t%c%d", "[]", 'F', k);
                k++;
            }
            printf("\n");
        }

        if (showCards == 0)
        {
            printf("%s\t", "[]");
        }
        else
        {
            printf("%c%c\t", current->card.num, current->card.suit);
        }

        i++;
        current = current->next;
    }
}

/// @brief helper method for randomShuffleCards
/// @param shuffledDeck the new deck with shuffled cards
/// @param card the card to add to shuffledDeck
/// @param numberOfNodes number of nodes in shuffledDeck
/// @return
node *randomInsertNode(node *shuffledDeck, node *card, int numberOfNodes)
{
    int nodePos = (rand() % (numberOfNodes + 1)); // Find random position to the new card
    node *current = shuffledDeck;
    node *next;
    node *newCard = card;

    // Add the node as the first card
    if (nodePos == 0)
    {
        newCard->next = shuffledDeck;
        return newCard;
    }

    for (int i = 0; i <= numberOfNodes; i++)
    {
        if (i + 1 == nodePos)
        {

            next = current->next;
            current->next = newCard;
            newCard->next = next;
            return shuffledDeck;
        }
        current = current->next;
    }
}

// Not too pretty, but works
node *splitShuffleCards(node *head, int split)
{
    // Random split
    if (split == 0)
    {
        split = (rand() % 51) + 1;
    }

    node *stack1;
    node *stack2;
    node *stack3 = NULL;

    node *tmp = head;
    // Find last node before split
    for (int i = 0; i < split - 1; i++)
    {
        tmp = tmp->next;
    }
    stack2 = tmp->next;
    tmp->next = NULL;
    stack1 = head;

    for (int i = 0; i < 52; i++)
    {
        if (stack1->next == NULL)
        {
            if (stack3 == NULL)
            {
                stack3 = stack1;
            }
            else
            {
                tmp = findLastCard(stack3);
                tmp->next = stack1;
            }
            stack1->next = stack2;
            break;
        }

        if (stack2->next == NULL)
        {
            if (stack3 == NULL)
            {
                stack3 = stack2;
            }
            else
            {
                tmp = findLastCard(stack3);
                tmp->next = stack2;
            }
            stack2->next = stack1;
            break;
        }

        // First card
        if (i == 0)
        {
            tmp = findSecondLast(stack2);
            if (tmp->next == NULL)
            {
                stack3 = tmp;
            }
            else
            {
                stack3 = tmp->next;
                tmp->next = NULL;
            }
        }
        else if (i % 2 == 0)
        {
            node *upper = findLastCard(stack3);
            tmp = findSecondLast(stack2);
            if (tmp->next == NULL)
            {
                upper->next = tmp;
                upper->next->next = stack1;
                break;
            }
            else
            {
                upper->next = tmp->next;
                tmp->next = NULL;
            }
        }
        else
        {
            node *upper = findLastCard(stack3);
            tmp = findSecondLast(stack1);
            if (tmp->next == NULL)
            {
                upper->next = tmp;
                upper->next->next = stack2;
                break;
            }
            else
            {
                upper->next = tmp->next;
                tmp->next = NULL;
            }
        }
    }
    return stack3;
}

node *randomShuffleCards(node *head)
{
    node *shuffledDeck = NULL;

    // Add first card
    shuffledDeck = head;
    head = head->next;
    shuffledDeck->next = NULL;

    int numberOfCards = 1; // Number of nodes in shuffled deck

    while (head != NULL)
    {
        node *card = head;
        head = head->next;
        shuffledDeck = randomInsertNode(shuffledDeck, card, numberOfCards);
        numberOfCards++;
    }
    return shuffledDeck;
}

void playGame(node *head)
{
    node *cards;
    char lastCommand[2];
    char message[100];
}

void getInput(char *input, char *command, char *arg, int size)
{

    fgets(input, size, stdin);

    // Split input and filename
    for (int i = 0; i < 2; i++)
    {
        command[i] = input[i];
    }

    for (int i = 0; i < 40; i++)
    {
        int j = i + 3; // Skip space
        if (input[j] == '\n')
        {
            break;
        }
        arg[i] = input[j];
    }
}

/// @brief
/// @param cards
/// @param lastCommand
/// @param message
/// @param showCards 0 false, 1 true
void printBoard(node *cards, char *lastCommand, char *message, int showCards)
{
    printHeader();
    if (cards == NULL)
    {
        printEmptyTable();
    }
    else
    {
        printDeck(cards, showCards);
    }

    printFooter(lastCommand, message);
}

// https://stackoverflow.com/questions/30678905/what-is-the-proper-equivalent-of-whiletrue-in-plain-c
#define EVER \
    ;        \
    ;
int main()
{
    // Seed for random function
    srand(time(NULL));
    char *lastCommand = "";
    char *message = (char *)malloc(256);
    sprintf(message, "");

    // First print
    printBoard(NULL, lastCommand, message, 0);

    // Input
    char input[50];
    char command[3];
    char arg[47];

    getInput(input, command, arg, sizeof(input));
    lastCommand = command;
    int showCards = 0;

    node *cards = NULL;
    // Funny bit of code - equivalent of while(true)
    for (EVER)
    {
        if (strcmp(command, "ld") == 0)
        {
            cards = getCards(arg, message);
        }
        else if (strcmp(command, "si") == 0)
        {
            if (cards != NULL)
            {
                int split;
                sscanf(arg, "%d", &split);
                cards = splitShuffleCards(cards, split);
                message = "Ok";
            }
            else
            {
                message = "No cards loaded";
            }
        }
        else if (strcmp(command, "sw") == 0)
        {
            showCards = 1;
        }
        else if (strcmp(command, "sr") == 0)
        {
            if (cards != NULL)
            {
                cards = randomShuffleCards(cards);
                message = "OK";
            }
            else
            {
                message = "No cards loaded";
            }
        }
        else if (strcmp(command, "sd") == 0)
        {
            // Not implemented
        }
        else if (strcmp(command, "qq") == 0)
        {
            printf("GG, Loser");
            return 0;
        }
        else
        {
            // Unknown command
            lastCommand = command;
            message = "Unknown command";
        }

        printBoard(cards, lastCommand, message, 1);
        getInput(input, command, arg, sizeof(input));
        showCards = 0;
    }
    return 0;
}
