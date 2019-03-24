#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "level.h"
#include "space.h"
#include "camera.h"
#include "player.h"
#include "monster.h"
#include "obsticle.h"
#include "collisions.h"
#include "simple_logger.h"
#include "delivery.h"
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
	char* other = "";
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
			other = "";
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
				if (strlen(numbers) > 0 && isWall && !paramMode){
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
				if (paramMode && strlen(numbers) > 0 && isWall){
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
				if (strlen(numbers) > 0 && isMonster && !paramMode){
					y = atof(numbers);
					//Vector2D s = vector2d(x, y); //Malloc vecoter to keep outside stack
					Vector2D *s;
					s = malloc(sizeof(Vector2D));
					s->x = x;
					s->y = y;
					void *data = s;
					//check = (Vector2D *)data;
					slog("%lf %lf", s->x, s->y);

					if (out->spawnLocations == NULL) {
						out->spawnLocations = list_new();
						slog("List allocated");
					}
					slog(" Trying to appending data");
					out->spawnLocations = list_append(out->spawnLocations, data);
					//check =(Vector2D *) list_get_nth(out->shapeLocations, 0); //Checking if its in list
					//slog("%lf %lf", check->x, check->y);                      //DOESNT PRINT
					x = 0;
					y = 0;
					numbers = "";
					paramMode = 1;
				}
				if (strlen(other) > 0 && isMonster && paramMode){
					char * strStor = malloc(sizeof(other));
					strStor = other;
					void *data = strStor;
					if (out->spawnList == NULL) {
						out->spawnList = list_new();
						slog("List allocated");
					}
					slog(" Trying to appending data");
					out->spawnList = list_append(out->spawnList, data);
					//check =(Vector2D *) list_get_nth(out->shapeLocations, 0); //Checking if its in list
					//slog("%lf %lf", check->x, check->y);                      //DOESNT PRINT
					other = "";
					paramMode = 0;
				}
				if (strlen(numbers) > 0 && isObsticle && !paramMode){
					y = atof(numbers);
					//Vector2D s = vector2d(x, y); //Malloc vecoter to keep outside stack
					Vector2D *s;
					s = malloc(sizeof(Vector2D));
					s->x = x;
					s->y = y;
					void *data = s;
					//check = (Vector2D *)data;
					slog("%lf %lf", s->x, s->y);

					if (out->spawnLocations == NULL) {
						out->spawnLocations = list_new();
						slog("List allocated");
					}
					slog(" Trying to appending data");
					out->spawnLocations = list_append(out->spawnLocations, data);
					//check =(Vector2D *) list_get_nth(out->shapeLocations, 0); //Checking if its in list
					//slog("%lf %lf", check->x, check->y);                      //DOESNT PRINT
					x = 0;
					y = 0;
					numbers = "";
					paramMode = 1;
				}
				if (strlen(other) > 0 && isObsticle && paramMode){
					char * strStor = malloc(sizeof(other));
					strStor = other;
					void *data = strStor;
					if (out->spawnList == NULL) {
						out->spawnList = list_new();
						slog("List allocated");
					}
					slog(" Trying to appending data");
					out->spawnList = list_append(out->spawnList, data);
					//check =(Vector2D *) list_get_nth(out->shapeLocations, 0); //Checking if its in list
					//slog("%lf %lf", check->x, check->y);                      //DOESNT PRINT
					other = "";
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
			else if (isalpha(c)){
				char *temp;
				char stor[2];
				stor[0] = tolower(c);
				stor[1] = '\0';
				temp = malloc(sizeof(other));
				strcpy(temp, other);
				other = NULL;
				other = (char *)realloc(other, sizeof(other) + 2);
				strcpy(other, temp);
				strcat(other, stor);
			}

			//Part of Final section: PLACEHOLDER, IN CASE PARAMS DIVERSIFY
		}
	}
	fclose(reader);
	return out;
	
}
//Create a fucntion to load in the places to deliever mail (call after level init)
void route_load(char *name){
	//uses same read in as level init but simpler
	//create variables
	slog(" OPENING FILE ");
	FILE *reader = fopen(name, "r");
	if (reader == NULL){
		slog("ERROR!!!!");
		return NULL;
	}
	slog(" File Opened! ");
	char* numbers = "";
	int skip = 0; // Skips a line
	double x = 0;
	double y = 0;
	char c = 'N'; //A dummy char. Shouldnt acutally be N
	while (c != EOF){
		//read input and gather data
		c = fgetc(reader);
		//slog(c);
		if (c == '#') skip = 1;
		if (c == '\n'){
			numbers = "";
			skip = 0;
			x = 0;
			y = 0;
			continue;
		}
		if (c == ' ') continue;
		if (c == '|'){
			//spawn the delivery points
			int round = (int)atof(numbers);
			y = round;
			delivery_spawn(vector2d(x, y));
			x = 0;
			y = 0;
			numbers = "";
		
		}else{
		if (isdigit(c) || c == '.'){
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
			int round = (int)atof(numbers);
			x = round;
			numbers = "";
		}
	  }
	}
	fclose(reader);
}
void save_game(){
	FILE *reader = fopen("saves/save.txt", "w");
	if (reader == NULL){
		slog("ERROR!!!!");
		return NULL;
	}
	//Stores player position
	char number[12];
	fputs("position ", reader);
	sprintf(number,"%lf",player_get()->position.x);
	fputs(number, reader);
	fputc(',', reader);
	sprintf(number, "%lf", player_get()->position.y);
	fputs(number, reader);
	fputs("|\n", reader);
	//Store players current route
	fclose(reader);
}

void load_game(){
	slog(" OPENING FILE ");
	FILE *reader = fopen("saves/save.txt", "r");
	if (reader == NULL){
		slog("ERROR!!!!");
		return NULL;
	}
	slog(" File Opened! ");
	int isPosition = 0;
	char *input = "";
	char c = 'N';
	double x = 0;
	double y = 0;
	while (c != EOF){
		//read input and gather data
		c = fgetc(reader);
		if (c == ' ') continue;
		if (c == '\n'){
			input = "";
			x = 0;
			y = 0;
			isPosition = 0;
		}
		if (strcmp(input, "position") == 0){
			isPosition = 1;
			input = "";
		}
		if (c == '|'){
			y = atof(input);
			player_new(vector2d(x,y));
		}
		else{
			if (c == ','){
				x = atof(input);
				input = "";
		}
			else{
				char *temp;
				char stor[2];
				stor[0] = tolower(c);
				stor[1] = '\0';
				temp = malloc(sizeof(input));
				strcpy(temp, input);
				input = NULL;
				input = (char *)realloc(input, sizeof(input) + 2);
				strcpy(input, temp);
				strcat(input, stor);
		}
		}
	}
	fclose(reader);
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
	for (int i = 0; i < list_get_count(linfo->shapeLocations); i++){
		slog("Size: %d", list_get_count(linfo->shapeLocations));
		slog("Size: %d", list_get_count(linfo->shapeParams));
		Vector2D *loc = (Vector2D *)list_get_nth(linfo->shapeLocations, i);
		Vector2D *param = (Vector2D *)list_get_nth(linfo->shapeParams, i);
		//slog("%lf",loc->x);
		//slog("%lf",param->x);
		if (loc != NULL && param != NULL){
			slog("%lf", loc->x);
			slog("%lf", param->x);
			//Replace with a spawn wall fucntion once you dod graphicall overhaul (turn this into a ent!)
			Shape wall = shape_rect(loc->x, loc->y, param->x, param->y);
			space_add_static_shape(gamelevel.space, wall);
			//Have fucntions here to tie shape to a static entity to represent the wall
			}
		}
		//Monster and obstcle calls go here
	for (int z = 0;z < list_get_count(linfo->spawnLocations); z++){
		Vector2D *loc = (Vector2D *)list_get_nth(linfo->spawnLocations, z);
		char *type = (char *)list_get_nth(linfo->spawnList, z);
		if (strcmp(type, "flinger") == 0){
			obsticle_spawn(*loc, FLINGER);
		}
		else if (strcmp(type, "block") == 0){
			obsticle_spawn(*loc, BLOCK);
		}
		else if(strcmp(type, "shooter") == 0){
			monster_spawn(*loc, SHOOTER);
		}
		else if(strcmp(type, "patroller") == 0){
			monster_spawn(*loc, PATROLLER);
		}
		else if(strcmp(type, "chaser") == 0){
			monster_spawn(*loc, CHASER);
		}
	}
	//Intilize route from here (Remember this for save system)
	//free data here
}
void create_space(){
	gamelevel.space = space_new_full(
		15,
		shape_rect(0, 0, 2400, 2400).s.r,
		0.1,
		vector2d(0, 0),
		0.0,
		 1); //Oringallly 0.5

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


void level_transition(char *filename, Vector2D pos){
	Entity *player;
	LevelInfo *linfo = NULL;


	linfo = level_info_load("levels/section1.txt");
	if (!linfo)return;

	entity_clear_all_but_player();
	level_init(linfo, 1);
	route_load(filename);
	
	player_set_position(pos);

	adding_all_bodies_to_space(gamelevel.space);

}
//Add proper documentation later calls all touch fucntions in collisionList
int body_body_touch(Body *self, List *collisionList)
{
	Entity *selfEnt;
	Collision *c;
	int i, count;
	if (!self)return 0;
	if (!self->data) return 0;
	selfEnt = (Entity*)self->data;
	//slog("TOUCHING!");
	if (!selfEnt->touch)return 0;
	count = list_get_count(collisionList);
	
	for (i = 0; i < count; i++)
	{
		c = (Collision *)list_get_nth(collisionList, i);
		if (!c)continue;
		if (!c->body)continue;
		if (!c->body->data)continue;
		selfEnt->touch(selfEnt, (Entity*)c->body->data);
	}
	return 0;
}