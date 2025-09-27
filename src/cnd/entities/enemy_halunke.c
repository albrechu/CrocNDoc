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
		else
			e->velocity.x = 0;

		i8 localDy = I8(dy);
		const i8 localDx = I8(dx);

		if (WORLD.gravity > 0)
		{
			draw_queue_push(localDx >= 0 ? halunke_left : halunke_right, localDy + 3, localDx);
		}
		else
		{
			draw_queue_push(localDx >= 0 ? halunke_left_r : halunke_right_r, localDy - 4, localDx);
		}

		if (e->isSameTile && manhattan(localDy, localDx) < 0xA)
		{
			entity_camera_hit_detection(e, localDx);
		}
    }
}

void prefab_halunke(entity e)
{
    e->update = update_halunke;
    e->kill   = update_kill;
	e->score  = Score_50;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}