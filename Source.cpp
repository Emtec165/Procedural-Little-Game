#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include "objects.h"
#include <cmath>

//global variables
int width = 1920;
int height = 1080;
enum KEYS {UP, DOWN, LEFT, RIGHT, JUMP};
bool key[5] = { false, false, false, false, false };

//prototypes
void InitPlayer(Player &gracz);
void MovePlayerJump(Player &gracz);
void MovePlayerDown(Player &gracz);
void MovePlayerLeft(Player &gracz);
void MovePlayerRight(Player &gracz);

int main(void) {
	//primitive variables
	const float gravity = 1;
	int liczbaKlatek = 0;
	bool done = false, redraw = false;
	int szerokosc_bitmapy = 1554;

	//object variables
	Player gracz;

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL; //inicjalizacja wskaznika na bitmape glownego okna
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;//tworzenie wskaznika na event
	ALLEGRO_TIMER *czasToPiniadz = NULL;
	ALLEGRO_BITMAP *player = NULL;
	ALLEGRO_BITMAP *trawa1 = NULL;
	ALLEGRO_BITMAP *trawa2 = NULL;
	ALLEGRO_FONT *font = NULL;
	ALLEGRO_TRANSFORM camera;

	//initialization functions
	if (!al_init())
		return -1;
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	display = al_create_display(width, height);//definiowanie wysokosci i szerokosci glownego okna
	if (!display)
		return -1;

	//Allegro module unit
	al_install_keyboard();//inicjalizacja klawiatury
	al_init_primitives_addon();// inicjalizacja prymitywów
	al_init_font_addon();// inicjalizacja czcionek
	al_init_ttf_addon(); // inicjalizacja czcionek
	al_init_image_addon();

	player = al_load_bitmap("dragon.gif");
	trawa1 = al_load_bitmap("trawa.png");
	trawa2 = al_load_bitmap("trawa2.gif");
	font = al_load_font("times.ttf", 24, 0);
	czasToPiniadz = al_create_timer(1.0 / 60);

	//Game Init
	InitPlayer(gracz);

	event_queue = al_create_event_queue();//tworzenie instancji eventu
	al_register_event_source(event_queue, al_get_timer_event_source(czasToPiniadz));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());//difiniowanie skad pochodzi event

	al_start_timer(czasToPiniadz);

	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				key[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				key[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				key[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[RIGHT] = true;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				key[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				key[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				key[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[RIGHT] = false;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			liczbaKlatek++;
			if (key[UP] && key[JUMP])
				MovePlayerJump(gracz);
			if (key[LEFT])
				MovePlayerLeft(gracz);
			if (key[RIGHT])
				MovePlayerRight(gracz);
			if (!key[JUMP])
				gracz.vely += gravity;
			else
				gracz.vely = 0;
			gracz.y += gracz.vely;
			key[JUMP] = (gracz.y + 64 >= height -350);
			if (key[JUMP])
				gracz.y = height - 64 - 350;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_identity_transform(&camera);
			al_translate_transform(&camera, std::floorf(width / 2 - gracz.x), std::floorf((height / 2 - gracz.y) + 200));
			al_use_transform(&camera);

			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_textf(font, al_map_rgb(255, 255, 0), 2, 2, 0, "Liczba klatek %i", liczbaKlatek); // rysownanie liczby klatek
			al_draw_bitmap(player, gracz.x, gracz.y, 0);
			al_draw_bitmap(trawa1, 0, 475, 0);
			al_draw_bitmap(trawa1, szerokosc_bitmapy, 475, 0);
			al_draw_bitmap(trawa1, szerokosc_bitmapy * 2, 475, 0);
			al_draw_bitmap(trawa1, szerokosc_bitmapy * 3, 475, 0);
			al_draw_bitmap(trawa1, szerokosc_bitmapy * 4, 475, 0);
			al_draw_bitmap(trawa1, szerokosc_bitmapy * 5, 475, 0);
			al_draw_bitmap(trawa1, szerokosc_bitmapy * 6, 475, 0);

			//Default camera position (to draw GUI)
			al_identity_transform(&camera);
			al_use_transform(&camera);

			al_flip_display();
			
		}
	}
	al_destroy_event_queue(event_queue);
	al_destroy_timer(czasToPiniadz);
	al_destroy_display(display);

	return 0;
}

void InitPlayer(Player &gracz) {
	gracz.x = 50;
	gracz.y = 150;
	gracz.ID = PLAYER;
	gracz.jumpspeed = 25;
	gracz.movmentspeed = 30;
	gracz.vely = 0;
}
void MovePlayerJump(Player &gracz) {
	gracz.vely = -gracz.jumpspeed;
	key[JUMP] = false;
}
void MovePlayerLeft(Player &gracz) {
	gracz.x -= gracz.movmentspeed;
	if (gracz.x < 0)
		gracz.x = 0;
}
void MovePlayerRight(Player &gracz) {
	gracz.x += gracz.movmentspeed;
	//  gracz.y = funckja(gracz.x);
	//if (gracz.x > width - 64)
		//gracz.x = width - 64;
}
void MovePlayerDown(Player &gracz) {
	gracz.y += gracz.movmentspeed;
	if (gracz.y > height)
		gracz.y = height;
}

//int funkcja(int x)  {
//	if x < 100
//		y = 100;
//	if x> 100 i x < 1000
//		y = 0.5 * x + 200; 
//}