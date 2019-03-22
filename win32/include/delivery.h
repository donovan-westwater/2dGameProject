#ifndef __DELIVERY_H__
#define __DELIVERY_H__

#include "entity.h"

//change to return entity
Entity *delivery_spawn(Vector2D position);

void delivery_touch(Entity *self, Entity *other);

void delivery_update(Entity *self);
#endif