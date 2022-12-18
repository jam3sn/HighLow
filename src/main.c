#include <gb/gb.h>
#include <gbdk/font.h>

#include "./game.c"

void main()
{
	// Font init
	font_init();

	// Initial state
	round round = {.round_number = 0};
	game_state.round = round;
	game_state.scene.number = 0;

	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;

	while (1)
	{
		uint8_t input = joypad();

		switch (game_state.scene.number)
		{
		case 1:
			round_start();
			break;
		case 2:
			round_input(input);
			break;
		case 3:
			round_end();
			break;
		default:
			start_screen();
			break;
		}

		wait_vbl_done();
	}
}