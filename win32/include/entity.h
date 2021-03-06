#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"
#include "shape.h"
#include "Body.h"

//Writing this code by following DJ, so is NOT MINE (This should act as a partially filled road map. Rework as needed!
//If an include doesnt work, put the include into the win32 include folder and the c file into the 2d game src folder
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
	Vector2D velocity;
	Vector2D flip;
	Vector2D facing;
	Shape shape;    /**The shape of the hitbox for the entity. Good for touch fucntions**/
	Body hitbox;   /**The hitbox for the entity. Good for touch fucntions**/
	//Add a velocity vector here to test if the intersection math works
	Sprite *sprite; /**<EXPLAIN TYPE>*/
	Vector2D scale; /**<EXPLAIN TYPE>*/
	Uint32 frame; /**<EXPLAIN TYPE>*/

	//This are function pointers that act as functions that entities need. Note the pointers to keep track of self
	void(*update)(struct Entity_S * self);
	void(*touch)(struct Entity_S * self, struct Entity_S * other);
	void(*think)(struct Entity_S * self);
	void(*free)(struct Entity_S *self);     /**<called when the entity is freed for any custom cleanup*/

	float health;
	float maxHealth;

	int timer; 
	float deliveries;
	float deliverTotal;
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
/*
*
*@brief calls all entities think functions in the manager
*
*/
void entity_think_all();
/*
*
*@brief sets the current postion to the hitbox
*@param takes its self as an entity 
*/
void entity_pre_sync_body(Entity *self);
/*
*
*@brief sets the current hitbox postion to the entity's
*@param takes its self as an entity
*/
void entity_post_sync_body(Entity *self);
/*
*
*@Calls a pre sync functions in the manager
*
*/
void entity_pre_sync_all();
/*
*
*@brief Calls all post sync functions in the manager
*
*/
void entity_post_sync_all();
/**
* @brief draws all entities in manager
*/
void entity_draw_all();

/**
* @brief called when enetity is near or made a collsion with another entitiy
* @param the entity itself and the entity making contact
*/
void entity_touch(Entity *self, Entity *other);

void entity_clear_all_but_player();

/**
* @brief Fires a projectile from entity
* @param the entity to fire from/ the entity who fires the projectile
*/
Entity *entity_projectile(Entity *self, Vector2D dir);
/**
* @brief Calls when a projectile hits another entity
* @param Passes in itself and the other object its colliding with
*/
void projectile_touch(Entity *self, Entity *other);

void projectile_update(Entity *self);

Entity *get_entityList();

Uint32 get_maxEntites();

int entity_wall_check(Entity *self, Vector2D dir);

void entity_world_snap(Entity *self);
#endif
