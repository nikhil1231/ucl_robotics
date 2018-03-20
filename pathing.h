const int TURN_STRENGTH = 57;

void buildPathTree(int map[][7], int path[]){
    int index = 0;

    int visitedCells[7][7];
    memset(visitedCells, 0, sizeof(visitedCells));
    visitedCells[0][0] = 1;

    struct queueNode startingNode = {{0,0}, -1};
    push(startingNode);

    while (1){
        struct queueNode curr = get(index);
        struct Point p = curr.p;

        if (p.x == 6 && p.y == 6){
            getPath(curr ,path);
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            int row, col;
            switch(i){
                case 0:
                    row = p.x + 1;
                    col = p.y;
                    break;
                case 1:
                    row = p.x - 1;
                    col = p.y;
                    break;
                case 2:
                    row = p.x;
                    col = p.y + 1;
                    break;
                case 3:
                    row = p.x;
                    col = p.y - 1;
                    break;
            }
            
            if (row >= 0 && row < 7 && col >= 0 && col < 7
                && (map[row][col] == 0) && !visitedCells[row][col])
            {
                visitedCells[row][col] = 1;
                struct queueNode cell = {{row, col}, index};
                push(cell);
            }
        }
        
        index ++;
    }
}

void movePath(int path[]){
    int i = 0,j;
    int tempDir = 0;
    int isModdedTurn = 0;
    int numStraight = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int lastTime = 0;

    while(path[i+1] >= 0){
        switch(path[i+1]-path[i]){
            case 4:
                j = - tempDir;
                if(j < -1) j = 1;
                break;
            case 1:
                j = 1 - tempDir;
                break;
            case -4:
                j = 2 - tempDir;
                break;
            case -1:
                j = 3 - tempDir;
                if(j > 1) j = -1;
                break;
        }
        // turn off altered movement if accelerated sufficiently
        if(numStraight == 1) isModdedTurn = 0;

        if(!j){
            numStraight ++;
        }else{
            numStraight = 0;
        }

        int speed = getSpeed(&lastTime,&tv);
        if(!i && j==1){
            isModdedTurn = 1;
            speed = -1;
            if(path[2] == 5){
                isModdedTurn = 2;
            }
        }

        if(isModdedTurn == 2){
            setTurnMove(j*TURN_STRENGTH, speed, 1400);
            tempDir = 0;
            setTurnMove(-j*TURN_STRENGTH, speed, 300);
            isModdedTurn = 0;
        }else if(isModdedTurn == 1){
            setTurnMove(j*TURN_STRENGTH*1.3, speed, 970);
        }else{
            setMove(j*TURN_STRENGTH, speed);
        }

        tempDir += j;
        if(tempDir < 0) tempDir += 4;
        tempDir %= 4;
        i++;
    }
    setMove(-2,getSpeed(&lastTime,&tv));
}

