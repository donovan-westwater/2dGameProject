#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"
#include "space.h"
#include "Body.h"
#include "player.h"
#include "camera.h"
#include "monster.h"
#include "level.h"
#include "obsticle.h"
#include "gui.h"
#include "delivery.h"
#include "editor.h"
#include "audio.h"
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	int editorMode = 0;
	

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
	//For basic collision, create a space variable here (Keep in mind it is going to get merged into the level system later!)
	Space *space;
	LevelInfo *linfo = NULL;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        800, //720
        1200,
        800, //720
        vector4d(0,0,0,255),
        0); //Window size is 1200 x 600
    gf2d_graphics_set_frame_delay(16);
	audio_init(256, 16, 4, 1, 1, 1);
    gf2d_sprite_init(1024);
	entity_system_init(1024);

	camera_set_dimensions(0, 0, 1200, 800); //1/2 * bounds.x, 1/3 * bounds.y
	gui_setup_hud();
	camera_set_bounds(0, 0,9600, 9600);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/road.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	space = space_new_full(
		1,
		shape_rect(0, 0, 9600, 9600).s.r, //ori 2400 1440
		0.1,
		vector2d(0, 0),
		0.0,
		0.5); //was oringally 0.5
   
	/*Starting entities*/ //Put all of this in the level file
	//player_new(vector2d(900, 600));
	
	//linfo = level_info_load("levels/section1.txt");// Test overworld
	//level_wall_delete(vector2d(1, 1), "levels/editorTest.txt");
	//level_wall_save(vector2d(310, 1), "levels/editorTest.txt");
	//level_delivery_save(vector2d(200, 400), "levels/route1.txt");
	linfo = level_info_load("levels/editorTest.txt");
	//Vector2D* check = (Vector2D *)list_get_nth(linfo->shapeLocations, 0);
	//slog("%lf",check.x);
	if (linfo != NULL){
		load_game();
		linfo->curRoute = 1;
		level_init(linfo, 1);
		route_load("levels/route1.txt");
		
		if (player_get() == NULL ){ //|| (player_get()->position.x == 0 && player_get()->position.y == 0
			player_new(vector2d(600, 600));
		}
		//if(check =! NULL) slog("%lf", check->x);
	}
	
	//entity_new();
	//Entity *out = monster_spawn(vector2d(100, 150), SHOOTER);
	//Entity *crash = monster_spawn(vector2d(100, 600), SHOOTER);
	//Entity *sinkhole = obsticle_spawn(vector2d(200,200),FLINGER);
	//Entity *stockade = obsticle_spawn(vector2d(500, 500), BLOCK);
	
	//Entity *delivery = delivery_spawn(vector2d(400,500));
	//crash->velocity = vector2d(0, 0);
	//Entity *other = entity_new();
	//other->position = vector2d(100,200);
	//other->velocity = vector2d(0, -1);
	//other->hitbox.position = other->position;
	//other->hitbox.velocity = other->velocity;
	adding_all_bodies_to_space(level_get_space());
	//Playing around with static shapes to figure out how to level
	//Shape wall = shape_rect(500,200,100,50); 
	//space_add_static_shape(space, wall);
	//editorMode = 1;
	if (editorMode){
		SDL_ShowCursor(SDL_ENABLE);
		editor_launch();
	}
	/*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		gf2d_sprite_draw_image(sprite, vector2d(0, 0));
		if (keys[SDL_SCANCODE_M] && (int)mf % 5 == 0){
			editorMode = !editorMode;
			SDL_ShowCursor(SDL_ENABLE);
			editor_launch();
		}
		if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
		//WIP
		if (level_get_lose()){
			//Vector2D drawPosition;
			//vector2d_add(drawPosition, camera_get_position(), vector2d(0,0)); //Will return a 0 0 vector
			Sprite *failure = gf2d_sprite_load_image("images/failure.png");
			Vector2D scale = vector2d(1, 1);
			gf2d_sprite_draw(failure, vector2d(0, 0), &scale, NULL, NULL, NULL, NULL,(int)mf);
		//	if (keys[SDL_SCANCODE_RETURN]){
		//		entity_clear_all_but_player();
		//		level_init(linfo, 1);
		//		route_load("levels/route1.txt");
//
//				player_set_position(vector2d(600,600));
//
//				adding_all_bodies_to_space(level_get_space());
//			}
			continue;
		}
		//WIP
		if (level_get_win()){
			//Vector2D drawPosition;
			//vector2d_add(drawPosition, camera_get_position(), vector2d(0,0)); //Will return a 0 0 vector
			Sprite *victory = gf2d_sprite_load_image("images/victory.png");
			Vector2D scale = vector2d(1, 1);
			gf2d_sprite_draw(victory, vector2d(0, 0), &scale, NULL, NULL, NULL, NULL,(int) mf);
			if(keys[SDL_SCANCODE_RETURN])level_transition(vector2d(600, 600));
			continue;
		}

          
		if (!editorMode){
		   entity_think_all();
			//Physics update (put into level update)
		  // entity_pre_sync_all();
		  // space_update(space);
		  // entity_post_sync_all();
		   level_update();
		   //Entites drawn and thinks called
			entity_update_all(); 

            //UI elements last
			//space_draw(level_get_space(), vector2d(0, 0)); //put into level draw
			
		}
		else{
			//space_update(level_get_space());
			editor_update();
			
		}
		level_draw();
		if (editorMode) editor_draw();
			gui_draw_hud();
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		
     //  slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second()); // TURN BACK ON ONCE LEVEL LOADING IS FIXED
		
    }
	save_game();
	free(linfo);
	gf2d_entity_system_close();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
