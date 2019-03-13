#include "player.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
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
	//Define shape
	self->shape = shape_rect(0, 0, 10, 10);
	//Define hitbox
	body_set(&self->hitbox,
		"player",
		1,
		1,
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
	self->scale = vector2d(50, 50);
	self->shape = shape_rect(-self->scale.x / 2, -self->scale.y, self->scale.x, self->scale.y*2);
	//Assign function pointers
	self->update = player_update;
	//Assign entity to _player to finalize player
	_player = self;
	//return self aka the player
	return self;
}
Entity *player_get(){
	return _player;
}

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
	self->frame += 0.1;
	if (self->frame >= 16.0)self->frame = 0;
	//call draw fucntion here
	player_draw(self);
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