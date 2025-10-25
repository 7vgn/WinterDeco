/**
 * @file programs.c
 * @date 2025-10-21
 * @brief Implements programs.h
 */

#include<xc.h>
#include<stdio.h>
#include"led.h"
#include"programs.h"

// Dummy functions that do nothing
void nullInit() {}
void nullUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS]) {}

// VERY simple (and terrible) PRNG
uint8_t random()
{
	static uint8_t seed;
	seed = (seed * 35 + 87);
	return seed;
}

//-----------------------------------------------------------------------------
// Program: Typewriter

void typewriterInit() {}

void typewriterUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS])
{
	// Act only every 200ms
	if(clk % 20 != 0)
		return;
	
	static uint8_t page[8][8] =
	{
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};
	// Current line (shown at the bottom)
	static uint8_t line = 0;
	// Current column
	static uint8_t col = 0;
	
	// Typing action
	uint8_t rand = random() % 100;
	if(rand <= 5						// 5% chance
		|| col == 8)					// Always at end of line
	{
		// New line
		line = (line + 1) % 8;
		for(uint8_t i = 0; i < 8; i++)
			page[line][i] = 0;
		// Carriage return
		col = 0;
	}
	else if(rand <= 30					// 30% chance
		&& col > 0						// But not at start of line
		&& page[line][col - 1] != 0)	// And not twice in a row
	{
		// Type a space
		col++;
	}
	else
	{
		// Type a character
		page[line][col++] = 255;
	}
	
	// Draw everything
	for(uint8_t y = 0; y < 8; y++)
		for(uint8_t x = 0; x < 8; x++)
			ledSet(x, y, page[(y + line + 1) % 8][x]);
}

//-----------------------------------------------------------------------------
// Program: Matrix

void matrixInit() {}

void matrixUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS])
{
	// Act only every 100ms
	if(clk % 10 != 0)
		return;
	
	// Store position of flare in each column, 12 if none
	static uint8_t flares[8] = {12, 12, 12, 12, 12, 12, 12, 12};
	// Flare intensity
	static const uint8_t INTENSITY[5] = {255, 150, 100, 50, 10};
	
	// Advance and reset flares
	for(uint8_t x = 0; x < 8; x++)
	{
		if(flares[x] < 12)
			flares[x]++;
		else if(random() <= 40) // 40% chance to reset the flare
			flares[x] = 0;
	}
	
	// Draw everything
	for(uint8_t y = 0; y < 8; y++)
		for(uint8_t x = 0; x < 8; x++)
			ledSet(x, y, y + 4 >= flares[x] && y <= flares[x] ? INTENSITY[flares[x] - y] : 0);
}

//-----------------------------------------------------------------------------
// Program: Bouncy

// Current coordinates
// 0 corresponds to the first LED in a row or column and 255 to the last one. 
static uint8_t bouncyX, bouncyY;

// Current velocity
// Velocity is in coordinate points per tick.
static int8_t bouncyVX, bouncyVY;

// Length of the velocity vector
static const int8_t BOUNCY_VELOCITY = 10;

void bouncyRollVelocity()
{
	// Choose random x component between -BOUNCY_VELOCITY and +BOUNCY_VELOCITY
	bouncyVX = (int8_t)(random() % (2 * BOUNCY_VELOCITY)) - BOUNCY_VELOCITY;
	
	// Calculate the y component to get the desired total length of the vector
	bouncyVY = 0;
	// In this case, a loop is faster than other methods for computing the square root
	while((bouncyVY + 1) * (bouncyVY + 1) + bouncyVX * bouncyVX <= BOUNCY_VELOCITY * BOUNCY_VELOCITY)
		bouncyVY++;
	// Random sign
	if(random() % 2 == 0)
		bouncyVY = -bouncyVY;
}

void bouncyInit()
{
	// Random starting point
	bouncyX = random(); bouncyY = random();
	// Random initial velocity vector
	bouncyRollVelocity();
}

void bouncyUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS])
{
	// When center button was pressed, choose a new random velocity vector
	if(events[BTN_CENTER] == EVENT_RELEASE_SHORT)
	{
		bouncyRollVelocity();
		events[BTN_CENTER] = EVENT_NONE;
	}
	
	// Act only every 50ms
	if(clk % 5 != 0)
		return;
	
	// Move in x direction
	if(bouncyVX < 0 && bouncyX < (uint8_t)-bouncyVX)
	{
		bouncyX = (uint8_t)-bouncyVX - bouncyX;
		bouncyVX = -bouncyVX;
	}
	else if(bouncyVX > 0 && bouncyX > 255 - bouncyVX)
	{
		bouncyX = (255 - bouncyX) + (255 - (uint8_t)bouncyVX);
		bouncyVX = -bouncyVX;
	}
	else
		bouncyX += bouncyVX;
	
	// Move in y direction
	if(bouncyVY < 0 && bouncyY < (uint8_t)-bouncyVY)
	{
		bouncyY = (uint8_t)-bouncyVY - bouncyY;
		bouncyVY = -bouncyVY;
	}
	else if(bouncyVY > 0 && bouncyY > 255 - bouncyVY)
	{
		bouncyY = (255 - bouncyY) + (255 - (uint8_t)bouncyVY);
		bouncyVY = -bouncyVY;
	}
	else
		bouncyY += bouncyVY;
	
	// Draw
	// LED coordinates scaled up to the [0,255] space
	static const int16_t ledCoords[8] = {0, 36, 73, 109, 146, 182, 219, 255};
	ledSetAll(0);
	// Go through all LEDs that are no more than 2 away from the current
	// position in the x and y direction
	uint8_t ix = (uint8_t)((uint16_t)bouncyX * 7 / 255);
	uint8_t iy = (uint8_t)((uint16_t)bouncyY * 7 / 255);
	for(uint8_t y = iy >= 2 ? iy - 2 : 0; y <= iy + 2 && y < 8; y++)
	{
		for(uint8_t x = ix >= 2 ? ix - 2 : 0; x <= ix + 2 && x < 8; x++)
		{
			// Squared distance to this LED
			int16_t dist2 = (ledCoords[x] - (int16_t)bouncyX) * (ledCoords[x] - (int16_t)bouncyX)
			              + (ledCoords[y] - (int16_t)bouncyY) * (ledCoords[y] - (int16_t)bouncyY);
			// Ball radius: 1.78 LEDs (65 units)
			if(dist2 <= 65 * 65)
				ledSet(x, y, (uint8_t)((65 * 65 - dist2) / 17/*17=65²/255*/));
			// Ball radius: 2 LEDs (72.86 units)
			//if(dist2 <= 5308)
			//	ledSet(x, y, (uint8_t)((5308 - dist2) / 21));
		}
	}
}

//-----------------------------------------------------------------------------
// Program: New Year

void newyearInit()
{
	// Draw the "2"
	ledSetAll(0);
	ledSet(0, 2, 255);
	ledSet(1, 1, 255);
	ledSet(2, 1, 255);
	ledSet(3, 2, 255);
	ledSet(2, 3, 255);
	ledSet(1, 4, 255);
	ledSet(0, 5, 255);
	ledSet(0, 6, 255);
	ledSet(1, 6, 255);
	ledSet(2, 6, 255);
	ledSet(3, 6, 255);
}

void newyearUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS])
{
	static const uint8_t BITMAP[15][4] =
	{
		{  0,   0,   0,   0},
		{255, 255, 255, 255},
		{255,   0,   0,   0},
		{255, 255, 255,   0},
		{  0,   0,   0, 255},
		{255,   0,   0, 255},
		{  0, 255, 255,   0},
		{  0,   0,   0,   0},
		{  0, 255, 255,   0},
		{255,   0,   0,   0},
		{255, 255, 255,   0},
		{255,   0,   0, 255},
		{255,   0,   0, 255},
		{  0, 255, 255,   0},
		{  0,   0,   0,   0}
	};

	// Act only every 100ms
	if(clk % 10 != 0)
		return;
	
	uint8_t yOff;
	uint8_t phase = (uint8_t)clk;
	if(phase > 127) phase = 255 - phase;
	if(phase <= 32) yOff = 0;
	else if(phase > 96) yOff = 7;
	else yOff = (phase - 32) * 7 / 64;
	
	for(uint8_t y = 0; y < 8; y++)
	{
		for(uint8_t x = 0; x < 4; x++)
			ledSet(x + 4, y, BITMAP[y + yOff][x]);
	}
}

//-----------------------------------------------------------------------------
// Program: Snake

typedef enum
{
	SNAKE_LEFT, SNAKE_RIGHT, SNAKE_UP, SNAKE_DOWN
} SnakeDirection;

#define SNAKE_LENGTH 10 // Must be between 1 and 254
static SnakeDirection snakeDirection;
static struct {uint8_t x, y;} snake[SNAKE_LENGTH];

void snakeInit()
{
	ledSetAll(0);
	// Start with the whole snake balled up at (3,3)
	for(uint8_t i = 0; i < SNAKE_LENGTH; i++)
	{
		snake[i].x = 3;
		snake[i].y = 3;
	}
	snakeDirection = SNAKE_RIGHT;
}

void snakeUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS])
{
	if(clk % 10 != 0)
		return;
	
	// Choose a direction for the next move
	uint8_t rand = random();
	switch(snakeDirection)
	{
		case SNAKE_LEFT:
			if(snake[0].x > 0)
			{
				// 25% turn right (if possible)
				if(rand < 64 && snake[0].y > 0) snakeDirection = SNAKE_UP;
				// 25% turn left (if possible)
				else if(rand >= 64 && rand < 128 && snake[0].y < 7) snakeDirection = SNAKE_DOWN;
				// Otherwise keep going
			}
			else
			{
				// 50% turn right (if possible)
				if((rand < 128 && snake[0].y > 0) || snake[0].y == 7) snakeDirection = SNAKE_UP;
				// 50% turn left (if possible)
				else snakeDirection = SNAKE_DOWN;
			}
			break;
		case SNAKE_RIGHT:
			if(snake[0].x < 7)
			{
				// 25% turn left (if possible)
				if(rand < 64 && snake[0].y > 0) snakeDirection = SNAKE_UP;
				// 25% turn right (if possible)
				else if(rand >= 64 && rand < 128 && snake[0].y < 7) snakeDirection = SNAKE_DOWN;
				// Otherwise keep going
			}
			else
			{
				// 50% turn left (if possible)
				if((rand < 128 && snake[0].y > 0) || snake[0].y == 7) snakeDirection = SNAKE_UP;
				// 50% turn right (if possible)
				else snakeDirection = SNAKE_DOWN;
			}
			break;
		case SNAKE_UP:
			if(snake[0].y > 0)
			{
				// 25% turn left (if possible)
				if(rand < 64 && snake[0].x > 0) snakeDirection = SNAKE_LEFT;
				// 25% turn right (if possible)
				else if(rand >= 64 && rand < 128 && snake[0].x < 7) snakeDirection = SNAKE_RIGHT;
				// Otherwise keep going
			}
			else
			{
				// 50% turn left (if possible)
				if((rand < 128 && snake[0].x > 0) || snake[0].x == 7) snakeDirection = SNAKE_LEFT;
				// 50% turn right (if possible)
				else snakeDirection = SNAKE_RIGHT;
			}
			break;
		case SNAKE_DOWN:
			if(snake[0].y < 7)
			{
				// 25% turn right (if possible)
				if(rand < 64 && snake[0].x > 0) snakeDirection = SNAKE_LEFT;
				// 25% turn left (if possible)
				else if(rand >= 64 && rand < 128 && snake[0].x < 7) snakeDirection = SNAKE_RIGHT;
				// Otherwise keep going
			}
			else
			{
				// 50% turn right (if possible)
				if((rand < 128 && snake[0].x > 0) || snake[0].x == 7) snakeDirection = SNAKE_LEFT;
				// 50% turn right (if possible)
				else snakeDirection = SNAKE_RIGHT;
			}
			break;
	}
	
	// Erase tail end from screen
	ledSet(snake[SNAKE_LENGTH - 1].x, snake[SNAKE_LENGTH - 1].y, 0);

	// Advance snake by one
	for(uint8_t i = SNAKE_LENGTH; i > 1; i--)
		snake[i - 1] = snake[i - 2];
	
	// Calculate new head position
	switch(snakeDirection)
	{
		case SNAKE_LEFT: snake[0].x--; break;
		case SNAKE_RIGHT: snake[0].x++; break;
		case SNAKE_UP: snake[0].y--; break;
		case SNAKE_DOWN: snake[0].y++; break;
	}
	
	// Redraw snake
	for(uint8_t i = SNAKE_LENGTH; i > 0; i--)
		ledSet(snake[i - 1].x, snake[i - 1].y, (uint8_t)((uint16_t)(SNAKE_LENGTH - i + 1) * 255 / SNAKE_LENGTH));
}

//-----------------------------------------------------------------------------
// Program: Tetris

// Enumeration type for tetrominos
typedef enum
{
	TETROMINO_I,
	TETROMINO_O,
	TETROMINO_T,
	TETROMINO_L,
	TETROMINO_J,
	TETROMINO_S,
	TETROMINO_Z,
	NUM_TETROMINOS
} TetrominoType;

// Enumeration type for tetromino rotation
typedef enum
{
	ROT_0,
	ROT_90,
	ROT_180,
	ROT_270,
	NUM_ROTATIONS
} TetrominoRotation;


// Tetromino bitmaps
// The first index is TetrominoType, the second index is TetrominoRotation.
// Each entry is a 16-bit integer that contains the bitmap row by row from
// the least to the most significant bit.
static const uint16_t TETROMINO_BITMAPS[NUM_TETROMINOS][NUM_ROTATIONS] =
{
	// TETROMINO_I
	{
		// 0°   90°  180° 270°
		// □□□□ □■□□ □□□□ □■□□
		// □□□□ □■□□ □□□□ □■□□
		// ■■■■ □■□□ ■■■■ □■□□
		// □□□□ □■□□ □□□□ □■□□
		0b0000111100000000,
		0b0010001000100010,
		0b0000111100000000,
		0b0010001000100010
	},
	// TETROMINO_O
	{
		// 0°   90°  180° 270°
		// □□□□ □□□□ □□□□ □□□□
		// □□□□ □□□□ □□□□ □□□□
		// ■■□□ ■■□□ ■■□□ ■■□□
		// ■■□□ ■■□□ ■■□□ ■■□□
		0b0011001100000000,
		0b0011001100000000,
		0b0011001100000000,
		0b0011001100000000
	},
	// TETROMINO_T
	{
		// 0°   90°  180° 270°
		// □□□□ □□□□ □□□□ □□□□
		// □□□□ □■□□ □■□□ □■□□
		// ■■■□ □■■□ ■■■□ ■■□□
		// □■□□ □■□□ □□□□ □■□□
		0b0010011100000000,
		0b0010011000100000,
		0b0000011100100000,
		0b0010001100100000
	},
	// TETROMINO_L
	{
		// 0°   90°  180° 270°
		// □□□□ □□□□ □□□□ □□□□
		// □□□□ □■□□ □□■□ ■■□□
		// ■■■□ □■□□ ■■■□ □■□□
		// ■□□□ □■■□ □□□□ □■□□
		0b0001011100000000,
		0b0110001000100000,
		0b0000011101000000,
		0b0010001000110000
	},
	// TETROMINO_J
	{
		// 0°   90°  180° 270°
		// □□□□ □□□□ □□□□ □□□□
		// □□□□ □■■□ ■□□□ □■□□
		// ■■■□ □■□□ ■■■□ □■□□
		// □□■□ □■□□ □□□□ ■■□□
		0b0100011100000000,
		0b0010001001100000,
		0b0000011100010000,
		0b0011001000100000
	},
	// TETROMINO_S
	{
		// 0°   90°  180° 270°
		// □□□□ □□□□ □□□□ □□□□
		// □□□□ ■□□□ □□□□ ■□□□
		// □■■□ ■■□□ □■■□ ■■□□
		// ■■□□ □■□□ ■■□□ □■□□
		0b0011011000000000,
		0b0010001100010000,
		0b0011011000000000,
		0b0010001100010000
	},
	// TETROMINO_Z
	{
		// 0°   90°  180° 270°
		// □□□□ □□□□ □□□□ □□□□
		// □□□□ □■□□ □□□□ □■□□
		// ■■□□ ■■□□ ■■□□ ■■□□
		// □■■□ ■□□□ □■■□ ■□□□
		0b0110001100000000,
		0b0001001100100000,
		0b0110001100000000,
		0b0001001100100000
	}
};

// Draw a tetromino at a given position
// Note that the position (x,y) is the lower left corner of the tetromino.
void tetrominoDraw(TetrominoType type, TetrominoRotation rotation, int8_t x, int8_t y, bool color)
{
	for(int8_t ty = 0; ty < 4; ty++)
	{
		if(y - ty < 0 || y - ty >= 8) continue;
		for(int8_t tx = 0; tx < 4; tx++)
		{
			if(x + tx < 0 || x + tx >= 8) continue;
			if(TETROMINO_BITMAPS[type][rotation] & (1 << (4 * ty + tx)))
				ledSet((uint8_t)(x + tx), (uint8_t)(y - ty), color ? 255 : 0);
		}
	}
}

// Lock the given tetromino, i.e. put it into the playing field
// Note that the position (x,y) is the lower left corner of the tetromino.
// If locking loses the game, the function returns true.
bool tetrominoLock(TetrominoType type, TetrominoRotation rotation, bool field[8][8], int8_t x, int8_t y)
{
	for(int8_t ty = 0; ty < 4; ty++)
	{
		for(int8_t tx = 0; tx < 4; tx++)
		{
			if(TETROMINO_BITMAPS[type][rotation] & (1 << (4 * ty + tx)))
			{
				// The tetromino occupies position (x+tx,y-ty) on the playing field
				// Check if this position is above the upper border
				if(y - ty < 0)
					return true;
				// Otherwise mark the position on the playing field
				if(x + tx >= 0 && x + tx < 8 && y - ty < 8)
					field[x + tx][y - ty] = true;
			}
		}
	}
	return false;
}

// Check if a tetromino at a given position would collide with anything in the
// playing field or its borders (except the top)
// Note that the position (x,y) is the lower left corner of the tetromino.
bool tetrominoCollides(TetrominoType type, TetrominoRotation rotation, bool field[8][8], int8_t x, int8_t y)
{
	for(int8_t ty = 0; ty < 4; ty++)
	{
		for(int8_t tx = 0; tx < 4; tx++)
		{
			if(TETROMINO_BITMAPS[type][rotation] & (1 << (4 * ty + tx)))
			{
				// The tetromino occupies position (x+tx,y-ty) on the playing field
				// Check if this position is beyond the left, right, or bottom border
				if(x + tx < 0 || x + tx >= 8 || y - ty >= 8)
					return true;
				// Check playing field content at this position
				if(y - ty >= 0 && field[x + tx][y - ty])
					return true;
			}
		}
	}
	return false;
}

// Erase the display and redraw the playing field
void tetrisDrawField(bool field[8][8])
{
	for(uint8_t y = 0; y < 8; y++)
		for(uint8_t x = 0; x < 8; x++)
			ledSet(x, y, field[x][y] ? 64 : 0);
}

// Check if a given row collapses
bool tetrisRowCollapse(bool field[8][8], uint8_t row)
{
	for(uint8_t x = 0; x < 8; x++)
		if(!field[x][row])
			return false;
	return true;
}

// Check if any row collapses
bool tetrisAnyCollapse(bool field[8][8])
{
	for(uint8_t row = 0; row < 8; row++)
		if(tetrisRowCollapse(field, row))
			return true;
	return false;
}

// Current game state
enum
{
	// Tetromino is falling while being controlled by the user
	TETRIS_FALLING,
	// One or more rows are collapsing, user input ignored
	TETRIS_COLLAPSING,
	// The game has ended
	TETRIS_END
} tetrisState;

// Countdown timer for current game state
// When the countdown reaches zero, the state might change
uint8_t tetrisCountdown;

// Delays (in multiples of 10ms)
static const uint8_t TETRIS_DELAY_FALL = 50;
static const uint8_t TETRIS_DELAY_COLLAPSE = 101;
static const uint8_t TETRIS_DELAY_COLLAPSE_BLINK = 20;
static const uint8_t TETRIS_DELAY_END = 100;


// Contents of the playing field
// Does not include the currently falling tetromino.
static bool tetrisField[8][8];

// Type, position and rotation of currently falling tetromino
TetrominoType tetrominoType;
int8_t tetrominoX, tetrominoY;
TetrominoRotation tetrominoRotation;

// Spawn a new random tetromino at the top of the screen
void tetrominoSpawn()
{
	tetrominoType = random() % NUM_TETROMINOS;
	tetrominoX = 2;
	tetrominoY = 0;
	tetrominoRotation = ROT_0;
}

void tetrisInit()
{
	// Reset state
	tetrisState = TETRIS_FALLING;
	tetrisCountdown = 0;
	// Empty the playing field
	for(uint8_t y = 0; y < 8; y++)
		for(uint8_t x = 0; x < 8; x++)
			tetrisField[y][x] = false;
	// Spawn the first tetromino
	tetrominoSpawn();
	// Draw playing field and tetromino
	tetrisDrawField(tetrisField);
	tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
}

void tetrisUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS])
{
	switch(tetrisState)
	{
		case TETRIS_FALLING:
		{
			// Check for user input
			if(events[BTN_LEFT] == EVENT_PRESS)
			{
				// Check if move to the left is possible
				if(!tetrominoCollides(tetrominoType, tetrominoRotation, tetrisField, tetrominoX - 1, tetrominoY))
				{
					// Erase, move, redraw
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, false);
					tetrominoX--;
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
				}
			}
			if(events[BTN_RIGHT] == EVENT_PRESS)
			{
				// Check if move to the right is possible
				if(!tetrominoCollides(tetrominoType, tetrominoRotation, tetrisField, tetrominoX + 1, tetrominoY))
				{
					// Erase, move, redraw
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, false);
					tetrominoX++;
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
				}
			}
			if(events[BTN_CENTER] == EVENT_PRESS)
			{
				// Check if a rotation is possible
				TetrominoRotation newRotation = (tetrominoRotation + 1) % NUM_ROTATIONS;
				if(!tetrominoCollides(tetrominoType, newRotation, tetrisField, tetrominoX, tetrominoY))
				{
					// Erase, rotate, redraw
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, false);
					tetrominoRotation = newRotation;
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
				}
				// Try again with "wall kick" to the left
				else if(!tetrominoCollides(tetrominoType, newRotation, tetrisField, tetrominoX - 1, tetrominoY))
				{
					// Erase, rotate&move, redraw
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, false);
					tetrominoRotation = newRotation;
					tetrominoX--;
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
				}
				// Try again with "wall kick" to the right
				else if(!tetrominoCollides(tetrominoType, newRotation, tetrisField, tetrominoX + 1, tetrominoY))
				{
					// Erase, rotate&move, redraw
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, false);
					tetrominoRotation = newRotation;
					tetrominoX++;
					tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
				}
				events[BTN_CENTER] = EVENT_NONE;
			}
			
			// Is there anything else to do?
			if(tetrisCountdown > 0)
			{
				tetrisCountdown--;
				break;
			}
			
			// Move the tetronimo down
			// Check if moving it down by one causes a collision
			if(tetrominoCollides(tetrominoType, tetrominoRotation, tetrisField, tetrominoX, tetrominoY + 1))
			{
				// Lock the tetromino
				if(tetrominoLock(tetrominoType, tetrominoRotation, tetrisField, tetrominoX, tetrominoY))
				{
					// Playing field overflowed, game ends
					tetrisState = TETRIS_END;
				}
				else
				{
					// Spawn a new tetromino
					tetrominoSpawn();
					// Reset countdown
					tetrisCountdown = TETRIS_DELAY_FALL;
				}
			}
			else
			{
				// Erase the tetromino from screen
				tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, false);
				// Move it down
				tetrominoY++;
				// Draw it onto the screen in its new position
				tetrominoDraw(tetrominoType, tetrominoRotation, tetrominoX, tetrominoY, true);
				// Check if this causes collapse
				if(tetrisAnyCollapse(tetrisField))
				{
					tetrisState = TETRIS_COLLAPSING;
					tetrisCountdown = TETRIS_DELAY_COLLAPSE;
				}
				else
				{
					tetrisCountdown = TETRIS_DELAY_FALL;
				}
			}
			break;
		}
		case TETRIS_COLLAPSING:
		{
			if(tetrisCountdown > 0)
			{
				tetrisCountdown--;
			
				// Blink collapsing rows
				if(tetrisCountdown % TETRIS_DELAY_COLLAPSE_BLINK == 0)
				{
					uint8_t color = tetrisCountdown % (2 * TETRIS_DELAY_COLLAPSE_BLINK) == 0 ? 255 : 0;
					for(uint8_t row = 0; row < 8; row++)
					{
						if(tetrisRowCollapse(tetrisField, row))
						{
							for(uint8_t x = 0; x < 8; x++)
								ledSet(x, row, color);
						}
					}
				}
			}
			else
			{
				// Remove collapsing rows
				int8_t nonCollapsedRow = 7;
				for(int8_t row = 7; row >= 0; row--)
				{
					// Find the non-collapsed row that moves here
					while(nonCollapsedRow >= 0 && tetrisRowCollapse(tetrisField, (uint8_t)nonCollapsedRow))
						nonCollapsedRow--;
					
					// Either copy or empty the row
					if(nonCollapsedRow >= 0)
					{
						for(uint8_t x = 0; x < 8; x++)
							tetrisField[x][row] = tetrisField[x][nonCollapsedRow];
						nonCollapsedRow--;
					}
					else
					{
						for(uint8_t x = 0; x < 8; x++)
							tetrisField[x][row] = false;
					}
				}
				tetrisDrawField(tetrisField);
				
				// Spawn new tetromino
				tetrominoSpawn();
				tetrisState = TETRIS_FALLING;
				tetrisCountdown = TETRIS_DELAY_FALL;
			}
			break;
		}
		case TETRIS_END:
		{
			// Short delay just in case the user had buttons pressed right before losing
			if(tetrisCountdown > 0)
				tetrisCountdown--;
			else
			{
				// Any button goes back to normal operation
				if(events[BTN_LEFT] != EVENT_NONE || events[BTN_CENTER] != EVENT_NONE || events[BTN_RIGHT] != EVENT_NONE)
				{
					events[BTN_LEFT] = EVENT_NONE;
					events[BTN_RIGHT] = EVENT_RELEASE_SHORT;
					events[BTN_CENTER] = EVENT_NONE;
					return;
				}
			}
			break;
		}
	}
	
	// Clear events on left and right button so main loop won't cycle to other
	// programs.
	events[BTN_LEFT] = events[BTN_RIGHT] = EVENT_NONE;
}

//-----------------------------------------------------------------------------
// Program (unused): Test pattern

void testInit()
{
	ledSetAll(0x00);
	uint8_t i = 0;
	for(uint8_t y = 0; y < 8; y++)
		for(uint8_t x = 0; x < 8; x++)
			ledSet(x, y, (y * 8 + x) * 4);
}

void testUpdate(uint16_t clk, InputEvent events[NUM_BUTTONS]) {}

//-----------------------------------------------------------------------------

/**
 * @brief Array containing all implemented programs
 */
const Program PROGRAMS[] = {
	{"Typewriter", typewriterInit, typewriterUpdate},
	{"Matrix", matrixInit, matrixUpdate},
	{"Bouncy", bouncyInit, bouncyUpdate},
	{"Happy New Year", newyearInit, newyearUpdate},
	{"Snake", snakeInit, snakeUpdate},
	{"Tetris", tetrisInit, tetrisUpdate}
};

// Number of available programs to cycle through
// (Tetris doesn't count as a normal program)
const uint8_t NUM_PROGRAMS = (sizeof(PROGRAMS) / sizeof(Program)) - 1;