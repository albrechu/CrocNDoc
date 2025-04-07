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

#ifndef GAME_H
#define GAME_H

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <defines.h>
#include <types.h>
#include <mesh.h>
#include <stage.h>

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define GAME   g_game
#define PLAYER GAME.player
#define BTNS   PLAYER.buttons
#define JOYS   PLAYER.joystick

/////////////////////////////////////////////////////////////////////////
//	Enumerations
//
enum Player_
{
    Player_ArmEdges = 4,
    // [[Hurtbox]]
    Player_HurtboxWidthIdle     = 15,
    Player_HurtboxHeightIdle    = 30,
    Player_HurtboxHeightDucking = 15,
    // [[Position]]
    Player_XMax              = 90,
	Player_YMax              = 100,
	Player_XMin              = -100,
	Player_YMin              = -55,
};

enum GameState_
{
    GameState_Finished,
    GameState_Play,
    GameState_Plot,
    GameState_Pause,
    GameState_GameOver,
};
typedef i8 GameState;

enum Velocity_
{
    Velocity_Run      =  3,
    Velocity_Bite     =  6,
    Velocity_Gravity  = -1,
	Velocity_Friction = -1,
    Velocity_Jump     = 12,
    Velocity_JumpX    = 1,
    Velocity_ThrowX   = 5,
    Velocity_ThrowY   = 8,

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

enum Character_
{
    Character_Croc,
    Character_Doc,
};
typedef i8 Character;

enum CharacterState_
{
    CharacterState_Idle,
    CharacterState_HoldsProp,
    CharacterState_Glide,
};
typedef i8 CharacterState;

enum Entity_
{
    Entity_Box,
    Entity_Barrel,
    Entity_Mantis,
};

/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef struct entity_t
{
    vec2 position;
    v2i  velocity;
    i8   transform;    
} entity_t, *entity;


typedef struct player_t
{
    vec2           camera; // Global position
    v2i            velocity;
    Input          buttons, joystick;
    Action         action;
    i8             transform;
    i8             hurtboxHeight;
    Mesh           mesh;
    Character      character;
    CharacterState state;
    i8             armY;
    i8             arm[13];
    i8             tail[9];
} player_t, *player;

typedef struct game_t
{
    // Entities
    player_t player;
    entity_t props[2];

    // Game State
    GameState state;
    Stage     stage;
    v2i       stageTile;
    u8        ticks;
} game_t, *game;

/////////////////////////////////////////////////////////////////////////
//	Globals
//
extern game_t g_game;

/////////////////////////////////////////////////////////////////////////
//	Game Functions
//
void game_init(void);
void game_soft_reset(void);
void game_start_frame(void);

force_inline void game_update(void);
void game_update_play(void);
void game_update_plot(void);

void game_render(void);
void game_render_play(void);
void game_render_plot(void);
void game_render_pause(void);
void game_render_gameover(void);

/////////////////////////////////////////////////////////////////////////
//	Action Functions
//
void action_update(void);

/////////////////////////////////////////////////////////////////////////
//	Input Functions
//
force_inline void input_fetch(void);

/////////////////////////////////////////////////////////////////////////
//	Rest Functions
//
void __stub(void);


#endif /* GAME_H */