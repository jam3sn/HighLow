#include <stdio.h>
#include <stdbool.h>
#include <rand.h>

#include "./card.c"

#include "../sprites/blank_data.c"
#include "../sprites/blank_map.c"
#include "../sprites/start_screen_data.c"
#include "../sprites/start_screen_map.c"

typedef struct round {
	uint8_t roundNumber;
	card knownCard;
	card comparisonCard;
	BOOLEAN playedHigher;
} round;

struct {
	uint8_t scene;
	round round;
} gameState;

struct {
	uint8_t phase;
	uint8_t frame;
} sceneAnimation;

// 0
void startScreen() {
	set_bkg_data(0, 84, start_screen_data);
	set_bkg_tiles(0, 0, 20, 18, start_screen_map);

	waitpad(J_A);

	// Clear screen
	set_bkg_data(0, 1, blank_data);
	set_bkg_tiles(0, 0, 20, 18, blank_map);
	set_sprite_data(85, 1, blank_data);

	// roundStart scene
	gameState.scene++;
	sceneAnimation.phase = 0;
	sceneAnimation.frame = 0;

	loadCardSprites();
}

// 1
void roundStart() {
	switch (sceneAnimation.phase) {
		case 0:
			// Seed ran¬d()
			initarand(DIV_REG);

			card knownCard = {
				.x = 74,
				.y = 32,
				.number = ((uint8_t)rand() % 8) + 2, // Generate a random number from 2 to 9
				.side = BACK,
			};
			gameState.round.knownCard = knownCard;

			card comparisonCard = {
				.x = 94,
				.y = 32,
				.number = ((uint8_t)rand() % 9) + 1, // Generate a random number from 1 to 10
				.side = BACK,
			};
			gameState.round.comparisonCard = comparisonCard;

			sceneAnimation.phase++;
			break;
		case 1:
			initSprites(0, &gameState.round.knownCard);
			moveCard(gameState.round.knownCard);
			sceneAnimation.phase++;
			break;
		case 2:
			sceneAnimation.frame++;
			gameState.round.knownCard.x = 74-sceneAnimation.frame;
			moveCard(gameState.round.knownCard);

			if (sceneAnimation.frame == 30) sceneAnimation.phase++;
			break;
		case 3:
			gameState.round.knownCard.side = FRONT;
			initSprites(0, &gameState.round.knownCard);
			moveCard(gameState.round.knownCard);

			// roundInput
			gameState.scene++;
			sceneAnimation.phase = 0;
			sceneAnimation.frame = 0;
		default:
			break;
	}
}

// 2
void roundInput(uint8_t input) {
	bool progress = false;

	if (input & J_UP) {
		gameState.round.playedHigher = true;
		progress = true;
	} else if (input & J_DOWN) {
		gameState.round.playedHigher = false;
		progress = true;
	}

	// roundEnd
	if (progress) gameState.scene++;
	sceneAnimation.phase = 0;
	sceneAnimation.frame = 0;
}

// 3
void roundEnd() {
	switch (sceneAnimation.phase) {
		case 0:
			initSprites((CARD_HEIGHT*CARD_WIDTH+1), &gameState.round.comparisonCard);
			moveCard(gameState.round.comparisonCard);

			sceneAnimation.phase++;
			break;
		case 1:
			sceneAnimation.frame++;
			if (sceneAnimation.frame == 60) sceneAnimation.phase++;
			break;
		case 2:
			gameState.round.knownCard.side = FRONT;
			initSprites((CARD_HEIGHT*CARD_WIDTH+1), &gameState.round.comparisonCard);
			moveCard(gameState.round.comparisonCard);

			sceneAnimation.phase++;
			break;
		case 3:
			if ((gameState.round.playedHigher && gameState.round.comparisonCard.number > gameState.round.knownCard.number) 
				|| (!gameState.round.playedHigher && gameState.round.comparisonCard.number < gameState.round.knownCard.number)
			) {
				printf("%d - You Win!\n", gameState.round.comparisonCard.number);
			} else {
				printf("%d - You Lose!\n", gameState.round.comparisonCard.number);
			}

			waitpad(J_A);

			// roundStart
			gameState.scene = 1;
			sceneAnimation.phase = 0;
			sceneAnimation.frame = 0;
			
			clearCards();
			printf("next round");
			waitpad(J_B);
		default:
			break;
	}
	

}