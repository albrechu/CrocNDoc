/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/game.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>
#include <lib/assert.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_barrel_thrown(entity e)
{
    if (e->isGrounded)
    {
        e->update = update_death;
    }
    else
    {
        e->velocity.x = e->data[0];
    }

    if (e->inLocalSpace)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        draw_queue_push(e->transform == 1 ? barrel_right : barrel_left, dy, dx);
    }
}

void update_barrel(entity e)
{
    if (e->inLocalSpace)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        if (WORLD.gravity > 0)
        {
            draw_queue_push(barrel, dy, dx);
        }
        else
        {
            draw_queue_push(barrel_r, dy, dx);
        }
    }
}

void update_barrel_held(entity e)
{
    e->position = CAMERA.position;
    e->tile = CAMERA.tile;
    e->velocity.x = 0;
    e->velocity.y = 0;
    if (WORLD.gravity > 0)
    {
        draw_queue_push(CAMERA.transform == 1 ? barrel_right : barrel_left, 0, 0);
    }
    else
    {
        draw_queue_push(CAMERA.transform == 1 ? barrel_right_r : barrel_left_r, 0, 0);
    }
}

void prefab_barrel(entity e)
{
    e->update = update_barrel;
    e->kill   = update_kill_revive;
    e->score  = Score_50;
    entity_set_animation(e, explosion2, ELEMENT_SIZE(explosion2), ARRAY_SIZE(explosion2));
}

void prefab_barrel_throw(entity e)
{
    e->update    = update_barrel_thrown;
    e->transform = CAMERA.transform;
    
    e->position.y += 2;
    e->isGrounded = false;
    e->velocity.x = CAMERA.transform * Velocity_ThrowX;
    e->velocity.y = WORLD.gravity * Velocity_ThrowY;
    e->data[0] = e->velocity.x;
    entity_set_animation(e, explosion2, ELEMENT_SIZE(explosion2), ARRAY_SIZE(explosion2));
}