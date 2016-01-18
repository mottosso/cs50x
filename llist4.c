/**
 * Linked list implementation
 *
 * The `root` is an immovable starting point for the list
 * whose value is always 0xff and points to the first item
 * in the list and its.
 *
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/**
 * Node
 *
 * A node of an 8-bit integer value.
 *
*/
typedef struct Node
{
    uint8_t value;
    struct Node* next;
} Node;


/**
 * Print a linked list
*/
void print(Node* root)
{
    Node* current = root->next;

    while(current)
    {
        printf("%i\n", current->value);
        current = current->next;
    }
}

/**
 * Free memory used by a linked list
*/
void cleanup(Node* root)
{
    Node* current = root;

    while(current)
    {
        Node* tmp = current->next;
        free(current);
        current = tmp;
    }
}

/**
 * Return length of linked list
*/
int length(Node* root)
{
    Node* current = root->next;

    int len = 0;
    while (current != NULL)
    {
        len += 1;
        current = current->next;
    }

    return len;
}

/**
 * Reverse values in linked list
 *
 * Pros: Position of reference to an item is preserved; e.g.
 *       pointing to the next-to-last position in the list.
 *
 * Cons: Where the value is large, or where many values
 *       are present, physically moving the data can be an
 *       expensive operation.
 */
void reverseValues(Node* root)
{
    Node* current = root->next;

    // figure out length of the linked list
    int len = length(root);

    // convert linked list to plain array
    uint8_t* tmp = (uint8_t *) malloc(len);
    current = root->next;
    for (int i = 0; i < len; i++)
    {
        tmp[i] = current->value;
        current = current->next;
    }

    // reverse linked list
    current = root->next;
    for (int i = len - 1; i >= 0; i--)
    {
        current->value = tmp[i];
        current = current->next;
    }

    free(tmp);
}

/**
 * Reverse pointers in linked list
 */
void reverse(Node* root)
{
    Node* current = root->next;
    int len = length(root);

    // store all pointers in a plain array
    Node** tmp = (Node **) malloc(sizeof(Node*) * len);
    current = root->next;

    if (current == NULL)
    {
        return;
    }

    for (int i = 0; i < len; i++)
    {
        tmp[i] = current;
        current = current->next;
    }

    // reverse linked list
    current = tmp[len - 1];
    for (int i = len - 1; i >= 1; i--)
    {
        current->next = tmp[i - 1];
        current = tmp[i - 1];
    }

    // first node now points to nothing
    current->next = NULL;

    // and root points to the last node
    root->next = tmp[len - 1];

    free(tmp);
}

/**
 * From http://www.geeksforgeeks.org/write-a-function-to-reverse-the-nodes-of-a-linked-list/
 *
 * Wow, this is so concise!
 */
void reverse2(Node* root)
{
    Node* prev = NULL;
    Node* current = root->next;
    Node* next;

    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    root->next = prev;
}


/**
 * Append `item` to end of linked list
*/
void push(Node* root, uint8_t value)
{
    Node* last;
    Node* current = root->next;

    while(current != NULL)
    {
        last = current;
        current = current->next;
    }

    Node* item = (Node *) calloc(1, sizeof(Node));
    item->value = value;
    last->next = item;
}


/**
 * Fill linked list with consecutive numbers, starting from `amount`
*/
void fill(Node* root, int amount)
{
    Node* next;
    Node* previous = root;

    for (int i = 0; i < amount; i++)
    {
        next = (Node *) malloc(sizeof(Node));
        next->value = i;
        next->next = NULL;

        previous->next = next;
        previous = next;
    }
}

/**
 * Insert item at position, moving the item at position one index forward.
*/
void insert(Node* root, uint8_t value, int position)
{
    Node* item = (Node *) calloc(1, sizeof(Node));
    item->value = value;

    Node* current = root->next;

    if (position < 0)
    {
        printf("Error: Position must be positive\n");
        return;
    }

    if (position == 0)
    {
        item->next = current;
        root->next = item;
        return;
    }

    for (int i = 0; i < position; i++)
    {
        current = current->next;
        if (current == NULL)
        {
            printf("Error: Index %i not available "
                   "in array of length %i.\n",
                   position, length(root));
            return;
        }
    }

    item->next = current->next;    
    current->next = item;
}


/**
 * Discard last item
*/
void pop(Node* root)
{
    Node* current = root->next;
    while (current->next != NULL)
    {
        current = current->next;
    }
}

/**
 * Return new linked list containing items from `begin` to `end`
 *
 * E.g. [0, 1, 2, 3, 4]
 *         |_______| slice 1:4
*/
Node* slice(Node* root, int begin, int end)
{
    assert(begin > 0);
    assert((end - begin) > 0);

    Node* slice = (Node *) calloc(1, sizeof(Node) * (end - begin));
    Node* currentSlice = slice;
    Node* current = root->next;

    int count = 0;
    while (current != NULL)
    {
        if (count >= end)
        {
            break;
        }

        if (count >= begin)
        {
            Node* next = (Node *) malloc(sizeof(Node));
            next->value = current->value;

            currentSlice->next = next;
            currentSlice = next;
        }

        current = current->next;
        count += 1;
    }

    return slice;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./llist4 n\n");
        return 1;
    }

    int count = atoi(argv[1]);

    Node* root = (Node *) calloc(1, sizeof(Node));

    fill(root, count);

    printf("Printing linked list..\n");
    print(root);

    printf("Reversing values, starts with: %i\n", root->next->value);
    reverseValues(root);
    printf("Reversing pointers, starts with: %i\n", root->next->value);
    reverse(root);
    printf("Reversing pointers 2, starts with: %i\n", root->next->value);
    reverse2(root);
    reverse2(root);

    printf("Pushing 99 to end of list.\n");
    push(root, 99);

    printf("Inserting 55 after 4\n");
    insert(root, 55, -1); // error
    insert(root, 55, 12); // error
    insert(root, 55, 4);

    printf("Printing linked list..\n");
    print(root);

    printf("Slicing 1-4..\n");
    Node* part = slice(root, 1, 4);

    printf("Printing slice\n");
    print(part);

    printf("Cleaning up..\n");
    cleanup(root);
    cleanup(part);

    return 0;

    /* Output
    0
    1
    2
    3
    4
    5
    6
    7
    8
    9
    Reversing values, starts with: 0
    Reversing pointers, starts with: 9
    Reversing pointers 2, starts with: 0
    Pushing 99 to end of list.
    Inserting 55 after 4
    Error: Position must be positive
    Error: Index 12 not available in array of length 11.
    Printing linked list..
    0
    1
    2
    3
    4
    55
    5
    6
    7
    8
    9
    99
    Slicing 1-4..
    Printing slice
    1
    2
    3
    Cleaning up..

    */
}
