#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>

void routine_death0(entity e)
{
    if (e->stopwatch != 0) // Play Animation
    {
        e->velocity.y = 0;
        e->velocity.x = 0;
        if (WORLD.freq8_8)
        {
            --e->stopwatch;
            if (e->isLocal)
            {
                i16 dy = CAMERA.position.y - e->position.y;
                i16 dx = e->position.x - CAMERA.position.x;
                beam_set_position((i8)dy, (i8)dx);
                Draw_VLc((void* const)explosion[WORLD.freq8_8]);
            }
        }
    }
    else // Kill it
    {
        PLAYER.isOtherCharacterDead = false; // Gets the other character back
        entity_set_status(e, EntityStatus_Dead);
    }
}

void routine_death1(entity e)
{
    if (e->stopwatch != 0) // Play Animation
    {
        e->velocity.y = 0;
        e->velocity.x = 0;
        if (WORLD.freq8_8)
        {
            --e->stopwatch;
            if (e->isLocal)
            {
                i16 dy = CAMERA.position.y - e->position.y;
                i16 dx = e->position.x - CAMERA.position.x;
                beam_set_position((i8)dy, (i8)dx);
                Draw_VLc((void* const)explosion[WORLD.freq8_8]);
            }
        }
    }
    else // Kill it
    {
        entity_set_status(e, EntityStatus_Dead);
    }
}