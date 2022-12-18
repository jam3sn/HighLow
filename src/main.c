#include <gb/gb.h>
#include <gbdk/font.h>

#include "./scenes.c"

void main() {
	// Font init
	font_init();

	// Initial state
	round round = {.roundNumber = 0};
	gameState.round = round;
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