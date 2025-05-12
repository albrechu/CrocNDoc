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

#ifndef TYPES_H
#define TYPES_H

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/defines.h>

/////////////////////////////////////////////////////////////////////////
//	Integral Types
//
typedef unsigned int  u8;
typedef signed   int  i8;
typedef unsigned long int u16;
typedef signed long int   i16;
typedef u8            event_t;
typedef u8            result_t;
typedef u8            tick_t;
typedef i8            bool;
typedef i8            idx_t;

enum /* Generic */
{
	true  = 1,
	false = 0,
	null  = 0,
};


/////////////////////////////////////////////////////////////////////////
//	Function Pointers
//
typedef void(*procedure_t)(void);

/////////////////////////////////////////////////////////////////////////
//	Math Types
//
typedef struct v2i
{
	i8 y;
	i8 x;
} v2i;

typedef union v2l
{
	struct 
	{
		i8 y_lsb;
		i8 y_msb;
		i8 x_lsb;
		i8 x_msb;
	};
	struct
	{
		i16 y;
		i16 x;
	};
} v2l;

//#define VEC_IS_LOCAL(vec) ((vec.x_msb | vec.y_msb) == 0 || (vec.x_msb | vec.y_msb) == -1) // Checks whether a 16 bit vector is within 8-bit coords.
//#define VEC_IS_LOCAL(vec) (((vec.x_msb) != ((vec.x_lsb) >> 7))) && (((vec.y_msb) != ((vec.y_lsb) >> 7)))
//#define VEC_IS_LOCAL(v)  (( (v.x_msb + 1) | (-v.x_msb) | (v.y_msb + 1) | (-v.y_msb) ) >= 0)

/** 
* @brief rect_t is a rectangle defined by its position and size.
* 
* Right-Handed Coordinate System (RHCS):
*              127
*               + top
*               |
* left 128 - ---0--- + 127 right
*               |
*               - bottom
*              128
*/
typedef union rect_t
{
	struct // attributes
	{
		v2i pos; // bottom-left
		v2i size;
	};
	struct // components
	{
		i8 y; // bottom
		i8 x; // left
		i8 height;
		i8 width; 
	};
} rect_t, *rect;

/////////////////////////////////////////////////////////////////////////
//	Enumerations
//
enum Player_
{
    Player_ArmEdges = 4,
    // [[Position]]
    Player_XMax     = 90,
	Player_YMax     = 100,
	Player_XMin     = -100,
	Player_YMin     = 0,
};

enum Velocity_
{
    Velocity_Run      = 2,
    Velocity_Bite     = 8,
	Velocity_Friction = 1,
    Velocity_Jump     = 10,
    Velocity_JumpX    = 0,
    Velocity_ThrowX   = 5,
    Velocity_ThrowY   = 8,
    Velocity_Jumper   = 16,

};
typedef i8 Velocity;

/** 
* @brief Input_ contains all bit values available for controller inputs
*/
enum Input_
{
	// P0: Bit 0 - 3, P1: Bit 4 - 7,
    Input_Idle      = 0x00u,
	Input_Button1   = 0x01u,
	Input_Button2   = 0x02u,
	Input_Button3   = 0x04u,
	Input_Button4   = 0x08u,
	Input_AnyButton = 0x0Fu,
	// Joystick Directions
    Input_JoyIdle      = 0x00u,
    Input_JoyLeftBit   = 4u,
    Input_JoyRightBit  = 5u,
    Input_JoyUpBit     = 6u,
    Input_JoyDownBit   = 7u,
    Input_JoyLeft      = 0x1u << Input_JoyLeftBit,
    Input_JoyRight     = 0x1u << Input_JoyRightBit,
    Input_JoyUp        = 0x1u << Input_JoyUpBit,
    Input_JoyDown      = 0x1u << Input_JoyDownBit,
    Input_JoyLeftDown  = Input_JoyLeft | Input_JoyDown,
    Input_JoyRightDown = Input_JoyRight | Input_JoyDown,
    Input_JoyLeftUp    = Input_JoyLeft | Input_JoyUp,
    Input_JoyRightUp   = Input_JoyRight | Input_JoyUp,
};
typedef u8 Input;

enum Action_
{
    Action_Idle,
};
typedef i8 Action;

enum EntityState_
{
    EntityState_Dead,
    // CharacterState_
    CharacterState_Idle,
    CharacterState_HoldsProp,
    CharacterState_Glide,
    // PropState_
    PropState_Idle,
    PropState_Held,
    PropState_Thrown,
    // EnemyState_
    EnemyState_Follow,
};

typedef i8 EntityState;

enum Character_
{
    Character_Croc,
    Character_Doc,
};
enum Enemy_
{
    Prop_Crate,
    Prop_Barrel,
    Prop_Max = Prop_Barrel,
    Enemy_Tunichtgut,
    Enemy_Halunke,
    Enemy_Gauner,
    Enemy_Schuft,
    Enemy_Strolch,
    Enemy_Boesewicht,
    // Enemy_Ganove,
    // Enemy_Spitzbube
    // Enemy_Schurke
    // Enemy_Betrüger
    // Enemy_Bandit
    // Enemy_Schelm
    // Enemy_Schwindler
    // Enemy_Verbrecher
    // Enemy_Lump
    // Enemy_Dieb
    // Enemy_Schlawiner
    // Enemy_Gaunerkönig
};
typedef u8 EntityType;

/**
 * @brief Mesh_ is a look up enumeration
 */
// enum Mesh_
// {
// 	Mesh_CrocIdleRight,
// 	Mesh_CrocIdleLeft,
// 	Mesh_DocIdleRight,
// 	Mesh_DocIdleLeft,
// 	Mesh_MantisRight,
// 	Mesh_MantisLeft,
// 	Mesh_BarrelLeft,
// 	Mesh_Barrel,
// 	Mesh_BarrelRight,
// 	Mesh_CrateLeft,
// 	Mesh_Crate,
// 	Mesh_CrateRight,
//     Mesh_Halunke,
// };
// typedef i8 Mesh;

enum Material_
{
    Substance_Air,
    Substance_Water,
};
typedef i8 Material;

enum Stage_
{
    Stage_Sewers,   
    Stage_Waterways,
    Stage_Moltenlands,
    Stage_Bonus,
};
typedef i8 Stage;

enum Track_
{
    Track_Yellow   = 0,
    Track_Forgot   = 1,
    Track_Corneria = 2,
    Track_JibJib   = 3,
};
typedef i8 Track;

/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef struct entity_t entity_t, *entity;
typedef void (*routine_t)(entity);
typedef void (*normal_routine_t)(void);
typedef void (*stage_routine_t)(Stage);
struct entity_t
{
    const void*       mesh;
    routine_t   routine;
    v2l         position;
    v2i         velocity;
	v2i         tile;
    v2i         tileCandidate;
    i8          transform;  
    EntityType  type;  
    EntityState state;
    bool        isLocal;
    bool        isSameTile;
    bool        isGrounded;
    bool        isEnemy;
    Material    substance;
    idx_t       id;
    i8          stopwatch;
};

typedef struct player_t
{
    Input    buttons, joystick;
    Action   action;
    u16      score;
    bool     isOtherCharacterDead;
} player_t, *player;

typedef struct game_t
{
    procedure_t progress;
    void const* explosion;
    // Entities
    player_t player;
    // Game State
    u8    ticks;
    Track track;
    bool  isFinished;
    i8    data[4];
} game_t, *game;

#endif /* TYPES_H */