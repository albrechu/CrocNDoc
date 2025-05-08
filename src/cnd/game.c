/*
 * MIT License
 * 
 * Copyright (c) 2025 Julian Albrecht
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/game.h>
// [[Local]]
#include <cnd/xutils.h>
#include <cnd/plot.h>
#include <cnd/track.h>
// [[Lib]]
#include <vectrex.h>
#include <lib/assert.h>
#include <lib/print.h>
#include <lib/monitor.h>

/////////////////////////////////////////////////////////////////////////
//	Globals
//
game_t g_game;

/////////////////////////////////////////////////////////////////////////
//	Jump Tables
//
const procedure_t g_render_table[] = 
{
    __stub,
    game_render_play,
    game_render_plot,
};

const procedure_t g_update_table[] = 
{
    __stub,
    game_update_play,
    game_update_plot,
};

/////////////////////////////////////////////////////////////////////////
// Game Functions
//
void game_init(void)
{
	Intensity_7F();
	Vec_Music_Flag = 0;
	Clear_Sound();
	Stop_Sound();
	
	Vec_Joy_Mux_1_X = 1;
	Vec_Joy_Mux_1_Y = 3;
	Vec_Joy_Mux_2_X = 0;
	Vec_Joy_Mux_2_Y = 0;
	Vec_Text_Height = -11;
	Vec_Text_Width  = 45;
}

void game_soft_reset(void) 
{
	MEMZERO(GAME);
    game_enter_stage(Stage_Sewers);
    // world_load_stage(Stage_Sewers);
    plot_init();
    // Assign the initial player state 
    // Assign Game State
    
    // plot_set_plot(Plot_WakeUp);
}

void game_enter_stage(Stage stage)
{
    world_create(stage);
    // Install callbacks
    WORLD.entityAdded  = game_entity_added;
    WORLD.stageEntered = game_enter_stage;

    GAME.state       = GameState_Play;
    CAMERA.mesh      = Mesh_CrocIdleRight;
    CAMERA.isLocal   = true;
    CAMERA.state     = CharacterState_Idle;
    CAMERA.transform = 1;
}

void game_entity_added(entity e)
{
    switch (e->type)
    {
    case Prop_Crate:
        e->mesh = Mesh_Crate;
        goto basic_prop;
    case Prop_Barrel:
        e->mesh = Mesh_Barrel;
        goto basic_prop;
    case Enemy_Tunichtgut:
        goto basic_enemy;
    case Enemy_Halunke:
        e->mesh = Mesh_Halunke;
        goto basic_enemy;
    case Enemy_Gauner:
        goto basic_enemy;
    case Enemy_Schuft:
        goto basic_enemy;
    case Enemy_Strolch:
        goto basic_enemy;
    case Enemy_Boesewicht:
        goto basic_enemy;
    default:
        assert(false); // ???
        return;
    }
basic_prop:
    e->state = PropState_Idle;
    return;
basic_enemy:
    e->state = EnemyState_Follow;
    return;
}

void game_set_gameover(void)
{
    GAME.state = GameState_GameOver;
    // Check if score is larger and perhaps add to highscore
    //PLAYER.score
}

void game_start_frame(void)
{
    DP_to_C8();
    dp_VIA_t1_cnt_lo = 1;
    if (Vec_Music_Flag == 0)
        Vec_Music_Flag = 1;
    
    Init_Music_chk((const void* const)g_tracks[GAME.track]);
    // Explosion_Snd(GAME.explosion);
    Wait_Recal(); // Synchronize to frame
    Do_Sound();
}

void routine_barrel_break(entity e)
{
    if (--e->stopwatch != 0)
    {
        e->velocity.y = 0;    
        e->velocity.x = 0;
    }
    else // Kill it
    {

    }
}

void routine_barrel_thrown(entity e)
{
    if (e->velocity.x == 0)
    {
        world_entity_set_status(e, -1, EntityStatus_Dead);
        e->stopwatch = 10;
        e->routine   = routine_barrel_break;
    }
}

void routine_halunke_follow(entity e)
{
    e->velocity.x += I8(U8(CAMERA.position.x - e->position.x) & 0x80) >> 7;
    if (e->isLocal)
    {
        i8 dy = I8(e->position.y - e->position.y);
        i8 dx = I8(e->position.x - e->position.x);
        if (manhattan(dy, dx) < 0x15)
        {
            dy = (i8)Abs_b(dy);
            dx = (i8)Abs_b(dx);
            if (dy > dx)
            {
            }
            else
            {
                if (PLAYER.isOtherCharacterDead)
                {
                    game_set_gameover();
                }
                else
                {
                    CAMERA.type ^= Character_Doc;
                    PLAYER.isOtherCharacterDead = true;
                }
            }
        }
    }
}

void game_update_play(void)
{
    // Process player input
	action_update();
    
    for (idx_t i = 1; i < WORLD.entityCount; ++i)
    {
        const idx_t idx = WORLD.entityIdxs[i];
        entity e = &WORLD.entities[idx];

        // Check if entity is local.
        i8 distX = e->tile.x - CAMERA.tile.x;
        i8 distY = e->tile.y - CAMERA.tile.y;
        e->isLocal = distX <= 4 && distY <= 4;
        if (!e->isLocal)
        {
            if (distX >= 10 || distY >= 10) // Possibly remove entity if too far away
            {
                world_entity_set_status(e, i, EntityStatus_Inactive);
            }
        }
     
        e->routine(e);

        switch(e->type)
        {
        case Enemy_Gauner: // Jump and shoot
            if (e->velocity.y == 0 && (GAME.ticks & 0x3F) == 0)
            {
                e->velocity.y += Velocity_Jump;
            }
            if ((GAME.ticks & 0x3F) == 0)
            {
                // Spawn projectile
            }
            break;
        case Enemy_Schuft: // Fliegeviech
        {
            const i16 dx = CAMERA.position.x - e->position.x;
            const i16 dy = CAMERA.position.y - e->position.y;
            if (e->isLocal)
            {

            }
            else
            {

            }
            e->velocity.x += ((dx > 0) - (dx < 0)) * 1;
            e->velocity.y += ((dy > 0) - (dy < 0)) * 1;
        }
            break;
        case Enemy_Strolch:
            break;
        case Enemy_Boesewicht:
            break;
        default:
            break;
        }

    }
    
    world_progress();
    ++GAME.ticks;
}

void game_update_plot(void)
{
    if (BTNS & Input_Button1)
    {
        if (plot_skip())
        {
            GAME.state = GameState_Play;
            return;
        }
    }
    plot_typewriter_next(GAME.ticks++);
}

void game_render_play(void)
{
    // Draw Parallax


    // Draw Player
    switch (CAMERA.type)
    {
    case Character_Croc:
        beam_set_position(4, CAMERA.transform * -4);
        Draw_VLc(mesh_get(CAMERA.mesh)); // Draw body
        // Moveto_d(PLAYER.armY - 10, (PLAYER.transform == 1) * 7 - 5);
        Moveto_d(5, (CAMERA.transform) * 7 - 4);
        Draw_VLc(mesh_get(Mesh_CrocArm)); 
        break;
    case Character_Doc:
        beam_set_position(4, CAMERA.transform * -4);
        Draw_VLc(mesh_get(CAMERA.mesh));
        if (CAMERA.state == CharacterState_Glide)
        {
            beam_set_position(12, -3 * CAMERA.transform);
            Draw_Line_d(10, -CAMERA.transform * ((((i8)GAME.ticks & 1) * -15) | 15)); 
        }
        break;    
    default:
        break;
    }

    // Draw Entities
    for (i8 i = 1; i < WORLD.entityCount; ++i)
    {
        entity e = &WORLD.entities[WORLD.entityIdxs[i]];
        if (e->isLocal)
        {
            beam_set_position(I8(CAMERA.position.y - e->position.y), I8(e->position.x - CAMERA.position.x));
            Draw_VLc(mesh_get(e->mesh + CAMERA.transform));
        }
    }
}

void game_render_plot(void)
{
    plot_print();   
    game_render_play();
}

void game_render_pause(void)
{
}

void game_render_gameover(void)
{
}

/////////////////////////////////////////////////////////////////////////
//	Action Functions
//
void action_update(void)
{
    switch (CAMERA.state)
    {
    case CharacterState_Glide:
        if (Vec_Btn_State & Input_Button1)
            CAMERA.velocity.y = 0;
        else
            CAMERA.state = CharacterState_Idle;
        break;
    
    default:
        break;
    }

    switch (BTNS)
    {
    case Input_Button1: // Jump
        if (CAMERA.velocity.y == 0)
        {
            CAMERA.velocity.y += Velocity_Jump;
            CAMERA.velocity.x += CAMERA.transform * Velocity_JumpX;
        }
        else if (CAMERA.type == Character_Doc)
        {
            CAMERA.state = CharacterState_Glide;
            CAMERA.velocity.y = 0;
        }
        break;
    case Input_Button2: // Hit | Grab
    {
        switch (CAMERA.state)
        {
        case CharacterState_Idle: // Grab if one is close enough
        {
            for (idx_t pi = 1; pi < WORLD.entityCount; ++pi)
            {
                entity e = &WORLD.entities[WORLD.entityIdxs[pi]];
                if (!e->isEnemy)
                {
                    if (e->isLocal && e->state == PropState_Idle)
                    {
                        v2i delta;
                        delta.y = I8(e->position.y - CAMERA.position.y);
                        delta.x = I8(e->position.x - CAMERA.position.x);
                        if (manhattan(delta.x, delta.y) < 0x15) // Manhattan
                        {
                            CAMERA.state = CharacterState_HoldsProp;
                            e->state = PropState_Held;
                            if (pi != 0) // Game needs to ensure that a hold prop is always at idx 0
                            {
                                idx_t tmpIdx = WORLD.entityIdxs[0];
                                WORLD.entityIdxs[0] = WORLD.entityIdxs[pi];
                                WORLD.entityIdxs[pi] = tmpIdx;    
                            }
                            goto bite_skip;
                        }
                    }
                }
            }
            if (CAMERA.velocity.y == 0)
                CAMERA.velocity.x = CAMERA.transform * Velocity_Bite;
        }
            break;
        case CharacterState_HoldsProp: // Throw
        {
            CAMERA.state = CharacterState_Idle;
            entity e = &WORLD.entities[WORLD.entityIdxs[0]];
            e->state     = PropState_Thrown;
            e->transform = CAMERA.transform;
            if (JOYS == Input_JoyDown)
            {
                e->velocity.y = -Velocity_ThrowY;
            }
            else
            {
                e->velocity.x = CAMERA.transform * Velocity_ThrowX;
                e->velocity.y = Velocity_ThrowY;
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case Input_Button3:
        break;
    case Input_Button4: // Swap
        if (CAMERA.velocity.y == 0)
        {
            switch (CAMERA.type)
            {
            case Character_Croc:
                CAMERA.type = Character_Doc;
                CAMERA.mesh = Mesh_DocIdleLeft + (CAMERA.transform >> 7);
                break;
            case Character_Doc:
                CAMERA.type = Character_Croc;
                CAMERA.mesh = Mesh_CrocIdleLeft + (CAMERA.transform >> 7);
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

bite_skip:
    switch (JOYS)
    {
    case Input_JoyLeftUp:
    case Input_JoyLeftDown:
    case Input_JoyLeft:
        if (CAMERA.transform == 1)
        {
            CAMERA.transform = -1;
            CAMERA.mesh = CAMERA.type == Character_Croc ? Mesh_CrocIdleLeft : Mesh_DocIdleLeft;
            CAMERA.velocity.x = (-Velocity_Run) << (CAMERA.velocity.y == 0);
        }
        else if (CAMERA.velocity.x > -Velocity_Run)
        {
            CAMERA.velocity.x = (-Velocity_Run) << (CAMERA.velocity.y == 0);
        }
        break;
    case Input_JoyRightUp:
    case Input_JoyRightDown:
    case Input_JoyRight:
        if (CAMERA.transform == -1)
        {
            CAMERA.transform = 1;
            CAMERA.mesh = CAMERA.type == Character_Croc ? Mesh_CrocIdleRight : Mesh_DocIdleRight;
            CAMERA.velocity.x = Velocity_Run << (CAMERA.velocity.y == 0);
        }
        else if (CAMERA.velocity.x < Velocity_Run)
        {
            CAMERA.velocity.x = Velocity_Run << (CAMERA.velocity.y == 0);
        }
        break;
    default:
        break;
    }

    if (JOYS & Input_JoyUp)
    {
        CAMERA.velocity.y = -Velocity_Run;
    }
    if (JOYS & Input_JoyDown)
    {
        CAMERA.velocity.y = Velocity_Run;
    }

    assert(WORLD.entityCount <= ENTITIES_ACTIVE_MAX);
}
/////////////////////////////////////////////////////////////////////////
//	Rest Functions
//
void __stub(void) { }
