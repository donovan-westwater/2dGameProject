#include "collisions.h"
#include "simple_logger.h"
#include "dynamic_body.h"
#include "gf2d_draw.h"
#include <stdlib.h>

Collision *collision_new()
{
	Collision *collision = NULL;
	collision = (Collision *)malloc(sizeof(Collision));
	if (!collision)
	{
		slog("failed to allocate data for a collision object");
		return NULL;
	}
	memset(collision, 0, sizeof(Collision));
	return collision;
}

void collision_free(Collision *collision)
{
	if (!collision)return;
	free(collision);
}

void collision_list_clear(List *list)
{
	int i, count;
	Collision *collision;
	if (!list)return;
	count = list_get_count(list);
	for (i = 0; i < count; i++)
	{
		collision = (Collision*)list_get_nth(list, i);
		if (!collision)continue;
		collision_free(collision);
	}
	for (i = 0; i < count; i++)
	{
		list_delete_last(list);
	}
}

void collision_list_free(List *list)
{
	int i, count;
	Collision *collision;
	if (!list)return;
	count = list_get_count(list);
	for (i = 0; i < count; i++)
	{
		collision = (Collision*)list_get_nth(list, i);
		if (!collision)continue;
		collision_free(collision);
	}
	list_delete(list);
}

Collision *collision_space_static_shape_clip(Shape a, Shape *s)
{
	Collision *collision;
	Vector2D poc, normal;
	if (!shape_overlap_poc(a, *s, &poc, &normal))
	{
		return NULL;
	}
	collision = collision_new();
	collision->collided = 1;
	collision->blocked = 1;
	vector2d_copy(collision->pointOfContact, poc);
	vector2d_copy(collision->normal, normal);
	collision->shape = s;
	collision->body = NULL;
	collision->bounds = 0;
	collision->timeStep = 0;
	return collision;
}

Collision *collision_space_dynamic_body_clip(Shape a, DynamicBody *d)
{
	Shape s;
	Collision *collision;
	Vector2D poc, normal;
	if (!d)return NULL;
	s = dynamic_body_to_shape(d);
	if (!shape_overlap_poc(a, s, &poc, &normal))
	{
		return NULL;
	}
	collision = collision_new();
	collision->collided = 1;
	collision->blocked = 1;
	vector2d_copy(collision->pointOfContact, poc);
	vector2d_copy(collision->normal, normal);
	collision->shape = d->body->shape;
	collision->body = d->body;
	collision->bounds = 0;
	collision->timeStep = 0;
	return collision;
}


List *collision_check_space_shape(Space *space, Shape shape, CollisionFilter filter)
{
	int i, count;
	Shape *staticShape;
	DynamicBody *db;
	Collision *collision;
	List *collisionList = NULL;
	if (filter.worldclip)
	{
		count = list_get_count(space->staticShapes);
		for (i = 0; i < count; i++)
		{
			staticShape = (Shape*)list_get_nth(space->staticShapes, i);
			if (!staticShape)continue;
			// check for layer compatibility
			collision = collision_space_static_shape_clip(shape, staticShape);
			if (collision == NULL)continue;
			if (!collisionList)collisionList = list_new();
			collisionList = list_append(collisionList, (void*)collision);
		}
		//check if the shape clips the level bounds
		/*        collision = dynamic_body_bounds_collision_check(db,space->bounds,t);
		if (collision != NULL)
		{
		db->collisionList = list_append(db->collisionList,(void*)collision);
		}*/
	}
	if (filter.cliplayer)
	{
		count = list_get_count(space->dynamicBodyList);
		for (i = 0; i < count; i++)
		{
			db = (DynamicBody*)list_get_nth(space->dynamicBodyList, i);
			if (!db)continue;
			if (db->body == filter.ignore)continue;
			if (!(filter.cliplayer & db->body->cliplayer))continue;
			// check for layer compatibility
			collision = collision_space_dynamic_body_clip(shape, db);
			if (collision == NULL)continue;
			if (!collisionList)collisionList = list_new();
			collisionList = list_append(collisionList, (void*)collision);
		}

	}

	return collisionList;
}

Collision collision_trace_space(Space *space, Vector2D start, Vector2D end, CollisionFilter filter)
{
	Collision out = { 0 };
	Collision *collision = NULL;
	Collision *best = NULL;
	double     bestDistance = -1;
	double     distance;
	double     length;
	int count, i;
	List *collisionList;
	collisionList = collision_check_space_shape(space, shape_from_edge(edge_from_vectors(start, end)), filter);
	if (!collisionList)
	{
		return out;
	}
	count = list_get_count(collisionList);
	for (i = 0; i < count; i++)
	{
		collision = (Collision*)list_get_nth(collisionList, i);
		if (!collision)continue;
		if (!best)
		{
			best = collision;
			bestDistance = vector2d_magnitude_between(start, collision->pointOfContact);
			continue;
		}
		distance = vector2d_magnitude_between(start, collision->pointOfContact);
		if (distance < bestDistance)
		{
			best = collision;
			bestDistance = distance;
		}
	}
	if (best != NULL)
	{
		length = vector2d_magnitude_between(start, end);
		if (!length)
		{
			best->timeStep = 0;
		}
		else
		{
			best->timeStep = bestDistance / length;
		}
		memcpy(&out, best, sizeof(Collision));
	}
	collision_list_free(collisionList);
	return out;
}
