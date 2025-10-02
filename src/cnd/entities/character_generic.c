/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/game.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <lib/assert.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
bool character_grab(void)
{
    for (i8 i = 1; i < WORLD.list.aliveCount; i++)
    {
        entity e = &WORLD.list.entities[WORLD.list.alive[i]];
        if (IS_SAME_TILE(e, &CAMERA) && !e->isEnemy)
        {
            v2i delta;
            delta.y = I8(e->position.y - CAMERA.position.y);
            delta.x = I8(e->position.x - CAMERA.position.x);
            if (manhattan(delta.x, delta.y) < 0x15) // Manhattan Distance
            {
                if (CAMERA.substance & Substance_Water)
                {
                    e->update = update_death;
                    add_score(Score_50);
                }
                else
                {
                    CAMERA.state     = CharacterState_HoldsProp;
                    e->state         = PropState_Held;
                    e->update        = update_barrel_held;
                    CAMERA.reference = e->handle;
                }
                CAMERA.isGrounded = false;
                return true;
            }
        }
    }
    return false;
}

void character_throw(void)
{
    if (handle_valid(CAMERA.reference))
    {
        CAMERA.state = CharacterState_Idle;
        prefab_barrel_throw(entity_get(CAMERA.reference));
    }
}

void character_damage(void)
{
    if (!CAMERA.invincibilityTicks && !GAME.isFinished)
    {
        if (PLAYER.isOtherCharacterDead)
        {
            GAME.progress = game_remove_live;
        }
        else
        {
            CAMERA.invincibilityTicks   = 50;
            character_swap();
            PLAYER.isOtherCharacterDead = true;
        }
    }
}

void character_off_ground_impulse_response(entity e)
{
    if (!e->isGrounded)
    {
        if (e->offGroundImpulseResponseTicks < 0) // Fallen of ground in the previous frame.
        {
            e->offGroundImpulseResponseTicks = 6;
            e->recoveryTicks                 = 0;
        }
        else if (e->offGroundImpulseResponseTicks > 0) // Tick down response timer
        {
            --e->offGroundImpulseResponseTicks;
        }
        // else: == 0, Until one touches ground, the response will be nil.
    }
    else if (e->offGroundImpulseResponseTicks != -1)
    {
        e->velocity.x = 0;
        e->offGroundImpulseResponseTicks = -1; // Resets to allow the impulse again after falling.
    }
}

void character_swap(void)
{
    if (CAMERA.substance & Substance_Water)
    {
        if (!PLAYER.isOtherCharacterDead)
        {
            CAMERA.isAttacking = false;
            CAMERA.type  ^= Character_Doc;
            if (CAMERA.type == Character_Doc)
            {
                CAMERA.update = update_doc_water;
                CAMERA.hitbox = (v2i){ Hitbox_DocY, Hitbox_DocX };
            }
            else
            {
                CAMERA.update = update_croc_water;
                CAMERA.hitbox = (v2i){ Hitbox_CrocY, Hitbox_CrocX };
            }
        }

    }
    else // Substance_Air
    {
        if ((CAMERA.isGrounded || CAMERA.offGroundImpulseResponseTicks) && !PLAYER.isOtherCharacterDead)
        {
            CAMERA.isAttacking = false;
            CAMERA.type       ^= Character_Doc;
            if (CAMERA.type == Character_Doc)
            {
                CAMERA.update = update_doc_air;
                CAMERA.hitbox = (v2i){ Hitbox_DocY, Hitbox_DocX };
            }
            else
            {
                CAMERA.update = update_croc_air;
                CAMERA.hitbox = (v2i){ Hitbox_CrocY, Hitbox_CrocX };
            }
        }
    }
}
