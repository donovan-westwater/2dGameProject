#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "space.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "List.h"


/**
* @Purpose game specific level code
* This game is going to be a tile based side scrolling 2d platformer
*/

/**
* @brief The level structure holds data for a given game level.
*/
typedef struct
{
	TextLine    backgroundImage;    /**<background image for this level*/ //Might keep
	TextLine    connectedSections;  /**Sections that are connected to this one  NOT GOING TO BE USED  */
	List    *spawnList;		    /**Monsters and obsticles to spawn*/
	List   *spawnLocations;     /**Locations for Monsters and Obsticles to spawn*/
	List    *shapeLocations;     /**Locations for static shapes*/
	List  *shapeParams;			/**The length and widths of each static shape*/
}LevelInfo;

/**
* @brief clears all assets loaded for the live level
* Called at program exit and before loading the next level
*/
void level_clear();

void level_draw();
void level_update();

/**
* @brief create the live level based on the provided level info
* @param linfo the level info to create the level for
* @param space if true, create the physical space for the game to run
*/
void level_init(LevelInfo *linfo, Uint8 space);

void level_info_free(LevelInfo *linfo);
LevelInfo *level_info_load(char *filename);

/**
* @brief create a new level info and populate based on parameters
* @param backgroundImage the image file to use as the background
* @param connectedSections the Sections that are connected to this one
* @param spawnList the Monsters and obsticles to spawn
* @param spawnLocations the Locations for Monsters and Obsticles to spawn
* @param shapeParams The length and widths of each static shape
*/
LevelInfo *level_info_create(
	TextLine    backgroundImage,    
TextLine    connectedSections,  
TextLine    spawnList,		   
Vector2D*   spawnLocations,     
Vector2D*    shapeLocations,     
Vector2D*  shapeParams		
	);


/**
* @brief adds an entity to the level physics space
* @note entity must have a Body defined
* @param ent the entity to add to the space
*/
void level_add_entity(Entity *ent);

/**
* @brief removes an entity from the level physics space
* @note this should not be done during a space update (ie: collision callbacks)
* @param ent the entity to be removed
*/
void level_remove_entity(Entity *ent);

/**
* @brief build the collision space
* @note unneeded in editor mode
*/
void level_make_space();
void level_build_tile_space(LevelInfo *linfo);

Space *level_get_space();
void create_space();
/**
* @brief load up a new game level and move the player into it
* @param
*/
void level_transition(char *filename, const char *playerTarget, Uint32 targetId);

int body_body_touch(Body *self, List *collisionList);
#endif