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
#include <cnd/music.h>
#include <cnd/sound.h>
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
// Data
//
const char* stringsEvents[] =
{
    "ENEMY SHUFFLE\x80",
    "    STORMY\x80",
    "  MOON WALK\x80",
    " FLOOR IS LAVA\x80",
    "   PURSUIT\x80",
    "FROM BACK HERE?\x80",
    " TIME ATTACK\x80",
    "     NONE\x80",
    // -- "Disabled" --
};

const char* stringScore[] =
{
    " +1\x80", 
    " +2\x80",
    " +4\x80",
    " +8\x80",
    "+16\x80",
    "+32\x80",
};

const i8 windSequence[] = { 0,1,0,-1 };

/////////////////////////////////////////////////////////////////////////
// Game Functions
//
void game_soft_reset(void) 
{
    Vec_Joy_Mux_1_X = 1; // Only need Left and Right of the Joystick
    Vec_Joy_Mux_1_Y = 0;
    Vec_Joy_Mux_2_X = 0;
    Vec_Joy_Mux_2_Y = 0;

	MEMZERO(GAME);
    GAME.stage   = Stage_Tutorial;
    PLAYER.lives = 3;
    game_prepare_next_stage(GameState_Play);
    GAME.event = Event_Disabled;
}

void game_enter_stage(Stage stage)
{
    GAME.stage = stage;

    sound_clear();
    sound_push_music(g_tracks[stage]);
    world_create(stage);
    PLAYER.isOtherCharacterDead = true;
}

void game_draw_eye()
{
    beam_set_position(WORLD.eyePosition.y, WORLD.eyePosition.x);
    Dot_here();
}

void game_prepare_prepare(void)
{
    WORLD.ticks = 0;
    sound_clear();
    entity_list_clear();
    entity_create_anonymous(Entity_Stub, (v2i){ 0, 0 });
    entity_create_anonymous(Entity_Prepare, (v2i) { 1, 1 });
    GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
    GAME.progress = game_update_prepare;
}

void game_prepare_next_stage(GameState state)
{
    GAME.state = state;
    GAME.progress = game_prepare_prepare; // Delegate initialization to next frame.
}

void game_update_prepare(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x6A;
    game_set_frequencies();

    draw_stack_clear();
    
    if (PREPARE.isAllocated)
        PREPARE.update(&PREPARE);
    
    TEXT_SET_BIG();
    // Visualize hearts
    switch (PLAYER.lives)
    {
    case 1:
        draw_stack_push(heart, HEART_Y, -(HEART_DX >> 1));
        break;
    case 2:
        draw_stack_push(heart, HEART_Y, -(HEART_DX + (HEART_MARGIN >> 1)));
        draw_stack_push(heart, HEART_Y, HEART_MARGIN >> 1);
        break;
    default: // More than 4 hearts
        Print_Str_d(HEART_Y + 10, (HEART_DX << 1) - (HEART_MARGIN >> 1), "++\x80");
        /* fallthrough */
    case 3:
        draw_stack_push(heart, HEART_Y, -((HEART_DX >> 1) + HEART_MARGIN + HEART_DX));
        draw_stack_push(heart, HEART_Y, -(HEART_DX >> 1));
        draw_stack_push(heart, HEART_Y, +((HEART_DX >> 1) + HEART_MARGIN));
        break;
    }

    print_long_unsigned_int(80, -40, PLAYER.score);

    if (GAME.event < Event_Disabled && (WORLD.freq8_8 & 1))
    {
        TEXT_SET_SMALL();
        Print_Str_d(-15, -(TEXT_SMALL_WIDTH >> 1), "EVENT!\x80");
        Print_Str_d(-30, -TEXT_SMALL_WIDTH, (void* const)stringsEvents[GAME.event]);
    }

    switch (GAME.state)
    {
    case GameState_Play:
        if (WORLD.ticks == 100)
        {
            PREPARE.kill(&PREPARE);
            Intensity_7F();
            game_enter_stage(GAME.stage);
            GAME.progress = game_update_play;
        }
        break;
    case GameState_Died:
        if (WORLD.ticks == 50)
        {
            entity_set_pancaked_death(&PREPARE);
            WORLD.ticks = 0;
        }
        if (PREPARE.isAllocated == false)
        {
            GAME.state = GameState_Play;
            entity_create_anonymous(Entity_Prepare, (v2i) { 1, 1 });
            --PLAYER.lives;
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
        GAME.progress          = game_update_gameover;
        WORLD.windPhase        = 0;
        WORLD.wind             = 0;
        world_freeze();
        entity_set_pancaked_death(&CAMERA);
        GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
    }   
    else // Play lost live animation
    {
        game_prepare_next_stage(GameState_Died);
    }
}

void game_update_gameover(void)
{
    dp_VIA_t1_cnt_lo = 0x65;
    game_set_frequencies();
    draw_stack_clear();

    if (CAMERA.isAllocated)
        CAMERA.update(&CAMERA);

    Joy_Analog();
    TEXT_SET_BIG();
    
    beam_set_position(-(Vec_Text_Height << 1), Vec_Joy_1_X >> 1);
    Print_Str_d(0, -60, "GAME OVER\x80");
    if (WORLD.freq16)
    {
        TEXT_SET_SMALL();
        Print_Str_d(-30, -TEXT_SMALL_WIDTH, "HOLD BUTTON 4\x80");
        TEXT_SET_BIG();
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
    ++WORLD.ticks;
    WORLD.freq2      = I8(WORLD.ticks & 1);
    WORLD.freq16     = I8((WORLD.ticks >> 4) & 1);
    WORLD.freq8_8    = I8((WORLD.ticks >> 3) & 7);
}

void game_update_play(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x55;
    game_set_frequencies();

    draw_stack_clear();

    switch (GAME.event)
    {
    case Event_Storm:
    {
        if (WORLD.ticks == 255)
        {
            WORLD.windPhase = (WORLD.windPhase + 1) & 3;
            WORLD.wind = windSequence[WORLD.windPhase];
        }
        else if (WORLD.ticks < 32)
        {
            Intensity_7F();
            draw_stack_push(&wind[1 + WORLD.wind][0], (WORLD.gravity << 5), 0);   
        }
        else
        {
            Intensity_3F();
            i8 shrink = I8(WORLD.ticks >> 2);
            beam_set_position((WORLD.gravity << 5) + (WORLD.gravity << 3), -31 + (shrink >> 1));
            Draw_Line_d(0, 63 - shrink);
        }
    }
        break;
    case Event_MoonWalk:
        Intensity_3F();
        WORLD.moon = ((WORLD.ticks & 3) == 0) ? WORLD.gravity : 0;
        break;
    case Event_TheFloorIsLava:
        if (CAMERA.isGrounded)
        {
            if (++WORLD.heat > 63)
            {
                character_damage();
                WORLD.heat = 0;
            }
            else
            {
                beam_set_position((WORLD.gravity << 5), 0);
                Draw_Line_d((-WORLD.gravity) << 3, 0);
                Moveto_d((-WORLD.gravity) << 1, 0);
                Dot_here();
                Moveto_d((WORLD.gravity << 4), -31 + (WORLD.heat >> 1));
                Draw_Line_d(0, 63 - WORLD.heat);
            }
        }
        else
        {
            WORLD.heat = 0;
        }
        break;
    case Event_YouAreBeingFollowed:
    {
        // Next
        EVENT_ENT.update(&EVENT_ENT);
        // Update entity manually without physics.
        EVENT_ENT.position.y -= EVENT_ENT.velocity.y;
        EVENT_ENT.position.x += EVENT_ENT.velocity.x;
        EVENT_ENT.tile.y = I8((EVENT_ENT.position.y) >> TILE_SCALE_BITS);
        EVENT_ENT.tile.x = I8((EVENT_ENT.position.x) >> TILE_SCALE_BITS);
        EVENT_ENT.tileAbsDelta.y = EVENT_ENT.tile.y - CAMERA.tile.y;
        const i8 dyMask = EVENT_ENT.tileAbsDelta.y >> 7;
        EVENT_ENT.tileAbsDelta.y = (EVENT_ENT.tileAbsDelta.y ^ dyMask) - dyMask;
        EVENT_ENT.tileAbsDelta.x = EVENT_ENT.tile.x - CAMERA.tile.x;
        const i8 dxMask = EVENT_ENT.tileAbsDelta.x >> 7;
        EVENT_ENT.tileAbsDelta.x = ((EVENT_ENT.tileAbsDelta.x ^ dxMask) - dxMask);
        EVENT_ENT.inLocalSpace = EVENT_ENT.tileAbsDelta.x < 4 && EVENT_ENT.tileAbsDelta.y < 4;
    }
        break;
    case Event_Tiny:
        dp_VIA_t1_cnt_lo = 0x25;
        break;
    case Event_TimeAttack:
        if ((WORLD.ticks & 15) == 0)
        {
            if (++WORLD.time == 255)
            {
                character_damage();
                CAMERA.invincibilityTicks = false;
                character_damage();
                WORLD.gameIsOver = true;
            }
        }
        else
        {
            i8 shrink = I8(WORLD.time >> 2);
            beam_set_position((WORLD.gravity << 5), (-63 >> 1) + (shrink >> 1));
            Draw_Line_d(0, 63 - shrink);
        }
        break;
    default:
        break;
    }

    if (!PLAYER.isOtherCharacterDead)
        draw_stack_push(heart, 115, 100);

    world_progress();

    draw_stack_draw();

    if (GAME.ticksScoreGainedVisible > 0)
    {
        TEXT_SET_SMALL();
        beam_set_position(30, -TEXT_SMALL_WIDTH + 15);
        Print_Str((void* const)stringScore[PLAYER.scoreGained]);
        --GAME.ticksScoreGainedVisible;
    }
    game_draw_eye();
}
