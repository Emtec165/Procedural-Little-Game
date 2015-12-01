#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>

void kolizja_okna(int &x, int &y, int &szerokosc, int &wysokosc){
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x + 64 > szerokosc)
		x = szerokosc - 64;
	if (y + 64 > wysokosc)
		y = wysokosc - 64;
}

int main(void)
{
	al_init_primitives_addon();// inicjalizacja prymitywów
	al_init_font_addon();// inicjalizacja czcionek
	al_init_ttf_addon(); // inicjalizacja czcionek
	al_init_image_addon();

	const float gravity = 1;
	int width = 1920, height = 1080;// rozmiar okna w pikselach
	int x = 50, y = 150;
	int movmentspeed = 5, liczbaKlatek = 0;
	float velx = 0, vely = 0, jumpspeed = 15;
	bool done = false, up = false, down = false, left = false, right = false, jump = false;

	ALLEGRO_DISPLAY *display = NULL; //inicjalizacja wskaznika na bitmape glownego okna
	if (!al_init())
		return -1;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(width, height);//definiowanie wysokosci i szerokosci glownego okna
	if (!display)
		return -1;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ALLEGRO_FONT *font = al_load_font("times.ttf", 24, 0); //tworzenie wskaznika bitmapy dla czcionek

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;//tworzenie wskaznika na event
	event_queue = al_create_event_queue();//tworzenie instancji eventu
	al_install_keyboard();//inicjalizacja klawiatury
	al_register_event_source(event_queue, al_get_keyboard_event_source());//difiniowanie skad pochodzi event

	ALLEGRO_BITMAP *smoczysko = NULL;
	smoczysko = al_load_bitmap("dragon.gif");
	ALLEGRO_BITMAP *trawa1 = NULL;
	trawa1 = al_load_bitmap("trawa1.bmp");

	ALLEGRO_TIMER *czasToPiniadz = NULL;
	czasToPiniadz = al_create_timer(1.0/60);
	al_register_event_source(event_queue, al_get_timer_event_source(czasToPiniadz));
	al_start_timer(czasToPiniadz);

	while (!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				up = true;
				break;
			case ALLEGRO_KEY_DOWN:
				down = true;
				break;
			case ALLEGRO_KEY_LEFT:
				left = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				right = true;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				up = false;
				break;
			case ALLEGRO_KEY_DOWN:
				down = false;
				break;
			case ALLEGRO_KEY_LEFT:
				left = false;
				velx = 0;
				break;
			case ALLEGRO_KEY_RIGHT:
				right = false;
				velx = 0;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_TIMER){
			//if (down)
				//velx = movmentspeed;
			if (up && jump) {
				vely = -jumpspeed;
				jump = false;
			}
			if (left)
				velx = -movmentspeed;
			if (right)
				velx = movmentspeed;
			if (!jump)
				vely += gravity;
			else
				vely = 0;
			x += velx;
			y += vely;
			jump = (y + 64 >= height);
			if (jump)
				y = height - 64;
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(font, al_map_rgb(255, 255, 0), 2, 2, 0, "Liczba klatek %i", liczbaKlatek); // rysownanie liczby klatek
		kolizja_okna(x, y, width, height);
		al_draw_bitmap(trawa1, 0, 328, 0);
		al_draw_bitmap(smoczysko, x, y, 0);
		al_flip_display();
		liczbaKlatek++;
	}
	al_flip_display();
	//al_rest(5.0);
	al_destroy_display(display);

	return 0;
}