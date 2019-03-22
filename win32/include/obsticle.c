#include "obsticle.h"
#include "level.h"
#include "simple_logger.h"
#include "collisions.h"
#include "shape.h"


void flinger_update(Entity *self);

Entity *obsticle_spawn(Vector2D position,ObsticleType type){
	Entity *self;
	self = entity_new();
	self->position = position;
	self->velocity = vector2d(0, 0);
	if (!self)return NULL;

	self->shape = shape_rect(position.x, position.y, 64, 64);

	if (&self->hitbox != NULL){
		memset(&self->hitbox, 0, sizeof(self->hitbox));
	}
	self->hitbox.inactive = 1;
	self->update = obsticle_update;
	
	if (type == BLOCK){
		self->touch = block_touch;
	}
	else{
		self->think = flinger_think;
		self->touch = obsticle_touch;

	}
	//have update fuction that pins delivery points in place
	space_add_static_shape(level_get_space(), self->shape);
	return self;
}

void obsticle_touch(Entity *self, Entity *other){
	//same as block touch but less damage;
	slog(" FLINGER ");
}

void obsticle_update(Entity *self){
	Entity *other;
	Entity *list = get_entityList();
	for (int i = 0; i < get_maxEntites(); i++){
		if (list[i]._inuse == 1){
			other = &list[i];
			if (strcmp(other->hitbox.name, "player") == 0) {
				if (vector2d_magnitude_compare(vector2d(self->position.x - other->position.x, self->position.y - other->position.y), 100) < 0){
					//call block or obsticle touch here!
					if (self->touch != NULL) self->touch(self, other);
					
				}
			}
		}
	}

}

void flinger_think(Entity *self){
	Entity *other;
	Entity *list = get_entityList();
	for (int i = 0; i < get_maxEntites(); i++){
		if (list[i]._inuse == 1){
			other = &list[i];
			if (strcmp(other->hitbox.name, "player")== 0) {
				if (vector2d_magnitude_compare(vector2d(self->position.x - other->position.x, self->position.y - other->position.y), 500) < 0){
					//slog(" FLINGER ");
					//Starts pulling player toward iself
					Vector2D pull = vector2d(self->position.x - other->position.x, self->position.y - other->position.y);
					double mag = vector2d_magnitude(pull);
					pull = vector2d(pull.x / mag, pull.y / mag);
					other->position.x +=  pull.x;
					other->position.y +=  pull.y;
					//other->hitbox.position.x == pull.x;
					//other->hitbox.position.y -= pull.y;


				}
			}
		}
	}
}

void block_touch(Entity *self, Entity *other){
	slog(" BLOCK ");
}

