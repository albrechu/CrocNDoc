/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/mesh.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <lib/monitor.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
//void routine_schelm_waiting(entity e)
//{
//    if (e->isLocal && WORLD.ticks)
//    {
//        e->stopwatch -= WORLD.freq16;
//        if (e->stopwatch <= 0)
//        {
//            e->stopwatch = 7;
//            e->routine = routine_death2;
//        }
//
//        i16 dy = CAMERA.position.y - e->position.y;
//        i16 dx = e->position.x - CAMERA.position.x;
//        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
//        {
//            i8 localDx = (i8)dx;
//            i8 localDy = (i8)dy;
//            if (WORLD.freq2)
//            {
//                beam_set_position(localDy, localDx);
//                Draw_VLc((void* const)schelm);
//            }
//            if (manhattan(localDy, localDx) < 0x10 && CAMERA.invisiblityFrames == 0)
//            {
//                e->stopwatch = 7;
//                e->routine = routine_death2;
//                WORLD.playerDamage();
//            }
//        }
//    }
//}

void routine_schelm_thrown(entity e)
{
	/*if (!e->isGrounded)
	{
		e->velocity.x = e->data[0];
	}
	else
	{
		*((i16*)&e->velocity) = 0;
		e->routine = routine_schelm_waiting;
        e->stopwatch = 100;
	}*/
    e->velocity.x += e->data[0];
    e->velocity.y += WORLD.freq16;
    if (e->isLocal)
    {
        e->stopwatch -= WORLD.freq8_8;
        if (e->stopwatch <= 0)
        {
            e->stopwatch = 7;
            e->routine = routine_death2;
        }

        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            if (WORLD.freq2)
            {
                beam_set_position(localDy, localDx);
                Draw_VLc((void* const)schelm);
            }
            if (manhattan(localDy, localDx) < 0x10 && CAMERA.invisiblityFrames == 0)
            {
                e->stopwatch = 7;
                e->routine = routine_death2;
                WORLD.playerDamage();
            }
        }
    }
}

void schelm_create_prefab(entity e)
{
	e->routine = routine_schelm_thrown;

    const i16 dx = e->position.x - CAMERA.position.x;
#define PROJECTILE_SPEED_BITS 4 // It then takes 16 frames to hit
    e->position.y += TILE_HEIGHT_2;
    e->velocity.y = 0;
    e->velocity.x = ((dx < 0) - (dx > 0)) << 3;
    e->stopwatch = 20;
	e->data[0] = e->velocity.x; // For next frames until it hits the target
}
