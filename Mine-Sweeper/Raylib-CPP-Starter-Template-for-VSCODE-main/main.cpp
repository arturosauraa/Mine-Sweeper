#include <raylib.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <random>

const int screenWidth = 400;
const int screenHeight = 500;
const int cellSize = 40;
const int boardSize = 10;
const int mineCount = 10;
bool gameOver = false;
bool gameWon = false;
int score = 0;

struct Cell {
    bool mine;
    bool revealed;
    int surroundingMines;
};

std::vector<std::vector<Cell>> board(boardSize, std::vector<Cell>(boardSize));
bool gameLost = false;

void PlantMines() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, boardSize - 1);

    int plantedMines = 0;
    while (plantedMines < mineCount) {
        int x = distrib(gen);
        int y = distrib(gen);
        if (!board[y][x].mine) {
            board[y][x].mine = true;
            plantedMines++;
        }
    }
}

void CalculateSurroundingMines() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (!board[i][j].mine) {
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        int newX = j + k;
                        int newY = i + l;
                        if (newX >= 0 && newX < boardSize && newY >= 0 && newY < boardSize) {
                            if (board[newY][newX].mine)
                                board[i][j].surroundingMines++;
                        }
                    }
                }
            }
        }
    }
}

void DrawBoard() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Rectangle cell = { j * cellSize, i * cellSize, cellSize, cellSize };
            if (board[i][j].revealed) {
                DrawRectangle(cell.x, cell.y, cell.width, cell.height, WHITE);
                DrawRectangleLines(cell.x, cell.y, cell.width, cell.height, BLACK);
                if (board[i][j].mine) {
                    DrawText("*", cell.x + 9, cell.y + 5, cellSize - 5, RED);
                } else {
                    if (board[i][j].surroundingMines > 0) {
                        DrawText(std::to_string(board[i][j].surroundingMines).c_str(), cell.x + 9, cell.y + 5, cellSize - 5, BLACK);
                    }
                }
            }
            else {
                DrawRectangle(cell.x, cell.y, cell.width, cell.height, GRAY);
                DrawRectangleLines(cell.x, cell.y, cell.width, cell.height, DARKGRAY);
            }
        }
    }
}

int cellsRevealed = 0;
int totalNumberCells = boardSize * boardSize - mineCount;

void RevealCell(int x, int y) {
    if (x < 0 || x >= boardSize || y < 0 || y >= boardSize || board[y][x].revealed)
        return;

    board[y][x].revealed = true;
    cellsRevealed++;

    if (board[y][x].surroundingMines == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                RevealCell(x + i, y + j);
            }
        }
    }
    if (board[y][x].revealed && board[y][x].surroundingMines > 0 && board[y][x].surroundingMines <= 4) {
        if(!board[y][x].mine){
        score += board[y][x].surroundingMines;
        }
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Minesweeper");
    SetTargetFPS(60);

    PlantMines();
    CalculateSurroundingMines();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBoard();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = GetMouseX() / cellSize;
            int y = GetMouseY() / cellSize;
            if (board[y][x].mine) {
                RevealCell(x, y);
                gameOver = true;
            } else {
                RevealCell(x, y);
                if (cellsRevealed == totalNumberCells) {
                    gameWon = true;
                }
            }
        }
        if(gameOver)
        {
            DrawText("GAME OVER", GetScreenWidth()/2, GetScreenHeight() - 70, 30, BLACK);
        }
        if(gameWon)
        {
            DrawText("YOU WON", GetScreenWidth()/2, GetScreenHeight() - 70, 30, BLACK); 
        }
        DrawRectangle(10, GetScreenHeight() - 80, 170, 50, BLUE);
        DrawText("SCORE: ", 20, GetScreenHeight() - 70, 30, BLACK); 
        DrawText(TextFormat("%i", score), 140, GetScreenHeight() - 70, 30, BLACK); 

        EndDrawing();
    }

    CloseWindow();
    return 0;
}