#include "monster.h"
#include "level.h"
#include "player.h"
#include "simple_logger.h"
//WIP (Should return an entity) (MonsterType Determines what think fucntion is assigned!)
Monster *monster_spawn(Vector2D location){
	Monster prep;
	Monster *out;
	Entity *self;
	self = entity_new();
	self->position = location;
	self->velocity = vector2d(0, 0);
	if (!self)return NULL;

	self->shape = shape_rect(32,32, 64, 64);
	body_set(
		&self->hitbox,
		"monster",
		1,//world layer
		1,
		0,
		1,
		location,
		vector2d(0, 1),
		10,
		1,
		0,
		&self->shape,
		self,
		NULL);
	prep.parent = self;
	out = &prep;
	return out;
	//set functions to function pointers here
}