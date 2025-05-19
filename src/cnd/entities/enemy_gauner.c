#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>

void routine_gauner_watching(entity e)
{
    if (e->isLocal)
    {
        if (WORLD.freq16)
        {
            if (e->isGrounded)
            {
                e->velocity.y += Velocity_Jump;
            }
            else
            {
                entity_create_anonymous(Enemy_Schelm, e->tile);
            }
        }

        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            beam_set_position(localDy, localDx);
            Draw_VLc((void* const)(localDx >= 0 ? gauner_right : gauner_left)[WORLD.freq16]);
            if (manhattan(localDy, localDx) < 0x15)
            {
                const i8 localDyMask = localDy >> 7;
                localDy = (localDy ^ localDyMask) - localDyMask;
                const i8 localDxMask = localDx >> 7;
                const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;

                if (localDy > localDxAbs)
                {
                    e->stopwatch = 10;
                    e->routine = routine_death0;
                    CAMERA.velocity.y += Velocity_KillUpWind;
                }
                else if (CAMERA.isAttacking && (e->velocity.x ^ localDx) < 0)
                {

                }
                else
                {
                    WORLD.playerDamage();
                }
            }
        }
    }
}