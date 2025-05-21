/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void routine_marvin(entity e)
{
    CAMERA.invisiblityFrames = 99;
    switch (BTNS)
    {
    case Input_Button1: // Swap
        {
            Vec_Joy_Mux_1_Y = 0;
            CAMERA.invisiblityFrames    = 0;
            PLAYER.isOtherCharacterDead = false;
            e->type = Character_Doc;
            e->mesh = e->transform == 1 ? doc_idle_right[0] : doc_idle_left[0];
            e->routine = e->substance == Substance_Air ? routine_doc_air : routine_doc_water;
        }
        break;
    case Input_Button4: // Jump
        e->velocity.y += Velocity_SwimUp;
        e->velocity.y = MIN8(e->velocity.y, Velocity_SwimUp);
        break;
    default:
        if (e->velocity.y < 0)
            e->velocity.y = 0;
        break;
    }

    if (Vec_Joy_1_Y > 0)
        e->velocity.y = (Velocity_Run << 1);
    if (Vec_Joy_1_Y < 0)
        e->velocity.y = -(Velocity_Run << 1);
    if (Vec_Joy_1_X > 0)
        e->velocity.x = (Velocity_Run << 1) ;
    if (Vec_Joy_1_X < 0)
        e->velocity.x = -(Velocity_Run << 1);
    
    beam_set_position(0, 0);
    Draw_VLc((void* const)marvin);
}
