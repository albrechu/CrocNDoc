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

#pragma once

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/types.h>

/////////////////////////////////////////////////////////////////////////
//	General Player Functions
//
/**
* @brief  Tries to find an object to grab in close proximity. 
* 
* Iterates over all objects and finds props to check if they are close enough to be grabbed.
* It returns wheter an object was grabbed and if true saves the objects id at index 1 in the 
* entity id list. The grabbed entity therefore is at WORLD.entities[WORLD.entityIdxs[1]].
* 
* @return Returns true if an object was grabbed.
*/
bool character_grab(void);
/**
* @brief 
*/
void character_throw(void);
/**
* @brief 
*/
void character_damage(void);
/**
* @brief 
*/
void character_off_ground_impulse_response(entity e);
/**
* @brief
*/
void character_swap(void);

/////////////////////////////////////////////////////////////////////////
//	Generic Entity functions
//
/**
* @brief Removes the entity from the world. If it has a name it will be removed.
*/
void entity_set_status(entity e, EntityStatus status);

/**
* @brief Creates a named entity, meaning an object which is spawned from tiles.
*/
void entity_create_named(idx_t const globalId, v2i const tile);

/**
* @brief Creates a anonymous entity, meaning an object which was spawned from enemies
*/
void entity_create_anonymous(EntityType const type, v2i const tile);

/**
* @brief 
*/
void entity_set_animation(entity e, void const* keyframes, i8 keyframeSize, i8 keyframeCount);

/**
* @brief Checks whether the entity intersects the camera/player based on their respective hitboxes.
*/
bool entity_intersects_camera(entity e, i8 dy, i8 dx);

/**
* @biref
*/
void entity_exchange_blows(entity e, i8 dy);

/**
 * @brief Kills the entity e and tries to revives the players other character
 */
void update_kill_revive(entity e);
/**
* @brief Kills the entity e
*/
void update_kill(entity e);
/**
* @brief Runs the death animation and later calls the selected kill function.
*/
void update_death(entity e);
/**
* @brief Stub. Does nothing. 
*/
void update_stub(entity e);

void add_score(u8 score);

void entity_set_death(entity e);

void entity_camera_hit_detection(entity e, i8 localDx);

/////////////////////////////////////////////////////////////////////////
//	LIFO Free List allocator for entities
//
/**
* @brief Clears the list of entities and (re-)initializes the allocator.
*/
void   entity_list_clear();
/**
* @brief Try to allocate an entity.
* @return Pointer to entity on success, NULL otherwise. 
*/
entity entity_alloc();
/**
* @brief Frees an entity. Entity has to be allocated. Use handle_valid(e->handle) if unsure.
*/
void   entity_free(entity e);
/**
* @brief Get the entity by handle. Reference has to be valid. Use handle_valid(handle) if unsure.
*/
entity entity_get(handle h);
/**
* @brief Returns if the reference to the entity is still valid.
* @return True if entity is alive, false otherwise.
*/
bool handle_valid(handle h);

/////////////////////////////////////////////////////////////////////////
//	Croc
//
/**
* @brief Update function when Croc is in air
*/
void update_croc_air(entity e);
/**
* @brief Update function when Croc is underwater.
*/
void update_croc_water(entity e);
/**
* @brief Update function while Croc is attacking
*/
void update_croc_hit(entity e);
/**
 * @brief
 */
void prefab_croc(entity e);
/**
 * @brief
 */
void prefab_croc_prepare(entity e);

/////////////////////////////////////////////////////////////////////////
//	Doc
//
/**
* @brief 
*/
void update_doc_air(entity e);
/**
* @brief 
*/
void update_doc_water(entity e);
/**
* @brief 
*/
void update_doc_glide(entity e);

/////////////////////////////////////////////////////////////////////////
// Barrel
//
/**
 * @brief 
 */
void update_barrel_thrown(entity e);
/**
 * @brief
 */
void update_barrel(entity e);
/**
 * @brief
 */
void update_barrel_held(entity e);
/**
 * @brief
 */
void prefab_barrel(entity e);
/**
 * @brief
 */
void prefab_barrel_throw(entity e);

/////////////////////////////////////////////////////////////////////////
// Gauner
//
/**
 * @brief  
 */
void update_gauner(entity e);
void prefab_gauner(entity e);

/////////////////////////////////////////////////////////////////////////
// Halunke
//
/**
 * @brief Halunke follows the player on the horizontal axis
 */
void update_halunke(entity e);
void prefab_halunke(entity e);

/////////////////////////////////////////////////////////////////////////
// Schuft 
//
/**
 * @brief Schuft follows the player horizontally and vertically 
 */
void update_schuft(entity e);
void prefab_schuft(entity e);


/////////////////////////////////////////////////////////////////////////
// Schelm - Spike Projectile
//
/**
 * @brief Schelm is a thrown spike and is a danger to the player pre-explosion.
 */
void update_schelm_thrown(entity e);
/**
 * @brief Schelm template.
 */
void prefab_schelm(entity e);

/////////////////////////////////////////////////////////////////////////
// Bandit - Spider
//
/**
 * @brief Bandit is a spider, that can only go in x-direction and in y-direction with gravity.
 */
void update_bandit(entity e);
/**
 * @brief Bandit template.
 */
void prefab_bandit(entity e);

/////////////////////////////////////////////////////////////////////////
// Coin
//
/**
 * @brief 
 */
void update_coin_achieved(entity e);
void prefab_coin(entity e);

/////////////////////////////////////////////////////////////////////////
//	Function Table
//
extern const prefab_t prefabs[EntityType_Max];