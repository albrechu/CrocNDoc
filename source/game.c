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
#include <game.h>
// [[Local]]
#include <xutility.h>
#include <plot.h>
// [[Lib]]
#include <vectrex.h>

/////////////////////////////////////////////////////////////////////////
//	Globals
//
game_t g_game;

/////////////////////////////////////////////////////////////////////////
//	Jump Tables
//
static const procedure_t g_render_table[] = 
{
    __stub,
    game_render_play,
    game_render_plot,
};

static const procedure_t g_update_table[] = 
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
    // Assign the initial player state 
    PLAYER.transform  = 1,
    PLAYER.armY = 15;
    PLAYER.mesh = Mesh_CrocIdleRight;
    mesh_load(Mesh_CrocArm, 0, PLAYER.arm);
    GAME.props[0].position.x = 50;
    GAME.props[0].position.y = Player_YMin;
    // Assign Game State
    GAME.state = GameState_Plot;
    GAME.stage = Stage_Sewers;
    stage_load_tile_set(GAME.stage);
    draw_list_refresh(PLAYER.camera);
    plot_set_plot(Plot_WakeUp);
}

void game_start_frame(void)
{
    DP_to_C8();
    dp_VIA_t1_cnt_lo = 1;
    Wait_Recal(); // Synchronize to frame
}

void game_update(void)
{
    g_update_table[GAME.state]();
}


void game_update_play(void)
{
    // Process player input
	action_update();

    // Update bounding area
    PLAYER.camera.x += PLAYER.velocity.x;
    PLAYER.camera.y += PLAYER.velocity.y;

    // Camera Scroll
    if (PLAYER.velocity.x | PLAYER.velocity.y)
    {
        draw_list_refresh(PLAYER.camera);
    }

    v2i tile = tile_calculate(PLAYER.camera);
    if (tile.x != GAME.stageTile.x)
    {
        
    }

    /// Update kinemataics
    // Gravitas
	PLAYER.velocity.y += Velocity_Gravity;
	if (PLAYER.camera.y <= Player_YMin)
	{
		PLAYER.camera.y  = Player_YMin;
		PLAYER.velocity.y  = 0;
		PLAYER.velocity.x += Velocity_Friction * (PLAYER.velocity.x != 0) * PLAYER.transform;
        PLAYER.state &= ~CharacterState_Glide;
	}

    switch (PLAYER.state)
    {
    case CharacterState_Idle:    
        PLAYER.arm[1]  = -10 + PLAYER.velocity.y + PLAYER.transform * (PLAYER.velocity.x >> 2);
        PLAYER.arm[12] = -(PLAYER.arm[2] = PLAYER.velocity.x);

        GAME.props[0].position.x += GAME.props[0].velocity.x;
        GAME.props[0].position.y += GAME.props[0].velocity.y;
        GAME.props[0].velocity.y += Velocity_Gravity;
	    if (GAME.props[0].position.y <= Player_YMin)
	    {
	    	GAME.props[0].position.y  = Player_YMin;
	    	GAME.props[0].velocity.y  = 0;
	    	GAME.props[0].velocity.x += Velocity_Friction * ((GAME.props[0].velocity.x > 0) - (GAME.props[0].velocity.x < 0));
	    }
        break;
    case CharacterState_HoldsProp:
        GAME.props[0].position.x = PLAYER.camera.x + ((PLAYER.transform > 0) * 10 - (PLAYER.transform < 0) * 5);
        GAME.props[0].position.y = PLAYER.camera.y + 5;
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

void game_render(void)
{
    g_render_table[GAME.state]();
}

void game_render_play(void)
{
    // Draw Foreground

    // Draw Player
    switch (PLAYER.character)
    {
    case Character_Croc:
        beam_set_position(-8, -7);
        Draw_VLc(mesh_get(PLAYER.mesh)); // Draw body
        // Moveto_d(PLAYER.armY - 10, (PLAYER.transform == 1) * 7 - 5);
        Moveto_d(PLAYER.armY - 5, (PLAYER.transform) * 7 - 4);
        Draw_VLc(PLAYER.arm);
        break;
    case Character_Doc:
        beam_set_position(-5, 7);
        Draw_VLc(mesh_get(PLAYER.mesh));
        break;    
    default:
        break;
    }

    switch (PLAYER.state)
    {
        break;
    case CharacterState_HoldsProp:
        beam_set_position(-3, 0);
        Draw_VLc(mesh_get(Mesh_Crate + PLAYER.transform));
        break;
    case CharacterState_Glide:
        beam_set_position(3, 3 + 3 * (PLAYER.transform==1));
        Draw_Line_d(10, -PLAYER.transform * ((((i8)GAME.ticks & 1) * -15) | 15)); 
        /* fall through */
    case CharacterState_Idle:
    {
        vec2 propPos;
        propPos.x = GAME.props[0].position.x - PLAYER.camera.x;
        propPos.y = GAME.props[0].position.y - PLAYER.camera.y;
        
        if (VEC_IS_LOCAL(propPos))
        {
            beam_set_position(propPos.y_lsb - 10, propPos.x_lsb - 5);
            Draw_VLc(mesh_get(Mesh_Crate));
        }
    }
    default:
        break;
    }


    //beam_set_position(-10, 40);
    //Draw_VLc(mesh_get(Mesh_Crate));
    //beam_set_position(-10, 20);
    //Draw_VLc(mesh_get(Mesh_Jumper));

    // Draw Background
    draw_list_draw();
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
    switch (PLAYER.state)
    {
    case CharacterState_Glide:
        if (Vec_Btn_State & Input_Button1)
        {
            PLAYER.velocity.y = Velocity_Gravity;
        }
        else
        {
            PLAYER.state = CharacterState_Idle;
        }
        break;
    
    default:
        break;
    }

    switch (BTNS)
    {
    case Input_Button1: // Jump
        if (PLAYER.velocity.y == 0)
        {
            PLAYER.velocity.y += Velocity_Jump;
            PLAYER.velocity.x += PLAYER.transform * Velocity_JumpX;
        }
        else if (PLAYER.character == Character_Doc)
        {
            PLAYER.state = CharacterState_Glide;
            PLAYER.velocity.y = 0;
        }
        break;
    case Input_Button2: // Hit | Grab
    {
        switch (PLAYER.state)
        {
        case CharacterState_Idle: // Grab if one is close enough
        {
            vec2 propPos;
            propPos.x = GAME.props[0].position.x - PLAYER.camera.x;
            propPos.y = GAME.props[0].position.y - PLAYER.camera.y;
            if (VEC_IS_LOCAL(propPos))
            {
                u16 abs = Abs_a_b(propPos.x_lsb, propPos.y_lsb);
                if (((abs & 0xFF) + (abs >> 8)) < 0x15)
                {
                    PLAYER.state = CharacterState_HoldsProp;
                    PLAYER.armY  = 9;
                    mesh_load(Mesh_CrocArmForward, PLAYER.transform ^ 1, PLAYER.arm);
                }
                else
                {
                    PLAYER.velocity.x = PLAYER.transform * Velocity_Bite;
                }
            }
            
        }
            break;
        case CharacterState_HoldsProp: // Throw
            PLAYER.state = CharacterState_Idle;
            GAME.props[0].velocity.x = PLAYER.transform * Velocity_ThrowX;
            GAME.props[0].velocity.y = Velocity_ThrowY;
            mesh_load(Mesh_CrocArm, 0, PLAYER.arm);
            PLAYER.armY = 15;
            if (Vec_Joy_1_Y < 0) // Break
            {

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
        if (PLAYER.velocity.y == 0)
        {
            switch (PLAYER.character)
            {
            case Character_Croc:
                PLAYER.character = Character_Doc;
                PLAYER.mesh = Mesh_DocIdleLeft + (PLAYER.transform >> 7);
                break;
            case Character_Doc:
                PLAYER.character = Character_Croc;
                PLAYER.mesh = Mesh_CrocIdleLeft + (PLAYER.transform >> 7);
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

    PLAYER.hurtboxHeight = Player_HurtboxHeightIdle;
    switch (JOYS)
    {
    case Input_JoyLeftUp:
    case Input_JoyLeftDown:
    case Input_JoyLeft:
        if (PLAYER.transform == 1)
        {
            PLAYER.transform = -1;
            switch (PLAYER.character)
            {
            case Character_Croc:
                PLAYER.mesh = Mesh_CrocIdleLeft;
                if (PLAYER.state == CharacterState_HoldsProp)
                    mesh_load(Mesh_CrocArmForward, PLAYER.transform ^ 1, PLAYER.arm);
                break;
            case Character_Doc:
                PLAYER.mesh = Mesh_DocIdleLeft;
                break;
            default:
                break;
            }
        }
        else
        {
            PLAYER.velocity.x = -Velocity_Run;
        }
        break;
    case Input_JoyRightUp:
    case Input_JoyRightDown:
    case Input_JoyRight:
        if (PLAYER.transform == -1)
        {
            PLAYER.transform = 1;

            switch (PLAYER.character)
            {
            case Character_Croc:
                PLAYER.mesh = Mesh_CrocIdleRight;
                if (PLAYER.state == CharacterState_HoldsProp)
                    mesh_load(Mesh_CrocArmForward, PLAYER.transform ^ 1, PLAYER.arm);
                break;
            case Character_Doc:
                PLAYER.mesh = Mesh_DocIdleRight;
                break;
            default:
                break;
            }
        }
        else
        {
            PLAYER.velocity.x = Velocity_Run;
        }
        break;
    case Input_JoyUp: // Action
        break;
    case Input_JoyDown:
        PLAYER.hurtboxHeight = Player_HurtboxHeightDucking;
        break;
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////
//	Input Functions
//
force_inline void input_fetch(void)
{
    Joy_Digital();
	Intensity_7F(); // Joy_Digital disables the intensity for ✨✨MAGICAL✨✨ reasons when second controller is enabled.
	Read_Btns();    // Read buttons of controller

    BTNS = Vec_Buttons;
    v2i* j = ((v2i*)0xC81B);
    JOYS = ((((u8)(j->x < 0)) << Input_JoyLeftBit) | 
		   (((u8)(j->x > 0))  << Input_JoyRightBit) | 
		   (((u8)(j->y < 0))  << Input_JoyUpBit)    | 
		   (((u8)(j->y > 0))  << Input_JoyDownBit));
}

/////////////////////////////////////////////////////////////////////////
//	Rest Functions
//
void __stub(void) { }
