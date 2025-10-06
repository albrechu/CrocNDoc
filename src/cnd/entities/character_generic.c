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
#include <cnd/music.h>
#include <cnd/sound.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
bool character_grab(void)
{
    for (i8 i = 1; i < ENTITIES_ACTIVE_MAX; i++)
    {
        entity e = &WORLD.list.entities[i];
        if (e->isAllocated && IS_SAME_TILE(e, &CAMERA) && !e->isEnemy)
        {
            v2i delta;
            delta.y = I8(e->position.y - CAMERA.position.y);
            delta.x = I8(e->position.x - CAMERA.position.x);
            if (manhattan(delta.x, delta.y) < 0x15) // Manhattan Distance
            {
                if (CAMERA.substance & Substance_Water)
                {
                    e->update = update_death;
                    sound_push_sfx(&g_explosion2);
                    add_score(e->score);
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
            sound_push_sfx(&g_explosion2);
            GAME.progress = game_remove_live;
        }
        else
        {
            CAMERA.invincibilityTicks   = 50;
            CAMERA.isGrounded = true;
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
            sound_push_sfx(&g_swap);
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
            sound_push_sfx(&g_swap);
        }
    }
}
