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
#include <lib/assert/assert.h>
#include <lib/print/print.h>
#include <lib/debug/monitor.h>

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
    world_create(Stage_Sewers);
    // world_load_stage(Stage_Sewers);
    plot_init();
    // Assign the initial player state 
    // Assign Game State
    GAME.state = GameState_Play;
    mesh_load(Mesh_CrocArm, 0, PLAYER.arm);
    PLAYER.armY       = 15;
    CAMERA.mesh       = Mesh_CrocIdleRight;
    CAMERA.transform  = 1;
    // plot_set_plot(Plot_WakeUp);
}

void game_start_frame(void)
{
    DP_to_C8();
    dp_VIA_t1_cnt_lo = 1;
    if (Vec_Music_Flag == 0)
    {
        Vec_Music_Flag = 1;
    }
    
    Init_Music_chk((const void* const)g_tracks[GAME.track]);
    Wait_Recal(); // Synchronize to frame
    Do_Sound();
}

void game_update_play(void)
{
    // Process player input
	action_update();

    assert(WORLD.props == 0);
    /// Camera Scroll and physics
	// CAMERA.velocity.x += Velocity_Friction * (CAMERA.velocity.x != 0 && CAMERA.velocity.y == 0) * CAMERA.transform;
	// CAMERA.velocity.y += Velocity_Gravity;
    
    for (idx_t e = 0; e < WORLD.enemies; ++e)
    {
        switch (ENEMY(e).type)
        {
        case Enemy_Halunke: // Walk towards player
            ENEMY(e).velocity.x += I8(U8(CAMERA.position.x - ENEMY(e).position.x) & 0x80) >> 7;
            break;
        case Enemy_Gauner: // Jump and shoot
            if (ENEMY(e).velocity.y == 0 && (GAME.ticks & 0x3F) == 0)
            {
                ENEMY(e).velocity.y += Velocity_Jump;
            }
            if ((GAME.ticks & 0x3F) == 0)
            {
                // Spawn projectile
            }
            break;
        case Enemy_Schuft: // Fliegeviech
        {
            const i16 dx = CAMERA.position.x - ENEMY(e).position.x;
            const i16 dy = CAMERA.position.y - ENEMY(e).position.y;
            if (is_local(ENEMY(e).position))
            {

            }
            else
            {

            }
            ENEMY(e).velocity.x += ((dx > 0) - (dx < 0)) * 1;
            ENEMY(e).velocity.y += ((dy > 0) - (dy < 0)) * 1;
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

    switch (CAMERA.state)
    {
    case CharacterState_Idle:    
        PLAYER.arm[1]  = -10 + CAMERA.velocity.y + CAMERA.transform * (CAMERA.velocity.x >> 2);
        PLAYER.arm[12] = -(PLAYER.arm[2] = CAMERA.velocity.x);
        break;
    default:
        break;
    }

    
    

    ++GAME.ticks;
    // GAME.ticks = (GAME.ticks + 1) & 0x3F;
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
        Moveto_d(PLAYER.armY - 5, (CAMERA.transform) * 7 - 4);
        Draw_VLc(PLAYER.arm);
        break;
    case Character_Doc:
        beam_set_position(4, CAMERA.transform * -4);
        Draw_VLc(mesh_get(CAMERA.mesh));
        if (CAMERA.state == CharacterState_Glide)
        {
            assert(WORLD.props == 0);
            beam_set_position(12, -3 * CAMERA.transform);
            Draw_Line_d(10, -CAMERA.transform * ((((i8)GAME.ticks & 1) * -15) | 15)); 
        }
        break;    
    default:
        break;
    }

    // Draw Props
    for (idx_t p = 0; p < WORLD.props; ++p)
    {
        const entity prop = &WORLD.prop[p];
        switch (PROP(p).state)
        {
        case PropState_Held:
            beam_set_position(-3, 0);
            Draw_VLc(mesh_get(prop->mesh + CAMERA.transform));
            break;
        default:
            if (prop->isLocal)
            {
                v2l delta;
                delta.x = prop->position.x - CAMERA.position.x;
                delta.y = prop->position.y - CAMERA.position.y;
                beam_set_position(delta.y_lsb, delta.x_lsb);
                Draw_VLc(mesh_get(prop->mesh + CAMERA.transform));
            }
            break;
        }
    }
    
    // Draw Enemies
    for (idx_t e = 0; e < WORLD.enemies; ++e)
    {
        const entity enemy = &WORLD.enemy[e];
        if (enemy->isLocal)
        {
            v2l delta;
            delta.x = enemy->position.x - CAMERA.position.x;
            delta.y = enemy->position.y - CAMERA.position.y;
            beam_set_position(delta.y_lsb, delta.x_lsb);
            Draw_VLc(mesh_get(enemy->mesh + CAMERA.transform));
        }
    }

    beam_set_position(0, 0);
    Draw_Line_d(CAMERA.velocity.y, CAMERA.velocity.x);
    // Draw Background
    world_draw();
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
            CAMERA.velocity.y = Velocity_Gravity;
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
            for (idx_t pi = 0; pi < WORLD.props; ++pi)
            {
                v2l delta;
                delta.x = WORLD.prop[pi].position.x - CAMERA.position.x;
                delta.y = WORLD.prop[pi].position.y - CAMERA.position.y;
                if (is_local(delta))
                {
                    if (manhattan(delta.x_lsb, delta.y_lsb) < 0x15) // Manhattan
                    {
                        CAMERA.state = CharacterState_HoldsProp;
                        WORLD.prop[pi].state = PropState_Held;
                        if (pi != 0) // Game needs to insure that a hold prop is always at idx 0
                        {
                            entity_t tmp  = WORLD.prop[0];
                            WORLD.prop[0]  = WORLD.prop[pi];
                            WORLD.prop[pi] = tmp;
                        }

                        PLAYER.armY  = 9;
                        mesh_load(Mesh_CrocArmForward, CAMERA.transform ^ 1, PLAYER.arm);
                        goto bite_skip;
                    }
                }
            }
            CAMERA.velocity.x = CAMERA.transform * Velocity_Bite;
        }
            break;
        case CharacterState_HoldsProp: // Throw
            CAMERA.state = CharacterState_Idle;
            mesh_load(Mesh_CrocArm, 0, PLAYER.arm);
            PLAYER.armY = 15;

            WORLD.prop[0].state  = PropState_Thrown;
            if (JOYS == Input_JoyDown)
            {
                WORLD.prop[0].velocity.y = -Velocity_ThrowY;
            }
            else
            {
                WORLD.prop[0].velocity.x = CAMERA.transform * Velocity_ThrowX;
                WORLD.prop[0].velocity.y = Velocity_ThrowY;
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
            assert(WORLD.props == 0);
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
            assert(WORLD.props == 0);
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
            switch (CAMERA.type)
            {
            case Character_Croc:
                CAMERA.mesh = Mesh_CrocIdleLeft;
                if (CAMERA.state == CharacterState_HoldsProp)
                    mesh_load(Mesh_CrocArmForward, CAMERA.transform ^ 1, PLAYER.arm);
                break;
            case Character_Doc:
                CAMERA.mesh = Mesh_DocIdleLeft;
                break;
            default:
                break;
            }
        }
        else
        {
            CAMERA.velocity.x = -Velocity_Run;
        }
        break;
    case Input_JoyRightUp:
    case Input_JoyRightDown:
    case Input_JoyRight:
        if (CAMERA.transform == -1)
        {
            CAMERA.transform = 1;
            switch (CAMERA.type)
            {
            case Character_Croc:
                CAMERA.mesh = Mesh_CrocIdleRight;
                if (CAMERA.state == CharacterState_HoldsProp)
                    mesh_load(Mesh_CrocArmForward, CAMERA.transform ^ 1, PLAYER.arm);
                break;
            case Character_Doc:
                CAMERA.mesh = Mesh_DocIdleRight;
                break;
            default:
                break;
            }
        }
        else
        {
            CAMERA.velocity.x = Velocity_Run;
        }
        break;
    case Input_JoyUp: // Action
        CAMERA.velocity.y = -Velocity_Run;
        break;
    case Input_JoyDown:
        CAMERA.velocity.y = Velocity_Run;
        break;
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////
//	Rest Functions
//
void __stub(void) { }
