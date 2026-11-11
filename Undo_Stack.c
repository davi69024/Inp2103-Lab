#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ActionNode {
    int id;
    char actionType[10], text[100];
    struct ActionNode *next;
} ActionNode;

typedef struct {
    ActionNode *top;
    int count;
} UndoStack;

void initStack(UndoStack *s) { s->top = NULL; s->count = 0; }
int isEmpty(UndoStack *s) { return s->top == NULL; }

void push(UndoStack *s, int id, const char *type, const char *text) {
    ActionNode *n = malloc(sizeof(ActionNode));
    if (n == NULL) { printf("Memory allocation failed.\n"); return; }

    n->id = id;
    strcpy(n->actionType, type);
    strcpy(n->text, text);
    n->next = s->top;
    s->top = n;
    s->count++;
    printf("Action added to the Undo stack.\n");
}

int pop(UndoStack *s, ActionNode *outAction) {
    ActionNode *temp;
    if (isEmpty(s)) return 0;

    temp = s->top;
    *outAction = *temp;
    s->top = temp->next;
    free(temp);
    s->count--;
    return 1;
}

void printHistory(const UndoStack *s) {
    ActionNode *p = s->top;
    if (p == NULL) { printf("Undo history is empty.\n"); return; }

    printf("\nUndo history (newest to oldest):\n");
    while (p != NULL) {
        printf("ID: %d | Type: %s | Text: %s\n", p->id, p->actionType, p->text);
        p = p->next;
    }
}

ActionNode *findActionById(const UndoStack *s, int id) {
    ActionNode *p = s->top;
    while (p != NULL && p->id != id) p = p->next;
    return p;
}

void clearStack(UndoStack *s) {
    ActionNode *temp;
    while (s->top != NULL) {
        temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
    s->count = 0;
}

int main(void) {
    UndoStack stack;
    ActionNode removed, *found;
    int choice, id;
    char type[10], text[100];

    initStack(&stack);
    do {
        printf("\n=== TEXT EDITOR UNDO HISTORY ===\n");
        printf("1. Push an action (INSERT/DELETE)\n2. Undo (pop latest action)\n");
        printf("3. Display history\n4. Search action by ID\n5. Check whether stack is empty\n0. Exit\nChoose: ");

        if (scanf("%d", &choice) != 1) {
            choice = -1;
            while (getchar() != '\n');
        } else getchar();

        switch (choice) {
            case 1:
                printf("Action ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                    break;
                }
                getchar();

                printf("Action type (INSERT or DELETE): ");
                fgets(type, sizeof(type), stdin);
                type[strcspn(type, "\n")] = '\0';
                if (strcmp(type, "INSERT") != 0 && strcmp(type, "DELETE") != 0) {
                    printf("Type must be INSERT or DELETE.\n");
                    break;
                }

                printf("Text content: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';
                push(&stack, id, type, text);
                break;

            case 2:
                if (pop(&stack, &removed))
                    printf("Undo action: ID %d | %s | %s\n", removed.id, removed.actionType, removed.text);
                else printf("Nothing to undo: stack is empty.\n");
                break;

            case 3:
                printHistory(&stack);
                break;

            case 4:
                printf("Enter action ID to find: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                    break;
                }
                getchar();
                found = findActionById(&stack, id);
                if (found) printf("Found: ID %d | %s | %s\n", found->id, found->actionType, found->text);
                else printf("Action ID %d was not found.\n", id);
                break;

            case 5:
                printf(isEmpty(&stack) ? "The stack is empty.\n" : "The stack is not empty.\n");
                break;

            case 0:
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    clearStack(&stack);
    printf("All allocated memory has been freed. Goodbye.\n");
    return 0;
}
