#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/mesh.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>

void routine_schelm_waiting(entity e)
{
    if (e->isLocal && WORLD.ticks)
    {
        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            beam_set_position(localDy, localDx);
            Draw_VLc((void* const)schelm);
            if (manhattan(localDy, localDx) < 0x25 && CAMERA.invisiblityFrames == 0)
            {
                e->routine = routine_death0;
                WORLD.playerDamage();
            }
        }
    }
}

void routine_schelm_thrown(entity e)
{
	if (!e->isGrounded)
	{
		e->velocity.x = e->data[0];
	}
	else
	{
		*((i16*)&e->velocity) = 0;
		e->routine = routine_schelm_waiting;
	}

    if (e->isLocal)
    {
        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            beam_set_position(localDy, localDx);
            Draw_VLc((void* const)schelm);
        }
    }
}

void schelm_create_prefab(entity e)
{
	e->routine = routine_schelm_waiting;

	i16 dy = CAMERA.position.y - e->position.y;
	i16 dx = e->position.x - CAMERA.position.x;
	
#define PROJECTILE_SPEED_BITS 4 // It then takes 16 frames to hit

    e->velocity.y = I8((dy - I8((Velocity_Friction * U8(1 << PROJECTILE_SPEED_BITS) * U8((1 << PROJECTILE_SPEED_BITS) - 1)) >> 1)) >> PROJECTILE_SPEED_BITS);
    e->velocity.x = I8(dx >> PROJECTILE_SPEED_BITS);
	e->data[0] = e->velocity.x; // For next frames until it hits the target
}
