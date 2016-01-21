#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "objects.h"
#include <cmath>
#include <random>

//global variables
int width = 1920;
int height = 1080;
enum KEYS {UP, DOWN, LEFT, RIGHT, JUMP};
bool key[5] = { false, false, false, false, false };
int kierunek = 0;

//prototypes
void InitPlayer(Player &gracz);
void MovePlayerJump(Player &gracz);
void MovePlayerDown(Player &gracz);
void MovePlayerLeft(Player &gracz, int &kierunek);
void MovePlayerRight(Player &gracz, int &kierunek);
bool EndGame(Player &gracz){
	if (gracz.x >= 28000)
		return true;
	else return false;
}


int main(void) {
	//primitive variables
	const float gravity = 1;
	int liczbaKlatek = 0;
	bool done = false, redraw = false;
	int szerokosc_bitmapy = 1554;
	int drzewa_rand[30];
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(-500, 500);

	for (int i = 0; i < 30; i++)
		drzewa_rand[i] = dist(mt);

	//object variables
	Player gracz;

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL; //inicjalizacja wskaznika na bitmape glownego okna
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;//tworzenie wskaznika na event
	ALLEGRO_TIMER *czasToPiniadz = NULL;
	ALLEGRO_BITMAP *trawa1 = NULL;
	ALLEGRO_BITMAP *drzewo = NULL;
	ALLEGRO_BITMAP *spritesheet = NULL;
	ALLEGRO_FONT *font = NULL;
	ALLEGRO_TRANSFORM camera;
	ALLEGRO_SAMPLE *song = NULL;
	ALLEGRO_SAMPLE_INSTANCE *songInstance = NULL;

	//initialization functions
	if (!al_init())
		return -1;
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
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
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(10);
	song = al_load_sample("song1.ogg");
	songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	trawa1 = al_load_bitmap("trawa.png");
	drzewo = al_load_bitmap("drzewo.png");
	spritesheet = al_load_bitmap("spritesheet.png");

	font = al_load_font("arial.ttf", 24, 0);
	czasToPiniadz = al_create_timer(1.0 / 60);

	//Game Init
	InitPlayer(gracz);

	event_queue = al_create_event_queue();//tworzenie instancji eventu
	al_register_event_source(event_queue, al_get_timer_event_source(czasToPiniadz));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());//difiniowanie skad pochodzi event

	al_start_timer(czasToPiniadz);
	al_play_sample_instance(songInstance);
	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_SPACE:
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
			case ALLEGRO_KEY_SPACE:
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
				MovePlayerLeft(gracz, kierunek);
			if (key[RIGHT]){
				MovePlayerRight(gracz, kierunek);
				if (EndGame(gracz)){
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_textf(font, al_map_rgb(0, 0, 0), 900, 540, ALLEGRO_ALIGN_CENTRE, "More in next version!");
					al_flip_display();
					al_rest(5);
					done = true;
				}
			}
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
			al_translate_transform(&camera, std::floorf(width / 2 - gracz.x),80);
			al_use_transform(&camera);

			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_textf(font, al_map_rgb(0, 0, 0), 1300, 300, 0, "Use arrows to move"); // rysownanie liczby klatek

			for (int i = 0; i < 15; i++)
				al_draw_bitmap(drzewo, ((1500 * i) + drzewa_rand[i - 1]), -150, 0);
				

			al_draw_tinted_scaled_rotated_bitmap_region(spritesheet, gracz.currentframe * 300, 0, 300, 255, al_map_rgb(255,255,255), 127.5, 150, gracz.x, gracz.y, 1, 1, 0, kierunek);
			al_draw_bitmap(trawa1, 0, 475, 0);
			for (int i = 1; i < 20; i++)
				al_draw_bitmap(trawa1, szerokosc_bitmapy * i, 475, 0);

			//Default camera position (to draw GUI)
			al_identity_transform(&camera);
			al_use_transform(&camera);
			al_draw_textf(font, al_map_rgb(0, 0, 0), 2, 2, 0, "Liczba klatek %i", liczbaKlatek); // rysownanie liczby klatek
			al_draw_textf(font, al_map_rgb(0, 0, 0), 1850, 2, ALLEGRO_ALIGN_RIGHT, "x :%i  y :%i", gracz.x, gracz.y); // rysownanie liczby klatek

			al_flip_display();
		}
	}
	al_stop_sample_instance(songInstance);
	al_destroy_sample(song);
	al_destroy_sample_instance(songInstance);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(czasToPiniadz);
	al_destroy_display(display);

	return 0;
}

void InitPlayer(Player &gracz) {
	gracz.x = 960;
	gracz.y = 150;
	gracz.ID = PLAYER;
	gracz.jumpspeed = 25;
	gracz.movmentspeed = 10;
	gracz.vely = 0;

	gracz.currentframe = 0;
	gracz.framecount = 0;
	gracz.framedelay = 10;
	gracz.maxframe = 3;
	}
void MovePlayerJump(Player &gracz) {
	gracz.vely = -gracz.jumpspeed;
	key[JUMP] = false;
}
void MovePlayerLeft(Player &gracz, int &kierunek) {
	kierunek = 1;
	gracz.x -= gracz.movmentspeed;
	if (gracz.x < 960)
		gracz.x = 960;
	if (++gracz.framecount >= gracz.framedelay){
		gracz.currentframe++;
		if (gracz.currentframe >= gracz.maxframe)
			gracz.currentframe = 0;
		gracz.framecount = 0;
	}
}
void MovePlayerRight(Player &gracz, int &kierunek) {
	kierunek = 0;
	gracz.x += gracz.movmentspeed;
	if (++gracz.framecount >= gracz.framedelay){
		gracz.currentframe++;
		if (gracz.currentframe >= gracz.maxframe)
			gracz.currentframe = 0;
		gracz.framecount = 0;
	}
}
void MovePlayerDown(Player &gracz) {
	gracz.y += gracz.movmentspeed;
	if (gracz.y > height)
		gracz.y = height;
}