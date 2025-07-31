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
//void update_schelm_waiting(entity e)
//{
//    if (e->isLocal && WORLD.ticks)
//    {
//        e->stopwatch -= WORLD.freq16;
//        if (e->stopwatch <= 0)
//        {
//            e->stopwatch = 7;
//            e->update = update_death;
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
//            if (manhattan(localDy, localDx) < 0x10 && CAMERA.invincibilityTicks == 0)
//            {
//                e->stopwatch = 7;
//                e->update = update_death;
//                character_damage();
//            }
//        }
//    }
//}

void update_schelm_thrown(entity e)
{
	/*if (!e->isGrounded)
	{
		e->velocity.x = e->data[0];
	}
	else
	{
		*((i16*)&e->velocity) = 0;
		e->update = update_schelm_waiting;
        e->stopwatch = 100;
	}*/
    e->velocity.x += e->data[0];
    e->velocity.y += WORLD.freq16;
    if (e->inLocalSpace)
    {
        e->data[1] -= WORLD.freq8_8;
        if (e->data[1] <= 0)
        {
            e->update = update_death;
        }

        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            if (WORLD.freq2)
            {
                draw_queue_push(schelm, localDy, localDx);
            }
            if (manhattan(localDy, localDx) < 0x10 && CAMERA.invincibilityTicks == 0)
            {
                e->update = update_death;
                character_damage();
            }
        }
    }
}

void prefab_schelm(entity e)
{
	e->update = update_schelm_thrown;
    e->kill   = update_kill;

    const i16 dx = e->position.x - CAMERA.position.x;
#define PROJECTILE_SPEED_BITS 4 // It then takes 16 frames to hit
    e->position.y += TILE_HEIGHT_2;
    e->velocity.y = 0;
    e->velocity.x = ((dx < 0) - (dx > 0)) << 3;
	e->data[0] = e->velocity.x; // For next frames until it hits the target
    e->data[1] = 20;

    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}
