/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/game.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
bool routine_player_grab(void)
{
    for (idx_t pi = 1; pi < WORLD.entityCount; ++pi)
    {
        entity e = &WORLD.entities[WORLD.entityIdxs[pi]];
        if (e->isSameTile && !e->isEnemy && e->state == PropState_Idle)
        {
            v2i delta;
            delta.y = I8(e->position.y - CAMERA.position.y);
            delta.x = I8(e->position.x - CAMERA.position.x);
            if (manhattan(delta.x, delta.y) < 0x15) // Manhattan
            {
                CAMERA.state = CharacterState_HoldsProp;
                e->state = PropState_Held;
                e->routine = routine_barrel_held;
                if (pi != 1) // Game needs to ensure that a hold prop is always at idx 1
                {
                    idx_t tmpIdx = WORLD.entityIdxs[1];
                    WORLD.entityIdxs[1] = WORLD.entityIdxs[pi];
                    WORLD.entityIdxs[pi] = tmpIdx;
                    WORLD.entities[WORLD.entityIdxs[1]].id = 1;
                    WORLD.entities[WORLD.entityIdxs[pi]].id = pi;
                }
                CAMERA.isGrounded = false;
                return true;
            }
        }
    }
    return false;
}

void routine_player_throw(void)
{
    CAMERA.state = CharacterState_Idle;
    barrel_create_prefab(&WORLD.entities[WORLD.entityIdxs[1]]);
}

void routine_player_changed_fluid(void)
{

}

void routine_player_damage(void)
{
    if (!CAMERA.invisiblityFrames)
    {
        if (PLAYER.isOtherCharacterDead)
        {
            game_set_gameover();
        }
        else
        {
            CAMERA.type ^= Character_Doc;
            if (CAMERA.type == Character_Croc)
            {
                CAMERA.routine = CAMERA.substance == Substance_Air ? routine_croc_air : routine_croc_water;
            }
            else
            {
                CAMERA.routine = CAMERA.substance == Substance_Air ? routine_doc_air : routine_doc_water;
            }
            PLAYER.isOtherCharacterDead = true;
            CAMERA.invisiblityFrames = 30;
        }
    }
}