#include "entity.h"
#include "simple_logger.h"
#include "gf2d_draw.h"


//the code sample we made above
//A bunch of this code comes from dj's project. This is to give me a start and help me understand systems

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
			entityManager.entityList[i]._inuse = 1;
			
			return &entityManager.entityList[i];
		}
	}
}

void entity_draw(Entity *self){
	if (!self) return;
	gf2d_sprite_draw(
		self->sprite,
		self->position,
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
void entity_update(Entity *ent,float newFrame){
	ent->frame += newFrame;
	//put everything else here later
}
