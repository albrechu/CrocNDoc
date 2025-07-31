/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_tunichtgut(entity e)
{
	if (e->inLocalSpace)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        draw_queue_push(tunichtgut, dy, dx);
    }
}

void prefab_tunichtgut(entity e)
{
    e->update = update_tunichtgut;
    e->kill   = update_kill;
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}