/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/mesh.h>
#include <cnd/world.h>
#include <cnd/draw_queue.h>
#include <cnd/globals.h>
#include <cnd/sound.h>
#include <cnd/music.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_coin_achieved(entity e)
{
    e->velocity.y = 0;
    e->velocity.x = 0;
    draw_stack_push(diamond, 40 - (e->animation.remainder << 1), 0);
    if (--e->animation.remainder == 0) // Kill it
        e->kill(e);
}

void update_coin(entity e)
{
    e->velocity = (v2i){0, 0};
    if (e->inLocalSpace)
    {
        const i8 dy = I8(CAMERA.position.y - e->position.y);
        const i8 dx = I8(e->position.x - CAMERA.position.x);
        draw_stack_push(diamond, dy, dx);
        if (IS_SAME_TILE(e, &CAMERA))
        {
            e->update    = update_coin_achieved;
            e->animation.remainder = 20;
            add_score(e->score);
        }
    }
}

void prefab_coin(entity e)
{
    e->update = update_coin_achieved;
    e->animation.remainder = 20;
    e->kill   = update_kill_revive;
    add_score(Score_800);
    sound_push_sfx(&g_coinAchieved);
}