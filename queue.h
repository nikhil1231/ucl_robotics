#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Point{
    int x;
    int y;
};

// a data structure for queue used in BFS
struct queueNode{
    struct Point pt;  // The cordinates of a cell
    int prevNode;
};

// implement queue:
#define MAX 100

struct queueNode q[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;


struct queueNode get(int i) {
    return q[i];
}

void push(struct queueNode data) {

    if(itemCount < MAX) {

        if(rear == MAX-1) {
            rear = -1;            
        }       

        q[++rear] = data;
        itemCount++;
    }
}

int pointToLocation(struct Point p){
    return p.x * 2 + p.y / 2;
}

void getPath(struct queueNode dest, int path[]){
    struct queueNode n = dest;
    int j = 0;
    while(n.prevNode > -1){
        path[j++] = pointToLocation(n.pt);
        n = get(n.prevNode);
        n = get(n.prevNode);
    }
    path[j] = 0;
    path[j+1] = -2;

    int i = 0;
    
    // flip array
    while(i < j){
        int temp = path[j];
        path[j] = path[i];
        path[i] = temp;
        i++;
        j--;
    }
}
