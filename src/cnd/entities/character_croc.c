/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>
#include <lib/assert.h>
#include <cnd/draw_queue.h>
#include <cnd/sound.h>
#include <cnd/music.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void croc_draw_default(entity e)
{
    if (e->velocity.x)
    {
        if (e->transform < 0)
        {
            draw_stack_push(croc_idle_left[WORLD.freq16], 4, (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm[1], 7 + (WORLD.freq16 << 1), -5 +(Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ 13 + (WORLD.freq16 << 1), -4 + (Hitbox_CrocX >> 1) };
        }
        else
        {
            draw_stack_push(croc_idle_right[WORLD.freq16], 4, 0 - (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm[0], 7 + (WORLD.freq16 << 1), 1 - (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ 13 + (WORLD.freq16 << 1), 4 - (Hitbox_CrocX >> 1) };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(croc_idle_left[0], 4, (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm[1], 7, -5 + (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ 13, -4 + (Hitbox_CrocX >> 1) };
        }
        else
        {
            draw_stack_push(croc_idle_right[0], 4, 0 - (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm[0], 7, 1 - (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ 13, 4 - (Hitbox_CrocX >> 1) };
        }
    }
}

void croc_draw_mirrored(entity e)
{
    if (e->velocity.x)
    {
        if (e->transform < 0)
        {
            draw_stack_push(croc_idle_left_r[WORLD.freq16], -4, (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm_r[1], -8 - (WORLD.freq16 << 1), -5 + (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ -13 - (WORLD.freq16 << 1), -4 + (Hitbox_CrocX >> 1) };
        }
        else
        {
            draw_stack_push(croc_idle_right_r[WORLD.freq16], -4, 0 - (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm_r[0], -8 - (WORLD.freq16 << 1), 2 - (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ -13 - (WORLD.freq16 << 1), 4 - (Hitbox_CrocX >> 1) };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(croc_idle_left_r[0], -4, 0 + (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm_r[1], -8, -5 + (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ -13, -4 + (Hitbox_CrocX >> 1) };
        }
        else
        {
            draw_stack_push(croc_idle_right_r[0], -4, 0 - (Hitbox_CrocX >> 1));
            draw_stack_push(croc_arm_r[0], -8, 2 - (Hitbox_CrocX >> 1));
            WORLD.eyePosition = (v2i){ -13, 4 - (Hitbox_CrocX >> 1) };
        }
    }
}



void croc_draw_local_move(entity e, i8 const y, i8 const x)
{
    if (e->velocity.x)
    {
        if (e->transform < 0)
        {
            draw_stack_push(croc_idle_left[WORLD.freq16], 4 + y, 0 + x);
            draw_stack_push(croc_arm[1], 7 + (WORLD.freq16 << 1) + y, -5 + x);
            WORLD.eyePosition = (v2i){ 13 + (WORLD.freq16 << 1) + y, -4 + x };
        }
        else
        {
            draw_stack_push(croc_idle_right[WORLD.freq16], 4 + y, 0 + x);
            draw_stack_push(croc_arm[0], 7 + (WORLD.freq16 << 1) + y, 1 + x);
            WORLD.eyePosition = (v2i){ 13 + (WORLD.freq16 << 1) + y, 4 + x };
        }
    }
    else
    {
        if (e->transform < 0)
        {
            draw_stack_push(croc_idle_left[0], 4 + y, 0 + x);
            draw_stack_push(croc_arm[1], 7 + y, -5 + x);
            WORLD.eyePosition = (v2i){ 13 + y, -4 + x };
        }
        else
        {
            draw_stack_push(croc_idle_right[0], 4 + y, 0 + x);
            draw_stack_push(croc_arm[0], 7 + y, 1 + x);
            WORLD.eyePosition = (v2i){ 13 + y, 4 + x };
        }
    }
}

void update_croc_hit(entity e)
{
    character_off_ground_impulse_response(e);
    if (--e->velocityCache == 0)
    {
        e->isAttacking = false;
        e->update      = update_croc_air;
    }
    else
    {
        if (BTNS & Input_Button4 && (e->isGrounded || e->offGroundImpulseResponseTicks))
        {
            e->velocity.y = GRAVITY_DOWN() ? Velocity_Jump : -Velocity_Jump;
            e->offGroundImpulseResponseTicks = 0;
        }

        void* data = (void*)cloud[e->velocityCache & 0x7];
        if (GRAVITY_DOWN())
        {
            if (e->velocity.y < -Velocity_AirTerminal)
                e->velocity.y = -Velocity_AirTerminal;
            if (e->transform > 0)
            {
                e->velocity.x = Velocity_Hit;
                draw_stack_push(croc_idle_right[1], 4, 0);
                draw_stack_push(data, 10, 23 - (e->velocityCache << 1));
                WORLD.eyePosition = (v2i){ 15, 4 };
            }
            else
            {
                e->velocity.x = -Velocity_Hit;
                draw_stack_push(croc_idle_left[1], 4, 0);
                draw_stack_push(data, 10, -14 + (e->velocityCache << 1));
                WORLD.eyePosition = (v2i){ 15, -4 };
            }
        }
        else
        {
            if (e->velocity.y > Velocity_AirTerminal)
                e->velocity.y = Velocity_AirTerminal;
            if (e->transform > 0)
            {
                e->velocity.x = Velocity_Hit;
                draw_stack_push(croc_idle_right_r[1], -4, 0);
                draw_stack_push(data, -10, 23 - (e->velocityCache << 1));
                WORLD.eyePosition = (v2i){ -15, 4 };
            }
            else
            {
                e->velocity.x = -Velocity_Hit;
                draw_stack_push(croc_idle_left_r[1], -4, 0);
                draw_stack_push(data, -10, -14 + (e->velocityCache << 1));
                WORLD.eyePosition = (v2i){ -15, -4 };
            }
        }
    }
}

void update_croc_prepare(entity e)
{
    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform = -1;
            e->velocity.x = (-Velocity_Run) << 1;
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << 1;
        }
    }
    else if (Vec_Joy_1_X > 0)
    {
        if (e->transform == -1)
        {
            e->transform = 1;
            e->velocity.x = Velocity_Run << 1;
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << 1;
        }
    }

    if (BTNS == Input_Button4 && e->isGrounded)
    {
        e->velocity.y += Velocity_Jump;
    }
    e->velocity.y -= Velocity_Gravity;

    const i8 x = I8(e->position.x + e->velocity.x);
    const i8 y = I8(e->position.y + e->velocity.y);
    e->position.x = I16(x);
    e->position.y = I16(y);
    if (e->position.y < 0)
    {
        e->position.y = 0;
        e->velocity.y = 0;
    }
    e->isGrounded = e->position.y == 0;
    e->velocity.x = 0;

    croc_draw_local_move(e, y, x);
}

void prefab_croc(entity e)
{
    e->update       = update_croc_air;
    e->kill         = update_kill;
    e->score        = Score_0;
    e->inLocalSpace = true;
    e->type         = Character_Croc;
    e->state        = CharacterState_Idle;
    e->transform    = 1;
    e->hitbox       = (v2i){ Hitbox_CrocY, Hitbox_CrocX};
    entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}

void prefab_croc_prepare(entity e)
{
    prefab_croc(e);
    e->position.x = 0;
    e->position.y = 80;
    e->update     = update_croc_prepare;
}

void croc_hit()
{
    if (CAMERA.isGrounded || (GAME.event == Event_MoonWalk))
    {
        CAMERA.isAttacking = true;
        if ((CAMERA.substance & Substance_Water) == 0)
        {
            CAMERA.update = update_croc_hit;
            sound_push_sfx(&g_hit);
        }
        CAMERA.velocityCache = 12;
    }
}

void update_croc_air(entity e)
{
    character_off_ground_impulse_response(e);

    switch (BTNS)
    {
    case Input_Button2: // Transform
        character_swap();
        break;
    case Input_Button3: // Hit | Grab
    {
        switch (CAMERA.state)
        {
        case CharacterState_Idle: // Grab if one is close enough
        if (!character_grab())
        {
            croc_hit();
        }
        break;
        case CharacterState_HoldsProp: // Throw
            character_throw();
            break;
        default:
            break;
        }
    }
    break;
    case Input_Button4: // Jump
        if (e->isGrounded || e->offGroundImpulseResponseTicks)
        {
            e->velocity.y = GRAVITY_DOWN() ? Velocity_Jump : -Velocity_Jump;
            e->offGroundImpulseResponseTicks = 0;
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
            e->velocity.x = (-Velocity_Run) << e->isGrounded;
        }
        else if (e->velocity.x > -Velocity_Run)
        {
            e->velocity.x = (-Velocity_Run) << e->isGrounded;
        }
    }
    else if (Vec_Joy_1_X > 0)
    {
        if (e->transform == -1)
        {
            e->transform  = 1;
            e->velocity.x = Velocity_Run << e->isGrounded;
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << e->isGrounded;
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
        croc_draw_default(e);
    }
    else
    {
        if (e->velocity.y > Velocity_AirTerminal)
            e->velocity.y = Velocity_AirTerminal;
        croc_draw_mirrored(e);
    }
}

void update_croc_water(entity e)
{
    switch (BTNS)
    {
    case Input_Button2: // Swap
        character_swap();
        break;
    case Input_Button3: // Grab
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
            e->velocity.y = MAX8(e->velocity.y, -Velocity_SwimUp);
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
        croc_draw_default(e);
    }
    else
    {
        if (e->velocity.y > Velocity_WaterTerminal)
            e->velocity.y = Velocity_WaterTerminal;
        croc_draw_mirrored(e);
    }
}

