/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_halunke(entity e)
{
    if (e->inLocalSpace)
    {
        const i16 dx = e->position.x - CAMERA.position.x;
        const i16 dy = CAMERA.position.y - e->position.y;
        if (dx < -2 || dx > 2)
            e->velocity.x = ((dx < 0) - (dx > 0)) << 1;

		i8 localDy = I8(dy);
		const i8 localDx = I8(dx);

		draw_queue_push(localDx >= 0 ? halunke_left : halunke_right, localDy, localDx);

		if (e->isSameTile && manhattan(localDy, localDx) < 0xA)
		{
			const i8 localDxMask = localDx >> 7;
			const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;

			if ((CAMERA.velocity.y <= -2 && localDxAbs < 6) || (CAMERA.isAttacking && (CAMERA.velocity.x ^ localDx) >= 0))
			{
				e->update = update_death;
				PLAYER.score      += Score_50;
				CAMERA.velocity.y += Velocity_KillUpWind;
				CAMERA.velocity.y = MIN8(CAMERA.velocity.y, Velocity_KillUpWind);
			}
			else
			{
				character_damage();
			}
		}
    }
}

void prefab_halunke(entity e)
{
    e->update = update_halunke;
    e->kill   = update_kill;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}