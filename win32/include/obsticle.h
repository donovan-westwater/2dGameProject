#ifndef __OBSTICLE_H__
#define __OBSTICLE_H__

#include "entity.h"
typedef enum {
	BLOCK,
	FLINGER
}ObsticleType;
//change to return entity
Entity *obsticle_spawn(Vector2D position,ObsticleType type);

void obsticle_touch(Entity *self, Entity *other);

void obsticle_update(Entity *self);

void flinger_think(Entity *self);

void block_touch(Entity *self, Entity *other);
#endif