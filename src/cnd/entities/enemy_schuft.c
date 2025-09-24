/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/mesh.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_schuft(entity e)
{
    if (e->inLocalSpace)
    {
        const i16 dy = CAMERA.position.y - e->position.y;
        const i16 dx = e->position.x - CAMERA.position.x;
        if (dy > 2 || dy < -2 || dx > 2 || dx < -2)
        {
            e->velocity.x = ((dx < 0) - (dx > 0)) << 1;
            e->velocity.y = ((dy < 0) - (dy > 0)) << 1;
        }

		i8 localDy = I8(dy);
		const i8 localDx = I8(dx);

		draw_queue_push(localDx >= 0 ? schuft_left : schuft_right, localDy, localDx);

		if (e->isSameTile && manhattan(localDy, localDx) < 0xA)
		{
			const i8 localDxMask = localDx >> 7;
			const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;

			if ((CAMERA.velocity.y <= -2 && localDxAbs < 6) || (CAMERA.isAttacking && (CAMERA.velocity.x ^ localDx) >= 0))
			{
				e->update = update_death;
				add_score(Score_200);
				CAMERA.velocity.y = Velocity_KillUpWind;
			}
			else
			{
				character_damage();
			}
		}
    }
}

void prefab_schuft(entity e)
{
    e->update = update_schuft;
    e->kill   = update_kill;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}