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
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/draw_queue.h>
#include <cnd/world.h>
// [[Lib]]
#include <vectrex.h>
#include <lib/assert.h>
#include <lib/print.h>
#include <lib/monitor.h>

#define GAMEOVER_JOYSPEED      3
#define GAMEOVER_JOYDIM        50
#define GAMEOVER_PRESSED_SPEED 60

/////////////////////////////////////////////////////////////////////////
// Game Functions
//
void game_soft_reset(void) 
{
    Vec_Joy_Mux_1_X = 1; // Only need Left and Right of the Joystick
    Vec_Joy_Mux_1_Y = 0;
    Vec_Joy_Mux_2_X = 0;
    Vec_Joy_Mux_2_Y = 0;
    Vec_Text_Height = -20;
    Vec_Text_Width  = 20;

	MEMZERO(GAME);
    GAME.stage = Stage_JumpWorld;
    PLAYER.lives = 3;
    //game_remove_live();
    prefab_croc_prepare(&WORLD.entities[1]);
    GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
    GAME.progress          = game_update_prepare;
    GAME.state             = GameState_Play;
    GAME.track             = &musicOff;
}

void game_next_attempt(void)
{
    Intensity_7F();
    game_enter_stage(GAME.stage);
    GAME.progress = game_update_play;
    //plot_init();
    //plot_set_plot(Plot_Tutorial);
}

void game_enter_stage(Stage stage)
{
    GAME.stage = stage;
    switch (stage)
    {
    case Stage_Tutorial:
        GAME.track = &g_corneria;
        break;
    case Stage_JumpWorld:
        GAME.track = &g_crocodileCacophony;
        break;
    case Stage_Water:
        GAME.track = &g_champion;
        break;
    case Stage_Gravitas:
        GAME.track = &g_corneria;
        break;
    default:
        break;
    }
    Stop_Sound();
    Vec_Music_Flag = 0;
    Clear_Sound();
    world_create(stage);
    PLAYER.isOtherCharacterDead = true;
    // Install callbacks
    WORLD.gravity = Velocity_Gravity;
    prefab_croc(&CAMERA);
    
}

void game_draw_eye()
{
    beam_set_position(WORLD.eyePosition.y, WORLD.eyePosition.x);
    Dot_here();
}

#define HEART_Y      -60
#define HEART_DX     12
#define HEART_MARGIN 12

void game_visualize_hearts(void)
{
    switch (PLAYER.lives)
    {
    case 1:
        draw_queue_push(heart, HEART_Y, -(HEART_DX >> 1));
        break;
    case 2:
        draw_queue_push(heart, HEART_Y, -(HEART_DX + (HEART_MARGIN >> 1)));
        draw_queue_push(heart, HEART_Y, HEART_MARGIN >> 1);
        break;
    case 3:
        draw_queue_push(heart, HEART_Y, -((HEART_DX >> 1) + HEART_MARGIN + HEART_DX));
        draw_queue_push(heart, HEART_Y, -(HEART_DX >> 1));
        draw_queue_push(heart, HEART_Y, +((HEART_DX >> 1) + HEART_MARGIN));
        break;
    default:
        break;
    }
}

void game_update_prepare(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x7A;
    game_set_frequencies();

    draw_stack_clear();
    
    WORLD.entities[1].update(&WORLD.entities[1]);
    game_visualize_hearts();
    

    Vec_Text_Height = -15;
    Vec_Text_Width  = 80;
    print_long_unsigned_int(80, -40, PLAYER.score);

    switch (GAME.state)
    {
    case GameState_InGame:
        game_next_attempt();
        break;
    case GameState_DeathAnimation:
        if (--GAME.ticksUntilNewGame == 0)
        {
            --PLAYER.lives;
            GAME.state = GameState_Play;
            prefab_croc_prepare(&WORLD.entities[1]);
        }
        break;
    default:
        break;
    }
    draw_stack_draw();
    game_draw_eye();
}

void game_remove_live(void)
{
    if (PLAYER.lives <= 1)
    {
        GAME.progress = game_update_gameover;
        WORLD.ticks = 7;
        CAMERA.transform = 1;
        GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
        //Vec_Joy_Mux_1_Y  = 3;
        world_freeze();
    }   
    else // Play lost live animation
    {
        WORLD.ticks = 0;
        CAMERA.position.x = 0;
        CAMERA.position.y = 0;
        GAME.track = &musicOff;
        prefab_croc_prepare(&WORLD.entities[1]);
        GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
        GAME.progress          = game_update_prepare;
        GAME.state             = GameState_Died;
    }
}

void game_update_gameover(void)
{
    game_set_frequencies();
    draw_stack_clear();

    Joy_Analog();
    Vec_Text_Height = -16;
    Vec_Text_Width  = 80;
    
    beam_set_position(-(Vec_Text_Height << 1), Vec_Joy_1_X >> 1);
    Print_Str_d(0, -60, "GAME OVER\x80");
    print_long_unsigned_int(100, -40, PLAYER.score);

    Vec_Text_Height >>= 1;
    Vec_Text_Width  >>= 1;
    if (Vec_Btn_State)
    {
        if (--GAME.ticksUntilNewGame == 0)
        {
            GAME.isFinished = true;
            Vec_Joy_Mux_1_Y = 0;
        }
        beam_set_position(-100, -(GAMEOVER_PRESSED_SPEED));
        Draw_Line_d(0, (GAMEOVER_PRESSED_SPEED << 1) - (GAME.ticksUntilNewGame << 1));
        Draw_VLc((void* const)doc_glide_right[WORLD.freq16]);
    }
    else
    {
        GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
    }

    world_progress();

    draw_stack_draw();
}

void game_set_frequencies(void)
{
    const u16 ticks = ++WORLD.ticks;
    WORLD.freq2     = I8(ticks & 1);
    WORLD.freq16    = I8((ticks >> 4) & 1);
    WORLD.freq8_8   = I8((ticks >> 3) & 7);
}

void game_update_play(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x60;
    game_set_frequencies();

    draw_stack_clear();

    if (!PLAYER.isOtherCharacterDead)
        draw_queue_push(heart, 115, 100);

    world_progress();

    draw_stack_draw();
    game_draw_eye();
}

void game_update_plot(void)
{
    game_set_frequencies();
    if (BTNS & Input_Button1)
    {
        if (plot_skip())
        {
            GAME.progress = game_update_play;
            return;
        }
    }
    plot_typewriter_next();
}
