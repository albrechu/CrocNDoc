/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/mesh.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <lib/monitor.h>
#include <cnd/draw_queue.h>
#include <cnd/music.h>
#include <cnd/sound.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_schelm_thrown(entity e)
{
    e->velocity.x += (i8)e->velocityCache;
    e->velocity.y += WORLD.freq16;
    if (e->inLocalSpace)
    {
        if (--e->timer == 0)
        {
            e->update = update_death;
            sound_push_sfx(&g_explosion1);
        }

        const i8 localDy = I8(CAMERA.position.y - e->position.y);
        const i8 localDx = I8(e->position.x - CAMERA.position.x);
        if (WORLD.freq2)
        {
            if (GRAVITY_DOWN())
            {
                //e->hitbox.y = Hitbox_SchelmY;
                draw_stack_push(schelm, localDy, localDx);
            }
            else
            {
                //e->hitbox.y = -Hitbox_SchelmY;
                draw_stack_push(schelm_r, localDy, localDx);
            }
        }



        if (NEAR_CENTER(e))
        {
            if (entity_intersects_camera(e, localDy, localDx))
            {
                e->update = update_death;
                character_damage();
                sound_push_sfx(&g_explosion1);
            }
        }
    }
}

void prefab_schelm(entity e)
{
	e->update = update_schelm_thrown;
    e->kill   = update_kill;

    const i16 dx  = e->position.x - CAMERA.position.x;
    e->position.y += TILE_HEIGHT_2;
    e->velocity.y = 0;
    e->velocity.x = ((dx < 0) - (dx >= 0)) << 1;
	e->velocityCache = e->velocity.x; // For next frames until it hits the target
    e->timer      = 60;
    e->score      = Score_0;
    e->hitbox     = (v2i){ Hitbox_SchelmY, Hitbox_SchelmX };

    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}
