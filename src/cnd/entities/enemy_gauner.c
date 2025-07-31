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
            draw_queue_push((localDx >= 0 ? gauner_left : gauner_right)[WORLD.freq16], localDy, localDx);

            if (manhattan(localDy, localDx) < 0x15)
            {
                const i8 localDyMask = localDy >> 7;
                localDy = (localDy ^ localDyMask) - localDyMask;
                const i8 localDxMask = localDx >> 7;
                const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;

                if (localDy > localDxAbs)
                {
                    e->update = update_death;
                    CAMERA.velocity.y += Velocity_KillUpWind;
                }
                else if (CAMERA.isAttacking && (e->velocity.x ^ localDx) < 0)
                {
                    e->update = update_death;
                }
                else
                {
                    character_damage();
                }
            }
        }
    }
}

void prefab_gauner(entity e)
{
    e->update = update_gauner;
    e->kill   = update_kill;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}