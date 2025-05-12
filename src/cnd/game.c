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

#define GAMEOVER_JOYSPEED      3
#define GAMEOVER_JOYDIM        50
#define GAMEOVER_PRESSED_SPEED 60

/////////////////////////////////////////////////////////////////////////
//	Globals
//
game_t g_game;

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
	Vec_Text_Height = 11;
	Vec_Text_Width  = 45;
}

void game_soft_reset(void) 
{
	MEMZERO(GAME);
    game_enter_stage(Stage_Sewers);
    // world_load_stage(Stage_Sewers);
    plot_init();
    GAME.progress = game_update_play;
    // Assign the initial player state 
    // Assign Game State
    
    // plot_set_plot(Plot_WakeUp);
}

void game_enter_stage(Stage stage)
{
    world_create(stage);
    // Install callbacks
    WORLD.entityAdded            = game_entity_added;
    WORLD.stageEntered           = game_enter_stage;
    WORLD.playerDamage           = routine_player_damage;
    WORLD.playerChangedSubstance = routine_player_changed_substance;

    CAMERA.mesh      = croc_idle_right;
    CAMERA.routine   = routine_croc_air;
    CAMERA.isLocal   = true;
    CAMERA.state     = CharacterState_Idle;
    CAMERA.transform = 1;
}

void game_set_gameover(void)
{
    GAME.progress    = game_update_gameover;
    GAME.ticks = 7;
    CAMERA.transform = 1;
    CAMERA.stopwatch = GAMEOVER_PRESSED_SPEED;
    GAME.data[0] = 0;
    GAME.data[1] = 0;
    GAME.data[2] = 0;
    GAME.data[3] = 0;
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
        PLAYER.isOtherCharacterDead = false; // Gets the other character back
        world_entity_set_status(e, -1, EntityStatus_Dead);
    }
}

void routine_player_changed_substance(void)
{

}

void routine_player_damage(void)
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

void routine_barrel_thrown(entity e)
{
    if (e->velocity.x == 0)
    {
        e->stopwatch = 10;
        e->routine   = routine_barrel_break;
    }
}

void routine_barrel_idle(entity e)
{
    if (e->isLocal)
    {
        i8 dy = I8(CAMERA.position.y - e->position.y);
        i8 dx = I8(e->position.x - CAMERA.position.x);
        beam_set_position(dy, dx);
        Draw_VLc((void* const)barrel);
    }
}

void routine_barrel_held(entity e)
{
    (void)e;
    beam_set_position(0, 0);
    Draw_VLc((void* const)(CAMERA.transform == 1 ? barrel_right : barrel_left));
}

void routine_death0(entity e)
{
    if (--e->stopwatch != 0)
    {
        e->velocity.y = 0; 
        e->velocity.x = 0;
    }
    else // Kill it
    {
        PLAYER.isOtherCharacterDead = false; // Gets the other character back
        world_entity_set_status(e, -1, EntityStatus_Dead);
    }
}

void routine_halunke_follow(entity e)
{
    e->velocity.x = 1 | I8((CAMERA.position.x - e->position.x) >> 15);
    e->transform = e->velocity.x;
    if (e->isLocal)
    {
        i16 dy = CAMERA.position.y - e->position.y;
        i16 dx = e->position.x - CAMERA.position.x;
        if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            i8 localDx = (i8)dx;
            i8 localDy = (i8)dy;
            beam_set_position(localDy, localDx);
            Draw_VLc((void* const)halunke);
            if (manhattan(localDy, localDx) < 0x15)
            {
                localDy = (i8)Abs_b(localDy);
                i8 localDxAbs = (i8)Abs_b(localDx);
                if (localDy > localDxAbs)
                {
                    e->stopwatch = 10;
                    e->routine   = routine_death0;
                    CAMERA.velocity.y += Velocity_Jump;
                }
                else if(CAMERA.stopwatch != 0 && (localDx ^ CAMERA.velocity.x) >= 0)
                {
    
                }
                else
                {
                    WORLD.playerDamage();
                }
            }
        }
    }
}

void routine_gauner_watching(entity e)
{
    if (e->isGrounded && (GAME.ticks & 0x3F) == 0)
    {
        e->velocity.y += Velocity_Jump;
    }
    if ((GAME.ticks & 0x3F) == 0)
    {
        // Spawn projectile
    }
}

void routine_schuft_follow(entity e)
{
    const i16 dy = CAMERA.position.y - e->position.y;
    const i16 dx = e->position.x - CAMERA.position.x;
    e->velocity.x += ((dx > 0) - (dx < 0));
    e->velocity.y += ((dy > 0) - (dy < 0));
    // if (e->isLocal)
    {
        // if (dy >= -128 && dy <= 127 && dx >= -128 && dx <= 127)
        {
            const i8 localY = I8(dy);
            const i8 localX = I8(dx);
            
            beam_set_position(localY, localX);
            Draw_VLc((void* const)(e->velocity.x < 0 ? schuft_left : schuft_right));
            //if (manhattan(localY, localX) < 0x15)
            {
                // if (Abs_b(localY) > Abs_b(localX))
                // {
                //     e->stopwatch = 10;
                //     e->routine   = routine_death0;
                //     CAMERA.velocity.y += Velocity_Jump;
                // }
                // else
                // {
                //     WORLD.playerDamage();
                // }
            }
        }
    }
}

void game_update_gameover(void)
{
    Vec_Text_Height = -20;
    Vec_Text_Width  = 70;
    // u8 tick = (GAME.ticks >> 2) & 0xF;

    switch (JOYS)
    {
    case Input_JoyLeft:
        GAME.data[1] -= GAMEOVER_JOYSPEED;
        break;
    case Input_JoyRight:
        GAME.data[1] += GAMEOVER_JOYSPEED;
        break;
    case Input_JoyUp:
        GAME.data[0] -= GAMEOVER_JOYSPEED;
        break;
    case Input_JoyDown:
        GAME.data[0] += GAMEOVER_JOYSPEED;
        break;
    case Input_JoyLeftDown:
        GAME.data[0] += GAMEOVER_JOYSPEED;
        GAME.data[1] -= GAMEOVER_JOYSPEED;
        break;
    case Input_JoyRightDown:
        GAME.data[0] += GAMEOVER_JOYSPEED;
        GAME.data[1] += GAMEOVER_JOYSPEED;
        break;
    case Input_JoyLeftUp:
        GAME.data[0] -= GAMEOVER_JOYSPEED;
        GAME.data[1] -= GAMEOVER_JOYSPEED;
        break;
    case Input_JoyRightUp:
        GAME.data[0] -= GAMEOVER_JOYSPEED;
        GAME.data[1] += GAMEOVER_JOYSPEED;
        break;
    default:
        GAME.data[0] = 0;
        GAME.data[1] = 0;
        break;
    }
    GAME.data[0] = MIN8(GAMEOVER_JOYDIM, GAME.data[0]);
    GAME.data[0] = MAX8(-GAMEOVER_JOYDIM, GAME.data[0]);
    GAME.data[1] = MIN8(GAMEOVER_JOYDIM, GAME.data[1]);
    GAME.data[1] = MAX8(-GAMEOVER_JOYDIM, GAME.data[1]);
    beam_set_position(GAME.data[0], GAME.data[1]);
    Print_Str_d(-Vec_Text_Height, -60, "GAME OVER\x80");
    Vec_Text_Height >>= 1;
    Vec_Text_Width  >>= 1;
    beam_set_position(GAME.data[0], GAME.data[1]);
    Print_Str_d((Vec_Text_Height * 3), -30, "HOLD ANY\x80");
    if (Vec_Btn_State)
    {
        if (--CAMERA.stopwatch == 0)
        {
            GAME.isFinished = true;
        }
        beam_set_position(-100, -(GAMEOVER_PRESSED_SPEED));
        Draw_Line_d(0, (GAMEOVER_PRESSED_SPEED << 1) - (CAMERA.stopwatch << 1));
    }
    else
    {
        CAMERA.stopwatch = GAMEOVER_PRESSED_SPEED;
    }

    world_progress();
    ++GAME.ticks;
}

void game_update_play(void)
{
    WORLD.entities[0].routine(&WORLD.entities[0]);
    if (WORLD.ticks & 1)
    {
        WORLD.entities[1].routine(&WORLD.entities[1]);
        WORLD.entities[2].routine(&WORLD.entities[2]);
        WORLD.entities[3].routine(&WORLD.entities[3]);
        WORLD.entities[4].routine(&WORLD.entities[4]);
        WORLD.entities[5].routine(&WORLD.entities[5]);
        WORLD.entities[6].routine(&WORLD.entities[6]);
        WORLD.entities[7].routine(&WORLD.entities[7]);
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
            GAME.progress = game_update_play;
            return;
        }
    }
    plot_typewriter_next(GAME.ticks++);
}

void routine_doc_glide(entity e)
{
    switch (JOYS)
    {
    case Input_JoyLeftUp:
    case Input_JoyLeftDown:
    case Input_JoyLeft:
        if (e->transform == 1)
        {
            e->transform  = -1;
            e->mesh       = e->type == Character_Croc ? croc_idle_left : doc_body_left;
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        break;
    case Input_JoyRightUp:
    case Input_JoyRightDown:
    case Input_JoyRight:
        if (e->transform == -1)
        {
            e->transform  = 1;
            e->mesh       = e->type == Character_Croc ? croc_idle_right : doc_body_right;
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        break;
    default:
        break;
    }

    if (Vec_Btn_State & Input_Button1)
    {
        e->velocity.y = 0;
    }
    else
    {
        e->routine = routine_doc_air;
    }

    beam_set_position(0, 0);
    Mov_Draw_VLc_a((void* const)CAMERA.mesh);

    beam_set_position(10, -1 * e->transform);
    Draw_Line_d(10, -CAMERA.transform * ((((i8)GAME.ticks & 1) * -15) | 15)); 
}

force_inline void routine_player_air(entity e)
{
    switch (BTNS)
    {
    case Input_Button1: // Jump
        if (e->isGrounded)
        {
            e->velocity.y += Velocity_Jump;
            e->velocity.x += CAMERA.transform * Velocity_JumpX;
        }
        else if (e->type == Character_Doc)
        {
            e->routine = routine_doc_glide;
            e->velocity.y = 0;
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
                        if (pi != 0) // Game needs to ensure that a hold prop is always at idx 0
                        {
                            idx_t tmpIdx = WORLD.entityIdxs[1];
                            WORLD.entityIdxs[1] = WORLD.entityIdxs[pi];
                            WORLD.entityIdxs[pi] = tmpIdx;    
                        }
                        return;
                    }
                }
            }
            if (e->isGrounded && e->stopwatch <= 0)
            {
                e->velocity.x = e->transform * Velocity_Bite;
                e->stopwatch = 14;
            }
        }
            break;
        case CharacterState_HoldsProp: // Throw
        {
            e->state = CharacterState_Idle;
            entity e = &WORLD.entities[WORLD.entityIdxs[0]];
            e->state   = PropState_Thrown;
            e->routine = routine_barrel_thrown;
            e->transform = e->transform;
            if (JOYS == Input_JoyDown)
            {
                e->velocity.y = -Velocity_ThrowY;
            }
            else
            {
                e->velocity.x = e->transform * Velocity_ThrowX;
                e->velocity.y = Velocity_ThrowY;
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case Input_Button4: // Swap
        if (e->velocity.y == 0)
        {
            switch (e->type)
            {
            case Character_Croc:
                e->type = Character_Doc;
                e->mesh = e->transform == 1 ? doc_body_right : doc_body_left;
                e->routine = routine_doc_air;
                break;
            case Character_Doc:
                e->type = Character_Croc;
                e->mesh = e->transform == 1 ? croc_idle_right : croc_idle_left;
                e->routine = routine_croc_air;
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

    switch (JOYS)
    {
    case Input_JoyLeftUp:
    case Input_JoyLeftDown:
    case Input_JoyLeft:
        if (e->transform == 1)
        {
            e->transform  = -1;
            e->mesh       = e->type == Character_Croc ? croc_idle_left : doc_body_left;
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        break;
    case Input_JoyRightUp:
    case Input_JoyRightDown:
    case Input_JoyRight:
        if (e->transform == -1)
        {
            e->transform  = 1;
            e->mesh       = e->type == Character_Croc ? croc_idle_right : doc_body_right;
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        break;
    default:
        break;
    }
}

void routine_doc_air(entity e)
{
    routine_player_air(e);
    beam_set_position(0, 0);
    Mov_Draw_VLc_a((void* const)CAMERA.mesh);
}

void routine_croc_air(entity e)
{
    routine_player_air(e);
    beam_set_position(0, 0);
    Mov_Draw_VLc_a((void* const)CAMERA.mesh); // Draw body
    if (e->stopwatch != 0)
    {
        --e->stopwatch;
        beam_set_position(10, 14 * e->transform - e->stopwatch * (e->transform << 1));
        Draw_VLc((void* const)cloud[e->stopwatch & 0x7]);
    }
    else
    {
        Mov_Draw_VLc_a((void* const)croc_arm[-(CAMERA.transform >> 7)]); 
    }
}



/////////////////////////////////////////////////////////////////////////
//	Rest Functions
//
void __stub(void) { }

void game_entity_added(entity e)
{
    switch (e->type)
    {
    case Prop_Crate:
        e->mesh = crate;
        goto basic_prop;
    case Prop_Barrel:
        e->routine = routine_barrel_idle;
        e->mesh = barrel;
        goto basic_prop;
    case Enemy_Tunichtgut:
        // e->routine = 
        // Print_Str("HELLO\x80");
        goto basic_enemy;
    case Enemy_Halunke: // Follow
        e->mesh    = halunke;
        e->routine = routine_halunke_follow;
        goto basic_enemy;
    case Enemy_Gauner:
        e->routine = routine_gauner_watching;
        goto basic_enemy;
    case Enemy_Schuft: // Fliegeviech
        e->routine = routine_schuft_follow; 
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