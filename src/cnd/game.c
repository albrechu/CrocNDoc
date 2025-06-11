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
void game_create(void)
{
	Vec_Joy_Mux_1_X = 1; // Only need Left and Right of the Joystick
	Vec_Joy_Mux_1_Y = 0;
	Vec_Joy_Mux_2_X = 0;
	Vec_Joy_Mux_2_Y = 0;
    Vec_Text_Height = -20;
    Vec_Text_Width = 20;
    g_stage = Stage_Tutorial;
}

void game_soft_reset(void) 
{
    MEMZERO(PLAYER);
	MEMZERO(GAME);
    Intensity_7F();
    game_enter_stage(g_stage);
    plot_init();
    GAME.progress = game_update_play;
    // Assign the initial player state 
    // Assign Game State
    
    //plot_set_plot(Plot_Tutorial);
}

void game_enter_stage(Stage stage)
{
    g_stage = stage;
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
    WORLD.entityAdded        = game_entity_added;
    WORLD.stageEntered       = game_enter_stage;
    WORLD.playerDamage       = routine_player_damage;
    WORLD.playerChangedFluid = routine_player_changed_fluid;
    WORLD.gravity = 1;
    CAMERA.mesh      = croc_idle_right;
    CAMERA.routine   = routine_croc_air;
    CAMERA.isLocal   = true;
    CAMERA.state     = CharacterState_Idle;
    CAMERA.transform = 1;
}

void game_set_gameover(void)
{
    GAME.progress = game_update_gameover;
    WORLD.ticks = 7;
    CAMERA.transform = 1;
    CAMERA.stopwatch = GAMEOVER_PRESSED_SPEED;
    GAME.finTxtPos = (v2i){ 0, 0 };
    Vec_Joy_Mux_1_Y = 3;
}

void game_update_gameover(void)
{

    Joy_Analog();
    Vec_Text_Height = -20;
    Vec_Text_Width  = 70;
    dp_VIA_t1_cnt_lo = 0x80;
    
    GAME.finTxtPos.x = Vec_Joy_1_X >> 1;
    GAME.finTxtPos.y = Vec_Joy_1_Y >> 1;
    beam_set_position(GAME.finTxtPos.y, GAME.finTxtPos.x);
    //Print_Str_d(-Vec_Text_Height, -60, "GAME OVER\x80");
    Draw_VLc((void* const)game_over);
    Vec_Text_Height >>= 1;
    Vec_Text_Width  >>= 1;
    if (Vec_Btn_State)
    {
        if (--CAMERA.stopwatch == 0)
        {
            GAME.isFinished = true;
            Vec_Joy_Mux_1_Y = 0;
        }
        beam_set_position(-100, -(GAMEOVER_PRESSED_SPEED));
        Draw_Line_d(0, (GAMEOVER_PRESSED_SPEED << 1) - (CAMERA.stopwatch << 1));
    }
    else
    {
        CAMERA.stopwatch = GAMEOVER_PRESSED_SPEED;
    }

    world_progress();
}

void game_update_play(void)
{
    Joy_Digital();
    /*for (i8 i = 0; i < WORLD.entityCount; ++i)
    {
        WORLD.entities[i].routine(&WORLD.entities[i]);
    }*/
    dp_VIA_t1_cnt_lo = 0x80;
#if 1
    WORLD.entities[0].routine(&WORLD.entities[0]);
    // if (WORLD.freq2)
    {
        WORLD.entities[1].routine(&WORLD.entities[1]);
        WORLD.entities[2].routine(&WORLD.entities[2]);
        WORLD.entities[3].routine(&WORLD.entities[3]);
        /*WORLD.entities[4].routine(&WORLD.entities[4]);
        WORLD.entities[5].routine(&WORLD.entities[5]);*/
        //WORLD.entities[6].routine(&WORLD.entities[6]);
        //WORLD.entities[7].routine(&WORLD.entities[7]);
    }
#endif
    

    if (!PLAYER.isOtherCharacterDead)
    {
        beam_set_position(115, 100);
        Draw_VLc((void* const)heart);
    }

    const u16 ticks     = ++WORLD.ticks;
    WORLD.freq2   = I8(ticks & 1);
    WORLD.freq16  = I8((ticks >> 4) & 1);
    WORLD.freq8_8 = I8((ticks >> 3) & 7);

#define COLLISION(i) WORLD.entities[i].collision(&WORLD.entities[i])
    COLLISION(0);
    COLLISION(1);
    COLLISION(2);
    COLLISION(3);

    world_progress();
}

void game_update_plot(void)
{
    if (BTNS & Input_Button1)
    {
        if (plot_skip())
        {
            GAME.progress = game_update_play;
            return;
        }
    }
    plot_typewriter_next();
    WORLD.ticks++;
}

void game_entity_added(entity e)
{
    switch (e->type)
    {
    case Prop_Crate:
        e->mesh = crate;
        goto basic_prop;
    case Prop_Barrel:
        e->routine = routine_barrel_idle;
        //e->mesh    = barrel;
        goto basic_prop;
    case Enemy_Tunichtgut: // Peng Peng!
        e->routine = routine_tunichtgut_boom;
        goto basic_enemy;
    case Enemy_Halunke: // Follow
        //e->mesh    = halunke;
        e->routine = routine_halunke_follow;
        goto basic_enemy;
    case Enemy_Gauner: // Jumper + Shoot
        e->routine = routine_gauner_watching;
        goto basic_enemy;
    case Enemy_Schuft: // Fliegeviech
        e->routine = routine_schuft_follow; 
        goto basic_enemy;
    case Enemy_Schelm: // Be careful were you go
        schelm_create_prefab(e);
        goto basic_enemy;
    case Enemy_Bandit:
        e->routine = routine_bandit_stolen;
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