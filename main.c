#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define GRID_SIZE 8
#define HEIGHT 550
#define WIDTH 512

Vector2 start;
Vector2 target;


typedef enum {
	EMPTY,
	WALL ,
	START ,
	TARGET 
}state;

typedef struct {
	
	Vector2 pos;
	state state;
	Color col;
	int f;
	bool isCalculated;
	bool way;

}setGrid;



setGrid grid[GRID_SIZE][GRID_SIZE];

void DrawGrids() {
	
	for (int j = 0; j < GRID_SIZE; j++)
	{
		for (int i = 0; i < GRID_SIZE; i++) {

			
			grid[j][i].pos.x = i * GRID_SIZE*GRID_SIZE;
			grid[j][i].pos.y = j * GRID_SIZE * GRID_SIZE;
			
			if (grid[j][i].f == 0) {
				DrawText(TextFormat("%d", grid[j][i].f), grid[j][i].pos.x + 2, grid[j][i].pos.y + 2, 10, LIGHTGRAY);
			}
			else {
				if (grid[j][i].way) {
					DrawText(TextFormat("%d", grid[j][i].f), grid[j][i].pos.x + 2, grid[j][i].pos.y + 2, 20, PINK);
					
				}
				else {
					DrawText(TextFormat("%d", grid[j][i].f), grid[j][i].pos.x + 2, grid[j][i].pos.y + 2, 17, GREEN);
					
				}
				
			}
			
			DrawRectangleLines(i * GRID_SIZE * GRID_SIZE, j * GRID_SIZE * GRID_SIZE, GRID_SIZE * GRID_SIZE, GRID_SIZE * GRID_SIZE, BLACK);

		}

	}
	
}




bool founded;
int lowesF = 1000;
void StartFind(Vector2 base) {
	
	int searchedCell = 1;
	setGrid* way = (setGrid*)malloc(sizeof(setGrid));
	Vector2 newBase;

	for (int row = base.x - 1; row < base.x + 2; row++)
	{
		for (int col = base.y - 1; col < base.y + 2; col++)
		{

			if (row >= 0 && col >= 0 && row < GRID_SIZE && col < GRID_SIZE) {
				if (grid[row][col].state == EMPTY && !grid[row][col].isCalculated) {

					

					float g = 0;
					if (base.x != row && base.y != col) {
						g += 14;
					}
					else {
						g += 10;
					}
					float h = (sqrt(pow(row - target.x,2) + pow(col - target.y, 2)))*10;
					float f = (g + h);
					grid[row][col].f = f;
					grid[row][col].isCalculated = true;

					if (f <= lowesF) {
						lowesF = f;
						newBase.x = row;
						newBase.y = col;

					}
					
					printf("**%d,%d ** %d\n", row, col, grid[row][col].f);
					
				}
				if (grid[row][col].state == TARGET) {
					founded = true;
				}

			}

		}
	}
	
	
	if (!founded) {
		
		grid[(int)newBase.x][(int)newBase.y].way = true;
		StartFind(newBase);
		
	}
	
}




void Clear() {
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++) {
			grid[j][i].state = EMPTY;
			grid[j][i].f = 0;
			grid[j][i].isCalculated = false;
			grid[j][i].way = false;
			grid[j][i].col = BLANK;
			lowesF = 1000;
			
		}
	}
	founded = false;
}

void SelectCell(Vector2 mouse) {
	
	static Color col;
	static state cell_state ;
	
	int keyCode = GetKeyPressed();

	switch (keyCode)
	{
	case KEY_ONE:col = SKYBLUE; cell_state = START; break;// start pos
	case KEY_TWO:col = BLACK; cell_state = WALL; break; // walls
	case KEY_THREE:col = RED; cell_state = TARGET; break;//target
	case KEY_FOUR:Clear(); break;//ERASER
	default:
		break;
	}
	
	int cellCol, cellRow;
	cellCol = ceil(mouse.x / (GRID_SIZE * GRID_SIZE)) - 1;
	cellRow = ceil(mouse.y / (GRID_SIZE * GRID_SIZE)) - 1;


	
	//Mouse Cell;
	Rectangle rec = { grid[cellRow][cellCol].pos.x, grid[cellRow][cellCol].pos.y, 63, 63 };
	DrawRectangleRec(rec, col);
	

	//Selected Cell
	if(IsMouseButtonPressed(0)){
		grid[cellRow][cellCol].state = cell_state;
		grid[cellRow][cellCol].col = col;
		if (cell_state == START) {
			start.x = cellRow;
			start.y = cellCol;
			printf("StartPos: %d, %d\n", cellRow, cellCol);
		}
		else if (cell_state == TARGET) {
			target.x = cellRow; target.y = cellCol; printf("TargetPos: %d, %d\n", cellRow, cellCol);
		}
		else if (cell_state == EMPTY)
		{
			grid[cellRow][cellCol].isCalculated = false;
			grid[cellRow][cellCol].f = 0;

		}
			
	}
	
	//Draw Selected Cells
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++) {
			if (grid[j][i].state != NULL) {
				DrawRectangle(grid[j][i].pos.x, grid[j][i].pos.y, 63, 63, grid[j][i].col);
			}
				
				
		}
	}
	
	
}



int main() {
	const int screenWidth = WIDTH;
	const int screenHeight = HEIGHT;
	

	InitWindow(screenWidth, screenHeight, "Test");
	SetTargetFPS(30);

	//Main Loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		Vector2 mouse = GetMousePosition();
		char* mousePos = TextFormat("[x:%.2f, y:%.2f]", mouse.x, mouse.y);
	
	
		DrawText(mousePos, 10, screenWidth+(screenHeight-screenWidth)/2-5, 20, DARKGRAY);
		DrawGrids();
		SelectCell(mouse);
		
		if (IsKeyPressed(KEY_SPACE)) {
			
			StartFind(start);
		}
	
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}