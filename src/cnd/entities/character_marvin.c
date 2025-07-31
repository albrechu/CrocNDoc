/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/world.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_marvin(entity e)
{
    CAMERA.invincibilityTicks = 99;
    switch (BTNS)
    {
    case Input_Button1: // Swap
         Vec_Joy_Mux_1_Y = 0;
         CAMERA.invincibilityTicks   = false;
         PLAYER.isOtherCharacterDead = false;
         e->type = Character_Doc;
         e->update = e->substance == Substance_Air ? update_doc_air : update_doc_water;
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
    
    draw_queue_push(marvin, 0, 0);
}
