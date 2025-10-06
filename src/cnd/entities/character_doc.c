/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>
#include <cnd/draw_queue.h>
#include <cnd/music.h>
#include <cnd/sound.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void doc_draw_default(entity e)
{
    if (e->velocity.x)
    {
        i8 ad = WORLD.freq16 << 1;
        if (e->transform < 0)
        {
            draw_stack_push(doc_idle_left[WORLD.freq16], WORLD.freq16, -3 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 12 - WORLD.freq16, (-6 + (Hitbox_DocX >> 1)) - ad };
        }
        else
        {
            draw_stack_push(doc_idle_right[WORLD.freq16], WORLD.freq16, 3 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 12 - WORLD.freq16, (6 - (Hitbox_DocX >> 1)) + ad };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(doc_idle_left[0], 0, -3 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 12, -6 + (Hitbox_DocX >> 1) };
        }
        else
        {
            draw_stack_push(doc_idle_right[0], 0, 3 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 12, 6 - (Hitbox_DocX >> 1) };
        }
    }
}

void doc_draw_mirrored(entity e)
{
    if (e->velocity.x)
    {
        i8 ad = (WORLD.freq16 << 1);
        if (e->transform < 0)
        {
            draw_stack_push(doc_idle_left_r[WORLD.freq16], -WORLD.freq16, -3 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -12 + WORLD.freq16, (-6 + (Hitbox_DocX >> 1)) - ad };
        }
        else
        {
            draw_stack_push(doc_idle_right_r[WORLD.freq16], -WORLD.freq16, 3 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -12 + WORLD.freq16, (6 - (Hitbox_DocX >> 1)) + ad };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(doc_idle_left_r[0], 0, -3 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -12, -6 + (Hitbox_DocX >> 1) };
        }
        else
        {
            draw_stack_push(doc_idle_right_r[0], 0, 3 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -12, 6 - (Hitbox_DocX >> 1) };
        }
    }
}


void doc_draw_glide(entity e)
{
    static bool freqLast = false;
    if (e->velocity.x)
    {
        if (freqLast != WORLD.freq16)
        {
            sound_push_sfx(&g_fly);
            freqLast = WORLD.freq16;
        }
        if (e->transform < 0)
        {
            draw_stack_push(doc_glide_left[WORLD.freq16], -2, -1 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 9, -9 + (Hitbox_DocX >> 1) };
        }
        else
        {
            draw_stack_push(doc_glide_right[WORLD.freq16], -2, 1 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 9, 9 - (Hitbox_DocX >> 1) };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(doc_glide_left[0], -2, -1 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 9, -9 + (Hitbox_DocX >> 1) };
        }
        else
        {
            draw_stack_push(doc_glide_right[0], -2, 1 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ 9, 9 - (Hitbox_DocX >> 1) };
        }
    }
}

void doc_draw_glide_mirrored(entity e)
{
    if (e->velocity.x)
    {
        if (e->transform < 0)
        {
            draw_stack_push(doc_glide_left_r[WORLD.freq16], 2, -1 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -9, -9 + (Hitbox_DocX >> 1) };
        }
        else
        {
            draw_stack_push(doc_glide_right_r[WORLD.freq16], 2, 1 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -9, 9 - (Hitbox_DocX >> 1) };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(doc_glide_left_r[0], 2, -1 + (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -9, -9 + (Hitbox_DocX >> 1) };
        }
        else
        {
            draw_stack_push(doc_glide_right_r[0], 2, 1 - (Hitbox_DocX >> 1));
            WORLD.eyePosition = (v2i){ -9, 9 - (Hitbox_DocX >> 1) };
        }
    }
}

void doc_action(entity e)
{
    switch (e->state)
    {
    case CharacterState_Idle: // Grab if one is close enough
        character_grab();
        break;
    case CharacterState_HoldsProp: // Throw
        character_throw();
        break;
    default:
        break;
    }
}



void update_doc_air(entity e)
{
    character_off_ground_impulse_response(e);
    
    if (BTNS & Input_Button2) // Swap
    {
        character_swap();
    }
    else if (BTNS & Input_Button3)
    {
        doc_action(e);
    }
    else if (BTN_STATE & Input_Button4) // Jump
    {
        
        if (e->isGrounded || e->offGroundImpulseResponseTicks)
        {
            if ((Vec_Prev_Btns & Input_Button4) == 0)
            {
                e->velocity.y                    = GRAVITY_DOWN() ? Velocity_Jump : -Velocity_Jump;
                e->offGroundImpulseResponseTicks = 0;
                e->recoveryTicks                 = 8;
            }
        }
        else if (e->recoveryTicks == 0)
        {
            if (!e->glideBlocked)
            {
                e->update        = update_doc_glide;
                e->velocity.y    = 0;
                e->recoveryTicks = 0;
                sound_push_sfx(&g_fly);
            }
        }
        else
        {
            --e->recoveryTicks;
        }
    }
    else
    {
        e->glideBlocked = false;
    }

    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform = -1;
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
    }
    else if (Vec_Joy_1_X > 0)
    {
        if (e->transform == -1)
        {
            e->transform = 1;
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
    }

    if (CAMERA.invincibilityTicks)
    {
        --CAMERA.invincibilityTicks;
        if (WORLD.freq2)
            return;
    }

    if (GRAVITY_DOWN())
    {
        if (e->velocity.y < -Velocity_AirTerminal)
            e->velocity.y = -Velocity_AirTerminal;
        doc_draw_default(e);
    }
    else
    {
        if (e->velocity.y > Velocity_AirTerminal)
            e->velocity.y = Velocity_AirTerminal;
        doc_draw_mirrored(e);
    }
}

void update_doc_glide(entity e)
{
    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform  = -1;
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
    }
    else if (Vec_Joy_1_X > 0)
    {
        if (e->transform == -1)
        {
            e->transform = 1;
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
    }
    
    if (Vec_Btn_State & Input_Button4 && !CAMERA.isGrounded)
    {
        e->velocity.y = 0;
    }
    else
    {
        e->glideBlocked = true;
        e->update = update_doc_air;
    }

    if (CAMERA.invincibilityTicks)
    {
        --CAMERA.invincibilityTicks;
        if (WORLD.freq2)
            return;
    }
    
    if (GRAVITY_DOWN())
        doc_draw_glide(e);
    else 
        doc_draw_glide_mirrored(e);
}

void update_doc_water(entity e)
{
    switch (BTNS)
    {
    case Input_Button2: // Swap
        character_swap();
        break;
    case Input_Button3:
        character_grab();
        break;
    case Input_Button4: // Swim Up
        if (GRAVITY_DOWN())
        {
            e->velocity.y += Velocity_SwimUp;
            e->velocity.y = MIN8(e->velocity.y, Velocity_SwimUp);
        }
        else
        {
            e->velocity.y -= Velocity_SwimUp;
            e->velocity.y = MAX8(e->velocity.y, -(Velocity_SwimUp));
        }
        
        break;
    default:
        break;
    }

    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform  = -1;
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << (e->velocity.y == 0);
        }
    }
    else if (Vec_Joy_1_X > 0)
    {
        if (e->transform == -1)
        {
            e->transform = 1;
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
    }
    
    if (CAMERA.invincibilityTicks)
    {
        --CAMERA.invincibilityTicks;
        if (WORLD.freq2)
            return;
    }

    if (GRAVITY_DOWN())
    {
        if (e->velocity.y < -Velocity_WaterTerminal)
            e->velocity.y = -Velocity_WaterTerminal;
        doc_draw_default(e);
    }
    else
    {
        if (e->velocity.y > Velocity_WaterTerminal)
            e->velocity.y = Velocity_WaterTerminal;
        doc_draw_mirrored(e);
    }
}
