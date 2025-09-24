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
    for (idx_t pi = 1; pi < WORLD.entityCount; ++pi)
    {
        entity e = &WORLD.entities[WORLD.entityIdxs[pi]];
        if (e->isSameTile && !e->isEnemy)
        {
            v2i delta;
            delta.y = I8(e->position.y - CAMERA.position.y);
            delta.x = I8(e->position.x - CAMERA.position.x);
            if (manhattan(delta.x, delta.y) < 0x15) // Manhattan Distance
            {
                CAMERA.state = CharacterState_HoldsProp;
                if (CAMERA.substance & Substance_Water)
                {
                    e->update = update_death;
                    add_score(Score_50);
                }
                else
                {
                    e->state     = PropState_Held;
                    e->update    = update_barrel_held;

                    if (pi != 1) // Game needs to ensure that a hold prop is always at idx 1
                    {
                        idx_t tmpIdx = WORLD.entityIdxs[1];
                        WORLD.entityIdxs[1] = WORLD.entityIdxs[pi];
                        WORLD.entityIdxs[pi] = tmpIdx;
                        WORLD.entities[WORLD.entityIdxs[1]].id = 1;
                        WORLD.entities[WORLD.entityIdxs[pi]].id = pi;
                    }
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
    CAMERA.state = CharacterState_Idle;
    prefab_barrel_throw(&WORLD.entities[WORLD.entityIdxs[1]]);
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