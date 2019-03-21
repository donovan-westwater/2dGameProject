#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"
//Create monster struct here with its enum for different types
typedef enum {
	PATROLLER,
	CHASER,
	SHOOTER
}MonsterType;
//Get rid of this stuct
typedef struct Monster_S{

	Entity* parent;
	MonsterType type;
}Monster;

//change to return entity
Monster *monster_spawn(Vector2D position);

#endif