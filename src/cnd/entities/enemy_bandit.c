/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_bandit(entity e) 
{
	LOCAL_POS(e, dy, dx);
    
    e->velocity.x = ((dx < 0) - (dx > 0));
    e->velocity.y = ((dy < 0) - (dy > 0));

    if (e->inLocalSpace)
    {
        i8 localDy       = I8(dy);
		const i8 localDx = I8(dx);

		if (GRAVITY_DOWN())
		{
			draw_stack_push(dx >= 0 ? spider_left_r : spider_right_r, localDy, localDx);
		}
		else
		{
			draw_stack_push(dx >= 0 ? spider_left : spider_right, localDy, localDx);
		}
		
        if (NEAR_CENTER(e))
        {
            if (entity_intersects_camera(e, localDy, localDx))
                entity_exchange_blows(e, localDy);
        }
    }
}

void prefab_bandit(entity e)
{
    e->update = update_bandit;
    e->kill   = update_kill;
	e->score  = Score_100;
    e->hitbox = (v2i){ Hitbox_BanditY, Hitbox_BanditX };
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}