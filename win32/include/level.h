#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "space.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "List.h"
#include "particles.h"

/**
* @Purpose game specific level code
* This game is going to be a tile based side scrolling 2d platformer
*/

/**
* @brief The level structure holds data for a given game level.
*/
typedef struct
{
	int curRoute;				  /**The route number or which route the player is on*/
	char * file;                 /** Name of the file used to load the level info*/
	List    *spawnList;		    /**Monsters and obsticles to spawn*/
	List   *spawnLocations;     /**Locations for Monsters and Obsticles to spawn*/
	List    *shapeLocations;     /**Locations for static shapes*/
	List  *shapeParams;			/**The length and widths of each static shape*/
	Vector2D tilesize;          /**The length and width of each static shape in the Tile Format*/
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

void level_set_win(int n);

int level_get_win();

void level_set_lose(int n);

int level_get_lose();

void create_space();
/**
* @brief load up a new game level and move the player into it
* @param Vector2D of the position of where the player will spawn
*/
void level_transition(Vector2D pos);
/**
* @brief the touch function of the hitbox of entites. Calls the touch functions of thouse that collided with it
* @param the hitbox of the entity and its list of collisions
*/
int body_body_touch(Body *self, List *collisionList);
/**
* @brief Loads up a paper route for the game
* @param name of the route
*/
void route_load(char *name);
/**
* @brief saves the current postion of the player to save file
*/
void save_game();
/**
* @brief Loads the current player location saved to save file
*/
void load_game();
/**
* @brief creates a static shape with an entity assoicated with it
* @param doubles for x pos, y pos, w pos, h pos
*/
Entity *wall_spawn(double x, double y, double w, double h);
/**
* @brief entity assoicated with static shape is deleted
* @param Vector2D the postion of where the the shape you want to delete is
*/
void *level_wall_kill(Vector2D position);
/**
* @brief Saves the static shape paramaters onto a file
* @param Vector2D the postion you want saved, and the file you want saved to
*/
void level_wall_save(Vector2D pos, char *filename);
/**
* @brief Deletes the static shape paramaters saved onto a file
* @param Vector2D the postion you want deleted, and the file you want deleted from
*/
void level_wall_delete(Vector2D pos, char *filename);
/**
* @brief Saves a delievery point to a file
* @param Vector2D the postion you want saved, and the route you want saved to
*/
void level_delivery_save(Vector2D pos, char *filename);
/**
* @brief Saves the entity paramaters onto a file
* @param Vector2D the postion you want saved, name of the entity type desired, and the file you want saved to
*/
void level_entity_save(Vector2D pos,char* entityType ,char *filename);
/**
* @brief returns levels particle emitter
*/
ParticleEmitter *level_get_particle_emitter();
#endif