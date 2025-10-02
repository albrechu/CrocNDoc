/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_gauner(entity e)
{
    if (e->inLocalSpace)
    {
        if (--e->recoveryTicks < 0)
        {
            e->recoveryTicks = 70;
            if (e->isAttacking)
            {
                e->velocity.y += Velocity_Jump;
                e->isAttacking = false;
            }
            else
            {
                entity_create_anonymous(Enemy_Schelm, e->tile);
                e->isAttacking = true;
            }
        }

        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        draw_stack_push((dx >= 0 ? gauner_left : gauner_right)[WORLD.freq16], dy + 2, dx);
        if (NEAR_CENTER(e))
        {
            if (entity_intersects_camera(e, dy, dx))
                entity_exchange_blows(e, dy);
        }
    }
}

void prefab_gauner(entity e)
{
    e->update = update_gauner;
    e->kill   = update_kill;
    e->score  = Score_200;
    e->hitbox = (v2i){ Hitbox_HalunkeY, Hitbox_HalunkeX };
    e->recoveryTicks = -1;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}