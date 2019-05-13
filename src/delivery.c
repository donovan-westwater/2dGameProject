#include "delivery.h"
#include "level.h"
#include "simple_logger.h"
#include "collisions.h"
#include "shape.h"
#include "player.h"
#include"audio.h"
Entity *delivery_spawn(Vector2D position){
	Entity *self;
	self = entity_new();
	self->scale = vector2d(0.13,0.13); // 0.3 0.3
	self->sprite = gf2d_sprite_load_image("images/delivery.png");
	self->position = position;
	self->velocity = vector2d(0, 0);
	if (!self)return NULL;

	self->shape = shape_rect(position.x, position.y, 64,64);
	
	if (&self->hitbox != NULL){
		memset(&self->hitbox, 0, sizeof(self->hitbox));
	}
	self->hitbox.inactive = 1;
	self->update = delivery_update;
	self->touch = delivery_touch;
	//have update fuction that pins delivery points in place
	space_add_static_shape(level_get_space(), self->shape);
	return self;
}
void delivery_update(Entity *self){
	//self->position = self->position;
	//self->velocity = vector2d(0, 0);
	//iterate through entities to look for shape overlaps
	Entity *other;
	Entity *list = get_entityList();
	for (int i = 0; i < get_maxEntites(); i++){
		if (list[i]._inuse == 1){
			other = &list[i];
			if (strcmp(other->hitbox.name, "projectile") == 0 && other->hitbox.team == 2) {
				if (vector2d_magnitude_compare(vector2d(self->position.x - other->position.x, self->position.y - other->position.y), 100) < 0){
					slog(" ITS WORKING ");
					//call delivery touch here! (Not nessesary atm)
					Sound *stor = sound_load("sounds/Deliverymade.mp3", 0, -1);
					sound_play(stor, 1, 0.2, -1, -1);
					player_get()->deliveries += 1;
					entity_free(other);
					Shape *del = &self->shape;
  					list_delete_data(level_get_space()->staticShapes, del);
					entity_free(self);
					}
				}
			}
		}
}
void delivery_touch(Entity *self, Entity *other){
	//Should push away other and keep self from moving
	
}