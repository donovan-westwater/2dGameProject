
#include "editor.h"
#include "level.h"
#include "gf2d_text.h"
#include "camera.h"
#include "shape.h"
#include "simple_logger.h"



typedef struct EditorData_S
{
	TextLine filename;
	LevelInfo *level;       /**<working level*/
	Vector2D currentTile;
	Vector2D tilesize;
}EditorData;

EditorData editorData = { 0 };
static Window *_editor = NULL;

int editor_draw() //Formaly had Window *win as parameter
{
	int mx, my;
	const Uint32 *mouse;
	mouse = SDL_GetMouseState(&mx, &my);
	Rect tileHighlight;
	//if (!editorData.level)return 0;

	

	tileHighlight.x = (Uint32)(mx / editorData.tilesize.x) * editorData.tilesize.x;
	tileHighlight.y = (Uint32)(my / editorData.tilesize.y) * editorData.tilesize.y;
	tileHighlight.w = editorData.tilesize.x;
	tileHighlight.h = editorData.tilesize.y;
	rect_draw(tileHighlight, gf2d_color(255, 200, 0, 250));

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
int editor_is_in_list(){
	Shape in = shape_rect(editorData.currentTile.x, editorData.currentTile.y, editorData.tilesize.x, editorData.tilesize.y);
	Shape *current = &in;
	for (int i = 0; i < list_get_count(level_get_space()->staticShapes); i++)
	{
		Shape *other = list_get_nth(level_get_space()->staticShapes, i);
		if (other->s.r.x == current->s.r.x && other->s.r.y == current->s.r.y){
			return 1;
		}
	}
	return 0;
}


Shape *editor_delete_tile(Shape *other, Shape *out){
	if (editorData.currentTile.x == other->s.r.x  && editorData.currentTile.y == other->s.r.y){
		list_delete_data(level_get_space()->staticShapes, other);
		wall_kill(vector2d(other->s.r.x,other->s.r.y));
		level_wall_delete(vector2d(other->s.r.x, other->s.r.y), "levels/editorTest.txt");
		return out;
	}
	//out = NULL;
	return NULL;
}
//WIP - needed!
int editor_update() //Window *win, List *updateList
{
	int mx, my;
	Uint32 mouse;
	mouse = SDL_GetMouseState(&mx, &my);
	int tileX = (int)mx;
	int tileY = (int)my;
	Shape *out = NULL;
	Vector2D check = vector2d((double)tileX, (double)tileY);
	//Detects if the right mouse button has been pressed
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
		//wall_spawn();
		//slog("Button is pressed!");
		list_foreach(level_get_space()->staticShapes, editor_delete_tile, out);
		
	//	if (out != NULL) list_delete_data(level_get_space()->staticShapes, out);
		//call a foreach for the static shapes 
		
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		if (!editor_is_in_list()){
			level_wall_save(editorData.currentTile, "levels/editorTest.txt"); //Need to create a filename infomation chain
			wall_spawn(editorData.currentTile.x, editorData.currentTile.y, editorData.tilesize.x, editorData.tilesize.y);
		}
		
		//slog("Button is pressed!");
		//list_foreach(level_get_space()->staticShapes, editor_check_tile, out);
		//	if (out != NULL) list_delete_data(level_get_space()->staticShapes, out);
		//call a foreach for the static shapes 

	}

	if (tileX > editorData.currentTile.x ){
		editorData.currentTile.x += editorData.tilesize.x;
	}
	if (tileY > editorData.currentTile.y ){
		editorData.currentTile.y += editorData.tilesize.y;
	}
	if (tileX < editorData.currentTile.x ){
		editorData.currentTile.x -= editorData.tilesize.x;
	}
	if (tileY < editorData.currentTile.y ){
		editorData.currentTile.y -= editorData.tilesize.y;
	}
	slog("%lf %lf", editorData.currentTile.x, editorData.currentTile.y);
//	slog("%d %d", tileX, tileY);
	//TEMP
	if (mx > (camera_get_position().x + camera_get_dimensions().w)){
		camera_move(vector2d(camera_get_dimensions().w, 0));
	}
	if (my > (camera_get_position().y + camera_get_dimensions().h)){
		camera_move(vector2d(0, camera_get_dimensions().h));
	}
	if (mx < (camera_get_position().x)){
		camera_move(vector2d(-camera_get_dimensions().w, 0));
	}
	if (my < (camera_get_position().y)){
		camera_move(vector2d(0, -camera_get_dimensions().h));
	}
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
void editor_launch()
{
	editorData.tilesize = vector2d(200, 200);
	editorData.currentTile = vector2d(0, 0);
	/*
	if (_editor != NULL)return;
	_editor = gf2d_window_load("config/editor_main.json");
	if (!_editor)
	{
		slog("failed to load editor window");
		return;
	}
	_editor->update = editor_update;
	_editor->draw = editor_draw;

	*/
}

/*eol@eof*/