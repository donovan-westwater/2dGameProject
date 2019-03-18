#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"
#include "space.h"
#include "Body.h"
#include "player.h"
#include "level.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
	

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
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
	entity_system_init(16);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	space = space_new_full(
		1,
		shape_rect(0, 0, 1200, 720).s.r,
		0.1,
		vector2d(0, 0),
		0.0,
		0.5); //was oringally 0.5
   
	/*Starting entities*/ //Put all of this in the level file
	
	linfo = level_info_load("levels/section1.txt");  //ERROR HERE
	if (linfo != NULL){
		level_init(linfo, space);
	}
	player_new(vector2d(600,600));
	//entity_new();
	//Entity *other = entity_new();
	//other->position = vector2d(100,150);
	//other->velocity = vector2d(1, -1);
	//other->hitbox.position = other->position;
	//other->hitbox.velocity = other->velocity;
	adding_all_bodies_to_space(space);
	//Playing around with static shapes to figure out how to level
	//Shape wall = shape_rect(500,200,100,50); 
	//space_add_static_shape(space, wall);

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
           gf2d_sprite_draw_image(sprite,vector2d(0,0));
		   
		   entity_think_all();
			//Physics update (put into level update)
		   entity_pre_sync_all();
		   space_update(space);
		   entity_post_sync_all();

		   //Entites drawn and thinks called
			entity_update_all(); 

            //UI elements last
			space_draw(space, vector2d(0, 0)); 
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
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
		
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
