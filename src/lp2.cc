#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "lp2.h"

Menu SPLASH_SCREEN, CHARACTER_SELECT, STAGE_SELECT, PLAY_GAME;
Menu* currentMenu = &SPLASH_SCREEN;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTimer stepTimer;
//Event handler
SDL_Event e;

////////////////////////////////////////

void menuConstructor(){
	SPLASH_SCREEN.next = &CHARACTER_SELECT;
	SPLASH_SCREEN.prev = NULL;
	SPLASH_SCREEN.fail = 0;
	SPLASH_SCREEN.bg = NULL;
	SPLASH_SCREEN.func = splashScreen;
	CHARACTER_SELECT.next = &STAGE_SELECT;
	CHARACTER_SELECT.prev = &SPLASH_SCREEN;
	CHARACTER_SELECT.fail = 0;
	CHARACTER_SELECT.bg = NULL;
	CHARACTER_SELECT.func = characterSelect; 
	STAGE_SELECT.next = &PLAY_GAME; 
	STAGE_SELECT.prev = &CHARACTER_SELECT;
	STAGE_SELECT.fail = 0; 
	STAGE_SELECT.bg = NULL;
	STAGE_SELECT.func = stageSelect;
	PLAY_GAME.next = NULL;
	PLAY_GAME.prev = &CHARACTER_SELECT;
	PLAY_GAME.fail = 0;
	PLAY_GAME.bg = NULL; 
	PLAY_GAME.func = playGame; 
}

bool init(){
	//Initialization flag
	bool success = true;
	ally_objects[0] = currentChar;
	enemy_objects[0] = versusChar;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	if(TTF_Init()==-1){
		printf("SDL_ttf could not initialize! TTF Error: %s\n", TTF_GetError());
	}
	else{
		//Create window
		gWindow = SDL_CreateWindow( "xxxxxxx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Get window surface
			//Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) &imgFlags))
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
		}
	}
	return success;
}

SDL_Surface* loadSurface(std::string path){
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if( loadedSurface == NULL ){
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	return loadedSurface;
}

SDL_Texture* loadTexture(std::string path){
  //The final texture
  SDL_Texture* newTexture = NULL;
  //Load image at specified path
  SDL_Surface* loadedSurface = loadSurface(path.c_str());
  //Create texture from surface pixels
  newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
  if(newTexture == NULL){
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
  }
  //Get rid of old loaded surface
  SDL_FreeSurface(loadedSurface);
  return newTexture;
}

bool loadMedia(){
	//Loading success flag
	bool success = true;
	//Load default surface
	SPLASH_SCREEN.bg = loadTexture("backgrounds/splash0.bmp");
	if( SPLASH_SCREEN.bg == NULL ){
		printf( "Failed to load default image!\n" );
		success = false;
	}
	//Load up surface
	CHARACTER_SELECT.bg = loadTexture("backgrounds/charsel.bmp" );
	if( CHARACTER_SELECT.bg == NULL ){
		printf( "Failed to load up image!\n" );
		success = false;
	}
	STAGE_SELECT.bg = loadTexture( "backgrounds/charsel.bmp" );
	if( STAGE_SELECT.bg == NULL ){
		printf( "Failed to load up image!\n" );
		success = false;
	}
	PLAY_GAME.bg = loadTexture("backgrounds/level1.bmp");
	if(PLAY_GAME.bg == NULL){
		printf("Failed to load up image!\n");
		success = false;
	}
	CHAR_1.pic = loadTexture("sprites/CHAR_1.bmp");
	if( CHAR_1.pic == NULL ){
		printf( "Failed to load up char_1 image!\n" );
		success = false;
	}
	CHAR_1.sprite[0] = loadTexture("sprites/char1_idle.bmp");
	if( CHAR_1.sprite[0] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[1] = loadTexture("sprites/char1_walk0.bmp");
	if( CHAR_1.sprite[1] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[2] = loadTexture("sprites/char1_walk1.bmp");
	if( CHAR_1.sprite[2] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[3] = loadTexture("sprites/char1_walk2.bmp");
	if( CHAR_1.sprite[3] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[4] = loadTexture("sprites/char1_walk3.bmp");
	if( CHAR_1.sprite[4] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[5] = loadTexture("sprites/char1_punch0.bmp");
	if( CHAR_1.sprite[5] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[6] = loadTexture("sprites/char1_punch1.bmp");
	if( CHAR_1.sprite[6] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[7] = loadTexture("sprites/char1_punch2.bmp");
	if( CHAR_1.sprite[7] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[8] = loadTexture("sprites/char1_punch3.bmp");
	if( CHAR_1.sprite[8] == NULL ){
		printf( "Failed to load up char_1 sprite!\n" );
		success = false;
	}
	CHAR_1.sprite[9] = loadTexture("sprites/char1_block.bmp");
	if(CHAR_1.sprite[9] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[10] = loadTexture("sprites/char1_fall.bmp");
	if(CHAR_1.sprite[10] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[11] = loadTexture("sprites/char1_knockdown.bmp");
	if(CHAR_1.sprite[11] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[12] = loadTexture("sprites/char1_hopkick0.bmp");
	if(CHAR_1.sprite[12] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[13] = loadTexture("sprites/char1_hopkick0.bmp");
	if(CHAR_1.sprite[13] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[14] = loadTexture("sprites/char1_hopkick1.bmp");
	if(CHAR_1.sprite[14] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[15] = loadTexture("sprites/char1_hopkick2.bmp");
	if(CHAR_1.sprite[15] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[16] = loadTexture("sprites/char1_hopkick3.bmp");
	if(CHAR_1.sprite[16] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[17] = loadTexture("sprites/char1_hopkick4.bmp");
	if(CHAR_1.sprite[17] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[18] = loadTexture("sprites/char1_hado0.bmp");
	if(CHAR_1.sprite[18] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[19] = loadTexture("sprites/char1_hado1.bmp");
	if(CHAR_1.sprite[19] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[20] = loadTexture("sprites/char1_hado2.bmp");
	if(CHAR_1.sprite[20] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[21] = loadTexture("sprites/char1_pistol0.bmp");
	if(CHAR_1.sprite[21] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[22] = loadTexture("sprites/char1_pistol1.bmp");
	if(CHAR_1.sprite[22] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[23] = loadTexture("sprites/char1_pistol2.bmp");
	if(CHAR_1.sprite[23] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[24] = loadTexture("sprites/char1_pistol3.bmp");
	if(CHAR_1.sprite[24] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[25] = loadTexture("sprites/char1_pistol4.bmp");
	if(CHAR_1.sprite[25] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[26] = loadTexture("sprites/char1_pistol5.bmp");
	if(CHAR_1.sprite[26] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[27] = loadTexture("sprites/char1_wakeup0.bmp");
	if(CHAR_1.sprite[27] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[28] = loadTexture("sprites/char1_wakeup0.bmp");
	if(CHAR_1.sprite[28] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[29] = loadTexture("sprites/char1_wakeup1.bmp");
	if(CHAR_1.sprite[29] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[30] = loadTexture("sprites/char1_wakeup1.bmp");
	if(CHAR_1.sprite[30] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_1.sprite[31] = loadTexture("sprites/char1_hurt.bmp");
	if(CHAR_1.sprite[31] == NULL){
		printf("Failed to load up char_1 sprite!\n");
		success = false; 
	}
	CHAR_2.pic = loadTexture("sprites/CHAR_1a.bmp");
	if( CHAR_2.pic == NULL ){
		printf( "Failed to load up char_2 image!\n" );
		success = false;
	}
	CHAR_2.sprite[0] = loadTexture("sprites/char1ib_idle.bmp");
	if( CHAR_1.sprite[0] == NULL ){
		printf( "Failed to load up char_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[1] = loadTexture("sprites/char1ib_walk0.bmp");
	if( CHAR_2.sprite[1] == NULL ){
		printf( "Failed to load up char_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[2] = loadTexture("sprites/char1ib_walk1.bmp");
	if( CHAR_2.sprite[2] == NULL ){
		printf( "Failed to load up char_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[3] = loadTexture("sprites/char1ib_walk2.bmp");
	if( CHAR_2.sprite[3] == NULL ){
		printf( "Failed to load up char_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[4] = loadTexture("sprites/char1ib_walk3.bmp");
	if( CHAR_2.sprite[4] == NULL ){
		printf( "Failed to load up char_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[5] = loadTexture("sprites/char1ib_punch0.bmp");
	if( CHAR_2.sprite[5] == NULL ){
		printf( "Failed to load up CHAR_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[6] = loadTexture("sprites/char1ib_punch1.bmp");
	if( CHAR_2.sprite[6] == NULL ){
		printf( "Failed to load up CHAR_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[7] = loadTexture("sprites/char1ib_punch2.bmp");
	if( CHAR_2.sprite[7] == NULL ){
		printf( "Failed to load up CHAR_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[8] = loadTexture("sprites/char1ib_punch3.bmp");
	if( CHAR_2.sprite[8] == NULL ){
		printf( "Failed to load up CHAR_2 sprite!\n" );
		success = false;
	}
	CHAR_2.sprite[9] = loadTexture("sprites/char1ib_block.bmp");
	if(CHAR_2.sprite[9] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[10] = loadTexture("sprites/char1ib_fall.bmp");
	if(CHAR_2.sprite[10] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[11] = loadTexture("sprites/char1ib_knockdown.bmp");
	if(CHAR_2.sprite[11] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[12] = loadTexture("sprites/char1ib_hopkick0.bmp");
	if(CHAR_2.sprite[12] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[13] = loadTexture("sprites/char1ib_hopkick0.bmp");
	if(CHAR_2.sprite[13] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[14] = loadTexture("sprites/char1ib_hopkick1.bmp");
	if(CHAR_2.sprite[14] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[15] = loadTexture("sprites/char1ib_hopkick2.bmp");
	if(CHAR_2.sprite[15] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[16] = loadTexture("sprites/char1ib_hopkick3.bmp");
	if(CHAR_2.sprite[16] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[17] = loadTexture("sprites/char1ib_hopkick4.bmp");
	if(CHAR_2.sprite[17] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	/*CHAR_2.sprite[18] = loadTexture("sprites/char1ib_hado0.bmp");
	if(CHAR_2.sprite[18] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[19] = loadTexture("sprites/char1ib_hado1.bmp");
	if(CHAR_2.sprite[19] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[20] = loadTexture("sprites/char1ib_hado2.bmp");
	if(CHAR_2.sprite[20] == NULL){
		printf("Failed to load up CHAR_2 sprite!\n");
		success = false; 
	}*/
	CHAR_2.sprite[21] = loadTexture("sprites/char1ib_pistol0.bmp");
	if(CHAR_2.sprite[21] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[22] = loadTexture("sprites/char1ib_pistol1.bmp");
	if(CHAR_2.sprite[22] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[23] = loadTexture("sprites/char1ib_pistol2.bmp");
	if(CHAR_2.sprite[23] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[24] = loadTexture("sprites/char1ib_pistol3.bmp");
	if(CHAR_2.sprite[24] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[25] = loadTexture("sprites/char1ib_pistol4.bmp");
	if(CHAR_2.sprite[25] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[26] = loadTexture("sprites/char1ib_pistol5.bmp");
	if(CHAR_2.sprite[26] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[27] = loadTexture("sprites/char1ib_wakeup0.bmp");
	if(CHAR_2.sprite[27] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[28] = loadTexture("sprites/char1ib_wakeup0.bmp");
	if(CHAR_2.sprite[28] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[29] = loadTexture("sprites/char1ib_wakeup1.bmp");
	if(CHAR_2.sprite[29] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[30] = loadTexture("sprites/char1ib_wakeup1.bmp");
	if(CHAR_2.sprite[30] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	CHAR_2.sprite[31] = loadTexture("sprites/char1ib_hurt.bmp");
	if(CHAR_2.sprite[31] == NULL){
		printf("Failed to load up char_2 sprite!\n");
		success = false; 
	}
	return success;
}

void close(){
	//Deallocate surfaces
	Menu* menuDeAlloc = &SPLASH_SCREEN;
	while(menuDeAlloc != NULL){
		SDL_DestroyTexture(menuDeAlloc->bg);
		menuDeAlloc->bg = NULL;
		menuDeAlloc = menuDeAlloc->next;
	}
	Character* charDeAlloc = &CHAR_1;
    while(charDeAlloc->pic != NULL){
		SDL_DestroyTexture(charDeAlloc->pic);
		charDeAlloc->pic = NULL;
		charDeAlloc = charDeAlloc->next;
	}
	//Destroy window
	SDL_DestroyWindow( gWindow );
	SDL_DestroyRenderer( gRenderer );
	gWindow = NULL;
	gRenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]){
	//Start up SDL and create window
	//The window renderer
	if(!init()){
		printf("Failed to initialize!\n");
	}
	else{
		//Load media
		Character::addChar(headNode, &CHAR_1);
		Character::addChar(headNode, &CHAR_2);
		menuConstructor();
		if(!loadMedia()){
			printf( "Failed to load media!\n" );
		}
		else{
			//Main loop flag
			bool quit = false;
			//While application is running
			while( !quit ){
				currentMenu->fail = currentMenu->func(e);
				if(currentMenu->fail){
					if(currentMenu->prev == NULL){
						break;
					}
					currentMenu = currentMenu->prev; 
					continue;
				}
				currentMenu = currentMenu->next;
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}

int splashScreen(SDL_Event e){
	//Clear screen
  SDL_RenderClear( gRenderer );
  //Render texture to screen
	SDL_RenderCopy( gRenderer, SPLASH_SCREEN.bg, NULL, NULL );
  //Update screen
  SDL_RenderPresent( gRenderer );
	//Handle events on queue
	//this loop is for the main Menu/splash screen ONLY
	while(1){
		if(SDL_PollEvent(&e) != 0){
			//User requests quit
			if( e.type == SDL_QUIT ){
				close();
				exit(1);
			}
			//User presses a key
			else if( e.type == SDL_KEYDOWN )
			{
				//Select surfaces based on key press
				switch( e.key.keysym.sym )
				{
					case SDLK_p:
					return 0;
					case SDLK_q:
					close();
					exit(1);
				}
			}
		}
	}
}

int characterSelect(SDL_Event e){
	SDL_Rect currentCharRect;
	SDL_Rect versusCharRect;  
	const Uint8* keystate;
	currentCharRect.x = SCREEN_WIDTH/16; 
	currentCharRect.y = SCREEN_HEIGHT/4; 
	currentCharRect.w = 233*1.5;
	currentCharRect.h = 237*1.5; 

	versusCharRect.x = SCREEN_WIDTH/1.7;
	versusCharRect.y = SCREEN_HEIGHT/4;
	versusCharRect.w = 233*1.5;
	versusCharRect.h = 237*1.5; 
	while(1){if(SDL_PollEvent(&e) != 0){
		//this loop is for the Character select screen
		//User requests quit
		if( e.type == SDL_QUIT ){
			close();
			exit(1);
		} 
		keystate = SDL_GetKeyboardState(NULL);
    	if(keystate[SDL_SCANCODE_LEFT]){    			
    		currentChar = currentChar->prev;
    	}
    	if(keystate[SDL_SCANCODE_RIGHT]){    			
	    	currentChar = currentChar->next;
    	}
    	if(keystate[SDL_SCANCODE_RETURN]/* && keystate[SDL_SCANCODE_F]*/){    			
	    	break;
    	}
    	if(keystate[SDL_SCANCODE_A]){
    		versusChar = versusChar->next; 
    	}
    	if(keystate[SDL_SCANCODE_D]){
    		versusChar = versusChar->next; 
    	}
    	if(keystate[SDL_SCANCODE_Q]){    			
	    	return 1;
    	}
		//Clear screen
    	SDL_RenderClear(gRenderer);
    	//Render texture to screen
    	SDL_RenderCopy(gRenderer, CHARACTER_SELECT.bg, NULL, NULL);      
    	SDL_RenderCopy(gRenderer, currentChar->pic, NULL, &currentCharRect);
    	SDL_RenderCopy(gRenderer, versusChar->pic, NULL, &versusCharRect);    	//Update screen
    	SDL_RenderPresent(gRenderer);
	}
	}
	return 0;
}

int stageSelect(SDL_Event e){
	//will be filled in later
	return 0;
}

int playGame(SDL_Event e){
	enum GAMESTATE state = NORMAL;
	int ko_frame_counter = 0;
	TTF_Font* Impact = TTF_OpenFont("impact.ttf", 13); //this opens a font style and sets a size
	SDL_Color Black = {0, 0, 0};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Rect ally_hp_rect; //create a rect
	ally_hp_rect.x = 80;  ally_hp_rect.y = 50; ally_hp_rect.w = 150; ally_hp_rect.h = 50; 
	SDL_Rect enemy_hp_rect = ally_hp_rect; 
	enemy_hp_rect.x = 550;
	SDL_Rect ko_rect;
	ko_rect.x = SCREEN_WIDTH/2 - 35; ko_rect.y = SCREEN_HEIGHT/2; ko_rect.h = 70; ko_rect.w = 70;
	SDL_Rect bg_rect; 
	bg_rect.x = 0; bg_rect.y = 0; bg_rect.w = SCREEN_WIDTH*2; bg_rect.h = SCREEN_HEIGHT;
	//Handle events on queue
	while(1){
		SDL_PollEvent(&e);
		//User requests quit
		if(e.type == SDL_QUIT){
			exit(1);
		}
		if(currentChar->KO || versusChar->KO){
			ko_frame_counter+=1;
			if(ko_frame_counter>=300){
				state = KILL;
				break; 
			}
		}
		//Calculate time step for movement
		float timestep = stepTimer.getTicks() / 1000.f;
		//the timer is started after the first tick is generated
		//this means the first timestep in the first iteration of the game loop wil always be zero
		stepTimer.start(); 
		//this is where the magic happens
		//take input, manage states 
		currentChar->handleInput(); currentChar->handleState(); 
		versusChar->handleInput(); versusChar->handleState();
		//move and handle collisions
		currentChar->move(timestep, &bg_rect, enemy_objects); currentChar->HandleCollision(enemy_objects); 
		versusChar->move(timestep, &bg_rect, ally_objects); versusChar->HandleCollision(ally_objects);
		if(currentChar->cooldown_counter != 0)
			currentChar->cooldown_counter += 1;
		if(versusChar->cooldown_counter != 0)
			versusChar->cooldown_counter +=1;
		SDL_Texture* currentSprite = currentChar->getSprite();
		SDL_Texture* versusSprite = versusChar->getSprite();
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		//Render everything 
    SDL_RenderCopy(gRenderer, PLAY_GAME.bg, NULL, &bg_rect);
		//SDL_RenderCopy(gRenderer, ally_hp, NULL, &ally_hp_rect);
		//SDL_RenderCopy(gRenderer, enemy_hp, NULL, &enemy_hp_rect);
		SDL_RenderCopy(gRenderer, versusSprite, NULL, &versusChar->pos);
		SDL_RenderCopy(gRenderer, currentSprite, NULL, &currentChar->pos);
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	if(state == KILL){
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Impact, "K.O.", Black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage); //now you can convert it into a texture
		SDL_RenderCopy(gRenderer, Message, NULL, &ko_rect);
		SDL_RenderPresent(gRenderer);
	}
	SDL_Delay(1000);
	close();
	exit(1);
	//return 0; 
}