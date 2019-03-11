#include "Body.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include <stdlib.h>

Uint8 body_shape_collide(Body *a, Shape *s, Vector2D *poc, Vector2D *normal);

void body_clear(Body *body)
{
	if (!body)return;
	memset(body, 0, sizeof(Body));
}

void body_push(Body *body, Vector2D direction, float force)
{
	if (!body)return;
	if (body->mass != 0)
	{
		force = force / body->mass;
	}
	vector2d_set_magnitude(&direction, force);
	vector2d_add(body->velocity, body->velocity, direction);
}

void body_draw(Body *body, Vector2D offset)
{
	Vector4D color;
	Shape shape;
	Vector2D center;
	if (!body)return;
	vector4d_set(color, 0, 255, 255, 255);
	// draw center point
	vector2d_add(center, body->position, offset);
	gf2d_draw_pixel(center, color);

	vector4d_set(color, 255, 0, 255, 255);
	shape_copy(&shape, *body->shape);
	shape_move(&shape, body->position);
	shape_draw(shape, gf2d_color_from_vector4(color), offset);
}

void body_set(
	Body       *body,
	char       *name,
	Uint8       worldclip,
	Uint32      cliplayer,
	Uint32      touchlayer,
	Uint32      team,
	Vector2D    position,
	Vector2D    velocity,
	float       mass,
	float       gravity,
	float       elasticity,
	Shape      *shape,
	void       *data,
	int(*touch)(struct Body_S *self, List *collision))
{
	if (!body)return;
	body->cliplayer = cliplayer;
	body->touchlayer = touchlayer;
	body->team = team;
	body->worldclip = worldclip;
	vector2d_copy(body->position, position);
	vector2d_copy(body->velocity, velocity);
	body->mass = mass;
	body->gravity = gravity;
	body->elasticity = elasticity;
	body->shape = shape;
	body->data = data;
	body->touch = touch;
	gf2d_word_cpy(body->name, name);
}

Shape body_to_shape(Body *a)
{
	Shape aS = { 0 };
	if (!a)return aS;
	shape_copy(&aS, *a->shape);
	shape_move(&aS, a->position);
	return aS;
}

Uint8 gf2d_body_body_collide(Body *a, Body *b)
{
	if ((!a) || (!b))
	{
		slog("missing body in collision check");
		return 0;
	}
	return shape_overlap(body_to_shape(a), body_to_shape(b));
}