#ifndef ENTITIES_H
#define ENTITIES_H

#include <cnd/types.h>
#include <cnd/xutils.h>
#include <cnd/globals.h>
#include <cnd/track.h>

/**
* @brief General Player Functions
*/
void routine_player_grab(void);
void routine_player_throw(void);
void routine_player_damage(void);
void routine_player_changed_fluid(void);

/**
* @brief Death Routines
*/
void routine_death0(entity e);
void routine_death1(entity e);

/**
* @brief Croc
*/
void routine_croc_air(entity e);
void routine_croc_water(entity e);
void routine_croc_hit(entity e);

/**
* @brief Doc
*/
void routine_doc_air(entity e);
void routine_doc_water(entity e);
void routine_doc_glide(entity e);

/**
* @brief Marvin-Mode
*/
void routine_marvin(entity e);

/**
 * @brief Barrel
 */
void routine_barrel_thrown(entity e);
void routine_barrel_idle(entity e);
void routine_barrel_held(entity e);
void barrel_create_prefab(entity e);

/**
 * @brief Gauner
 */
void routine_gauner_watching(entity e);

/**
 * @brief Tunichtgut
 */
void routine_tunichtgut_boom(entity e);

/**
 * @brief Halunke
 */
void routine_halunke_follow(entity e);

/**
 * @brief Schuft
 */
void routine_schuft_follow(entity e);

/**
 * @brief Schelm
 */
void routine_schelm_waiting(entity e);
void routine_schelm_thrown(entity e);
void schelm_create_prefab(entity e);

/**
 * @brief Bandit
 */
void routine_bandit_stolen(entity e);

/**
 * @brief Generic Enemy Functions
 */
force_inline void routine_enemy_collision(entity e, const i8 dx, const i8 dy, const i8* meshLeft, const i8* meshRight)
{
	i8 localDy = I8(dy);
	const i8 localDx = I8(dx);

	beam_set_position(localDy, localDx);
	Draw_VLc((void* const)(e->velocity.x < 0 ? meshLeft : meshRight));
	if (e->isSameTile && manhattan(localDy, localDx) < 0xA)
	{
		//const i8 localDyMask = localDy >> 7;
		//const i8 localDyAbs = (localDy ^ localDyMask) - localDyMask;
		const i8 localDxMask = localDx >> 7;
		const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;

		if ((CAMERA.velocity.y <= -2 && localDxAbs < 6) || (CAMERA.isAttacking && (CAMERA.velocity.x ^ localDx) >= 0))
		{
			e->stopwatch = 10;
			e->routine = routine_death1;
			CAMERA.velocity.y += Velocity_KillUpWind;
			CAMERA.velocity.y = MIN8(CAMERA.velocity.y, Velocity_KillUpWind);
			//GAME.explosion = &g_monsterPeng;
			/*Stop_Sound();
			Vec_Music_Flag = 0;
			Clear_Sound();*/
			//GAME.track = &musicOff;
			//Vec_Expl_Flag = 128;
		}
		else
		{
			WORLD.playerDamage();
		}
	}
}

#endif /* ENTITIES_H */
