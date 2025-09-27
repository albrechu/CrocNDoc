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
        if (--e->data[0] < 0)
        {
            e->data[0] = 70;
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

        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            draw_queue_push((localDx >= 0 ? gauner_left : gauner_right)[WORLD.freq16], localDy + 2, localDx);

            if (e->isSameTile && manhattan(localDy, localDx) < 0x15)
            {
                entity_camera_hit_detection(e, localDx);
            }
        }
    }
}

void prefab_gauner(entity e)
{
    e->update = update_gauner;
    e->kill   = update_kill;
    e->score  = Score_200;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}