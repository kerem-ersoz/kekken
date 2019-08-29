#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "lp2.h"

Object::Object(){}

Object::~Object(){
	this->pos.x = 0; 
	this->pos.y = 0; 
	this->collider.x = 0; 
	this->collider.y = 0; 
}

Object::Object(int x, int y, int w, int h, float xVel, float yVel){
	this->xVel = xVel; 
	this->yVel = yVel; 
	this->pos.x = x;
	this->pos.y = y; 
	this->pos.w = w;
	this->pos.h = h; 
}

void Character::HandleCollision(Character** enemy){
	SDL_Rect* collider1 = &this->collider; 
	SDL_Rect* collider2 = &enemy[0]->collider; 
	if(SDL_HasIntersection(collider1, collider2)){
		if(this->state == IDLE || this->state == WALK || this->state == JUMP){
			if(collider1->x > collider2->x){
				xVel = 0;
				collider1->x = collider2->x + collider1->w;
				this->pos.x = collider1->x;
				this->collider.x = collider1->x;
			}
			else{
				xVel = 0;
				collider1->x = collider2->x - collider1->w;
				this->collider.x = collider1->x;
				this->pos.x = collider1->x;
			}
		}
	}
	if(abs(this->collider.x - enemy[0]->collider.x)<80){
		if(abs(this->collider.y - enemy[0]->collider.y)<30){
			if(this->active_attack == PUNCH && enemy[0]->state != BLOCK){
					this->active_attack = IDLE;
					if(!enemy[0]->isHurt){
						enemy[0]->state = HURT;
						enemy[0]->HP -= 5;
						std::cout << "char #" << enemy[0]->number << " HP: " << enemy[0]->HP << std::endl;
						enemy[0]->isHurt = true; 
					}
			}
		}	
		else if(abs(this->collider.y - enemy[0]->collider.y)<50){
			if(this->active_attack == HOPKICK && enemy[0]->state != BLOCK){
				if(!enemy[0]->isHurt){
					enemy[0]->state = FALL;
					enemy[0]->HP -= 15;
					std::cout << "char #" << enemy[0]->number << " HP: " << enemy[0]->HP << std::endl;
					enemy[0]->isHurt = true; 
				}
			}
		}
	}
	if(this->active_attack == PISTOL && enemy[0]->onGround && enemy[0]->state!=KNOCKDOWN){
		if(!enemy[0]->isHurt){
			enemy[0]->state = FALL;
			enemy[0]->HP -= 20;
			std::cout << "char #" << enemy[0]->number << " HP: " << enemy[0]->HP << std::endl;
			enemy[0]->isHurt = true; 
		}
	}
}

void Character::move(float timestep, SDL_Rect* bg_rect, Character** enemy){
	//Move the dot left or right
  this->pos.x += this->xVel * timestep;
  this->collider.x = this->pos.x;
	//If the dot went too far to the left or right
	if(this->pos.x <= 0){
	    this->pos.x = 1;
	    this->collider.x = this->pos.x; 
	    //this->xVel = 0;
	    if(bg_rect->x < 1 && 
	    	enemy[0]->direction == this->direction && 
	    	this->direction != NONE){
	    	bg_rect->x += 1;
	    }
	}
	if(this->pos.x + this->pos.w >= SCREEN_WIDTH){
	    this->pos.x = SCREEN_WIDTH - this->pos.w - 1;
	    this->collider.x = this->pos.x;
	    //this->xVel = 0;
	    if(bg_rect->x > -700 &&
	    	enemy[0]->direction == this->direction &&
	    	this->direction != NONE){
	    	bg_rect->x-=1;
	    }
	}
	//Move the dot up or down
	this->pos.y += yVel * timestep;
	this->collider.y = this->pos.y; 
	if(SDL_HasIntersection(&this->pos, &GROUND.pos)){
		this->pos.y = GROUND.pos.y - this->pos.h - 10;
		this->collider.y = this->pos.y; 
		this->yVel = 0;
		this->xVel = 0;
		this->onGround = true; 
		this->direction = NONE;
		if(this->state == FALL){
			this->state = KNOCKDOWN;
			return;
		}
		this->state = IDLE; 
	}
  if(!this->onGround){
  	this->yVel+=5;
  }
}

Character::Character(unsigned number){
	this->pic = NULL;
	this->number = number; 
	this->dispName = "CHAR_";
	std::stringstream ss; 
	ss << number; 
	this->dispName.append(ss.str());
	addChar(headNode, this);
	this->pos.w = SPRITE_WIDTH;
	this->pos.h = SPRITE_HEIGHT;
	this->pos.x = 420 + (number*pow(-1, number)*100);
	this->pos.y = 500;
	this->HP = 120; 
	this->collider = this->pos; 
	this->collider.h = 30;
	this->collider.w = 51;
	this->onGround = true;
	this->isHurt = false; 
	this->active_attack = IDLE; 
	this->direction = NONE;
	this->state = IDLE; 
	this->cooldown_counter = 0;
	///
} 

SDL_Texture* Character::getSprite(){
	int num_frames = 0;
	if(this->state == IDLE){
		return this->sprite[0]; 
	}
	else if(this->state == WALK){
		num_frames = 4; 
		int sprite_index = (SDL_GetTicks()/100) % num_frames;
		return this->sprite[1+sprite_index];
	}
	else if(this->state == PUNCH){
		num_frames = 4;
		int sprite_index = (this->frame_counter/40) % num_frames;
		return this->sprite[5+sprite_index];
	}
	else if(this->state == BLOCK){
		return this->sprite[9];
	}
	else if(this->state == PISTOL){
		num_frames = 6;
		int sprite_index = (this->frame_counter/30) % num_frames;
		return this->sprite[21+sprite_index];
	}
	else if(this->state == FALL){
		return this->sprite[10];
	}
	else if(this->state == KNOCKDOWN){
		return this->sprite[11];
	}
	else if(this->state == WAKEUP){
		num_frames = 4; 
		int sprite_index = (SDL_GetTicks()/100) % num_frames; 
		return this->sprite[27+sprite_index];
	}
	else if(this->state == HOPKICK){
		num_frames = 6;
		int sprite_index = (this->frame_counter/40) % num_frames;
		return this->sprite[12+sprite_index];
	}
	else if(this->state == HADO){
		num_frames = 3;
		int sprite_index = (this->frame_counter/75) % num_frames;
		return this->sprite[18+sprite_index];
	}
	else if(this->state == HURT){
		return this->sprite[31];
	}
	else if(!this->onGround){
		return this->sprite[1];
	}
	return this->sprite[0]; 
}

void Character::addChar(Character* head, Character* insert){
	if (head == NULL){ 
	    head = insert; 
	    head->next = head; 
	    head->prev = head; 
	    return; 
	} 
	Character* last = head->prev; 
	insert->next = head; 
	head->prev = insert; 
	insert->prev = last; 
	last->next = insert; 
}

void Character::handleInput(){
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	this->idle_counter+=1;
	if(this->number == 1){
		if(this->state == JUMP){
			if(keystates[SDL_SCANCODE_RSHIFT]){
				this->state = HOPKICK;
				return;
			}
			if(keystates[SDL_SCANCODE_DOWN]){
				this->state = FALL;
				return;
			}	
		}
		else if(this->state == IDLE || this->state == WALK){
			if(keystates[SDL_SCANCODE_RGUI]){
				this->state = PISTOL;
				return;
			}
			else if(keystates[SDL_SCANCODE_UP]){
				this->state = JUMP;
				return;
			}
			else if(keystates[SDL_SCANCODE_LEFT]){
				this->direction = LEFT;
				this->state = WALK;
				return;
			}
			else if(keystates[SDL_SCANCODE_RIGHT]){
				this->direction = RIGHT;
				this->state = WALK;
				return;
			}
			else if(keystates[SDL_SCANCODE_SPACE]){
				this->state = PUNCH;
			}
			else if(keystates[SDL_SCANCODE_RALT]){
				this->state = BLOCK; 
			}
			else{
				this->state = IDLE;
				this->direction = NONE;
			}
		}
		else if(this->state == BLOCK){
			if(!keystates[SDL_SCANCODE_RALT]){
				this->state = IDLE;
			}
		}
	}
	if(this->number == 2){
		if(this->state == JUMP){
			if(keystates[SDL_SCANCODE_F]){
				this->state = HOPKICK;
				return;
			}
		}
		if(this->state == FALL){
			if(keystates[SDL_SCANCODE_F]){
				this->state = HOPKICK;
			}
		}
		else if(this->state == IDLE || this->state == WALK){
			if(keystates[SDL_SCANCODE_E]){
				this->state = PISTOL;
				return;
			}
			else if(keystates[SDL_SCANCODE_W]){
				this->state = JUMP;
				return;
			}
			else if(keystates[SDL_SCANCODE_A]){
				this->direction = LEFT;
				this->state = WALK;
				return;
			}
			else if(keystates[SDL_SCANCODE_D]){
				this->direction = RIGHT;
				this->state = WALK;
				return;
			}
			else if(keystates[SDL_SCANCODE_S]){
				this->state = PUNCH;
			}
			else if(keystates[SDL_SCANCODE_C]){
				this->state = BLOCK; 
			}
			else{
				this->state = IDLE;
				this->direction = NONE;
			}
		}
		else if(this->state == BLOCK){
			if(!keystates[SDL_SCANCODE_C]){
				this->state = IDLE;
			}
		}
	}
}

void Character::handleState(){
	if(this->HP <= 0){
		this->KO = true;
	}
	if(this->state == HOPKICK){
		if(this->frame_counter == 300){
			this->state = IDLE;
			this->frame_counter = 0;
			return;
		}
		else{
			if(this->number == 1){
				this->xVel = 500*.9;
				this->direction = RIGHT;
			}
			if(this->number == 2){
				this->xVel = -0.01*.8;
				this->direction = LEFT;
			}
			this->frame_counter += 1; 
			this->onGround = false;
			if(this->frame_counter >= 150)
				this->active_attack = IDLE;
			else
				this->active_attack = HOPKICK;
			//move_timer.start();
			return;
		}
	}
	else if(this->state == HADO){
			/*if(move_timer.isStarted()){
					std::cout << "HADO " << move_timer.getTicks() << std::endl; 
					if(move_timer.getTicks() > 100){
						move_timer.stop();
						return;
					}
					else{
						return;
					}
			}*/
			if(this->frame_counter == 200){
				this->state = IDLE;
				this->frame_counter = 0;
				return;
			}
			else if(this->onGround){
				//this->state = HADO;
				this->xVel = 0;
				this->frame_counter += 1;
				//move_timer.start();
				return;
			}
	}
	else if(this->state == PISTOL){
			/*if(move_timer.isStarted()){
					std::cout << "HADO " << move_timer.getTicks() << std::endl; 
					if(move_timer.getTicks() > 100){
						move_timer.stop();
						return;
					}
					else{
						return;
					}
			}*/
			if(this->cooldown_counter == 0){
				if(this->frame_counter == 100)
					this->active_attack = PISTOL; 
				if(this->frame_counter == 230){
					this->state = IDLE;
					this->frame_counter = 0;
					this->active_attack = IDLE;
					this->cooldown_counter +=1;
					return;
				}
				else if(this->onGround){
					this->xVel = 0;
					this->frame_counter += 1;
					//move_timer.start();
					return;
				}
			}
			else if(this->cooldown_counter < 1000){
				this->state = IDLE; 
			}
			else{
				this->cooldown_counter = 0;
			}
	}
	else if(this->state == JUMP){
		if(this->frame_counter == 50){
			this->state = IDLE;
			this->frame_counter = 0; 
		}
		if(this->onGround){
			if(this->direction == RIGHT)
				this->xVel = 500;
			if(this->direction == LEFT)
				this->xVel = -1;
			this->yVel = -500; 
			this->onGround = false;
			return;
		}
	}
	else if(this->state == FALL){
		if(this->onGround){
			this->yVel = -200;
			this->onGround = false;
			if(this->number == 1)
				this->xVel = -15; 
			if(this->number == 2)
				this->xVel = 990;
			return;
		}
	}
	else if(this->state == HURT){
		if(this->frame_counter >= 100){
			this->state = IDLE;
			this->isHurt = false; 
			this->frame_counter = 0; 
		}
		else{
			this->xVel = 0;
			this->frame_counter+=1;
			return;
		}
	}
	else if(this->state == WALK && this->onGround){
			if(this->frame_counter >= 50){
				this->state = IDLE;
				this->frame_counter = 0; 
			}
			else if(this->direction == LEFT){
				this->xVel = -80; 
				this->frame_counter+=1;
				//this->direction = LEFT;
				//this->state = WALK;
				return;
			}
			else if(this->direction == RIGHT){
				this->xVel = 500; 
				this->frame_counter+=1;
				//this->direction = RIGHT;
				//this->state = WALK;
				return;
			}
	}
	else if(this->state == PUNCH && this->onGround){
		if(this->frame_counter == 75)
			this->active_attack = PUNCH;
		if(this->frame_counter >= 150){
			this->state = IDLE;
			this->active_attack = IDLE;
			this->frame_counter =0; 
			this->idle_counter=0;
		}
		else if(this->idle_counter>=80){
			if(this->number == 1){
				this->collider.x += 10;
			}
			if(this->number == 2){
				this->collider.x -= 10;
			}
			this->xVel = 0;
			this->frame_counter += 1;
			this->direction = NONE;
		}
	}
	else if(this->state == BLOCK && this->onGround){
		this->xVel = 0;
		this->frame_counter +=1; 
		this->direction = NONE;
	}
	else if(this->state == KNOCKDOWN){
		if(this->frame_counter >= 400){
			this->state = WAKEUP;
			this->frame_counter = 0; 
		}
		else{
			this->frame_counter +=1;
			this->xVel = 0; 
		}
	}
	else if(this->state == WAKEUP){
		if(this->frame_counter >= 50){
			this->state = IDLE;
			this->isHurt = false;
			this->frame_counter = 0; 
		}
		else{
			this->frame_counter += 1;
			this->xVel = 0; 
		}
	}
	else{
			this->direction = NONE;
			if(this->state != JUMP || 
				 this->state != HOPKICK || 
				 this->state != HADO){
				this->state = IDLE;
			}
			if(this->xVel>0)
				this->xVel-=10;
			if(this->xVel<0)
				this->xVel+=3;
			return;
	}
}