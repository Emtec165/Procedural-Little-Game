//Objects IDS
enum IDS {PLAYER};

//Player
struct Player {
	int ID;
	int x;
	int y;
	int movmentspeed;
	float vely, jumpspeed;
	int currentframe;
	int framecount;
	int maxframe;
	int framedelay;
};