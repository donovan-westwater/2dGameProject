#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "level.h"
#include "space.h"
#include "camera.h"
#include "player.h"
#include "simple_logger.h"
//Level info
typedef struct{
	Space *space;
}Level;
//First create a level_inti fucntion, then a  load level fucntion, then level clear,then level update, then level draw

static Level gamelevel = { 0 };
LevelInfo *level_info_new()
{
	LevelInfo *linfo = NULL;
	linfo = (LevelInfo*)malloc(sizeof(LevelInfo));
	if (!linfo)
	{
		slog("failed to allocate level info");
		return NULL;
	}
	memset(linfo, 0, sizeof(LevelInfo));
	return linfo;
}
LevelInfo *level_info_load(char *filename){
	LevelInfo *out = level_info_new();
	slog(" OPENING FILE ");
	FILE *reader = fopen(filename,"r");
	if (reader == NULL){
		slog("ERROR!!!!");
		return NULL;
	}
	slog(" File Opened! ");
	char* names = "";
	char* numbers = "";
	int paramMode = 0;
	int isWall = 0;
	int isMonster = 0;
	int isObsticle = 0;
	int skip = 0; // Skips a line
	double x = 0;
	double y = 0;
	char c = 'N'; //A dummy char. Shouldnt acutally be N
	while (c != EOF){
		
		c = fgetc(reader);
		//slog(c);
		if (c == '#') skip = 1;
		if (c == '\n'){
			
			names = "";
			numbers = "";
			isWall = 0;
			isMonster = 0;
			isObsticle = 0;
			skip = 0;
			x = 0;
			y = 0;
			continue;
		}
		if (c == ' ' || skip) continue;
		if (c == '|'){
			
			//First section: type of object
			//slog(names);
			//slog(numbers);
			
			if (!isMonster && !isWall && !isObsticle){
				//slog("%d %d %d", !isMonster, !isWall, !isObsticle);
				//slog("%s%s", "wall", names);
				if (strcmp(names, "monster") == 0){
					isMonster = 1;
					//NOTHING HAPPENS YET. SHOULD BE EENTUALLY PLACED INTO THE SPAWN LIST!
				}
				else if (strcmp(names,"wall") == 0){
					isWall = 1;
				}
				else if (strcmp(names, "obsticle") == 0){
					isObsticle = 1;
					//NOTHING HAPPENS YET. SHOULD BE EVENTUALLY PLACED INTO SPAWN LIST!
				}
			}
			//Second section: location of objects
			else 
			{
				
				//Currently only puts in wall locations. Add in monsters and obstcles once they are created
				if (strlen(numbers) > 0 && !paramMode){
					y = atof(numbers);
					//Vector2D s = vector2d(x, y); //Malloc vecoter to keep outside stack
					Vector2D *s;
					s = malloc(sizeof(Vector2D));
					s->x = x;
					s->y = y;
					void *data = s;
					//check = (Vector2D *)data;
					slog("%lf %lf", s->x, s->y);

					if (out->shapeLocations == NULL) { 
						out->shapeLocations = list_new();
						slog("List allocated");
					}
					slog(" Trying to appending data");
					out->shapeLocations = list_append(out->shapeLocations, data);
					//check =(Vector2D *) list_get_nth(out->shapeLocations, 0); //Checking if its in list
					//slog("%lf %lf", check->x, check->y);                      //DOESNT PRINT
					x = 0;
					y = 0;
					numbers = "";
					paramMode = 1;
				}
			//Final Section: Params for each object Only has params for walls so far
				if (paramMode && strlen(numbers) > 0){
					y = atof(numbers);
					Vector2D *s;
					s = malloc(sizeof(Vector2D));
					s->x = x;
					s->y = y;
					void *data = s;
					if (out->shapeParams == NULL){
						out->shapeParams = list_new();
						slog("List allocated");
					}
					slog(" Trying to appending data");
					list_append(out->shapeParams, data);
					x = 0;
					y = 0;
					numbers = "";
					paramMode = 0;
				}
			}
		}
		else
		{ 
			//slog(" HERE ");
			
			//Part of First section
			if (!isMonster && !isWall && !isObsticle && !(isdigit(c) || ispunct(c))){
				//temp = *names + tolower(c);
				//names = temp; //error triggered here
				char *temp;
				char stor[2];
				stor[0] = tolower(c);
				stor[1] = '\0';
				temp = malloc(sizeof(names));
				strcpy(temp, names);
				names = NULL;
				names = (char *)realloc(names, sizeof(names) + 2);
				strcpy(names, temp);
				strcat(names, stor);
			}
			//Part of second section
			else if (isdigit(c) || c == '.'){
			//	temp = *numbers + tolower(c);
			//	numbers = temp;
				char *temp;
				char stor[2];
				stor[0] = c;
				stor[1] = '\0';
				temp = malloc(sizeof(numbers));
				strcpy(temp, numbers);
				numbers = NULL;
				numbers = (char *)realloc(numbers, sizeof(numbers) + 2);
				strcpy(numbers, temp);
				strcat(numbers, stor);
				//slog(numbers);
			}
			else if (c == ','){
				x = atof(numbers);
				
				numbers = "";
			}

			//Part of Final section: PLACEHOLDER, IN CASE PARAMS DIVERSIFY
		}
	}
	return out;
	
}
void level_init(LevelInfo *linfo, Uint8 space){
	if (!linfo)
	{
		return;
	}
	level_clear();
	slog("trying to add space!");
	if (space) create_space();
	//Should load up infomation from LevelInfo here! (SHOULD CHECK IF PARAMS AND LOC ARE THE SAME AMOUNT)
	for (int i = 0; i <  list_get_count(linfo->shapeLocations); i++){
		slog("Size: %d", list_get_count(linfo->shapeLocations));
		slog("Size: %d", list_get_count(linfo->shapeParams));
		Vector2D *loc = (Vector2D *) list_get_nth(linfo->shapeLocations, i);
		Vector2D *param = (Vector2D *) list_get_nth(linfo->shapeParams, i);
		//slog("%lf",loc->x);
		//slog("%lf",param->x);
		if (loc != NULL && param != NULL){
			slog("%lf", loc->x);
			slog("%lf", param->x);
			Shape wall = shape_rect(loc->x,loc->y,param->x,param->y);
			space_add_static_shape(gamelevel.space, wall);
			//Have fucntions here to tie shape to a static entity to represent the wall
		}
		//Monster and obstcle calls go here
	}
	//free data here
}
void create_space(){
	gamelevel.space = space_new_full(
		1,
		shape_rect(0, 0, 2400, 1440).s.r,
		0.1,
		vector2d(0, 0),
		0.0,
		0.5);

}
void level_clear(){
	space_free(gamelevel.space);
	memset(&gamelevel, 0, sizeof(Level));
}
Space* level_get_space(){
	return gamelevel.space;
}
void level_draw(){
	Vector2D cam;
	cam = camera_get_offset();
	space_draw(gamelevel.space, cam);
	//gf2d_sprite_draw_image(gamelevel.backgroundImage, cam);
	//gf2d_sprite_draw_image(gamelevel.tileLayer, cam);
	entity_draw_all(); 
	//entity_draw(player_get());
	//gf2d_particle_emitter_draw(gamelevel.pe, cam);
}
void level_update(){
	entity_pre_sync_all();
	space_update(gamelevel.space);
	entity_post_sync_all();
}