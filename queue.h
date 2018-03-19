struct Point{
    int x;
    int y;
};

struct queueNode{
    struct Point p;
    int prevNode;
};

struct queueNode q[40];
int end = -1;

struct queueNode get(int i) {
    return q[i];
}

void push(struct queueNode node) {
    q[++end] = node;
}

int pointToLocation(struct Point p){
    return p.x * 2 + p.y / 2;
}

void getPath(struct queueNode dest, int path[]){
    struct queueNode n = dest;
    int j = 0;
    while(n.prevNode > -1){
        path[j++] = pointToLocation(n.p);
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
