#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/game.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>

void routine_barrel_thrown(entity e)
{
    if (e->isGrounded)
    {
        e->stopwatch = 10;
        e->routine   = routine_death0;
    }
    else
    {
        e->velocity.x = e->data[0];
    }

    if (e->isLocal)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        beam_set_position(dy, dx);
        Draw_VLc((void* const)(e->transform == 1 ? barrel_right : barrel_left));
    }
}

void routine_barrel_idle(entity e)
{
    if (e->isLocal)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        beam_set_position(dy, dx);
        Draw_VLc((void* const)barrel);
    }
}

void routine_barrel_held(entity e)
{
    e->position = CAMERA.position;
    (void)e;
    beam_set_position(0, 0);
    Draw_VLc((void* const)(CAMERA.transform == 1 ? barrel_right : barrel_left));
}

void barrel_create_prefab(entity e)
{
    e->state = PropState_Thrown;
    e->routine = routine_barrel_thrown;
    e->transform = CAMERA.transform;
    if (Vec_Joy_1_Y < 0)
    {
        e->velocity.y = -Velocity_ThrowY;
    }
    else
    {
        e->velocity.x = CAMERA.transform * Velocity_ThrowX;
        e->velocity.y = Velocity_ThrowY;
        e->data[0] = e->velocity.x;
    }
}