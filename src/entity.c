#include "entity.h"
#include "simple_logger.h"
#include "gf2d_draw.h"


//the code sample we made above

typedef struct EntityManager_S{
	Entity *entityList;
	Uint32 maxEntities;
}EntityManager;

static EntityManager entityManager = { 0 };

void entity_system_init(Uint32 maxEntities){

	if (!maxEntities){
		//slog("Cannot allocate zero entities");     Doesnt work yet, likely just needs an include for simple logger
		return;
	}

	entityManager.entityList = (Entity*)malloc(sizeof(Entity)*maxEntities);
	if (!entityManager.entityList){
		//slog("failed to allocate %i entities for system", maxEntities);  Doesnt work yet, likely just needs an include for simple logger
	}
	entityManager.maxEntities = maxEntities;
	memset(entityManager.entityList, 0, sizeof(Entity)*maxEntities);
}

Entity *entity_new();

void entity_draw(Entity *self){
	if (!self) return;
	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0);
}
void entity_free(Entity *self);
