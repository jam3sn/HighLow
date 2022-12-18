#include <stdio.h>
#include <stdbool.h>
#include <rand.h>

#include "./card.c"

#include "../sprites/blank_data.c"
#include "../sprites/blank_map.c"
#include "../sprites/start_screen_data.c"
#include "../sprites/start_screen_map.c"

typedef struct round
{
	uint8_t round_number;
	card known_card;
	card comparison_card;
	BOOLEAN played_higher;
} round;

typedef struct scene
{
	uint8_t number;
	uint8_t phase;
	uint8_t frame;
} scene;

struct
{
	scene scene;
	round round;
} game_state;

void clear_bkg()
{
	set_bkg_data(0, 1, blank_data);
	set_bkg_tiles(0, 0, 20, 18, blank_map);
	set_sprite_data(85, 1, blank_data);
}

// 0
void start_screen()
{
	set_bkg_data(0, 84, start_screen_data);
	set_bkg_tiles(0, 0, 20, 18, start_screen_map);

	waitpad(J_A);

	// Clear screen
	clear_bkg();

	// round_start scene
	game_state.scene.number++;
	game_state.scene.phase = 0;
	game_state.scene.frame = 0;
}

// 1
void round_start()
{
	switch (game_state.scene.phase)
	{
	case 0:
		load_card_data();

		// Seed ran¬d()
		initarand(DIV_REG);

		card known_card = {
			.x = 81,
			.y = 32,
			.number = ((uint8_t)rand() % 8) + 2, // Generate a random number from 2 to 9
			.side = BACK,
		};
		game_state.round.known_card = known_card;

		card comparison_card = {
			.x = 101,
			.y = 32,
			.number = ((uint8_t)rand() % 9) + 1, // Generate a random number from 1 to 10
			.side = BACK,
		};
		game_state.round.comparison_card = comparison_card;

		game_state.scene.phase++;
		break;
	case 1:
		load_card_sprites(0, &game_state.round.known_card);
		move_card(game_state.round.known_card);
		game_state.scene.phase++;
		break;
	case 2:
		game_state.scene.frame++;
		game_state.round.known_card.x = 81 - game_state.scene.frame;
		move_card(game_state.round.known_card);

		if (game_state.scene.frame == 30)
			game_state.scene.phase++;
		break;
	case 3:
		game_state.round.known_card.side = FRONT;
		load_card_sprites(0, &game_state.round.known_card);
		move_card(game_state.round.known_card);

		// round_input
		game_state.scene.number++;
		game_state.scene.phase = 0;
		game_state.scene.frame = 0;
	default:
		break;
	}
}

// 2
void round_input(uint8_t input)
{
	bool progress = false;

	if (input & J_UP)
	{
		game_state.round.played_higher = true;
		progress = true;
	}
	else if (input & J_DOWN)
	{
		game_state.round.played_higher = false;
		progress = true;
	}

	// round_end
	if (progress)
		game_state.scene.number++;
	game_state.scene.phase = 0;
	game_state.scene.frame = 0;
}

// 3
void round_end()
{
	switch (game_state.scene.phase)
	{
	case 0:
		load_card_sprites((CARD_HEIGHT * CARD_WIDTH + 1), &game_state.round.comparison_card);
		move_card(game_state.round.comparison_card);

		game_state.scene.phase++;
		break;
	case 1:
		game_state.scene.frame++;
		if (game_state.scene.frame == 60)
			game_state.scene.phase++;
		break;
	case 2:
		game_state.round.comparison_card.side = FRONT;
		load_card_sprites(game_state.round.comparison_card.sprites[0][0], &game_state.round.comparison_card);
		move_card(game_state.round.comparison_card);

		game_state.scene.phase++;
		break;
	case 3:
		if ((game_state.round.played_higher && game_state.round.comparison_card.number > game_state.round.known_card.number) || (!game_state.round.played_higher && game_state.round.comparison_card.number < game_state.round.known_card.number))
		{
			printf("%d - You Win!\n", game_state.round.comparison_card.number);
		}
		else
		{
			printf("%d - You Lose!\n", game_state.round.comparison_card.number);
		}

		waitpad(J_A);

	 	clear_card(game_state.round.known_card);
	 	clear_card(game_state.round.comparison_card);
		clear_bkg();

		// round_start
		game_state.scene.number = 1;
		game_state.scene.phase = 0;
		game_state.scene.frame = 0;
	default:
		break;
	}
}