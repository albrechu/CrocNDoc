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

const char* events[] =
{
    "NONE\x80",
    "STORM\x80",
    "MOON WALK\x80",
    "FLOOR IS LAVA\x80",
    "ICY ROADS\x80",
    "YOU ARE FOLLOWED\x80",
    "EARTHQUAKE\x80",
    "FROM BACK HERE?\x80",
};

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
    GAME.stage = Stage_Tutorial;
    GAME.event = Event_None;
    PLAYER.lives = 3;
    game_prepare_next_stage(GameState_Play);
    sound_clear();
}

void game_enter_stage(Stage stage)
{
    GAME.stage = stage;

    sound_clear();
    sound_push_music(g_tracks[stage]);
    world_create(stage);
    PLAYER.isOtherCharacterDead = false;
    WORLD.gravity = Velocity_Gravity;
}

force_inline void game_draw_eye()
{
    beam_set_position(WORLD.eyePosition.y, WORLD.eyePosition.x);
    Dot_here();
}

void game_prepare_next_stage(GameState state)
{
    WORLD.ticks = 0;
    sound_stop_music();
    entity_list_clear();
    entity_create_anonymous(Entity_Prepare, (v2i) { 1, 1 });
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
    
    if (CAMERA.isAllocated)
        CAMERA.update(&CAMERA);

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
    default: // More than 3 hearts
        TEXT_SET_BIG();
        Print_Str_d(HEART_Y + 10, (HEART_DX << 1) - (HEART_MARGIN >> 1), "++\x80");
        /* fallthrough */
    case 3:
        draw_stack_push(heart, HEART_Y, -((HEART_DX >> 1) + HEART_MARGIN + HEART_DX));
        draw_stack_push(heart, HEART_Y, -(HEART_DX >> 1));
        draw_stack_push(heart, HEART_Y, +((HEART_DX >> 1) + HEART_MARGIN));
        break;
    }

    TEXT_SET_BIG();
    print_long_unsigned_int(80, -40, PLAYER.score);

    if (GAME.event != Event_None && WORLD.freq8_8 & 1)
    {
        TEXT_SET_SMALL();
        Print_Str_d(-15, -(TEXT_SMALL_WIDTH >> 1), "EVENT!\x80");
        Print_Str_d(-30, -TEXT_SMALL_WIDTH, (void* const)events[GAME.event]);
    }

    switch (GAME.state)
    {
    case GameState_Play:
        if (WORLD.ticks == 100)
        {
            CAMERA.kill(&CAMERA);
            Intensity_7F();
            game_enter_stage(GAME.stage);
            WORLD.windPhase = 0;
            GAME.progress = game_update_play;
        }
        break;
    case GameState_Died:
        if (WORLD.ticks == 100)
        {
            entity_set_death(&CAMERA);
            WORLD.ticks = 0;
        }
        if (WORLD.list.aliveCount == 0)
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
        entity_set_death(&CAMERA);
        GAME.ticksUntilNewGame = GAMEOVER_PRESSED_SPEED;
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

const i8 windSequence[] = { 0,1,0,-1 };

//void explosion_snd(void) 
//{
//    i8 A;
//
//    if (Vec_Expl_Flag & 0x80)
//    {
//        Vec_Expl_Flag &= 0x7F;
//
//        struct tmp_t { u8 bytes[4]; };
//        *((struct tmp_t*)&Vec_Expl_1) = *((struct tmp_t*)GAME.explosion); // Copy 4 bytes
//        Vec_Expl_Chans = ((Vec_Expl_1 >> 3) | Vec_Expl_1) & 7;
//        Vec_Expl_ChanA = Vec_Expl_1 & 0x38;
//        Vec_Expl_ChanB = Vec_Expl_1 & 0x07;
//        Vec_Expl_Chan = 2;
//        A = 0x7F;
//    }
//    else {
//        if (Vec_Expl_Timer == 0) return;
//
//        A = Vec_Expl_Timer - Vec_Expl_4;
//        if (A < 0) {
//            Vec_Expl_Timer = 0;
//            // Skip to channel noise section
//            goto noise_update;
//        }
//    }
//
//    Vec_Expl_Timer = A;
//    A >>= 2;
//
//    // Channel A effect
//    if (Vec_Expl_ChanA) {
//        Vec_Music_Wk_6 = A;
//        if (Vec_Expl_2 < 0) {            // BMI in 6809
//            Vec_Music_Wk_6 = ~A;        // COMB / bit inversion
//        }
//        else if (Vec_Expl_2 > 0) {    // BNE + positive check
//            Vec_Music_Wk_6 = ~A;
//        }
//    }
//
//    // Adjust value for next step
//    A >>= 1;
//    if ((A > 7) && (A != 0x0F)) ++A;
//
//    // Channel B / noise effect
//    if (Vec_Expl_3 != 0) {
//        if (Vec_Expl_3 > 0) {
//            A ^= 0x0F;  // EORA #$0F
//        }
//    }
//
//noise_update:
//    // Noise / channel assignment
//    if (Vec_Expl_ChanB != 0) {
//        while (true) {
//            Vec_Expl_Chan--;
//            if (Vec_Expl_Chan < 0) Vec_Expl_Chan = 2;
//
//            i8 mask = (i8)Bitmask_a((u8)Vec_Expl_Chan);
//            if (mask & Vec_Expl_ChanB) break; // Exit loop if channel is active
//        }
//
//        const i8 chan_index = 2 - Vec_Expl_Chan; // Map channel to index
//        i8 rnd = I8(Random() & 0x0F);
//        if (rnd <= 5) rnd = (rnd << 1) + 5;  // ASLA + ADDA #5
//        (&Vec_Music_Wk_1)[chan_index] = rnd;
//        (&Vec_Music_Wk_1)[chan_index + 1] = (i8)Vec_Random_Seed1;
//    }
//
//    // Mask explosion parameter into music work register
//    Vec_Music_Wk_7 &= ~Vec_Expl_1;
//
//    // Write noise values for active channels
//    i8 chans = Vec_Expl_Chans;
//    i8* ptr = &Vec_Music_Wk_7;
//    while (chans) 
//    {
//        ptr--;          // step backward through registers
//        if (chans & 1) *ptr = A;
//        chans >>= 1;
//    }
//}

void game_update_play(void)
{
    Joy_Digital();
    dp_VIA_t1_cnt_lo = 0x60;
    game_set_frequencies();

    draw_stack_clear();

    switch (GAME.event)
    {
    case Event_None:
        break;
    case Event_Storm:
    {
        if (WORLD.ticks == 255)
        {
            WORLD.windPhase = (WORLD.windPhase + 1) & 3;
            WORLD.wind = windSequence[WORLD.windPhase];
        }
        else
        {
            if ((WORLD.ticks < 96) && (WORLD.ticks > 64))
            {
                Intensity_7F();
            }
            else
            {
                Intensity_3F();
            }
            
            draw_stack_push(&wind[1 + WORLD.wind][0], (WORLD.gravity << 5), 0);
            beam_set_position((WORLD.gravity << 5) + (WORLD.gravity << 3), -15);
            Draw_Line_d(0, 32 - I8(WORLD.ticks >> 3));
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

            beam_set_position((WORLD.gravity << 5), 0);
            Draw_Line_d((-WORLD.gravity) << 3, 0);
            Moveto_d((-WORLD.gravity) << 1, 0);
            Dot_here();
            Moveto_d((WORLD.gravity << 4), -31);
            Draw_Line_d(0, 63 - WORLD.heat);
        }
        else
        {
            WORLD.heat = 0;
        }
        break;
    case Event_IcyRoads:
        break;
    case Event_YouAreBeingFollowed:
        break;
    case Event_EarthQuake:
        break;
    case Event_Tiny:
        dp_VIA_t1_cnt_lo = 0x25;
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
