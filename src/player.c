#include "player.h"
#include "level.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "camera.h"
#include "collisions.h"
#include "gui.h"
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
	//Define facing and velocity and rotation
	self->facing = vector2d(0, -1);
	self->velocity = vector2d(0, 0);

	self->deliveries = 0;
	self->deliverTotal = 4;
	self->health = 50;
	self->maxHealth = 50;
	self->sprite = gf2d_sprite_load_image("images/playerv2.png");
	
	//Define shape
	self->shape = shape_rect(0, 0, 10, 10);
	//Define hitbox
	
	body_set(&self->hitbox,
		"player",
		1,
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
	self->scale = vector2d(1, 1); // 1 1
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
	double N;
	double M;
	//GUI UPDATES HERE
	if (self->maxHealth)gui_set_health(self->health / self->maxHealth);
	if (self->deliverTotal)gui_set_progress(self->deliveries / self->deliverTotal);

	keys = SDL_GetKeyboardState(NULL);
	mouse = SDL_GetMouseState(&mx, &my);
	mx += camera_get_position().x;
	my += camera_get_position().y;
	
	//self->velocity.x = (self->velocity.x)*(self->facing.x);
	//self->velocity.y = (self->velocity.y)*(self->facing.y);
	
	
	
	
	//slog("%lf", self->rotation.z);
	//slog("  %lf %lf", (self->velocity.x+1)*self->facing.x, (self->velocity.y+1)*self->facing.y);
	double watch = SDL_sqrt(SDL_pow(self->velocity.x, 2) + SDL_pow(self->velocity.y, 2));
	if (SDL_sqrt(SDL_pow(self->velocity.x,2)+SDL_pow(self->velocity.y,2)) > 0.001){ //use pathagerion therorm to del with this ogif (abs(self->velocity.x) > 0.001 && abs(self->velocity.y) > 0.001)
		self->velocity.x += -0.1*self->velocity.x;
		self->velocity.y += -0.1*self->velocity.y;
	}
	else{
		self->velocity.x = 0;
		self->velocity.y = 0;
	}

	
	//Have next velocity be a rotated version of the previous velocity (Not moving is the exception)
	//To do this find a way to predict the magitude of the next velocity or have facing be rotated at the same time as velocity
	//leaning towards the latter
	if (keys[SDL_SCANCODE_W]){ 
		//self->position.y -= 2;
		if (vector2d_equal(self->velocity, vector2d(0, 0))) vector2d_copy(self->velocity, self->facing);
		self->velocity.x += 0.9*self->velocity.x;
		self->velocity.y += 0.9*self->velocity.y;
		//slog("%lf", vector2d_magnitude(self->velocity));
		//slog("%lf", vector2d_magnitude(self->velocity));
	}
	if (keys[SDL_SCANCODE_S]){
		//self->position.y += 2;
		Vector2D flip = vector2d(-self->facing.x, -self->facing.y);
		if (vector2d_equal(self->velocity, vector2d(0, 0))) vector2d_copy(self->velocity, flip);
		self->velocity.x -= 0.2*self->velocity.x;
		self->velocity.y -= 0.2*self->velocity.y;
		
		
	}
	//Get angle of rotation from curent vector!
	if (keys[SDL_SCANCODE_D]){
		//self->position.x += 2;
		self->rotation.z += 1;
		self->rotation.x = self->hitbox.shape->s.r.w / 2;
		self->rotation.y = self->hitbox.shape->s.r.h / 2;
		//self->facing = vector2d(SDL_cos((self->rotation.z - 90)*(M_PI / 180)), SDL_sin((self->rotation.z - 90) *(M_PI / 180)));
		self->facing = vector2d_rotate(self->facing, (1*M_PI) / 180);
		vector2d_copy(self->velocity, vector2d_rotate(self->velocity, (1 * M_PI) / 180));
		slog("%lf", vector2d_magnitude(self->velocity));
		//slog("%lf %lf", self->velocity.x, self->velocity.y);
		
	}
	if (keys[SDL_SCANCODE_A]){
		//self->position.x -= 2;
		self->rotation.z -= 1;
		self->rotation.x = self->hitbox.shape->s.r.w / 2;
		self->rotation.y = self->hitbox.shape->s.r.h / 2;
		//self->facing = vector2d(SDL_cos((self->rotation.z - 90) *(M_PI / 180)), SDL_sin((self->rotation.z - 90)*(M_PI / 180)));
		self->facing = vector2d_rotate(self->facing, (-1*M_PI) / 180);
		vector2d_copy(self->velocity, vector2d_rotate(self->velocity, (-1 * M_PI) / 180));
		slog("%lf", vector2d_magnitude(self->velocity));
		//slog("%lf %lf", self->velocity.x, self->velocity.y);
	}
	//The breaks
	if (keys[SDL_SCANCODE_F]){
		if (abs(self->velocity.x) > 0.001 && abs(self->velocity.y) > 0.001){
			self->velocity.x += -self->velocity.x / 2;
			self->velocity.y += -self->velocity.y / 2;
		}
		else{
			self->velocity.x = 0;
			self->velocity.y = 0;
		}
	}
	//Should act as the maximum velocity 
	//Ceiling should be higher than the actual
	if (vector2d_magnitude(self->velocity) > 2){
		double mag = vector2d_magnitude(self->velocity);
		Vector2D unit = vector2d(self->velocity.x/mag, self->velocity.y / mag);
		self->velocity.x = 4 * unit.x;
		self->velocity.y = 4 * unit.y;
	}
	/*
	if (self->velocity.x < -2){
		self->velocity.x = -2;
	}
	if (self->velocity.y < -2){
		self->velocity.y = -2;
	}
	if (self->velocity.x > 2){
		self->velocity.x = 2;
	}
	if (self->velocity.y > 2){
		self->velocity.y = 2;
	}
	*/
	entity_world_snap(self);

	if (keys[SDL_SCANCODE_UP]) camera_set_position(vector2d(camera_get_position().x, camera_get_position().y - 1));
	if (keys[SDL_SCANCODE_DOWN]) camera_set_position(vector2d(camera_get_position().x, camera_get_position().y + 1));
	if (keys[SDL_SCANCODE_LEFT]) camera_set_position(vector2d(camera_get_position().x - 1, camera_get_position().y));
	if (keys[SDL_SCANCODE_RIGHT]) camera_set_position(vector2d(camera_get_position().x + 1, camera_get_position().y));

	if (keys[SDL_SCANCODE_SPACE] && self->timer % 30 == 0) {
		double x = mx - self->position.x;
		double y = my - self->position.y;
		//slog("%d %d", mx, my);
		double mag = vector2d_magnitude(vector2d(x,y));
		Vector2D dir = vector2d(x/mag,y/mag);
		entity_projectile(self,dir);
	}
	if (keys[SDL_SCANCODE_O] && self->timer % 25 == 0)
	{
		level_transition(vector2d(900,600));
	}
	//BIKE TRICK [WIP]
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		//should lock in the spin!
		self->rotation.z += 1;
		self->rotation.x = self->hitbox.shape->s.r.w/2;
		self->rotation.y = self->hitbox.shape->s.r.h / 2;
		//self->rotation.x = self->rotation.x*SDL_cos(self->rotation.z + 50) - self->rotation.y*SDL_sin(self->rotation.z + 50);
		//self->rotation.y = self->rotation.x*SDL_cos(self->rotation.z+50) + self->rotation.y*SDL_sin(self->rotation.z+50);
		//slog("%lf", self->rotation.x);
	}
	//camera_set_position(cameraPos);
	//camera_set_position(vector2d(10,10));
	
	//This is camera tracking. To make more accurate, seperate the conditions so that all directions have their own check
	if (self->position.x > (camera_get_position().x + camera_get_dimensions().w)){
		camera_move(vector2d(camera_get_dimensions().w,0));
	}
	if (self->position.y > (camera_get_position().y + camera_get_dimensions().h)){
		camera_move(vector2d(0,camera_get_dimensions().h));
	}
	if (self->position.x < (camera_get_position().x )){
		camera_move(vector2d(-camera_get_dimensions().w, 0));
	}
	if (self->position.y < (camera_get_position().y)){
		camera_move(vector2d(0, -camera_get_dimensions().h));
	}
	//player_draw(self);
	if (self->health < 0){
		slog("YOU LOSE!");
		level_set_lose(1);
		//WIP NEED TO MOVE OUT INTO GAME
		//Sprite *failure = gf2d_sprite_load_image("images/failure.png");
		//Vector2D center = camera_get_position();
		//gf2d_sprite_draw_image(failure, center);
		
		self->_inuse = 0;
		entity_free(self);
			
		
	}
	if (self->deliveries >= self->deliverTotal && self->deliverTotal != 0){
		slog(" YOU COMPLETED THIS ROUTE!");
		self->deliveries = 0;
		level_set_win(1);
		//WIP NEED TO MOVE OUT INTO GAME 
		//Sprite *victory = gf2d_sprite_load_image("images/victory.png");
		//Vector2D center = camera_get_position();
		//gf2d_sprite_draw_image(victory, center);
		
		//level_transition("levels/route2.txt", vector2d(900, 600));
	}
	//slog("%lf %lf", self->position.x, self->position.y);
	//Vector2D  center = vector2d(self->position.x + (self->hitbox.shape->s.r.w / 2), self->position.x+ (self->hitbox.shape->s.r.h / 2));
	//slog("           %lf %lf", center.x, center.y);
	Vector2D drawpoint = vector2d(self->position.x + 50 * (self->facing.x+0.001), self->position.y + 50 * (self->facing.y+0.1));
	vector2d_add(drawpoint, drawpoint, camera_get_offset());
	gf2d_draw_line(self->position, drawpoint, vector4d(255, 255, 1, 255));

	//slog("%lf", (self->velocity.x / self->velocity.y)-(N/M));

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
	//slog(" is touching ");
	//Placeholder for future ideas. Doesnt nothing atm
	return 1;
}