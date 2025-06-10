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
    Velocity_Hit      = 7,
	Velocity_Friction = 1,
    Velocity_Jump     = 10,
    Velocity_Breaching = 12,
    Velocity_SwimUp   = 9,
    Velocity_KillUpWind = 12,
    Velocity_JumpX    = 0,
    Velocity_ThrowX   = 5,
    Velocity_ThrowY   = 8,
    Velocity_Jumper   = 12,
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

/**
* @brief Character_ is a EntityType
*/
enum Character_
{
    Character_Croc,
    Character_Doc,
    Character_Marvin,
};

/**
* @brief Enemy_ is a EntityType
*/
enum Enemy_
{
    Prop_Crate,
    Prop_Barrel,
    Prop_Max = Prop_Barrel,
    Enemy_Tunichtgut,
    Enemy_Halunke,
    Enemy_Gauner,
    Enemy_Schuft,
    Enemy_Schelm,
    Enemy_Bandit,
    Enemy_Boesewicht,
    //Enemy_Strolch,
    // Enemy_Ganove,
    // Enemy_Spitzbube
    // Enemy_Schurke
    // Enemy_Betrüger
    // Enemy_Schwindler
    // Enemy_Verbrecher
    // Enemy_Lump
    // Enemy_Dieb
    // Enemy_Schlawiner
    // Enemy_Gaunerkönig
};
typedef u8 EntityType;

enum Tile_
{
    // Materials
    Tile_Air,
    Tile_Water,
    Tile_Top2,
    Tile_Top,
    Tile_TopLeft,
    Tile_TopRight,
    Tile_Bottom2,
    Tile_Bottom,
    Tile_BottomLeft,
    Tile_BottomRight,
    Tile_Left2,
    Tile_Left,
    Tile_Right2,
    Tile_Right,
    Tile_MiddleLeftTop,
    Tile_MiddleLeft,
    Tile_MiddleRight,
    Tile_Middle,
    Tile_MiddleBottom,
    Tile_MiddleRightTop,
    Tile_MiddleTop,
    Tile_Spikes,
    Tile_SpikedBall,
    Tile_Jumper,
    Tile_BarrierVertical,
    Tile_BarrierHorizontal,
    Tile_WaterTop,
    // Entities
    Tile_E0,
    Tile_E1,
    Tile_E2,
    Tile_E3,
    Tile_E4,
    Tile_E5,
    Tile_E6,
    Tile_E7,
    Tile_E8,
    Tile_E9,
    Tile_E10,
    Tile_E11,
    Tile_E12,
    Tile_E13,
    Tile_E14,
    Tile_E15,
    // Portal
    Tile_Portal0,
    Tile_Portal1,
    Tile_GravitasUp,
    Tile_SpikesDown,
    Tile_Warning,
    Tile_NA,
    Tile_GravitasDown,
    // <Add New Tiles Here>
    Tile_Max,
    // Wormhole
    // Tile_Wormhole0,
    // Tile_Wormhole1,
    // Tile_Wormhole2,
    // Tile_Wormhole3,
};
typedef i8 Tile;

enum EntityStatus_
{
    EntityStatus_Inactive, // The entity is alive and well but takes not part in any calculations.
    EntityStatus_Active,   // The entity is currently active, takes part in the physics calculation and is possibly rendered.
    EntityStatus_Dead,     // May it rest in peace.
};
typedef i8 EntityStatus;

enum Material_
{
    Substance_Air,
    Substance_Water,
    Substance_GravitasAir,
    Substance_GravitasWater,
};
typedef i8 Material;

enum Stage_
{
    Stage_Tutorial,
    Stage_JumpWorld,   
    Stage_Water,
    Stage_Gravitas,
    Stage_Bonus,
};
typedef i8 Stage;

enum Score_
{
    Score_50,
    Score_100,
    Score_200,
    Score_500,
};

/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef struct entity_t entity_t, *entity;
typedef void (*routine_t)(entity);
typedef void (*normal_routine_t)(void);
typedef void (*stage_routine_t)(Stage);
struct entity_t
{
    const void* mesh;          // 0
    routine_t   routine;       // 2
    routine_t   collision;     // 4
    v2l         position;      // 6
    v2i         velocity;      // 10
	v2i         tile;          // 12
    v2i         tileCandidate; // 14
    i8          transform;     // 16
    EntityType  type;          // 17
    EntityState state;         // 18
    bool        isLocal;       // 19
    bool        isSameTile;    // 20
    bool        isGrounded;    // 21
    bool        isEnemy;       // 22
    bool        isAttacking;   // 23
    Material    substance;     // 24
    idx_t       id;            // 25
    idx_t       globalId;      // 26
    // Animation
    i8          stopwatch;     // 27
    i8          invisiblityFrames; // 28
    i8          data[4]; // 29
    // 33 + alignof(2) = 34
};
//static_assert(sizeof(entity_t) == 34);

typedef struct player_t
{
    Input    joystick;
    Action   action;
    u16      score;
    bool     isOtherCharacterDead;
} player_t, *player;

typedef struct game_t
{
    procedure_t progress;
    void const* explosion;
    void const* track;
    // Entities
    player_t player;
    // Game State
    bool  isFinished;
    union
    {
        struct 
        {
            v2i finTxtPos; // Position of the Game Over text.
        };
        i8  data[4];
    };
} game_t, *game;

typedef struct last_sighting_t
{
    // v2l       position; // Last known position. Probably impossible to do without wasting a bunch of cycles by searching a "look aside" list. Though I could do this every cycles or so. But not here.
    EntityStatus status;   // Last known state. Initially is EntityStatus_Alive.
} last_sighting_t, * last_sighting;

typedef struct level_t
{
    EntityType const* entities;
    Tile       const* level;
    v2i        const  startingTile;
    Stage      const  adjacentStages[2];
} level_t, *level;

typedef struct world_t
{
    // Stage information
    const Tile*    tiles; // 0
    const level_t* level; // 2
    // Callbacks
    routine_t        entityAdded; // 4
    stage_routine_t  stageEntered; // 6
    normal_routine_t playerDamage; // 8
    normal_routine_t playerChangedFluid; // 10
    // Entities
    entity_t        entities[ENTITIES_ACTIVE_MAX];  // 12
    idx_t           entityIdxs[ENTITIES_ACTIVE_MAX]; // 148
    last_sighting_t lastSeen[ENTITIES_MAX]; // 152

    // State
    bool gameIsOver; // 156
    bool tileFlags[4]; // 157
    i8   entityCount; // 161

    u8 ticks; // 162
    bool freq2; // 163
    bool freq8_8; // 164
    bool freq16; // 165
    // Physics
    i8 gravity; // 166
    // 167 + padded = 168
} world_t;

#endif /* TYPES_H */