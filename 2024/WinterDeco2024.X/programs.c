/**
 * @file programs.c
 * @date 2024-10-06
 * @brief Implements programs.h
 */

#include<xc.h>
#include<stdio.h>
#include"led.h"
#include"programs.h"

// Dummy functions that do nothing
void nullInit() {}
void nullUpdate(uint16_t clk, InputEvent events[NUM_SENSORS]) {}

// VERY simple (and terrible) PRNG
uint8_t random(uint8_t max)
{
	static uint8_t seed;
	seed = (seed * 35 + 87);
	return seed % max;
}

//-----------------------------------------------------------------------------
// Program: Smile and Blink

void smileAndBlinkInit()
{
	// Turn on all LEDs except upper lip (smile)
	ledSetAll(0xff);
	ledSet(LED_UPPER_LIP_LEFT, 0x00);
	ledSet(LED_UPPER_LIP_RIGHT, 0x00);
	// Dim the buttons a little since green LEDs are very bright
	ledSet(LED_BUTTON_1, 0x33);
	ledSet(LED_BUTTON_2, 0x33);
	ledSet(LED_BUTTON_3, 0x33);
	ledSet(LED_BUTTON_4, 0x33);
	ledSet(LED_BUTTON_5, 0x33);
}

void smileAndBlinkUpdate(uint16_t clk, InputEvent events[NUM_SENSORS])
{
	// For the first 100ms (10 clocks) of each 5s (500 clocks) interval, turn
	// off the eyes (blink)
	if(clk % 500  == 0)
	{
		ledSet(LED_EYE_LEFT, 0x00);
		ledSet(LED_EYE_RIGHT, 0x00);
	}
	else if(clk % 500  == 10)
	{
		ledSet(LED_EYE_LEFT, 0xff);
		ledSet(LED_EYE_RIGHT, 0xff);
	}
}

//-----------------------------------------------------------------------------
// Program: Let it Snow

void snowInit()
{
	// Turn on LEDs on face and buttons
	ledSetAll(0x00);
	ledSet(LED_EYE_LEFT, 0xff);
	ledSet(LED_EYE_RIGHT, 0xff);
	ledSet(LED_NOSE, 0xff);
	ledSet(LED_MOUTH_LEFT, 0xff);
	ledSet(LED_MOUTH_RIGHT, 0xff);
	ledSet(LED_UPPER_LIP_LEFT, 0xff);
	ledSet(LED_UPPER_LIP_RIGHT, 0xff);
	ledSet(LED_LOWER_LIP_LEFT, 0xff);
	ledSet(LED_LOWER_LIP_RIGHT, 0xff);
	// Dim the button a little since green LEDs are very bright
	ledSet(LED_BUTTON_1, 0x33);
	ledSet(LED_BUTTON_2, 0x33);
	ledSet(LED_BUTTON_3, 0x33);
	ledSet(LED_BUTTON_4, 0x33);
	ledSet(LED_BUTTON_5, 0x33);
}

void snowUpdate(uint16_t clk, InputEvent events[NUM_SENSORS])
{
	// Act only every 100ms
	if(clk % 10 != 0)
		return;

	// Position of the snowflakes on the left and right
	// -1: non-existent, 0: above hat, 1: hat, ..., 5: foot, 6: below foot
	static int8_t snowLeft = -1, snowRight = -1;

	if(snowLeft == -1)
	{
		// Create a new snowflake
		if(random(100) < 15)
			snowLeft = 0;
	}
	else
	{
		// Move snowflake down
		if(snowLeft < 6)
			snowLeft++;
		else
			snowLeft = -1;
	}
	if(snowRight == -1)
	{
		// Create a new snowflake
		if(random(100) < 14)
			snowRight = 0;
	}
	else
	{
		// Move snowflake down
		if(snowRight < 6)
			snowRight++;
		else
			snowRight = -1;
	}
	
	// Show snowflakes
	ledSet(LED_HAT_LEFT, snowLeft == 1 ? 0xff : (snowLeft == 0 || snowLeft == 2 ? 0x22 : 0x00));
	ledSet(LED_SHOULDER_LEFT, snowLeft == 2 ? 0xff : (snowLeft == 1 || snowLeft == 3 ? 0x22 : 0x00));
	ledSet(LED_HAND_LEFT, snowLeft == 3 ? 0xff : (snowLeft == 2 || snowLeft == 4 ? 0x22 : 0x00));
	ledSet(LED_KNEE_LEFT, snowLeft == 4 ? 0xff : (snowLeft == 3 || snowLeft == 5 ? 0x22 : 0x00));
	ledSet(LED_FOOT_LEFT, snowLeft == 5 ? 0xff : (snowLeft == 4 || snowLeft == 6 ? 0x22 : 0x00));
	ledSet(LED_HAT_RIGHT, snowRight == 1 ? 0xff : (snowRight == 0 || snowRight == 2 ? 0x22 : 0x00));
	ledSet(LED_SHOULDER_RIGHT, snowRight == 2 ? 0xff : (snowRight == 1 || snowRight == 3 ? 0x22 : 0x00));
	ledSet(LED_HAND_RIGHT, snowRight == 3 ? 0xff : (snowRight == 2 || snowRight == 4 ? 0x22 : 0x00));
	ledSet(LED_KNEE_RIGHT, snowRight == 4 ? 0xff : (snowRight == 3 || snowRight == 5 ? 0x22 : 0x00));
	ledSet(LED_FOOT_RIGHT, snowRight == 5 ? 0xff : (snowRight == 4 || snowRight == 6 ? 0x22 : 0x00));
}

//-----------------------------------------------------------------------------
// Program: Dance

#define DANCE_DELAY 20

void danceInit()
{
	ledSetAll(0x00);
	ledSet(LED_EYE_LEFT, 0xff);
	ledSet(LED_EYE_RIGHT, 0xff);
	ledSet(LED_NOSE, 0xff);
	ledSet(LED_MOUTH_LEFT, 0xff);
	ledSet(LED_MOUTH_RIGHT, 0xff);
	ledSet(LED_LOWER_LIP_LEFT, 0xff);
	ledSet(LED_LOWER_LIP_RIGHT, 0xff);
}

void danceUpdate(uint16_t clk, InputEvent events[NUM_SENSORS])
{
	if(clk % DANCE_DELAY != 0)
		return;
	clk /= DANCE_DELAY;

	// Alternate buttons
	ledSet(LED_BUTTON_1, clk % 2 == 0 ? 0x33 : 0x00);
	ledSet(LED_BUTTON_2, clk % 2 == 0 ? 0x00 : 0x33);
	ledSet(LED_BUTTON_3, clk % 2 == 0 ? 0x33 : 0x00);
	ledSet(LED_BUTTON_4, clk % 2 == 0 ? 0x00 : 0x33);
	ledSet(LED_BUTTON_5, clk % 2 == 0 ? 0x33 : 0x00);

	// Left side
	ledSet(LED_HAT_LEFT, clk % 4 == 0 ? 0xff : 0x00);
	ledSet(LED_SHOULDER_LEFT, clk % 4 == 0 ? 0xff : 0x00);
	ledSet(LED_HAND_LEFT, clk % 4 == 0 ? 0xff : 0x00);
	ledSet(LED_KNEE_LEFT, clk % 4 == 0 ? 0xff : 0x00);
	ledSet(LED_FOOT_LEFT, clk % 4 == 0 ? 0xff : 0x00);

	// Right side
	ledSet(LED_HAT_RIGHT, clk % 4 == 2 ? 0xff : 0x00);
	ledSet(LED_SHOULDER_RIGHT, clk % 4 == 2 ? 0xff : 0x00);
	ledSet(LED_HAND_RIGHT, clk % 4 == 2 ? 0xff : 0x00);
	ledSet(LED_KNEE_RIGHT, clk % 4 == 2 ? 0xff : 0x00);
	ledSet(LED_FOOT_RIGHT, clk % 4 == 2 ? 0xff : 0x00);
}

//-----------------------------------------------------------------------------
// Program: Zeus's Fury

void furyInit()
{
	ledSetAll(0x00);
	ledSet(LED_EYE_LEFT, 0x33);
	ledSet(LED_EYE_RIGHT, 0x33);
	ledSet(LED_NOSE, 0xff);
	ledSet(LED_MOUTH_LEFT, 0xff);
	ledSet(LED_MOUTH_RIGHT, 0xff);
	ledSet(LED_UPPER_LIP_LEFT, 0xff);
	ledSet(LED_UPPER_LIP_RIGHT, 0xff);
	ledSet(LED_BUTTON_1, 0x33);
	ledSet(LED_BUTTON_2, 0x33);
	ledSet(LED_BUTTON_3, 0x33);
	ledSet(LED_BUTTON_4, 0x33);
	ledSet(LED_BUTTON_5, 0x33);
}

void furyUpdate(uint16_t clk, InputEvent events[NUM_SENSORS])
{
	if(clk % 4 != 0)
		return;
	
	static uint8_t lightning = 0;
	
	if(lightning > 0)
	{
		lightning--;
		ledSet(LED_EYE_LEFT, lightning % 2 == 0 ? 0x33 : 0xff);
		ledSet(LED_EYE_RIGHT, lightning % 2 == 0 ? 0x33 : 0xff);
		ledSet(LED_HAT_LEFT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_HAT_RIGHT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_SHOULDER_LEFT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_SHOULDER_RIGHT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_HAND_LEFT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_HAND_RIGHT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_KNEE_LEFT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_KNEE_RIGHT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_FOOT_LEFT, lightning % 2 == 0 ? 0x00 : 0xff);
		ledSet(LED_FOOT_RIGHT, lightning % 2 == 0 ? 0x00 : 0xff);
	}
	else if(random(100) == 0)
		lightning = 2 * random(4);
}

//-----------------------------------------------------------------------------
// Program: Moody Swings

void moodyInit()
{
	ledSetAll(0x00);
	ledSet(LED_EYE_LEFT, 0xff);
	ledSet(LED_EYE_RIGHT, 0xff);
	ledSet(LED_NOSE, 0xff);
	ledSet(LED_MOUTH_LEFT, 0xff);
	ledSet(LED_MOUTH_RIGHT, 0xff);
	ledSet(LED_LOWER_LIP_LEFT, 0xff);
	ledSet(LED_LOWER_LIP_RIGHT, 0xff);
	ledSet(LED_BUTTON_1, 0x11);
	ledSet(LED_BUTTON_2, 0x11);
	ledSet(LED_BUTTON_3, 0x11);
	ledSet(LED_BUTTON_4, 0x11);
	ledSet(LED_BUTTON_5, 0x11);
	ledSet(LED_HAT_LEFT, 0x04);
	ledSet(LED_HAT_RIGHT, 0x04);
	ledSet(LED_SHOULDER_LEFT, 0x04);
	ledSet(LED_SHOULDER_RIGHT, 0x04);
	ledSet(LED_HAND_LEFT, 0x04);
	ledSet(LED_HAND_RIGHT, 0x04);
	ledSet(LED_KNEE_LEFT, 0x04);
	ledSet(LED_KNEE_RIGHT, 0x04);
	ledSet(LED_FOOT_LEFT, 0x04);
	ledSet(LED_FOOT_RIGHT, 0x04);
}

void moodyUpdate(uint16_t clk, InputEvent events[NUM_SENSORS])
{
	if(clk % 1000 != 0)
		return;
	
	switch((clk / 1000 + random(97)) % 4)
	{
	case 0:
		// Happy
		ledSet(LED_MOUTH_LEFT, 0xff);
		ledSet(LED_MOUTH_RIGHT, 0xff);
		ledSet(LED_LOWER_LIP_LEFT, 0xff);
		ledSet(LED_LOWER_LIP_RIGHT, 0xff);
		ledSet(LED_UPPER_LIP_LEFT, 0x00);
		ledSet(LED_UPPER_LIP_RIGHT, 0x00);
		printf("Now I'm happy :-)\n");
		break;
	case 1:
		// Sad
		ledSet(LED_MOUTH_LEFT, 0xff);
		ledSet(LED_MOUTH_RIGHT, 0xff);
		ledSet(LED_LOWER_LIP_LEFT, 0x00);
		ledSet(LED_LOWER_LIP_RIGHT, 0x00);
		ledSet(LED_UPPER_LIP_LEFT, 0xff);
		ledSet(LED_UPPER_LIP_RIGHT, 0xff);
		printf("Now I'm sad :-(\n");
		break;
	case 2:
		// Talkative
		ledSet(LED_MOUTH_LEFT, 0xff);
		ledSet(LED_MOUTH_RIGHT, 0xff);
		ledSet(LED_LOWER_LIP_LEFT, 0xff);
		ledSet(LED_LOWER_LIP_RIGHT, 0xff);
		ledSet(LED_UPPER_LIP_LEFT, 0xff);
		ledSet(LED_UPPER_LIP_RIGHT, 0xff);
		printf("Now I have to tell you something :-O\n");
		break;
	case 3:
		// Mute
		ledSet(LED_MOUTH_LEFT, 0x00);
		ledSet(LED_MOUTH_RIGHT, 0x00);
		ledSet(LED_LOWER_LIP_LEFT, 0xff);
		ledSet(LED_LOWER_LIP_RIGHT, 0xff);
		ledSet(LED_UPPER_LIP_LEFT, 0x00);
		ledSet(LED_UPPER_LIP_RIGHT, 0x00);
		printf("Shush! :-X\n");
		break;
	}
}

//-----------------------------------------------------------------------------
// Program: Simon Says

// Implements a "Simon Says" game with five stages. During each stage an
// increasingly longer sequence of elements (each corresponding to one of the
// five sensors) is shown. The player is then tasked with reproducing the
// sequence. Each successfully completed stage makes another button LED light
// up. If the player fails at reproducing the sequence, the game is lost. If
// they manage to light up all five buttons, they win. 

// Length of the initial sequence
#define SIMON_START_LENGTH 2
// How much longer does the sequence become for each following stage
#define SIMON_INCREMENT 1
// Total sequence length (used in last stage)
#define SIMON_TOTAL_LENGTH (SIMON_START_LENGTH + 4 * SIMON_INCREMENT)
// Delay between sequence elements during playback
#define SIMON_PLAYBACK_PAUSE_LENGTH 50
// Duration of each sequence element during playback
#define SIMON_PLAYBACK_SHOW_LENGTH 100

// Complete sequence for a game (elements are 1..5)
static uint8_t simonSequence[SIMON_TOTAL_LENGTH];
// Current state of the game
static enum
{
	// Wait until the player is ready (all sensors have been released)
	SIMON_INITIAL_WAIT,
	// The sequence is being played:
	// - Pause before showing the simonSubstage-th element of the sequence
	SIMON_PLAYING_SEQUENCE_PAUSE,
	// - Showing the simonSubstage-th element of the sequence
	SIMON_PLAYING_SEQUENCE_SHOW,
	// The player is trying to replicate the sequence:
	// Waiting for simonSubstage-th element of the sequence to be pressed
	SIMON_WAITING_FOR_INPUT,
	// The player has won the game
	SIMON_WON,
	// The player has lost the game
	SIMON_LOST
} simonState;
// Delay counter (counts down; delaying ends when 0 is reached)
static uint8_t simonDelay;
// Stage of the game, zero-based (i.e. the first
// SIMON_START_LENGTH + simonStage * SIMON_INCREMENT elements of the sequence
// are in play)
static uint8_t simonStage;
// Index  of the element of the sequence that is currently being shown/guessed,
// zero-based
static uint8_t simonElement;

// Lights up the LEDs belonging to a sequence element
// 0: all off
// 1: lower left sensor
// 2: lower right sensor
// 3: upper left sensor
// 4: upper right sensor
// 5: hat sensor
inline void simonShow(uint8_t element)
{
	ledSet(LED_FOOT_LEFT, element == 1 ? 0xff : 0x00);
	ledSet(LED_KNEE_LEFT, element == 1 ? 0xff : 0x00);
	ledSet(LED_FOOT_RIGHT, element == 2 ? 0xff : 0x00);
	ledSet(LED_KNEE_RIGHT, element == 2 ? 0xff : 0x00);
	ledSet(LED_HAND_LEFT, element == 3 ? 0xff : 0x00);
	ledSet(LED_SHOULDER_LEFT, element == 3 ? 0xff : 0x00);
	ledSet(LED_HAND_RIGHT, element == 4 ? 0xff : 0x00);
	ledSet(LED_SHOULDER_RIGHT, element == 4 ? 0xff : 0x00);
	ledSet(LED_HAT_LEFT, element == 5 ? 0xff : 0x00);
	ledSet(LED_HAT_RIGHT, element == 5 ? 0xff : 0x00);
}

void simonInit()
{
	// Create a new sequence
	printf("Simon Says Sequence: ");
	for(uint8_t i = 0; i < SIMON_TOTAL_LENGTH; i++)
	{
		simonSequence[i] = random(5);
		printf("%s, ", SENSOR_NAMES[simonSequence[i]]);
	}
	printf("<END>\n");
	// Initialise game state
	simonState = SIMON_INITIAL_WAIT;
	
	// Turn off all LEDs except eyes, nose and lower lip (smile)
	ledSetAll(0x00);
	ledSet(LED_EYE_LEFT, 0xff);
	ledSet(LED_EYE_RIGHT, 0xff);
	ledSet(LED_NOSE, 0xff);
	ledSet(LED_MOUTH_LEFT, 0xff);
	ledSet(LED_MOUTH_RIGHT, 0xff);
	ledSet(LED_LOWER_LIP_LEFT, 0xff);
	ledSet(LED_LOWER_LIP_RIGHT, 0xff);
}

void simonUpdate(uint16_t clk, InputEvent events[NUM_SENSORS])
{
	// If delay is running, do nothing
	if(simonDelay > 0)
	{
		simonDelay--;
		return;
	}
	
	switch(simonState)
	{
	case SIMON_INITIAL_WAIT:
		if(!inputPressedAny())
		{
			// Start the game
			simonStage = 0;
			simonElement = 0;
			simonState = SIMON_PLAYING_SEQUENCE_PAUSE;
			simonDelay = SIMON_PLAYBACK_PAUSE_LENGTH;
		}
		break;
	case SIMON_PLAYING_SEQUENCE_PAUSE:
		simonState = SIMON_PLAYING_SEQUENCE_SHOW;
		simonShow(simonSequence[simonElement] + 1);
		simonDelay = SIMON_PLAYBACK_SHOW_LENGTH;
		break;
	case SIMON_PLAYING_SEQUENCE_SHOW:
		simonShow(0);
		// Move to next element in the sequence
		simonElement++;
		if(simonElement == SIMON_START_LENGTH + simonStage * SIMON_INCREMENT)
		{
			// Playback is finished, start waiting for player input
			ledSet(LED_UPPER_LIP_LEFT, 0xff);
			ledSet(LED_UPPER_LIP_RIGHT, 0xff);
			simonState = SIMON_WAITING_FOR_INPUT;
			simonElement = 0;
		}
		else
		{
			// Pause before showing next element
			simonState = SIMON_PLAYING_SEQUENCE_PAUSE;
			simonDelay = SIMON_PLAYBACK_PAUSE_LENGTH;
		}
		break;
	case SIMON_WAITING_FOR_INPUT:
		// Look for player input
		for(uint8_t i = 0; i < 5; i++)
		{
			if(events[i] == EVENT_PRESS)
			{
				// Sensor was pressed, light up the corresponding LEDs
				simonShow(i + 1);
			}
			else if(events[i] == EVENT_RELEASE_SHORT)
			{
				// Clear event
				events[i] = EVENT_NONE;
				// LEDs off
				simonShow(0);
				// Check if this was correct
				if(i == simonSequence[simonElement])
				{
					// Move to next element in sequence
					simonElement++;
					if(simonElement == SIMON_START_LENGTH + simonStage * SIMON_INCREMENT)
					{
						// Stage is finished, light up button
						if(simonStage == 0) ledSet(LED_BUTTON_5, 0x33);
						else if(simonStage == 1) ledSet(LED_BUTTON_4, 0x33);
						else if(simonStage == 2) ledSet(LED_BUTTON_3, 0x33);
						else if(simonStage == 3) ledSet(LED_BUTTON_2, 0x33);
						else if(simonStage == 4) ledSet(LED_BUTTON_1, 0x33);
						// Smile
						ledSet(LED_UPPER_LIP_LEFT, 0x00);
						ledSet(LED_UPPER_LIP_RIGHT, 0x00);
						ledSet(LED_LOWER_LIP_LEFT, 0xff);
						ledSet(LED_LOWER_LIP_RIGHT, 0xff);
						// Move on to next stage
						simonStage++;
						simonElement = 0;
						simonState = SIMON_PLAYING_SEQUENCE_PAUSE;
						simonDelay = SIMON_PLAYBACK_PAUSE_LENGTH;
						// Check if player has won
						if(simonStage == 5)
							simonState = SIMON_WON;
					}
				}
				else
				{
					// Incorrect
					simonState = SIMON_LOST;
					// Frown
					ledSet(LED_UPPER_LIP_LEFT, 0xff);
					ledSet(LED_UPPER_LIP_RIGHT, 0xff);
					ledSet(LED_LOWER_LIP_LEFT, 0x00);
					ledSet(LED_LOWER_LIP_RIGHT, 0x00);
				}
			}
		}
		break;
	case SIMON_WON:
		if(clk % 64 < 32)
		{
			ledSet(LED_HAT_LEFT, 0xff);
			ledSet(LED_HAT_RIGHT, 0x00);
		}
		else
		{
			ledSet(LED_HAT_LEFT, 0x00);
			ledSet(LED_HAT_RIGHT, 0xff);
		}
		// Don't clear any events, this allows the main loop to enter other
		// programs whenever the user selects one. 
		break;
	case SIMON_LOST:
		// Don't clear any events, this allows the main loop to enter other
		// programs whenever the user selects one. 
		break;
	}
}

//-----------------------------------------------------------------------------

/**
 * @brief Array containing all implemented programs
 */
const Program PROGRAMS[] = {
	{"Smile and Blink", smileAndBlinkInit, smileAndBlinkUpdate},
	{"Let it Snow", snowInit, snowUpdate},
	{"Dance", danceInit, danceUpdate},
	{"Zeus's Fury", furyInit, furyUpdate},
	{"Mood Swings", moodyInit, moodyUpdate},
	{"Simon Says", simonInit, simonUpdate}
};

const uint8_t NUMBER_OF_PROGRAMS = (sizeof(PROGRAMS) / sizeof(Program));