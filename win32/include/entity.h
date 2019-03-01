#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"

//Writing this code by following DJ, so is NOT MINE (This should act as a partially filled road map. Rework as needed!
typedef enum
{
	ES_Idle, /**<EXPLAIN TYPE>*/
	ES_Moving, /**<EXPLAIN TYPE>*/
	ES_Pain, /**<EXPLAIN TYPE>*/
	ES_Dead, /**<EXPLAIN TYPE>*/
	ES_MAX, /**<EXPLAIN TYPE>*/
	ES_Attacking /**<EXPLAIN TYPE>*/
}EntityState;
typedef struct Entity_S
{
	//Makes a entity as private or shouldnt be touched from outside
	int id; /**<EXPLAIN TYPE>*/
	int _inuse; /**<EXPLAIN TYPE>*/
	EntityState state; /**<EXPLAIN TYPE>*/

	//does what it describes
	Vector3D rotation; /**<EXPLAIN TYPE>*/

	//This is the center point a.k.a postion
	Vector2D position; /**<EXPLAIN TYPE>*/
	Sprite *sprite; /**<EXPLAIN TYPE>*/
	Vector2D scale; /**<EXPLAIN TYPE>*/
	Uint32 frame; /**<EXPLAIN TYPE>*/

	//This are function pointers that act as functions that entities need. Note the pointers to keep track of self
	void(*update)(struct Entity_S * self);
	void(*touch)(struct Entity_S * self, struct Entity_S * other);
	void(*think)(struct Entity_S * self);
	void(*free)(struct Entity_S *self);     /**<called when the entity is freed for any custom cleanup*/


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
*@warning Currently does only the bare minimum and will need to be revisted
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
/*
*
*@brief updates value and current frame of entity
*@param get the entity that needs updating
*
*/
void entity_update(Entity *ent);
/*
*
*@brief updates all entities in the manager
*
*/
void entity_update_all();
#endif
