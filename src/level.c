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
#include "particles.h"
//Level info
typedef struct{
	Space *space;
	int win;
	int lose;
	char *worldname;
	int currentRoute;
	ParticleEmitter *pe;
}Level;
//First create a level_inti fucntion, then a  load level fucntion, then level clear,then level update, then level draw

static Level gamelevel = { 0 };

Entity *wall_spawn(double x, double y, double w, double h);
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
	out->tilesize = vector2d(200, 200);
	slog(" OPENING FILE ");
	FILE *reader = fopen(filename,"r");
	if (reader == NULL){
		slog("ERROR!!!!");
		return NULL;
	}
	slog(" File Opened! ");
	out->file = filename;
	//gamelevel.worldname = (char *)malloc(filename);
	//strcpy(gamelevel.worldname, filename);
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
					out->shapeParams = list_append(out->shapeParams, data);
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
	double x = -1;
	double y = -1;
	char c = 'N'; //A dummy char. Shouldnt acutally be N
	while (c != EOF){
		//read input and gather data
		c = fgetc(reader);
		//slog(c);
		if (c == '#') skip = 1;
		if (c == '\n'){
			numbers = "";
			skip = 0;
			x = -1;
			y = -1;
			continue;
		}
		if (c == ' ') continue;
		if (c == '|' && x > -1){
			//spawn the delivery points
			int round = (int)atof(numbers);
			y = round;
			if(y > -1) delivery_spawn(vector2d(x, y));
			x = -1;
			y = -1;
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
			if (x == 0 && y == 0) player_new(vector2d(600,600));
			else player_new(vector2d(x,y));
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
//Wall spawn here! Rewrite to make param const
Entity *wall_spawn(double x, double y, double w, double h){
	Entity *self;
	self = entity_new();
	
	self->position = vector2d(x, y);
	self->velocity = vector2d(0, 0);
	if (!self)return NULL;

	self->shape = shape_rect(x, y, w, h);
	
	//figure out how to properly automate scaling!!
	//Sprite *sprite = gf2d_sprite_load_all("images/wall.jpg",w-x,h-y,16);
	self->sprite = gf2d_sprite_load_image("images/house.png");
	self->scale = vector2d(0.5, 0.35);
	if (&self->hitbox != NULL){
		memset(&self->hitbox, 0, sizeof(self->hitbox));
	}
	self->hitbox.inactive = 1;

	//have update fuction that pins delivery points in place
	space_add_static_shape(level_get_space(), self->shape);
	return self;
}
//should kill the entity assoicated with walls, delivery points,  or obsticles
//Add a null check! this function is unsafe!!!!!!!
void *level_wall_kill(Vector2D position){
	Entity *entList = get_entityList();
	for (int i = 0; i < get_maxEntites(); i++)
	{
		Entity *other = &entList[i];
		if (other->position.x == position.x && other->position.y == position.y){
			entity_free(other);
		}
	}
	
}
void level_init(LevelInfo *linfo, Uint8 space){
	if (!linfo)
	{
		return;
	}
	level_clear();
	gamelevel.worldname = linfo->file;
	gamelevel.currentRoute = linfo->curRoute;
	slog("trying to add space!");
	if (space) create_space();
	gamelevel.pe = gf2d_particle_emitter_new_full(
		2048,
		10,
		5,
		PT_Pixel,
		vector2d(0, 0),
		vector2d(0, 0),
		vector2d(0, 0),
		vector2d(0, 0),
		vector2d(0, 0),
		vector2d(0, 0),
		gf2d_color(0, 0, 0, 1),
		gf2d_color(0, 0, 0, 0),
		gf2d_color(0, 0, 0, 0),
		NULL,
		0,
		0,
		0,
		"",
		0,
		0,
		0,
		0,
		SDL_BLENDMODE_BLEND);
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
			//Shape wall = shape_rect(loc->x, loc->y, param->x, param->y);
			//space_add_static_shape(gamelevel.space, wall);
			Entity *spawn  = wall_spawn(loc->x,loc->y,param->x,param->y);
			//Have fucntions here to tie shape to a static entity to represent the wall
			}
		}
		//Monster and obstcle calls go here 
		//CHANGE SHOOTER PATEROLLER TO USE PARAMS BY EDITING OUTPUTED ENTITY!!!! (Save spawn in a variable and change param that way) [HUGE WORKLOAD IF UNDERTOOK, WOULD CREATE REDOING LEVEL READ SYSTEM, SO AVOID FOR NOW]
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
	gamelevel.lose = 0;
	gamelevel.win = 0;
	//Intilize route from here (Remember this for save system)
	//free data here
}
void create_space(){
	gamelevel.space = space_new_full(
		15,
		shape_rect(0, 0, 9600, 9600).s.r, //2400 2400 INCREASE THE SIZE OF THE SPACE, SHOULD BE MULTIPLES OF 200 [OGI:4800, 2800]
		0.1,
		vector2d(0, 0),
		0.0,
		 1); //Oringallly 0.5

}
void level_clear(){
	space_free(gamelevel.space);
	gf2d_particle_emitter_free(gamelevel.pe);
	memset(&gamelevel, 0, sizeof(Level));
}
Space* level_get_space(){
	return gamelevel.space;
}
int level_get_lose(){
	return gamelevel.lose;
}
void level_set_lose(int n){
	gamelevel.lose = n;
}
int level_get_win(){
	return gamelevel.win;
}
void level_set_win(int n){
	gamelevel.win = n;
}
void level_draw(){
	Vector2D cam;
	cam = camera_get_offset();
	space_draw(gamelevel.space, cam);
	//gf2d_sprite_draw_image(gamelevel.backgroundImage, cam);
	//gf2d_sprite_draw_image(gamelevel.tileLayer, cam);
	entity_draw_all(); 
	//entity_draw(player_get());
	gf2d_particle_emitter_draw(gamelevel.pe, cam);
}
void level_update(){
	entity_pre_sync_all();
	space_update(gamelevel.space);
	entity_post_sync_all();
	gf2d_particle_emitter_update(gamelevel.pe);
}

Level *level_get_level(){
	return &gamelevel;
}

void level_transition(Vector2D pos){
	Entity *player;
	LevelInfo *linfo = NULL;
	int safefyCheck = 4;
	char nextroute[128] = { 0x0 };
	char tmp[16] = { 0x0 };
	strcat(nextroute, "levels/route");
	linfo = level_info_load(gamelevel.worldname);
	if (!linfo)return;
	gamelevel.currentRoute += 1;
	if (gamelevel.currentRoute > 4) gamelevel.currentRoute = 1;
	itoa(gamelevel.currentRoute, tmp, 10);
	strcat(nextroute,tmp);
	strcat(nextroute, ".txt");
	linfo->curRoute = gamelevel.currentRoute;
	entity_clear_all_but_player();
	level_init(linfo, 1);
	route_load(nextroute);
	
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
		if (!self->data) return 0;
		c = (Collision *)list_get_nth(collisionList, i);
		if (!c)continue;
		if (!c->body)continue;
		if (!c->body->data)continue;
		selfEnt->touch(selfEnt, (Entity*)c->body->data);
	}
	return 0;
}

void level_wall_save(Vector2D pos, char *filename){
	FILE *write;
	write = fopen(filename, "a");
	if (write == NULL) {
		perror("cannot open file");
		return 1;
	}
	char type[32] = "\nwall |";
	char position[256] = { 0x0 };
	char param[256] = " 200,200|";
	char tmp[32] = { 0x0 };
	char out[256] = { 0x0 };
	itoa(pos.x, tmp, 10);
	strcat(position, tmp);
	strcat(position, ",");
	itoa(pos.y, tmp, 10);
	strcat(position, tmp);
	strcat(position, "|");
	strcat(out, type);
	strcat(out, position);
	strcat(out, param);
	fprintf(write,out);
	fclose(write);
}
//Deletes any line containing the speafied postion, deleting the walll (or anything for that matter) from save data
void level_wall_delete(Vector2D pos, char *filename){
	//Run through the overworld file and match the position to the line or section with the corresponding postion
	FILE *fp1, *fp2;

	//consider 255 character string to store filename
	//char filename[256];
	int c, last, del_line, lineno;
	char tmp[256] = { 0x0 };
	char delWord[256] = { 0x0 };
	
	//asks user for file name
	//printf("Enter file name: ");
	strcat(delWord, "|");
	itoa(pos.x, tmp, 10);
	strcat(delWord, tmp);
	strcat(delWord, ",");
	itoa(pos.y, tmp, 10);
	strcat(delWord, tmp);
	strcat(delWord, "|");
	strcpy(tmp, "");
	///receives file name from user and stores in 'filename'
	/*if (scanf("%255s", filename) != 1) {
		perror("missing filename");
		return 1;
	}
	*/
	//open file in read mode
	fp1 = fopen(filename, "r");
	if (fp1 == NULL) {
		perror("cannot open file");
		return 1;
	}

	//until the last character of file is obtained
	last = '\n';
	lineno = 1;
	while ((c = fgets(tmp, sizeof(tmp), fp1))) {
		if (strstr(tmp, delWord)){
			del_line = lineno;
		}
		//print current character and read next character
		putchar(c);
		lineno += 1;
		/*
		if (last == '\n') {
			printf("%d: ", ++lineno);
		}
		if (strstr(tmp, delWord))
			del_line = lineno;
		//print current character and read next character
		putchar(c);
		last = c;
		*/
	}

	rewind(fp1);
	if (del_line < 1 || del_line > lineno) {
		printf("no such line: %d\n", del_line);
		return 1;
	}

	//open new file in write mode
	fp2 = fopen("levels/copy.txt", "w");
	if (fp2 == NULL) {
		perror("cannot open copy.c");
		return 1;
	}

	lineno = 1;
	while ((c = getc(fp1)) != EOF) {
		//except the line to be deleted
		if (lineno != del_line) {
			//copy all lines in file copy.c
			putc(c, fp2);
		}
		if (c == '\n')
			lineno++;
	}

	//close both files.
	fclose(fp1);
	if (fclose(fp2)) {
		perror("write error to copy.c");
		return 1;
	}

	// remove original file (unsafe)
	// uncomment this if your system does not allow rename
	// to overwrite existing files
	if (remove(filename)) {
	     perror("cannot remove source file");
	    return 1;
	 }

	//rename the file copy.c to original name

	if (rename("levels/copy.txt", filename)) {
		perror("cannot rename file");
		return 1;
	}
	/*
	if (rename("levels/copy.txt", "levels/editorTest.txt")) {
		perror("cannot rename file");
		return 1;
	}
	*/
	printf("\nThe contents of file after being modified are as  follows:\n");

	fp1 = fopen(filename, "r");
	if (fp1 == NULL) {
		perror("cannot re-open modified file");
		return 1;
	}

	while ((c = getc(fp1)) != EOF) {
		putchar(c);
	}
	fclose(fp1);
	return 0;
}
void level_delivery_save(Vector2D pos, char *filename){
	FILE *write;
	write = fopen(filename, "a");
	if (write == NULL) {
		perror("cannot open file");
		return 1;
	}
	//char type[32] = "\nwall |";
//	char newLine[10] = "";
	char position[256] = { 0x0 };
	//char param[256] = " 200,200|";
	char tmp[32] = { 0x0 };
	char out[256] = { 0x0 };
	strcat(out, "|");
	itoa(pos.x, tmp, 10);
	strcat(position, tmp);
	strcat(position, ",");
	itoa(pos.y, tmp, 10);
	strcat(position, tmp);
	strcat(position, "|\n");
//	strcat(out, newLine);
	strcat(out, position);
	//strcat(out, param);
	fprintf(write, out);
	fclose(write);
}

void level_entity_save(Vector2D pos, char* entityType, char *filename){
	FILE *write;
	write = fopen(filename, "a");
	if (write == NULL) {
		perror("cannot open file");
		return 1;
	}
	//Type
	char type[32];
	if (strcmp(entityType, "shooter") == 0 || strcmp(entityType, "patroller") == 0 || strcmp(entityType, "chaser") == 0){
		strcpy(type, "monster ");
	}
	else{
		strcpy(type, "obsticle ");
	}
	char position[256] = { 0x0 };
	//char param[256] = { 0x0 };
	char tmp[256] = { 0x0 };
	char out[256] = { 0x0 };
	strcat(tmp, "\n");
	strcat(tmp, type);
	strcat(out,tmp);
	strcat(out, "|");
	strcpy(tmp, "");
	//Position
	itoa(pos.x, tmp, 10);
	strcat(position, tmp);
	strcat(position, ",");
	itoa(pos.y, tmp, 10);
	strcat(position, tmp);
	strcat(position, "|");
	strcat(out, position);
	strcpy(tmp, "");
	//Entity Type
	strcat(out, " ");
	strcat(tmp, tolower(entityType));
	strcat(out, tmp);
	strcat(out, "|");
	//Param  
	/*
	itoa(param.x, tmp, 10);
	strcat(position, tmp);
	strcat(position, ",");
	itoa(param.y, tmp, 10);
	strcat(position, tmp);
	strcat(position, "|");
	strcpy(tmp, "");
	fclose(write);
	*/
	fprintf(write, out);
	fclose(write);
}
ParticleEmitter *level_get_particle_emitter()
{
	return gamelevel.pe;
}