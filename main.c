#include <stdio.h>
#include <stdbool.h>
#include <rand.h>
#include <gb/gb.h>
#include <gbdk/font.h>

#include "./sprites/blank_data.c"
#include "./sprites/blank_map.c"
#include "./sprites/card_back_data.c"
#include "./sprites/start_screen_data.c"
#include "./sprites/start_screen_map.c"

struct {
	uint8_t scene;
	uint8_t number;
	uint8_t comparison;
	uint8_t playedHigher;
} gameState;

// 0
void startScreen() {
	set_bkg_data(0, 84, start_screen_data);
	set_bkg_tiles(0, 0, 20, 18, start_screen_map);

	waitpad(J_A);

	// roundStart scene
	gameState.scene++;
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

	// Lay cards, show arrows
	set_sprite_data(0, 13, card_back_data);

	for (uint8_t i = 0; i <= 12; i++) set_sprite_tile(i, i);

	move_sprite(0, 40, 32);
	move_sprite(1, 48, 32);
	move_sprite(2, 56, 32);

	move_sprite(3, 40, 40);
	move_sprite(4, 48, 40);
	move_sprite(5, 56, 40);

	set_sprite_tile(13, 3);
	move_sprite(13, 40, 48);
	move_sprite(6, 48, 48);
	set_sprite_tile(14, 5);
	move_sprite(14, 56, 48);

	move_sprite(7, 40, 56);
	move_sprite(8, 48, 56);
	move_sprite(9, 56, 56);

	move_sprite(10, 40, 64);
	move_sprite(11, 48, 64);
	move_sprite(12, 56, 64);
	
	// roundInput
	gameState.scene++;
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
	if ((gameState.playedHigher && gameState.comparison > gameState.number) || (!gameState.playedHigher && gameState.comparison < gameState.number)) {
		printf("%d - You Win!\n", gameState.comparison);
	} else {
		printf("%d - You Lose!\n", gameState.comparison);
	}

	waitpad(J_A);

	// roundStart
	gameState.scene = 1;
}

void main() {
	// Font init
	font_init();

	// Initial state
	gameState.scene = 0;

	SHOW_BKG;
	SHOW_SPRITES;
    DISPLAY_ON;

	while (1) {
		uint8_t input = joypad();

		switch (gameState.scene) {
			case 1:
				roundStart();
				break;
			case 2:
				roundInput(input);
				break;
			case 3:
				roundEnd();
				break;
			default:
				startScreen();
				break;
		}

		wait_vbl_done();
	}
}