#include <gb/gb.h>
#include "../sprites/card_data.c"

void loadCardSprites() {
	set_sprite_data(0, 22, card_data);
}

void drawCardBack(uint8_t nb_start, uint8_t x, uint8_t y) {
	// 1st, 2nd, 3rd, 4th row
	for (uint8_t i = 0; i <= 10; i++) set_sprite_tile(i+nb_start, i);
	// 3rd row edge fix
	set_sprite_tile(nb_start+11, 3);
	set_sprite_tile(nb_start+12, 5);
	// 5th row
	for (uint8_t i = 0; i <= 2; i++) set_sprite_tile(i+nb_start+13, i+19);

	move_sprite(nb_start,    x, 	  y);
	move_sprite(nb_start+1,  x+8,  y);
	move_sprite(nb_start+2,  x+16, y);

	move_sprite(nb_start+3,  x,    y+8);
	move_sprite(nb_start+4,  x+8,  y+8);
	move_sprite(nb_start+5,  x+16, y+8);

	move_sprite(nb_start+11, x,	  y+16);
	move_sprite(nb_start+6,  x+8,  y+16);
	move_sprite(nb_start+12, x+16, y+16);

	move_sprite(nb_start+7,  x,	  y+24);
	move_sprite(nb_start+8,  x+8,  y+24);
	move_sprite(nb_start+9,  x+16, y+24);

	move_sprite(nb_start+13,  x,	  y+32);
	move_sprite(nb_start+14,  x+8,  y+32);
	move_sprite(nb_start+15,  x+16, y+32);
}


void drawCardFront(uint8_t nb_start, uint8_t x, uint8_t y) {
	// 1st row
	for (uint8_t i = 0; i <= 2; i++) set_sprite_tile(i+nb_start, i);
	// 2nd, 3rd, 4th row
	for (uint8_t i = 0; i <= 9; i++) set_sprite_tile(i+nb_start+3, i+10);
	// 5th row
	for (uint8_t i = 0; i <= 2; i++) set_sprite_tile(i+nb_start+12, i+19);

	move_sprite(nb_start,    x,    y);
	move_sprite(nb_start+1,  x+8,  y);
	move_sprite(nb_start+2,  x+16, y);

	move_sprite(nb_start+3,  x,    y+8);
	move_sprite(nb_start+4,  x+8,  y+8);
	move_sprite(nb_start+5,  x+16, y+8);

	move_sprite(nb_start+6,  x,	  y+16);
	move_sprite(nb_start+7,  x+8,  y+16);
	move_sprite(nb_start+8,  x+16, y+16);

	move_sprite(nb_start+9,  x,	  y+24);
	move_sprite(nb_start+10, x+8,  y+24);
	move_sprite(nb_start+11, x+16, y+24);

	move_sprite(nb_start+12, x,	  y+32);
	move_sprite(nb_start+13, x+8,  y+32);
	move_sprite(nb_start+14, x+16, y+32);
}