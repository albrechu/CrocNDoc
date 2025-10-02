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

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_schelm_thrown(entity e)
{
    e->velocity.x += e->data[0];
    e->velocity.y += WORLD.freq16;
    if (e->inLocalSpace)
    {
        e->data[1] -= WORLD.freq16;
        if (e->data[1] <= 0)
        {
            e->update = update_death;
        }

        const i8 localDy = I8(CAMERA.position.y - e->position.y);
        const i8 localDx = I8(e->position.x - CAMERA.position.x);
        if (WORLD.freq2)
        {
            draw_stack_push(schelm, localDy, localDx);
        }
        if (NEAR_CENTER(e))
        {
            if (entity_intersects_camera(e, localDy, localDx))
                character_damage();
        }
    }
}

void prefab_schelm(entity e)
{
	e->update = update_schelm_thrown;
    e->kill   = update_kill;

    const i16 dx  = e->position.x - CAMERA.position.x;
#define PROJECTILE_SPEED_BITS 1 
    e->position.y += TILE_HEIGHT_2;
    e->velocity.y = 0;
    e->velocity.x = ((dx < 0) - (dx > 0)) << PROJECTILE_SPEED_BITS;
	e->data[0]    = e->velocity.x; // For next frames until it hits the target
    e->data[1]    = 15;
    e->score      = Score_0;
    e->hitbox     = (v2i){ Hitbox_SchelmY, Hitbox_SchelmX };

    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}
