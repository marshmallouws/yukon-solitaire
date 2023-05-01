#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef struct table
{
    column col1;
    column col2;
    column col3;
    column col4;
    column col5;
    column col6;
    column col7;
} table;

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
                // Allocate memory
                head = (struct node *)malloc(sizeof(struct node));
                head->card.num = num;
                head->card.suit = suit;
                head->next = NULL;
            }
            else
            {
                // Find next node
                node *curr = head;
                while (curr->next != NULL)
                {
                    curr = curr->next;
                }

                // Allocate memory
                curr->next = (struct node *)malloc(sizeof(struct node));
                curr->next->card.num = num;
                curr->next->card.suit = suit;
                curr->next->next = NULL;
            }
        }
    }
    return head;
}

// Print all cards
void printCards(node *node)
{
    int i = 1;
    while (node != NULL)
    {
        printf("%d%c%c%c\n", i, ':', node->card.num, node->card.suit);
        node = node->next;
        i++;
    }
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

    // Checking if each card exists in linked list
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
                    break;
                }
                node = node->next;
            }

            if (found == 0)
            {
                card *missing;
                missing->num = num;
                missing->suit = suit;
                return missing;
            }
            j++;
        }
        i++;
    }

    // All cards are found
    return NULL;
}

// Finds the last node in a linked list
node *findLastNode(node *head)
{
    node *curr = head;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    return head;
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

// Reads a deck of cards from the specified file
node *readFromFile(FILE *fptr)
{
    node *head = NULL;
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
        }
        else
        {
            // Find next node
            node *curr = head;
            while (curr->next != NULL)
            {
                curr = curr->next;
            }

            // Allocate memory
            curr->next = (struct node *)malloc(sizeof(struct node));
            curr->next->card.num = line[0];
            curr->next->card.suit = line[1];
            curr->next->next = NULL;
        }
    }
    return head;
}

// Get cards from specified source
node *getCards(char *filename)
{
    // If no filname
    if (strlen(filename) == 0)
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
        printf("%s%d", "Duplicate found on line: ", dub);
        goto error;
    }

    card *mis = checkForMissingCards(head);
    if (mis != NULL)
    {
        printf("%s%c%c", "Missing card in file: ", mis->num, mis->suit);
        free(mis);
        goto error;
    }

    int valid = checkValidityOfCards(head);
    if (valid != 0)
    {
        printf("%s%d", "Invalid card on line: ", valid);
        goto error;
    }

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
    printf("\n");
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7");
    printf("\n");
}

void printFooter(char *lastInput[], char *message[])
{
    printf("\n\n");
    printf("LAST Command: %s\n", lastInput);
    printf("Message: %s\n", message);
    printf("INPUT > ");
}

void printDeck(node *n)
{
    // HEADER
    printHeader();

    // BOARD
    int i = 0;
    int j = 0;
    int k = 1;
    node *current = n;
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
        printf("%c%c\t", current->card.num, current->card.suit);

        i++;
        current = current->next;
    }

    printFooter(NULL, NULL);
}

void playGame(node *head)
{
    node *cards;
    char lastCommand[2];
    char message[100];
}

// https://stackoverflow.com/questions/30678905/what-is-the-proper-equivalent-of-whiletrue-in-plain-c
#define EVER \
    ;        \
    ;
int main()
{

    char *lastInput = "";
    char *message = "";

    // First prompt
    printHeader();
    printEmptyTable();
    printFooter(lastInput, message);
    char input[3];
    scanf("%s", input);
    lastInput = input;

    // Funny bit of code - equivalent of while(true)
    for (EVER)
    {
        node *cards;
        if (strcmp(input, "ld") == 0)
        {
            cards = getCards("");
        }
        printHeader();
        if (cards != NULL)
        {
            printDeck(cards);
        }
        else
        {
            printEmptyTable();
        }
        printFooter(lastInput, message);

        scanf("%s", input);
    }

    char *ch = "\0";

    node *cards = getCards("");
    printDeck(cards);
    // playGame(head);
    return 0;
}
