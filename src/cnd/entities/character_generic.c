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
        if (e->isSameTile && !e->isEnemy)
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
            CAMERA.type ^= Character_Doc;
            switch (CAMERA.substance)
            {
            case Substance_Water:
                CAMERA.isAttacking = false;
                CAMERA.velocity.y = 0;
                CAMERA.update      = CAMERA.type == Character_Croc ? update_croc_water : update_doc_water;
                break;
            case Substance_Air:
                CAMERA.isAttacking = false;
                CAMERA.velocity.y  = 0;
                CAMERA.update      = CAMERA.type == Character_Croc ? update_croc_air : update_doc_air;
                break;
            case Substance_GravitasAir:
                CAMERA.isAttacking = false;
                CAMERA.velocity.y = 0;
                CAMERA.update      = CAMERA.type == Character_Croc ? update_croc_gravitas_air : update_doc_gravitas_air;
                break;
            case Substance_GravitasWater:
                CAMERA.isAttacking = false;
                CAMERA.velocity.y = 0;
                CAMERA.update      = CAMERA.type == Character_Croc ? update_croc_gravitas_water : update_doc_gravitas_water;
                break;
            default:
                break;
            }
            PLAYER.isOtherCharacterDead = true;
            CAMERA.invincibilityTicks = 30;
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