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

        if (WORLD.gravity > 0)
        {
		    draw_queue_push(localDx >= 0 ? schuft_left : schuft_right, localDy, localDx);
        }
        else
        {
            draw_queue_push(localDx >= 0 ? schuft_left_r : schuft_right_r, localDy - 3, localDx);
        }

		if (e->isSameTile && manhattan(localDy, localDx) < 0xA)
		{
			entity_camera_hit_detection(e, localDx);
		}
    }
}

void prefab_schuft(entity e)
{
    e->update = update_schuft;
    e->kill   = update_kill;
    e->score  = Score_200;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}