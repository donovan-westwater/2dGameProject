#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"
//Create monster struct here with its enum for different types
typedef enum {
	PATROLLER,
	CHASER,
	SHOOTER
}MonsterType;

//change to return entity
Entity *monster_spawn(Vector2D position, MonsterType type);

void shooter_think(Entity *ent);
void patroller_think(Entity *ent);
void chaser_think(Entity *ent);
	
#endif