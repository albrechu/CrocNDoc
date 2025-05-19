#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>

void routine_tunichtgut_boom(entity e)
{
	if (e->isLocal)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        beam_set_position(dy, dx);
        Draw_VLc((void* const)tunichtgut);
    }
}