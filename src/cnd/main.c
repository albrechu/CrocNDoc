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

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/globals.h>
#include <cnd/game.h>
#include <vectrex.h>
#include <cnd/track.h>

/////////////////////////////////////////////////////////////////////////
//	>> Entry <<
//	
int main()
{
	// (Re-)Configure game 
soft_reset:
	game_soft_reset();
	
	/**
	* Game Loop
	*/
	while(!GAME.isFinished)
	{
        /**
        * Synchronize frame and music
        */
        DP_to_C8();
        Vec_Music_Flag |= 1;
		
		//GAME.explosion = &g_monsterPeng;
        Init_Music_chk((void* const)GAME.track);
        Explosion_Snd((void* const)GAME.explosion);
        Wait_Recal(); // Synchronize to frame
        Do_Sound();

        /**
        * Read Input
        */
        Intensity_5F(); 
        Read_Btns();    // Read buttons of controller

		/**	
		 * Update (entity updates, kinematics, collision detection, draw, ...)
		 */
		GAME.progress();
	}
	// Reset game
	goto soft_reset;
}
