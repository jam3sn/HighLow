#include <stdio.h>
#include <stdbool.h>
#include <rand.h>
#include <gb/gb.h>
#include <gbdk/font.h>

#include "./sprites/blank_data.c"
#include "./sprites/blank_map.c"
#include "./sprites/start_screen_data.c"
#include "./sprites/start_screen_map.c"

struct {
	bool roundStarted;
	bool voted;
	uint8_t number;
	uint8_t comparison;
} gameState;

/**
 * Determine if the player played the right hand
 */
void determineOutcome(bool playedHigher)
{
	gameState.voted = true;

	if ((playedHigher && gameState.comparison > gameState.number) || (!playedHigher && gameState.comparison < gameState.number)) {
		printf("%d - You Win!\n", gameState.comparison);
	} else {
		printf("%d - You Lose!\n", gameState.comparison);
	}

	gameState.roundStarted = false;
	waitpad(J_A);
}

void main() {
	// Font init
	font_init();

	// Initial state
gameState.roundStarted = false;

	SHOW_SPRITES;
	SHOW_BKG;
    DISPLAY_ON;

	while (1) {
		uint8_t input = joypad();
		
		// Start round
		if (!gameState.roundStarted) {
			// Start screen
			set_bkg_data(0, 84, start_screen_data);
			set_bkg_tiles(0, 0, 20, 18, start_screen_map);

			waitpad(J_A);

			// Blank screen
			set_bkg_data(0, 1, start_screen_data);
			set_bkg_tiles(0, 0, 20, 18, start_screen_map);

			gameState.roundStarted = true;
			gameState.voted = false;

			printf("Lets go!\n");
			
			// Seed rand()
			initarand(DIV_REG);
			
			// Generate a random number from 2 to 9
			gameState.number = ((uint8_t)rand() % 8) + 2;
			// Generate a random number from 1 to 10
			gameState.comparison = ((uint8_t)rand() % 9) + 1;

			printf("%d! Higher or Lower?\n", gameState.number);
		}

		// Higher or Lower controls
		if (input & J_UP && gameState.roundStarted && !gameState.voted) {
			determineOutcome(TRUE);
		} else if (input & J_DOWN && gameState.roundStarted && !gameState.voted) {
			determineOutcome(FALSE);
		}

		wait_vbl_done();
	}
}