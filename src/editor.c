
#include "editor.h"
#include "level.h"
#include "gf2d_text.h"

#include "shape.h"
#include "simple_logger.h"



typedef struct EditorData_S
{
	TextLine filename;
	LevelInfo *level;       /**<working level*/
}EditorData;

EditorData editorData = { 0 };
static Window *_editor = NULL;

int editor_draw() //Formaly had Window *win as parameter
{
	int mx, my;
	const Uint32 *mouse;
	mouse = SDL_GetMouseState(&mx, &my);
	Rect tileHighlight;
	if (!editorData.level)return 0;

	

	tileHighlight.x = (Uint32)(mx / editorData.level->tilesize.x) * editorData.level->tilesize.x;
	tileHighlight.y = (Uint32)(my / editorData.level->tilesize.y) * editorData.level->tilesize.y;
	tileHighlight.w = editorData.level->tilesize.x;
	tileHighlight.h = editorData.level->tilesize.y;
	rect_draw(tileHighlight, gf2d_color(255, 200, 0, 255));

	/*draw rects around the tile the mouse is over*/
	return 0;
}


//not used yet WIP
/*
void editor_new_map(void *data)
{
	editorData.level = level_info_create(
		"images/backgrounds/bg_flat.png",
		"music/old_city_theme.ogg",
		"images/tiles/basetile.png",
		vector2d(32, 32),
		vector2d(38, 24));
	level_init(editorData.level, 0);
}
*/

//WIP - needed!

int editor_update() //Window *win, List *updateList
{
	/*
	int i, count;
	Element *e;
	Vector2D mouse, tile;
	if (!win)return 0;
	if (!updateList)return 0;

	mouse = gf2d_mouse_get_position();
	if ((editorData.level != NULL) && (!gf2d_mouse_in_rect(win->dimensions)))
	{
		if (gf2d_mouse_button_pressed(0) || gf2d_mouse_button_held(0))
		{
			tile = level_position_to_tile(editorData.level, mouse);
			level_update_tile(editorData.level, tile, 1);
			level_make_tile_layer(editorData.level);
		}
		if (gf2d_mouse_button_pressed(2) || gf2d_mouse_button_held(2))
		{
			tile = level_position_to_tile(editorData.level, mouse);
			level_update_tile(editorData.level, tile, 0);
			level_make_tile_layer(editorData.level);
		}
	}
	count = gf2d_list_get_count(updateList);
	for (i = 0; i < count; i++)
	{
		e = (Element *)gf2d_list_get_nth(updateList, i);
		if (!e)continue;
		switch (e->index)
		{
		case 110:
			slog("load");
			window_text_entry("enter file to load", editorData.filename, GF2DTEXTLEN, NULL, NULL);
			break;
		case 130:
			slog("new file");
			window_yes_no("Create New Map, previous data lost?", editor_new_map, NULL, NULL, NULL);
			break;
		}
	}
	*/
	return 0;
}

//Not being used! WIP
/*
void editor_launch()
{
	if (_editor != NULL)return;
	_editor = gf2d_window_load("config/editor_main.json");
	if (!_editor)
	{
		slog("failed to load editor window");
		return;
	}
	_editor->update = editor_update;
	_editor->draw = editor_draw;
}
*/
/*eol@eof*/