#include "player.h"
#include "level.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "camera.h"
#include "collisions.h"
//ask about how to add control over player (ask Natlile or Omar)
static Entity *_player = NULL;  //represents the player

void player_draw(Entity *self);
void player_think(Entity *self); //Not going to be implimented yet
void player_update(Entity *self);
int  player_touch(Entity *self, Entity *other); //Not going to be implimented yet
int  player_damage(Entity *self, int amount, Entity *source); //Not going to be implimented yet
void player_die(Entity *self); //Not going to be implimented yet

//This is more of a placeholder
typedef struct
{
	int baseSpeed;
}PlayerData;

Entity *player_new(Vector2D position){
	Entity *self = entity_new();
	if (!self) return NULL;
	self->position = position;
	self->velocity = vector2d(0, 0);
	self->sprite = gf2d_sprite_load_image("images/motorcycle_blue.png");
	
	//Define shape
	self->shape = shape_rect(0, 0, 10, 10);
	//Define hitbox
	body_set(&self->hitbox,
		"player",
		PLAYER_LAYER,
		1, //clip layer
		1,
		2,
		position,
		vector2d(0, 0),
		10,
		0,
		100,
		&self->shape,
		self,
		NULL);
	//Define postion, scale and rotation (Opt, meant to overide existing values in entity)
	self->scale = vector2d(1, 1);
	self->shape = shape_rect(0, 0, 44, 100);
	//Assign function pointers
	self->update = player_update;
	self->touch = player_touch;
	self->hitbox.touch = body_body_touch;
	//Assign entity to _player to finalize player
	_player = self;
	//return self aka the player
	return self;
}
Entity *player_get(){
	return _player;
}
//test fucntion Dont use
void player_draw(Entity *self){
	double scalex = 5;
	double scaley = 10;
	if (&self->scale != NULL){
		scalex = self->scale.x;
		scaley = self->scale.y;
	}
	gf2d_draw_line(vector2d(self->position.x, self->position.y + scaley), vector2d(self->position.x+(scalex/2), self->position.y - scaley),vector4d(255,255,1,255));
	gf2d_draw_line(vector2d(self->position.x + (scalex / 2), self->position.y - scaley), vector2d(self->position.x, self->position.y - (scaley / 2)), vector4d(255, 255, 1, 255));
	gf2d_draw_line(vector2d(self->position.x, self->position.y - (scaley / 2)), vector2d(self->position.x - (scalex/2), self->position.y - scaley), vector4d(255, 255, 1, 255));
	gf2d_draw_line(vector2d(self->position.x - (scalex / 2), self->position.y - scaley), vector2d(self->position.x, self->position.y + scaley), vector4d(255, 255, 1, 255));
}

void player_update(Entity *self){
	Vector2D cameraPos = { 0, 0 };
	self->frame += 0.1;
	if (self->frame >= 16.0)self->frame = 0;
	self -> timer++;
	//call draw fucntion here
	const Uint8 * keys;
	const Uint32 * mouse;
	int mx, my;
	keys = SDL_GetKeyboardState(NULL);
	mouse = SDL_GetMouseState(&mx, &my);
	if (keys[SDL_SCANCODE_W]) self->position.y -= 2;
	if (keys[SDL_SCANCODE_S]) self->position.y += 2;
	if (keys[SDL_SCANCODE_D]) self->position.x += 2;
	if (keys[SDL_SCANCODE_A]) self->position.x -= 2;

	if (keys[SDL_SCANCODE_UP]) camera_set_position(vector2d(camera_get_position().x, camera_get_position().y - 1));
	if (keys[SDL_SCANCODE_DOWN]) camera_set_position(vector2d(camera_get_position().x, camera_get_position().y + 1));
	if (keys[SDL_SCANCODE_LEFT]) camera_set_position(vector2d(camera_get_position().x - 1, camera_get_position().y));
	if (keys[SDL_SCANCODE_RIGHT]) camera_set_position(vector2d(camera_get_position().x + 1, camera_get_position().y));
	if (keys[SDL_SCANCODE_SPACE] && self->timer % 40 == 0) {
		double x = mx - self->position.x;
		double y = my - self->position.y;
		double mag = vector2d_magnitude(vector2d(x,y));
		Vector2D dir = vector2d(x/mag,y/mag);
		entity_projectile(self,dir);
	}

	//camera_set_position(cameraPos);
	//camera_set_position(vector2d(10,10));
	//slog("%lf %lf", self->position.x, self->position.y);
	//This is camera tracking. To make more accurate, seperate the conditions so that all directions have their own check
	if (self->position.x > (camera_get_position().x + camera_get_dimensions().w) || self->position.y > (camera_get_position().y + camera_get_dimensions().h)){
		camera_set_position(self->position);
	}
	if (self->position.x < (camera_get_position().x ) || self->position.y < (camera_get_position().y )){
		Vector2D move = vector2d(self->position.x - camera_get_dimensions().w, self->position.y - camera_get_dimensions().h);
		camera_set_position(move);
	}

	//player_draw(self);
}

void player_set_position(Vector2D position)
{
	if (!_player)
	{
		slog("no player loaded");
		return;
	}
	vector2d_copy(_player->position, position);
	vector2d_copy(_player->hitbox.position, position);
}
int player_touch(Entity *self, Entity *other){
	slog(" is touching ");
	return 1;
}