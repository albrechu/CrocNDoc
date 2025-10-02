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
	LOCAL_POS(e, dy, dx);

	e->velocity.x = ((dx < 0) - (dx > 0)) << 1;

    if (e->inLocalSpace)
    {
		const i8 localDx = I8(dx);
		const i8 localDy = I8(dy);

		if (GRAVITY_DOWN())
		{
			draw_stack_push(localDx >= 0 ? halunke_left : halunke_right, localDy, localDx);
		}
		else
		{
			draw_stack_push(localDx >= 0 ? halunke_left_r : halunke_right_r, localDy, localDx);
		}

		if (NEAR_CENTER(e))
		{
			if (entity_intersects_camera(e, localDy, localDx))
				entity_exchange_blows(e, localDy);
		}
    }
}

void prefab_halunke(entity e)
{
    e->update = update_halunke;
    e->kill   = update_kill;
	e->score  = Score_50;
	e->hitbox = (v2i){ Hitbox_HalunkeY, Hitbox_HalunkeX };
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}