#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>

void routine_doc_air(entity e)
{
    switch (BTNS)
    {
    case Input_Button1: // Swap
        if (e->isGrounded && !PLAYER.isOtherCharacterDead)
        {
            e->type = Character_Croc;
            e->mesh = e->transform == 1 ? croc_idle_right[0] : croc_idle_left[0];
            e->routine = routine_croc_air;
        }
        break;
    case Input_Button2: // Marvin-Mode
//#ifdef NDEBUG
        Vec_Joy_Mux_1_Y = 3;
        e->type = Character_Marvin;
        e->routine = routine_marvin;
//#endif
        break;
    case Input_Button3: // Hit | Grab
    {
        switch (e->state)
        {
        case CharacterState_Idle: // Grab if one is close enough
            routine_player_grab();
            break;
        case CharacterState_HoldsProp: // Throw
            routine_player_throw();
            break;
        default:
            break;
        }
    }
    break;
    case Input_Button4: // Jump
        if (e->isGrounded)
        {
            e->velocity.y += Velocity_Jump;
            e->velocity.x += e->transform * Velocity_JumpX;
        }
        else
        {
            e->routine = routine_doc_glide;
            e->velocity.y = 0;
        }
        break;
    default:
        break;
    }

    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform = -1;
            e->mesh = e->type == Character_Croc ? croc_idle_left[0] : doc_idle_left[0];
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
            e->mesh = e->type == Character_Croc ? croc_idle_right[0] : doc_idle_right[0];
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
    }

    if (CAMERA.invisiblityFrames)
    {
        --CAMERA.invisiblityFrames;
        if (WORLD.ticks & 1)
            return;
    }

    beam_set_position(0, 0);
    if (e->velocity.x)
    {
        if (e->transform & -128)
        {
            Mov_Draw_VLc_a((void* const)doc_idle_left[WORLD.freq16]);
        }
        else
        {
            Mov_Draw_VLc_a((void* const)doc_idle_right[WORLD.freq16]);
        }
    }
    else
    {
        if (e->transform & -128)
        {
            Mov_Draw_VLc_a((void* const)doc_idle_left[0]);
        }
        else
        {
            Mov_Draw_VLc_a((void* const)doc_idle_right[0]);
        }
    }
}

void routine_doc_glide(entity e)
{
    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform  = -1;
            e->mesh       = doc_idle_left[0];
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
            e->mesh = doc_idle_right[0];
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
        e->routine = routine_doc_air;
    }

    if (CAMERA.invisiblityFrames)
    {
        --CAMERA.invisiblityFrames;
        if (WORLD.ticks & 1)
            return;
    }

    /*beam_set_position(0, 0);
    Mov_Draw_VLc_a((void* const)CAMERA.mesh);*/

    beam_set_position(0, 0);
    if (e->velocity.x)
    {
        if (e->transform & -128)
        {
            Mov_Draw_VLc_a((void* const)doc_glide_left[WORLD.freq16]);
        }
        else
        {
            Mov_Draw_VLc_a((void* const)doc_glide_right[WORLD.freq16]);
        }
    }
    else
    {
        if (e->transform & -128)
        {
            Mov_Draw_VLc_a((void* const)doc_glide_left[0]);
        }
        else
        {
            Mov_Draw_VLc_a((void* const)doc_glide_right[0]);
        }
    }

    /*beam_set_position(10, -1 * e->transform);
    Draw_Line_d(10, -CAMERA.transform * ((((i8)WORLD.ticks & 1) * -15) | 15)); */
}

void routine_doc_water(entity e)
{
    switch (BTNS)
    {
    case Input_Button1: // Swap
        if (!PLAYER.isOtherCharacterDead)
        {
            e->type = Character_Croc;
            e->mesh = e->transform == 1 ? doc_idle_right[0] : doc_idle_left[0];
            e->routine = routine_croc_water;
        }
        break;
    case Input_Button2: // Marvin-Mode
//#ifdef NDEBUG
        Vec_Joy_Mux_1_Y = 3;
        e->type = Character_Marvin;
        e->routine = routine_marvin;
//#endif
        break;
    case Input_Button4: // Swim Up
        e->velocity.y += Velocity_SwimUp;
        e->velocity.y = MIN8(e->velocity.y, Velocity_SwimUp);
        break;
    default:
        if (e->velocity.y < -2)
        {
            e->velocity.y = -2;
        }
        break;
    }

    if (Vec_Joy_1_X < 0)
    {
        if (e->transform == 1)
        {
            e->transform = -1;
            e->mesh = croc_idle_left[0];
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
            e->mesh = croc_idle_right[0];
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
        else if (e->velocity.x < Velocity_Run)
        {
            e->velocity.x = Velocity_Run << (e->velocity.y == 0);
        }
    }
    
    if (CAMERA.invisiblityFrames)
    {
        --CAMERA.invisiblityFrames;
        if (WORLD.ticks & 1)
            return;
    }

    beam_set_position(0, 0);
    if (e->velocity.x)
    {
        if (e->transform & -128)
        {
            Mov_Draw_VLc_a((void* const)doc_idle_left[WORLD.freq16]);
        }
        else
        {
            Mov_Draw_VLc_a((void* const)doc_idle_right[WORLD.freq16]);
        }
    }
    else
    {
        if (e->transform & -128)
        {
            Mov_Draw_VLc_a((void* const)doc_idle_left[0]);
        }
        else
        {
            Mov_Draw_VLc_a((void* const)doc_idle_right[0]);
        }
    }
}