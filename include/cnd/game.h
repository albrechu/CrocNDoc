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
#include <cnd/defines.h>
#include <cnd/types.h>
#include <cnd/world.h>

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define GAME   g_game
#define PLAYER GAME.player
#define BTNS   PLAYER.buttons
#define JOYS   PLAYER.joystick

/////////////////////////////////////////////////////////////////////////
//	Globals
//
extern game_t g_game;
extern const procedure_t g_render_table[];
extern const procedure_t g_update_table[];

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

void game_enter_stage(Stage stage);
void game_entity_added(entity e);

/////////////////////////////////////////////////////////////////////////
//	Action Functions
//
void action_update(void);

/////////////////////////////////////////////////////////////////////////
//	Rest Functions
//
void __stub(void);


#endif /* GAME_H */