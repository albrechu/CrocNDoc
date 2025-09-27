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

/////////////////////////////////////////////////////////////////////////
// Defines
//
#define GAMEOVER_PRESSED_SPEED 60
#define HEART_Y               -60
#define HEART_DX               12
#define HEART_MARGIN           12

/////////////////////////////////////////////////////////////////////////
// Game Functions
//
void game_soft_reset(void) 
{
    Vec_Joy_Mux_1_X = 1; // Only need Left and Right of the Joystick
    Vec_Joy_Mux_1_Y = 0;
    Vec_Joy_Mux_2_X = 0;
    Vec_Joy_Mux_2_Y = 0;
    Vec_Text_Height = TEXT_BIG_HEIGHT;
    Vec_Text_Width  = TEXT_BIG_WIDTH;

	MEMZERO(GAME);
    GAME.stage = Stage_Tutorial;
    PLAYER.lives = 3;
    
    prefab_croc_prepare(&WORLD.list.entities[1]);
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
}

void game_enter_stage(Stage stage)
{
    GAME.stage = stage;
    
    switch (stage)
    {
    case Stage_Tutorial:
        GAME.track = &g_corneria;
        break;
    case Stage_Sewers:
        GAME.track = &g_crocodileCacophony;
        break;
    case Stage_Water:
        GAME.track = &g_champion;
        break;
    case Stage_Gravitas:
        GAME.track = &g_day;
        break;
    default:
        break;
    }
    Stop_Sound();
    Vec_Music_Flag = 0;
    Clear_Sound();
    world_create(stage);
    PLAYER.isOtherCharacterDead = true;
    WORLD.gravity = Velocity_Gravity;
}

force_inline void game_draw_eye()
{
    beam_set_position(WORLD.eyePosition.y, WORLD.eyePosition.x);
    Dot_here();
}

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

void game_prepare_next_stage(GameState state)
{
    WORLD.ticks = 0;
    CAMERA.position.x = 0;
    CAMERA.position.y = 0;
    GAME.track = &musicOff;
    prefab_croc_prepare(&WORLD.list.entities[1]);
    GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
    GAME.progress = game_update_prepare;
    GAME.state = state;
}

void game_update_prepare(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x7A;
    game_set_frequencies();

    draw_stack_clear();
    
    WORLD.list.entities[1].update(&WORLD.list.entities[1]);
    game_visualize_hearts();

    Vec_Text_Height = TEXT_BIG_HEIGHT;
    Vec_Text_Width  = TEXT_BIG_WIDTH;
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
            prefab_croc_prepare(&WORLD.list.entities[1]);
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
        world_freeze();
    }   
    else // Play lost live animation
    {
        game_prepare_next_stage(GameState_Died);
    }
}

void game_update_gameover(void)
{
    game_set_frequencies();
    draw_stack_clear();

    Joy_Analog();
    Vec_Text_Height = TEXT_BIG_HEIGHT;
    Vec_Text_Width = TEXT_BIG_WIDTH;
    
    beam_set_position(-(Vec_Text_Height << 1), Vec_Joy_1_X >> 1);
    Print_Str_d(0, -60, "GAME OVER\x80");
    if (WORLD.freq16)
    {
        Vec_Text_Height = TEXT_SMALL_HEIGHT;
        Vec_Text_Width  = TEXT_SMALL_WIDTH;
        Print_Str_d(-30, -TEXT_SMALL_WIDTH, "HOLD BUTTON 4\x80");
        Vec_Text_Height = TEXT_BIG_HEIGHT;
        Vec_Text_Width  = TEXT_BIG_WIDTH;
    }
    print_long_unsigned_int(100, -40, PLAYER.score);

    if (Vec_Btn_State)
    {
        if (--GAME.ticksUntilNewGame == 0)
        {
            GAME.isFinished = true;
            Vec_Joy_Mux_1_Y = 0;
            Stop_Sound();
            Vec_Music_Flag = 0;
            Clear_Sound();
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

force_inline void game_set_frequencies(void)
{
    const u8 ticks  = ++WORLD.ticks;
    WORLD.freq2      = I8(ticks & 1);
    WORLD.freq16     = I8((ticks >> 4) & 1);
    WORLD.freq8_8    = I8((ticks >> 3) & 7);
}

void game_update_play(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x50;
    game_set_frequencies();

    draw_stack_clear();

    if (!PLAYER.isOtherCharacterDead)
        draw_queue_push(heart, 115, 100);

    world_progress();

    draw_stack_draw();
    if (GAME.ticksScoreGainedVisible > 0)
    {
        Vec_Text_Height = TEXT_SMALL_HEIGHT;
        Vec_Text_Width  = TEXT_SMALL_WIDTH;
        beam_set_position(115, -10);
        switch (PLAYER.scoreGained)
        {
        case Score_50:  Print_Str("+1\x80"); break;
        case Score_100: Print_Str("+2\x80"); break;
        case Score_200: Print_Str("+4\x80"); break;
        case Score_500: Print_Str("+10\x80"); break;
        default: break;
        }
        --GAME.ticksScoreGainedVisible;
    }
    game_draw_eye();
}
