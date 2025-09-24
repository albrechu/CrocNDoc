/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_bandit(entity e) 
{
    const i16 dy = CAMERA.position.y - e->position.y;
    const i16 dx = e->position.x - CAMERA.position.x;
    e->velocity.x = ((dx < 0) - (dx > 0));
    e->velocity.y = ((dy < 0) - (dy > 0));

    if (e->inLocalSpace)
    {
        i8 localDy = I8(dy);
		const i8 localDx = I8(dx);

		if (dy >= 0)
		{
			draw_queue_push(localDx >= 0 ? spider_left_up : spider_right_up, localDy, localDx);
		}
		else
		{
			draw_queue_push(localDx >= 0 ? spider_left_down : spider_right_down, localDy, localDx);
		}
		
		if (e->isSameTile && manhattan(localDy, localDx) < 0xA)
		{
			const i8 localDxMask = localDx >> 7;
			const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;

			if ((CAMERA.velocity.y <= -2 && localDxAbs < 6) || (CAMERA.isAttacking && (CAMERA.velocity.x ^ localDx) >= 0))
			{
				e->update = update_death;
				add_score(Score_100);
				CAMERA.velocity.y = Velocity_KillUpWind;
			}
			else
			{
				character_damage();
			}
		}
    }
}

void prefab_bandit(entity e)
{
    e->update = update_bandit;
    e->kill   = update_kill;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}