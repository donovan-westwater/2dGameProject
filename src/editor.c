
#include "editor.h"
#include "level.h"
#include "gf2d_text.h"
#include "camera.h"
#include "shape.h"
#include "simple_logger.h"
#include "delivery.h"
#include "monster.h"
#include "obsticle.h"

typedef struct EditorData_S
{
	TextLine filename;
	LevelInfo *level;       /**<working level*/
	Vector2D currentTile;
	Vector2D tilesize;
	int timer;
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
	//Entity Body detection should go here
	Entity *entList = get_entityList();
	for (int i = 0; i < get_maxEntites(); i++)
	{
		Entity *other = &entList[i];
		if (other->position.x == editorData.currentTile.x && other->position.y == editorData.currentTile.y){
			return 1;
		}
	}


	return 0;
}
//deletes any entity found on current Tile from world file and world proper
void *editor_delete_entity(){
	Entity *entList = get_entityList();
	for (int i = 0; i < get_maxEntites(); i++)
	{
		Entity *other = &entList[i];
		if (other->position.x == editorData.currentTile.x && other->position.y == editorData.currentTile.y){
			level_wall_delete(other->position,"levels/editorTest.txt");
			level_wall_kill(other->position);
			return;
		}
	}
}

Shape *editor_delete_tile(Shape *other, Shape *out){
	if (editorData.currentTile.x == other->s.r.x  && editorData.currentTile.y == other->s.r.y){
		list_delete_data(level_get_space()->staticShapes, other);
		level_wall_kill(vector2d(other->s.r.x,other->s.r.y));
		level_wall_delete(vector2d(other->s.r.x, other->s.r.y), "levels/editorTest.txt");
		level_wall_delete(vector2d(other->s.r.x, other->s.r.y), "levels/route1.txt");
		return out;
	}
	//out = NULL;
	return NULL;
}

int editor_update() 
{
	int mx, my;
	Uint32 mouse;
	Uint8 * keys;	
	if(editorData.timer < 101) editorData.timer += 1;
	else
	{
		editorData.timer = 0;
	}
	mouse = SDL_GetMouseState(&mx, &my);
	keys = SDL_GetKeyboardState(NULL);
	mx += camera_get_position().x;
	my += camera_get_position().y;
	int tileX = (int)mx;
	int tileY = (int)my;
	Shape *out = NULL;
	Vector2D check = vector2d((double)tileX, (double)tileY);
	//Detects if the right mouse button has been pressed
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
		//wall_spawn();
		//slog("Button is pressed!");
		list_foreach(level_get_space()->staticShapes, editor_delete_tile, out);
		editor_delete_entity(); 
		//level_wall_kill(editorData.currentTile);
		//Deleting entity, use normal for loop setup
		
	
		
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		if (!editor_is_in_list()){
			level_wall_save(editorData.currentTile, "levels/editorTest.txt"); //Need to create a filename infomation chain
			wall_spawn(editorData.currentTile.x, editorData.currentTile.y, editorData.tilesize.x, editorData.tilesize.y);
		}
		

	}//delivery points
	if (keys[SDL_SCANCODE_O] && !editor_is_in_list()){
		level_delivery_save(editorData.currentTile, "levels/route1.txt");
		delivery_spawn(editorData.currentTile);
	}
	//Chasers
	if (keys[SDL_SCANCODE_I] && !editor_is_in_list()){
		
		level_entity_save(editorData.currentTile,"chaser","levels/editorTest.txt");
		monster_spawn(editorData.currentTile,CHASER);
	}
	//Patrollers [Have a directional editor]
	if (keys[SDL_SCANCODE_U] && !editor_is_in_list()){
	
		level_entity_save(editorData.currentTile, "patroller", "levels/editorTest.txt");
		monster_spawn(editorData.currentTile, PATROLLER);
	}
	//Shooters  [Have a directional editor]
	if (keys[SDL_SCANCODE_Y] && !editor_is_in_list()){
		
		level_entity_save(editorData.currentTile, "shooter", "levels/editorTest.txt");
		monster_spawn(editorData.currentTile, SHOOTER);
	}
	//Flinger  (Black hole)
	if (keys[SDL_SCANCODE_T] && !editor_is_in_list()){
		
		level_entity_save(editorData.currentTile, "flinger", "levels/editorTest.txt");
		obsticle_spawn(editorData.currentTile, FLINGER);
	}
	//Block    (RoadBlock)
	if (keys[SDL_SCANCODE_P] && !editor_is_in_list()){
		
		level_entity_save(editorData.currentTile, "block", "levels/editorTest.txt");
		obsticle_spawn(editorData.currentTile, BLOCK);
	}
	if (keys[SDL_SCANCODE_W] ) camera_set_position(vector2d(camera_get_position().x, camera_get_position().y - 1));
	if (keys[SDL_SCANCODE_S] ) camera_set_position(vector2d(camera_get_position().x, camera_get_position().y + 1));
	if (keys[SDL_SCANCODE_A] ) camera_set_position(vector2d(camera_get_position().x - 1, camera_get_position().y));
	if (keys[SDL_SCANCODE_D] ) camera_set_position(vector2d(camera_get_position().x + 1, camera_get_position().y));

	if (keys[SDL_SCANCODE_UP] && editorData.timer % 15 == 0) camera_move(vector2d(0, -camera_get_dimensions().h));
	if (keys[SDL_SCANCODE_DOWN] && editorData.timer % 15 == 0) camera_move(vector2d(0, camera_get_dimensions().h));
	if (keys[SDL_SCANCODE_LEFT] && editorData.timer % 15 == 0) camera_move(vector2d(-camera_get_dimensions().w, 0));
	if (keys[SDL_SCANCODE_RIGHT] && editorData.timer % 15 == 0) camera_move(vector2d(camera_get_dimensions().w, 0));
	
	//slog("CAMERA: %lf %lf",camera_get_position().x,camera_get_position().y);
	
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
	//slog("Current Tile: %lf %lf", editorData.currentTile.x, editorData.currentTile.y);
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
	editorData.timer = 0;
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