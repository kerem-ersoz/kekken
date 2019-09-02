#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();
int splashScreen(SDL_Event e);
int characterSelect(SDL_Event e);
int stageSelect(SDL_Event e);
int playGame(SDL_Event e);
//Loads individual image
SDL_Surface* loadSurface(std::string path);
SDL_Texture* loadTexture(std::string path);

enum INDEX{IDLE, PUNCH, BLOCK, FALL, WALK, JUMP, HADO, HOPKICK, KNOCKDOWN, LAUNCHED, PISTOL, WAKEUP, HURT, KO};
enum DIRECTION{LEFT, RIGHT, NONE};
enum GAMESTATE{PAUSE, KILL, NORMAL};
struct Menu{
	int fail;
	Menu* next; 
	Menu* prev; 
	//ADD MUSIC
	SDL_Texture* bg;
	int (*func) (SDL_Event e);
};

class Object{
	public:
		float xVel; 
		float yVel;
		SDL_Rect pos;
		SDL_Rect collider; 
		SDL_Texture* sprite[5];
		Object();
		Object(int x, int y, int w, int h, float xVel = 0, float yVel = 0);
		~Object();
};

struct Stage{
	Object* objects; 
};

class Character: public Object{
	public:
		SDL_Texture* pic;
		std::string dispName;
		int number;
		int HP; 
		Character* next; 
		Character* prev;
		bool onGround;
		bool isHurt;
		bool KO;
		enum INDEX state;
		enum INDEX active_attack;
		enum DIRECTION direction;
		int frame_counter;
		int idle_counter;
		int cooldown_counter; 
		SDL_Texture* sprite[32];
		SDL_Texture* getSprite();
		Character(unsigned number);
		void move(float timestep, SDL_Rect* bg_rect, Character** enemy);
		static void addChar(Character*, Character*);
		void handleInput();
		void handleState();
		void HandleCollision(Character** enemy);
		//void render(SDL_Renderer* gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

//The application time based timer
class LTimer{
    public:
        //Initializes variables
        LTimer();
        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();
        //Gets the timer's time
        Uint32 getTicks();
        //Checks the status of the timer
        bool isStarted();
        bool isPaused();
    private:
        //The clock time when the timer started
        Uint32 mStartTicks;
        //The ticks stored when the timer was paused
        Uint32 mPausedTicks;
        //The timer status
        bool mPaused;
        bool mStarted;
};


//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SPRITE_WIDTH = 71*1.7;
const int SPRITE_HEIGHT = 70*1.7;
const int MAXFPS = 500; 
static Character* headNode = NULL;
static Object GROUND = Object(0, SCREEN_HEIGHT, SCREEN_WIDTH, 10);
static Character CHAR_1 = Character(1);
static Character CHAR_2 = Character(2);
static Character* currentChar = &CHAR_1;
static Character* versusChar = &CHAR_2;
static Character* ally_objects[4];
static Character* enemy_objects[4];
static LTimer move_timer; 
extern SDL_Rect bg_rect; 
