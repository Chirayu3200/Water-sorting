#include <graphics.h>
#include <stdlib.h>
#include <time.h>

const int COLORS[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, LIGHTGREEN, LIGHTBLUE, LIGHTCYAN};
const int COLOR_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);

int selectedTube = -1;

// Function to draw a single U-shaped test tube with boxes
void drawTestTubeWithBoxes(int x, int y, int width, int height, int colors[], int boxCount, bool isSelected) {
    setcolor(BLACK);
    rectangle(x, y, x + width, y + height);
    arc(x + width / 2, y + height, 0, 180, width / 2);
    line(x, y + height, x + width, y + height);

    int boxHeight = height / boxCount;
    for (int i = 0; i < boxCount; i++) {
        if (colors[i] != WHITE) {
            setfillstyle(SOLID_FILL, colors[i]);
        } else {
            setfillstyle(SOLID_FILL, WHITE);
        }
        bar(x + 1, y + i * boxHeight + 1, x + width - 1, y + (i + 1) * boxHeight - 1);
        
        if (isSelected && i == 0 && colors[i] != WHITE) {
            setcolor(DARKGRAY);
            setlinestyle(SOLID_LINE, 1, 3); // Make the outline thicker
        } else {
            setcolor(BLACK);
            setlinestyle(SOLID_LINE, 1, 1);
        }
        rectangle(x + 1, y + i * boxHeight + 1, x + width - 1, y + (i + 1) * boxHeight - 1);
    }
}

// Function to shuffle an array
void shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Function to handle user input for moving boxes
void moveTopBox(int tubes[7][6], int boxCount, int fromTube, int toTube) {
    if (fromTube == toTube) {
        return; // Prevent moving within the same tube
    }

    int fromBox = -1, toBox = -1;

    for (int i = 0; i < boxCount; i++) {
        if (tubes[fromTube][i] != WHITE) {
            fromBox = i;
            break;
        }
    }

    for (int i = boxCount - 1; i >= 0; i--) {
        if (tubes[toTube][i] == WHITE) {
            toBox = i;
            break;
        }
    }

    if (fromBox != -1 && toBox != -1) {
        tubes[toTube][toBox] = tubes[fromTube][fromBox];
        tubes[fromTube][fromBox] = WHITE;
    }
}


// Function to get the test tube index based on mouse click
int getTestTubeIndex(int x, int y, int tubeCount) {
    for (int i = 0; i < tubeCount; i++) {
        int tubeX = 50 + i * 100;
        if (x >= tubeX && x <= tubeX + 50 && y >= 150 && y <= 350) {
            return i;
        }
    }
    return -1;
}

// Function to draw all test tubes
void drawAllTestTubes(int tubes[7][6], int tubeCount, int boxCount, int width, int height) {
    cleardevice();
    for (int i = 0; i < tubeCount; i++) {
        int x = 50 + i * 100;
        int y = 150;
        drawTestTubeWithBoxes(x, y, width, height, tubes[i], boxCount, i == selectedTube);
    }
}

// Function to check if all tubes are complete
bool checkAllComplete(int tubes[7][6], int tubeCount, int boxCount) {
    int completeCount = 0;
    int emptyCount = 0;

    for (int i = 0; i < tubeCount; i++) {
        int color = tubes[i][0];
        bool isComplete = true;
        bool isEmpty = true;

        for (int j = 0; j < boxCount; j++) {
            if (tubes[i][j] != color || color == WHITE) {
                isComplete = false;
            }
            if (tubes[i][j] != WHITE) {
                isEmpty = false;
            }
        }

        if (isComplete) {
            completeCount++;
        }

        if (isEmpty) {
            emptyCount++;
        }
    }

    return (completeCount == tubeCount - 1 && emptyCount == 1);
}

// Function to display congratulations message
void displayCongratulations() {
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(100, 50, (char*)"Congratulations! You solved the puzzle!\nEnter any key to continue.");
}

// Function to display options
void displayOptions() {
    setcolor(BLACK);
    rectangle(100, 100, 500, 150);
    outtextxy(120, 115, (char*)"Play Again");

    rectangle(100, 200, 500, 250);
    outtextxy(120, 215, (char*)"Exit");
}

// Function to handle options click
int handleOptionsClick(int x, int y) {
    if (x >= 100 && x <= 500 && y >= 100 && y <= 150) {
        return 1; // Play Again
    } else if (x >= 100 && x <= 500 && y >= 200 && y <= 250) {
        return 2; // Exit
    }
    return 0; // No option selected
}

// Function to display welcome screen and level selection
int displayWelcomeAndLevelSelection() {
    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(200, 50, (char*)"Welcome Player!");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    rectangle(200, 150, 500, 200);
    outtextxy(250, 160, (char*)"Level 1");

    rectangle(200, 250, 500, 300);
    outtextxy(250, 260, (char*)"Level 2");

    rectangle(200, 350, 500, 400);
    outtextxy(250, 360, (char*)"Level 3");

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= 200 && x <= 500) {
                if (y >= 150 && y <= 200) {
                    return 1; // Level 1
                } else if (y >= 250 && y <= 300) {
                    return 2; // Level 2
                } else if (y >= 350 && y <= 400) {
                    return 3; // Level 3
                }
            }
        }
    }
}

int main() {
    int gd = DETECT, gm;
    initwindow(1500, 900, "Color Puzzle");

    while (true) {
        setbkcolor(WHITE);
        cleardevice();

        int level = displayWelcomeAndLevelSelection();

        int tubeCount = 0;
        int boxCount = 0;
        if (level == 1) {
            tubeCount = 5;
            boxCount = 4;
        } else if (level == 2) {
            tubeCount = 6;
            boxCount = 5;
        } else if (level == 3) {
            tubeCount = 7;
            boxCount = 6;
        }

        int tubes[7][6] = {WHITE};
        int width = 50, height = 200;

        srand(time(NULL));

        int allColors[7 * 6 - 6];
        for (int i = 0; i < tubeCount - 1; i++) {
            for (int j = 0; j < boxCount; j++) {
                allColors[i * boxCount + j] = COLORS[j % COLOR_COUNT];
            }
        }
        shuffle(allColors, tubeCount * boxCount - boxCount);

        int colorIndex = 0;
        for (int i = 0; i < tubeCount - 1; i++) {
            for (int j = 0; j < boxCount; j++) {
                tubes[i][j] = allColors[colorIndex++];
            }
        }
        for (int j = 0; j < boxCount; j++) {
            tubes[tubeCount - 1][j] = WHITE;
        }

        drawAllTestTubes(tubes, tubeCount, boxCount, width, height);

        bool gameWon = false;

        while (true) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);

                int tubeIndex = getTestTubeIndex(x, y, tubeCount);

                if (tubeIndex != -1) {
                    if (selectedTube == -1) {
                        for (int i = 0; i < boxCount; i++) {
                            if (tubes[tubeIndex][i] != WHITE) {
                                selectedTube = tubeIndex;
                                break;
                            }
                        }
                    } else {
                        moveTopBox(tubes, boxCount, selectedTube, tubeIndex);
                        selectedTube = -1;
                    }
                    drawAllTestTubes(tubes, tubeCount, boxCount, width, height);

                    if (checkAllComplete(tubes, tubeCount, boxCount)) {
                        displayCongratulations();
                        gameWon = true;
                        break;
                    }
                }
            }

            if (kbhit()) {
                char ch = getch();
                if (ch == 'q' || ch == 'Q') {
                    closegraph();
                    return 0;
                }
            }
        }

        if (gameWon) {
            getch();
            cleardevice();
            displayCongratulations();
            displayOptions();

            bool optionSelected = false;
            while (!optionSelected) {
                if (ismouseclick(WM_LBUTTONDOWN)) {
                    int x, y;
                    getmouseclick(WM_LBUTTONDOWN, x, y);

                    int option = handleOptionsClick(x, y);
                    if (option == 1) {
                        optionSelected = true;
                    } else if (option == 2) {
                        closegraph();
                        return 0;
                    }
                }
            }
        }
    }

    closegraph();
    return 0;
}