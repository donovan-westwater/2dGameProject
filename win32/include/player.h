#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"


/**
* @brief spawn a new player entity
* @param position place the player here
* @return a pointer to the player entity
*/
Entity *player_new(Vector2D position);

/**
* @brief get a pointer to the player entity
* @return a pointer to the player entity
*/
Entity *player_get();

/**
* @brief sets the player position to the one specified
* @note risk of solid collision
* @param position the new position for the player
*/
void player_set_position(Vector2D position);

void player_draw(Entity* self);

//Add player spawn later










#endif