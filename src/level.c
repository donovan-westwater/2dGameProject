#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "level.h"
#include "space.h"
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
			isWall = 0;
			isMonster = 0;
			isObsticle = 0;
			skip = 0;
			x = 0;
			y = 0;
		}
		if (c == ' ' || skip) continue;
		if (c == '|'){
			
			//First section: type of object
			slog(names);
			if (!isMonster && !isWall && !isObsticle){
				if (names == "monster"){
					isMonster = 1;
					//NOTHING HAPPENS YET. SHOULD BE EENTUALLY PLACED INTO THE SPAWN LIST!
				}
				else if (names == "wall"){
					isWall = 1;
				}
				else if(names == "obsticle"){
					isObsticle = 1;
					//NOTHING HAPPENS YET. SHOULD BE EVENTUALLY PLACED INTO SPAWN LIST!
				}
			}
			//Second section: location of objects
			else 
			{
				//Currently only puts in wall locations. Add in monsters and obstcles once they are created
				if (strlen(numbers) > 0){
					y = atof(numbers);
					Vector2D s = vector2d(x, y);
					Vector2D *data = &s;
					list_append(out->shapeLocations, (void*)data);
					x = 0;
					y = 0;
					numbers = "";
					paramMode = 1;
				}
			//Final Section: Params for each object Only has params for walls so far
				if (paramMode && strlen(numbers) > 0){
					y = atof(numbers);
					Vector2D s = vector2d(x, y);
					Vector2D *data = &s;
					list_append(out->shapeParams, (void*)data);
					x = 0;
					y = 0;
					numbers = "";
					paramMode = 0;
				}
			}
		}
		else
		{ 
			slog(" HERE ");
			
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
			else if (isdigit(c) || c == '\.'){
			//	temp = *numbers + tolower(c);
			//	numbers = temp;
				char *temp;
				char stor[2];
				stor[0] = tolower(c);
				stor[1] = '\0';
				temp = malloc(sizeof(numbers));
				strcpy(temp, numbers);
				numbers = NULL;
				numbers = (char *)realloc(numbers, sizeof(numbers) + 2);
				strcpy(numbers, temp);
				strcat(numbers, stor);
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
	if (space) gamelevel.space = &space;
	//Should load up infomation from LevelInfo here! (SHOULD CHECK IF PARAMS AND LOC ARE THE SAME AMOUNT)
	for (int x = 0; x <  list_get_count(linfo->shapeLocations); x++){
		Vector2D *loc = (Vector2D*) list_get_nth(linfo->shapeLocations, x);
		Vector2D *param = (Vector2D*)list_get_nth(linfo->shapeParams, x);
		Shape wall = shape_rect(loc->x,loc->y,param->x,param->y);
		space_add_static_shape(space, wall);
	}

}
void level_clear(){
	space_free(gamelevel.space);
	memset(&gamelevel, 0, sizeof(Level));
}

void level_draw();
void level_update();