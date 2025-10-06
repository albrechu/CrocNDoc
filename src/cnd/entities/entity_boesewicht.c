/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/draw_queue.h>
#include <cnd/music.h>
#include <cnd/sound.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_boesewicht(entity e)
{
	LOCAL_POS(e, dy, dx);
	if (e->timer == 0)
	{
		e->velocity.y = I8((dy < 0) - (dy > 0));
		e->velocity.x = I8((dx < 0) - (dx > 0));
		if ((CAMERA.substance & Tile_Water) == 0)
		{
			e->velocity.y <<= 1;
			e->velocity.x <<= 1;
		}
		
		if (e->inLocalSpace)
		{
			if (--e->recoveryTicks < 0 && !IS_SAME_TILE(e, &CAMERA))
			{
				e->recoveryTicks = 127;
				entity_create_anonymous(Enemy_Schelm, e->tile);
				e->velocity.y = -(e->velocity.y << 2);
				e->velocity.x = -(e->velocity.x << 2);
			}

			i8 localDy = (i8)dy;
			i8 localDx = (i8)dx;

			if (GRAVITY_DOWN())
			{
				draw_stack_push(localDx >= 0 ? boesewicht_left : boesewicht_right, localDy, localDx);
			}
			else
			{
				draw_stack_push(localDx >= 0 ? boesewicht_left_r : boesewicht_right_r, localDy, localDx);
			}
			
			/*e->hitbox.y = GRAVITY_DOWN() ? Hitbox_BoesewichtY : -Hitbox_BoesewichtY;*/

			if (NEAR_CENTER(e))
			{
				if (entity_intersects_camera(e, localDy, localDx))
				{
					if (GAME.event == Event_YouAreBeingFollowed)
					{
						if (CAMERA.isAttacking || (GRAVITY_DOWN() && (dy < -4)) || (GRAVITY_UP() && (dy > 4)))
						{
							e->timer = 255;
							sound_push_sfx(&g_explosion1);
							if (WORLD.gravity < 0)
								CAMERA.velocity.y = -Velocity_KillUpWind;
							else
								CAMERA.velocity.y = Velocity_KillUpWind;
						}
						else
						{
							e->timer = 255;
							character_damage();
						}
					}
					else
					{
						entity_exchange_blows(e, localDy);
					}
				}
			}
		}
	}
	else
	{
		--e->timer;
		e->velocity.y = 0;
		e->velocity.x = 0;
		if (e->inLocalSpace)
		{
			i8 localDy = (i8)dy;
			i8 localDx = (i8)dx;
			draw_stack_push(boesewicht_stunned, localDy, localDx);
		}
	}
}

void prefab_boesewicht(entity e)
{
	e->update        = update_boesewicht;
	e->kill          = update_kill;
	e->score         = Score_400;
	e->hitbox        = (v2i){ Hitbox_BoesewichtY, Hitbox_BoesewichtX };
	e->recoveryTicks = -1;
	e->timer         = 0;
	entity_set_animation(e, explosion, ELEMENT_SIZE(explosion), ARRAY_SIZE(explosion));
}