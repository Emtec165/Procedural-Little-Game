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

	int width = 640;// rozmiar okna w pikselach
	int height = 480, x = 50 , y = 150, movmentspeed = 5;
	float velx = 0, vely = 0, jumpspeed = 15;
	bool up = false, down = false, left = false, right = false, jump = false;

	ALLEGRO_DISPLAY *display = NULL; //inicjalizacja wskaznika na bitmape glownego okna
	if (!al_init())
		return -1;

	display = al_create_display(width, height);//definiowanie wysokosci i szerokosci glownego okna
	if (!display)
		return -1;
/*/////////////////////////////////////////////RYSOWANIE PRYMITYWÓW//////////////////////////////////////////////////////////

	al_draw_pixel(100, 100, al_map_rgb(255, 255, 255));
	al_draw_triangle(50, 150, 75, 50, 100, 150, al_map_rgb(255, 255, 255), 4);
	al_draw_circle(100, 300, 20, al_map_rgb(255, 255, 255), 2);
	al_draw_circle(140, 300, 20, al_map_rgb(255, 255, 255), 2);
	al_draw_line(100, 280, 100, 100, al_map_rgb(255, 255, 255), 2);
	al_draw_line(140, 280, 140, 100, al_map_rgb(255, 255, 255), 2);*/
	ALLEGRO_FONT *font = al_load_font("times.ttf", 24, 0); //tworzenie wskaznika bitmapy dla czcionek
	al_draw_textf(font, al_map_rgb(255, 255, 0), 150, 150, 0, "Tekst probny. Czcionka 24"); // rysownanie czcionek

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;//tworzenie wskaznika na event
	al_install_keyboard();//inicjalizacja klawiatury
	event_queue = al_create_event_queue();//tworzenie instancji eventu
	al_register_event_source(event_queue, al_get_keyboard_event_source());//difiniowanie skad pochodzi event

	///////////////////////////////////////////TIMER////////////////////////////////////////////////////////////////////////////////////////

	ALLEGRO_TIMER *czasToPiniadz = NULL;
	czasToPiniadz = al_create_timer(1.0/60);
	al_register_event_source(event_queue, al_get_timer_event_source(czasToPiniadz));
	al_start_timer(czasToPiniadz);
	int liczbaKlatek = 0;

	//////////////////////////////////////RUSZAJ SIE DRAGONKU, RUSZAJ!!!!/////////////////////////////////////////////////////////////////////
	ALLEGRO_BITMAP *smoczysko = NULL;
	smoczysko = al_load_bitmap("dragon.gif");

	bool done = false;
	while (!done){
		liczbaKlatek++;
		al_draw_textf(font, al_map_rgb(255, 255, 0), 2, 2, 0, "Liczba klatek %i", liczbaKlatek); // rysownanie liczby klatek
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				done = true;
			switch (ev.keyboard.keycode){
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

			if (ev.type == ALLEGRO_EVENT_TIMER){
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
			}

			if (ev.type == ALLEGRO_EVENT_KEY_UP) {
				if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					done = true;

				switch (ev.keyboard.keycode) {
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
		if (!jump)
			vely += gravity;
		else
			vely = 0;

		x += velx;
		y += vely;

		jump = (y + 64 >= height);

		if (jump)
			y = height - 64;

		kolizja_okna(x, y, width, height);
		al_draw_bitmap(smoczysko, x, y, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	//al_rest(5.0);
	al_destroy_display(display);

	return 0;
}