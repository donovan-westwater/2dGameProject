#include "entity.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "space.h"
#include "camera.h"

//the code sample we made above
//A bunch of this code comes from dj's project. This is to give me a start and help me understand systems
//ADD THINK SYSTEM!

typedef struct EntityManager_S{
	Entity *entityList;
	Uint32 maxEntities;
}EntityManager;

static EntityManager entityManager = { 0 };

void entity_system_init(Uint32 maxEntities){

	if (!maxEntities){
		slog("Cannot allocate zero entities");    // Doesnt work yet, likely just needs an include for simple logger
		return;
	}

	entityManager.entityList = (Entity*)malloc(sizeof(Entity)*maxEntities);
	if (!entityManager.entityList){
		slog("failed to allocate %i entities for system", maxEntities); // Doesnt work yet, likely just needs an include for simple logger
	}
	entityManager.maxEntities = maxEntities;
	memset(entityManager.entityList, 0, sizeof(Entity)*maxEntities);
	slog("ENTITY OPEN NOW!");
}
void gf2d_entity_system_close()
{
	int i;
	if (entityManager.entityList != NULL)
	{
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			entity_free(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	memset(&entityManager, 0, sizeof(EntityManager));
	slog("entity system closed");
}

Entity *entity_new(){
	for (int i = 0; i < entityManager.maxEntities; i++){
		if (entityManager.entityList[i]._inuse == 0){
			memset(&entityManager.entityList[i], 0, sizeof(Entity));
			//When creating different monsters, Get rid of this. This is temp, use inheirance to define these values!!!!!!
			entityManager.entityList[i].position = vector2d(0, 1);
			entityManager.entityList[i].sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
			entityManager.entityList[i].scale = vector2d(1, 1);
			//entityManager.entityList[i].shape = shape_circle(entityManager.entityList[i].position.x, 
			//							entityManager.entityList[i].position.y,10); //This should help tie the hitbox and the visual togehter
			entityManager.entityList[i].shape = shape_rect(32, 32, 64, 64); //To pefectly bound sprite, use the width and heigth of the sprite as the shape parameters
			entityManager.entityList[i].velocity = vector2d(1,1);
			entityManager.entityList[i].hitbox.velocity = vector2d(1, 1);
			entityManager.entityList[i].hitbox.position = vector2d(110, 500);
			entityManager.entityList[i].hitbox.shape = &entityManager.entityList[i].shape;
			entityManager.entityList[i]._inuse = 1;
			entityManager.entityList[i].frame = 0;
			entityManager.entityList[i].update = entity_update;
			return &entityManager.entityList[i];
		}
	}
}

void entity_draw(Entity *self){

	Vector2D drawPosition;
	if (!self)return;
	if (!self->_inuse)return;

	vector2d_add(drawPosition, self->position, camera_get_offset());

	if (!self) return;
	gf2d_sprite_draw(
		self->sprite,
		drawPosition,
		&self->scale,
		NULL,
		NULL,
		NULL,
		NULL,
		self->frame);
	//slog("IM DRAWN!!!");
}
void entity_free(Entity *self){
	if (!self){
		return;
	}
	if (self->free){
		self->free(self);
	}
	memset(self, 0, sizeof(Entity));


}

void entity_update(Entity *ent){
	ent->frame += 0.1;
	if (ent->frame >= 16.0)ent->frame = 0;
	//ent->position.x += 0.5;
	//slog("postion of x: %lf    hitbox postion of x: %lf", ent->position.x,ent->hitbox.position.x);
	//slog("Velocity of x: %lf    hitbox velocity of x: %lf", ent->velocity.x, ent->hitbox.velocity.x);
	//if (ent->position.x >= 100)ent->position.x = -ent->position.x;

	ent->position.x += ent->velocity.x;
	ent->position.y += ent->velocity.y;
	//Updates hitbox to new postion
	//vector2d_copy(ent->position, ent->hitbox.position);
	//vector2d_copy(ent->velocity, ent->hitbox.velocity);
	//entity_draw(ent);
	//Body *draw = &ent->hitbox;
	//body_draw(draw,vector2d(0,0));
	
	//put everything else here later
}

void entity_update_all(){
	Entity *self;
	for (int i = 0; i < entityManager.maxEntities; i++){
		if (entityManager.entityList[i]._inuse == 1){
			self = &entityManager.entityList[i];
			if (entityManager.entityList[i].update != NULL){
				entityManager.entityList[i].update(self);
			}
		}
	}
}
void entity_think_all(){
		int i;
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			if (entityManager.entityList[i]._inuse == 0)continue;
			if (entityManager.entityList[i].think != NULL){
				entityManager.entityList[i].think(&entityManager.entityList[i]);
			}
		}
}/** @brief adds all existing entites' bodies to the space's list
 * @param the space to add the bodies
 * @Note: this is more of a test function. Once the map map is added, this should not be used
 */
void adding_all_bodies_to_space(Space* space){
	for (int i = 0; i < entityManager.maxEntities; i++){
		if (entityManager.entityList[i]._inuse == 1){
			entityManager.entityList[i].hitbox.cliplayer = 1;
			entityManager.entityList[i].hitbox.touchlayer = 1;
			entityManager.entityList[i].hitbox.worldclip = 1;
			entityManager.entityList[i].hitbox.elasticity = 100;
			entityManager.entityList[i].hitbox.mass = 10;
			space_add_body(space,&entityManager.entityList[i].hitbox);
		}
	}
}

void entity_draw_all(){
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i]._inuse == 0)continue;
		entity_draw(&entityManager.entityList[i]);
	}

}
void entity_pre_sync_body(Entity *self)
{
	if (!self)return;// nothin to do
	vector2d_copy(self->hitbox.velocity, self->velocity);
	vector2d_copy(self->hitbox.position, self->position);
}

void entity_post_sync_body(Entity *self)
{
	if (!self)return;// nothin to do
	//    slog("entity %li : %s old position(%f,%f) => new position (%f,%f)",self->id,self->name,self->position,self->body.position);
	vector2d_copy(self->position, self->hitbox.position);
	vector2d_copy(self->velocity, self->hitbox.velocity);
}
void entity_pre_sync_all()
{
    int i;
    for (i = 0; i < entityManager.maxEntities;i++)
    {
        if (entityManager.entityList[i]._inuse == 0)continue;
        entity_pre_sync_body(&entityManager.entityList[i]);
    }
}

void entity_post_sync_all()
{
    int i;
    for (i = 0; i < entityManager.maxEntities;i++)
    {
        if (entityManager.entityList[i]._inuse == 0)continue;
        entity_post_sync_body(&entityManager.entityList[i]);
    }
}

