/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/mesh.h>
#include <cnd/draw_queue.h>
#include <cnd/globals.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_crate(entity e)
{
	if (e->inLocalSpace)
	{
		const i8 dy = I8(CAMERA.position.y - e->position.y);
		const i8 dx = I8(e->position.x - CAMERA.position.x);
		draw_queue_push(crate, dy, dx);
	}
}

void prefab_crate(entity e)
{
	e->update = update_crate;
	e->kill   = update_kill;
	entity_set_animation(e, explosion2, ELEMENT_SIZE(explosion2), ARRAY_SIZE(explosion2));
}