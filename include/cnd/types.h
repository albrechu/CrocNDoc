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
#include <cnd/defines.h>

/////////////////////////////////////////////////////////////////////////
//	Integral Types
//
typedef unsigned int      u8;
typedef signed   int      i8;
typedef unsigned long int u16;
typedef signed   long int i16;
typedef u8                event_t;
typedef u8                result_t;
typedef u8                tick_t;
typedef i8                bool;
typedef i8                idx_t;

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
enum Velocity_
{
    Velocity_Gravity    = 1,
    Velocity_Run        = 2,
    Velocity_MoonWalk   = 5,
    Velocity_Hit        = 6,
	Velocity_Friction   = 1,
    Velocity_Jump       = 10,
    Velocity_Breaching  = 12,
    Velocity_SwimUp     = 10,
    Velocity_KillUpWind = 10,
    Velocity_ThrowX     = 5,
    Velocity_ThrowY     = 8,
    Velocity_Jumper     = 12,
};
typedef i8 Velocity;

enum Event_
{
    Event_None,
    Event_Storm,
    Event_MoonWalk,
    Event_TheFloorIsLava,
    Event_IcyRoads,
    Event_YouAreBeingFollowed,
    Event_EarthQuake,
    Event_Tiny,
};
typedef i8 Event;

enum Hitbox_
{
    Hitbox_CrocY    = 20,
    Hitbox_CrocX    = 10,
    Hitbox_DocY     = 13,
    Hitbox_DocX     = 7,
    Hitbox_HalunkeY = 10,
    Hitbox_HalunkeX = 20,
    Hitbox_SchuftY = 12,
    Hitbox_SchuftX = -12,
    Hitbox_SchelmY = 10,
    Hitbox_SchelmX = 6,
    Hitbox_BanditY = 14,
    Hitbox_BanditX = 14,
};

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
    Character_Max,
};

/**
* @brief Enemy_ is a EntityType
*/
enum Enemy_
{
    Entity_Prepare,
    Entity_Croc,

    Prop_Barrel,
    Prop_Coin,
    // <add new props here>

    Prop_Max = Prop_Coin,
    Enemy_Halunke,
    Enemy_Gauner,
    Enemy_Schuft,
    Enemy_Schelm,
    Enemy_Bandit,
    Enemy_Boesewicht,
    //Enemy_Tunichtgut,
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
    EntityType_Max,
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
    Tile_Portal,
    Tile_Coin,
    Tile_GravitasUp,
    Tile_SpikesDown,
    Tile_Warning,
    Tile_NA,
    Tile_GravitasDown,
    Tile_Hole0,
    Tile_Hole1,
    Tile_Hole2,
    Tile_Hole3,
    // <Add New Tiles Here>
    Tile_Max,
    // Wormhole
    // Tile_Wormhole0,
    // Tile_Wormhole1,
    // Tile_Wormhole2,
    // Tile_Wormhole3,
};
typedef i8 Tile;

enum Note_
{
    G2 = 0x00U, GS2 = 0x01U, AB2 = 0x01U,
    A2 = 0x02U, AS2 = 0x03U, BB2 = 0x03U,
    BH = 0x04U,
    C3 = 0x05U, CS3 = 0x06U, DB3 = 0x06U,
    D3 = 0x07U, DS3 = 0x08U, EB3 = 0x08U,
    E3 = 0x09U, ES3 = 0x0AU, FB3 = 0x0AU,
    F3 = 0x0AU, FS3 = 0x0BU, GB3 = 0x0BU,
    G3 = 0x0CU, GS3 = 0x0DU, AB3 = 0x0DU,
    A3 = 0x0EU, AS3 = 0x0FU, BB3 = 0x0FU,
    B3 = 0x10U, BS3 = 0x11U,
    C4 = 0x11U, CS4 = 0x12U, DB4 = 0x12U,
    D4 = 0x13U, DS4 = 0x14U, EB4 = 0x14U,
    E4 = 0x15U, ES4 = 0x16U,
    F4 = 0x16U, FS4 = 0x17U, GB4 = 0x17U,
    G4 = 0x18U, GS4 = 0x19U, AB4 = 0x19U,
    A4 = 0x1AU, AS4 = 0x1BU, BB4 = 0x1BU,
    B4 = 0x1CU, BS4 = 0x1DU,
    C5 = 0x1DU, CS5 = 0x1EU, DB5 = 0x1EU,
    D5 = 0x1FU, DS5 = 0x20U, EB5 = 0x20U,
    E5 = 0x21U,
    F5 = 0x22U, FS5 = 0x23U, GB5 = 0x23U,
    G5 = 0x24U, GS5 = 0x25U, AB5 = 0x25U,
    A5 = 0x26U, AS5 = 0x27U, BB5 = 0x27U,
    B5 = 0x28U, BS5 = 0x29U, CB6 = 0x28U,
    C6 = 0x29U, CS6 = 0x2AU, DB6 = 0x2AU,
    D6 = 0x2BU, DS6 = 0x2CU, EB6 = 0x2CU,
    E6 = 0x2DU, ES6 = 0x2EU,
    F6 = 0x2EU, FS6 = 0x2FU, GB6 = 0x2FU,
    G6 = 0x30U, GS6 = 0x31U, AB6 = 0x31U,
    A6 = 0x32U, AS6 = 0x33U, BB6 = 0x33U,
    B6 = 0x34U, BS6 = 0x35U,
    C7 = 0x35U, CS7 = 0x36U, DB7 = 0x36U,
    D7 = 0x37U, DS7 = 0x38U, EB7 = 0x38U,
    E7 = 0x39U,
    F7 = 0x3AU, FS7 = 0x3BU, GB7 = 0x3BU,
    G7 = 0x3CU, GS7 = 0x3DU, AB7 = 0x3DU,
    A7 = 0x3EU, AS7 = 0x3FU, BB7 = 0x3FU,
    Note_End = 0x80U, CH0 = 0x80U, CH1 = 0x80U, CHN = 0x80U
};
typedef u8 Note;

enum WorldState_
{
    WorldState_Update,             // Entities are updated.
    WorldState_CollisionDetection, // Entities collision detection.
    WorldState_Next,               // World progression finished.
};
typedef i8 WorldState;

enum EntityStatus_
{
    EntityStatus_Inactive, // The entity is alive and well but takes not part in any calculations.
    EntityStatus_Active,   // The entity is currently active, takes part in the physics calculation and is possibly rendered.
    EntityStatus_Dead,     // May it rest in peace.
};
typedef i8 EntityStatus;

enum Material_
{
    Substance_Air           = 0, // Surrounded by air
    Substance_Water         = 1, // Surrounded by water
    Substance_GravitasAir   = 2, // Surrounded by gravitation inverting air
    Substance_GravitasWater = 3, // Surrounded by gravitation inverting water
};
typedef i8 Material;

enum Stage_
{
    Stage_Tutorial,
    Stage_Sewers,   
    Stage_Water,
    Stage_Gravitas,
};
typedef i8 Stage;

enum Score_
{
    Score_0    = 0,
    Score_50   = 1,
    Score_100  = 2,
    Score_200  = 4,
    Score_500  = 10,
    Score_1000 = 20,
};

enum GameState_
{
    GameState_Play,
    GameState_Died,
};
typedef i8 GameState;


typedef struct animation_t
{
    i8* keyframe;  // Current keyframe data of the animation played.
    i8  size;      // Size in bytes of a keyframe
    i8  remainder; // Remainder keyframes 
} animation_t;

/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef u8 handle;
#define HANDLE_INDEX_BITS   6
#define HANDLE_INVALID      0xFFu
#define HANDLE_VERSION_BITS ((sizeof(handle) * 8) - HANDLE_INDEX_BITS)
#define HANDLE_CREATE(idx, version) (((handle)(idx)) | (((handle)(version)) << HANDLE_INDEX_BITS))
#define HANDLE_INDEX(h) ((h) & (((handle)(1 << HANDLE_INDEX_BITS)) - 1))
#define HANDLE_VERSION(h) ((h) >> HANDLE_INDEX_BITS)
#define HANDLE_IS_CAMERA(h) (HANDLE_INDEX(h) == 0)

typedef struct entity_t entity_t, *entity;
typedef void (*update_t)(entity);
typedef void (*prefab_t)(entity);
typedef void (*normal_routine_t)(void);
typedef void (*stage_routine_t)(Stage);
struct entity_t
{
    // [[ Mandatory ]]
    // Hooks
    update_t    update; // Behavior and drawing function 
    update_t    kill;   // Kills the entity

    // Pose
    animation_t animation; 
    v2l         position;     // Position in world space.
    v2i         velocity;     // Speed measured in dots/tick.
	v2i         tile;         // Last calculated tile.
    v2i         tileAbsDelta; // Absolute delta tile distance from camera.
    bool        inLocalSpace; // Local space here refers to screen space.
    bool        isGrounded;   // On ground?

    // Entity info
    handle handle;
    handle reference; // Possibly contains the reference to an entity. (e.g. reference is an held object)
    idx_t  globalId;      
    bool   isEnemy; 
    bool   isAllocated;
    u8     score;

    // [[ Optional ]]
    i8          transform;   // x-direction the entity is facing. 1 for right, -1 for left
    v2i         hitbox;
    EntityType  type;        // May store the entity type 
    EntityState state;       // May store its state
    bool        isAttacking; 
    Material    substance;     
    i8          invincibilityTicks;
    i8          offGroundImpulseResponseTicks;
    i8          recoveryTicks;
    i8          data[2]; 
};
//static_assert(sizeof(entity_t) == 34);

typedef struct player_t
{
    Input    joystick;             // Last read joystick value.
    Action   action;   
    u16      score;                // Current player score (attempt).
    u8       stagesDone;
    u8       scoreGained;
    bool     isOtherCharacterDead; // Croc or Doc dead?
    i8       lives;                // Lives available
} player_t, *player;

typedef struct amplitudes_t
{
    u8 values[16];
} amplitudes_t, *amplitudes;

typedef struct frequencies_t
{
    u8 values[8];
} frequencies_t, *frequencies;

typedef struct music_t
{
    amplitudes_t const*  amplitudes;
    frequencies_t const* frequencies;
    Note notes[];
} music_t;

typedef struct sfx_t
{
    i8 enable;	 // noise (bits 5-3) and tone channel (bits 2-0) enable (0b00nnnccc), 
    i8 noise;	 // noise source sweep, =0 up, >0 down, <0 inhibit  
    i8 volume;	 // volume sweep, =0 up, >0 down, <0 inhibit
    u8 duration; // explosion duration, 0x01 longest to 0x80 shortest 
} sfx_t;

typedef struct sound_t
{
    music_t const* music;
    sfx_t   const* sfx[3];

    i8 soundEffects;
} sound_t;

typedef struct game_t
{
    procedure_t progress;  // Selected game procedure progressing the game.
    // Entities
    player_t player; // Attempt information
    // Game State
    Stage     stage;             // Selected stage
    Event     event;             // Selected event.
    GameState state;             // Selected state
    bool      isFinished;        // No lives left.
    i8        ticksUntilNewGame; // Ticks for game over timing.
    u8        ticksScoreGainedVisible;
} game_t, *game;

typedef struct last_sighting_t
{
    // v2l       position; // Last known position. Could have been implemented with a "look aside" list search while drawing. Got tossed.
    EntityStatus status;   // Last known state. Initially is EntityStatus_Alive.
} last_sighting_t, * last_sighting;

typedef struct level_t
{
    EntityType const* entities;      // Enemies and Props of the level.
    Tile       const* level;         // Tiles 
    v2i        const  startingTile;  // Tile the player starts at.
    Stage      const  adjacentStage; // Stage reachable through the portal.
} level_t, *level;

/**
* @brief free "entity" list
*/
typedef struct entity_list_t
{
    entity   iterator;
    entity_t entities[ENTITIES_ACTIVE_MAX];
    handle   free[ENTITIES_ACTIVE_MAX]; 
    handle   alive[ENTITIES_ACTIVE_MAX];
    i8       freeCount;
    i8       aliveCount;
} entity_list_t;

typedef struct world_t
{
    // Level information
    const Tile*    tiles; // Tiles of the selected level
    const level_t* level; // Selected Level
    entity_list_t  list;  // Entity allocation list (free list).
    // Entities
    //entity_t        entities[ENTITIES_ACTIVE_MAX];   // Contains active entities in the scene.
    //idx_t           entityIdxs[ENTITIES_ACTIVE_MAX]; // Contains indices into entities for easier swapping.
    last_sighting_t lastSeen[ENTITIES_MAX];          // Information about all entities in the scene.
    
    // State
    WorldState worldState;     // World state while drawing or post tile iteration.
    bool       gameIsOver;     // Game over flag to stop damaging hit collision.
    bool       tileFlags[4];   // Tile state flags. [0] = BarrierVertical, [1] = BarrierHorizontal, [2] = Coin, [3] = Reserved.
    //i8         entityCount;    // Active entities
    v2i        eyePosition;    // Position of the player characters eye. Yes its really done here! 

    u8   ticks;   // Iteration
    bool freq2;   // Impulse function at frequency 2 measured in 1/tick
    bool freq16;  // Impulse function at frequency 16 measured in 1/tick
    i8   freq8_8; // Impulse response at frequency 8 with a 3-bit value linearily increasing. Useful for animations.

    // Physics
    i8 gravity; // Earth attraction
    i8 wind;    // Wind strength
    i8 windPhase;
    i8 heat;
    i8 moon;
} world_t;
