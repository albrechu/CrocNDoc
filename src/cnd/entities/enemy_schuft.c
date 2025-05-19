#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/mesh.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>

void routine_schuft_follow(entity e)
{
    if (e->isLocal)
    {
        const i16 dy = CAMERA.position.y - e->position.y;
        const i16 dx = e->position.x - CAMERA.position.x;
        if (dy > 2 || dy < -2)
        {
            e->velocity.x = ((dx < 0) - (dx > 0)) << 1;
            e->velocity.y = ((dy < 0) - (dy > 0)) << 1;
        }
        routine_enemy_collision(e, I8(dx), I8(dy), schuft_left, schuft_right);
    }
}