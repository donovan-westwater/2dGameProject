#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"

//Writing this code by following DJ, so is NOT MINE (This should act as a partially filled road map. Rework as needed!
typedef enum
{
	ES_Idle,
	ES_Moving,
	ES_Pain,
	ES_Dead,
	ES_MAX,
	ES_Attacking
}EntityState;
typedef struct Entity_S
{
	//Makes a entity as private or shouldnt be touched from outside
	int _inuse;
	EntityState state;
	//does what it describes
	Vector3D rotation;
	//This is the center point a.k.a postion
	Vector2D position;
	Sprite *sprite;
	float frame;

	//This are function pointers that act as functions that entities need. Note the pointers to keep track of self
	void(*update)(struct Entity_S * self);
	void(*touch)(struct Entity_S * self, struct Entity_S * other);
	void(*think)(struct Entity_S * self);


}Entity;

/*
*
*@brief initialize entity manager system
*@param maxEntities  maximum total entities to support
*
*/
void entity_system_init(Uint32 maxEntities);

/*
*
*@brief return a pointer to a new entity
*@param NULL if out of entities or system not initialized, a pointer to a block entity otherwise
*
*/
Entity *entity_new();

/*
*
*@brief free an entity
*@param didnt catch this
*
*/
void entity_free(Entity *self);

/*
*
*@brief idraw the entity sprite
*@param didnt catch this
*
*/
void entity_draw(Entity *self);



#endif
