#include <stdio.h>
#include <stdbool.h>
#include <rand.h>
#include <gb/gb.h>

#include "./sprites.c"

#include "../sprites/blank_data.c"
#include "../sprites/blank_map.c"
#include "../sprites/start_screen_data.c"
#include "../sprites/start_screen_map.c"

struct {
	uint8_t scene;
	uint8_t number;
	uint8_t comparison;
	uint8_t playedHigher;
} gameState;

struct {
	uint8_t animationPhase;
	uint8_t frame;
} roundStartState;

// 0
void startScreen() {
	set_bkg_data(0, 84, start_screen_data);
	set_bkg_tiles(0, 0, 20, 18, start_screen_map);

	waitpad(J_A);

	// roundStart scene
	gameState.scene++;
	roundStartState.animationPhase = 0;
	roundStartState.frame = 0;
}

// 1
void roundStart() {
	// Blank screen
	set_bkg_data(0, 1, blank_data);
	set_bkg_tiles(0, 0, 20, 18, blank_map);

	// Seed rand()
	initarand(DIV_REG);
	
	// Generate a random number from 2 to 9
	gameState.number = ((uint8_t)rand() % 8) + 2;
	// Generate a random number from 1 to 10
	gameState.comparison = ((uint8_t)rand() % 9) + 1;

	switch (roundStartState.animationPhase) {
		case 0:
			loadCardSprites();
			drawCardBack(0, 74, 32);
			roundStartState.animationPhase++;
			break;
		case 1:
			roundStartState.frame++;
			drawCardBack(0, 74-roundStartState.frame, 32);
			if (roundStartState.frame == 30) roundStartState.animationPhase++;
			break;
		case 2:
			drawCardFront(0, 44, 32);
			// roundInput
			gameState.scene++;
		default:
			break;
	}
}

// 2
void roundInput(uint8_t input) {
	bool progress = false;

	if (input & J_UP) {
		gameState.playedHigher = true;
		progress = true;
	} else if (input & J_DOWN) {
		gameState.playedHigher = false;
		progress = true;
	}

	// roundEnd
	if (progress) gameState.scene++;
}

// 3
void roundEnd() {
	drawCardFront(16, 94, 32);
	
	if ((gameState.playedHigher && gameState.comparison > gameState.number) || (!gameState.playedHigher && gameState.comparison < gameState.number)) {
		printf("%d - You Win!\n", gameState.comparison);
	} else {
		printf("%d - You Lose!\n", gameState.comparison);
	}

	waitpad(J_A);

	// roundStart
	gameState.scene = 1;
}