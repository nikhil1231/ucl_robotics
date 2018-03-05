#include <stdio.h>
#include <stdlib.h>

typedef struct list_node{
    int loc;
    struct list_node *next;
} ListNode;

int getLength(ListNode* h){
    int i = 0;
    ListNode* c = h;
    while(c != NULL){
        i++;
        c = c->next;
    }
    return i;
}

void printList(ListNode* h){
    printf("[");
    while(h != NULL){
        printf("%i, ", h->loc);
        h = h->next;
    }
    printf("]\n");
}
