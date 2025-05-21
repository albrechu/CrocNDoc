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
#include <cnd/world.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>
#include <vectrex.h>
#include <lib/print.h>
#include <lib/assert.h>
#include <lib/static_assert.h>
#include <lib/monitor.h>
#include <cnd/levels.h>
#include <cnd/entities.h>

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define POS(y, x) y, x // Starting position
#define TILE(y, x) y, x
#define METADATA_BYTES 4
#define TILE_FLAG(id) WORLD.tileFlags[id]
#define LBL(x) lbl_##x
#define W WORLD

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void __stub(void) {}

#define relative_x(e) I8(e->position.x - tX)
#define relative_y(e) I8(e->position.y - tY)
#define set_ground(e) \
{\
    e->velocity.y = 0;\
    e->isGrounded = true;\
}

void check_tiles(entity e)
{
	e->velocity.y -= WORLD.gravity;
    e->isGrounded = false;
    
    static const void* jump[] =
    {
        &&LBL(Tile_Air), 
        &&LBL(Tile_Water),
        &&LBL(Tile_Top2),
        &&LBL(Tile_Top),
        &&LBL(Tile_TopLeft),
        &&LBL(Tile_TopRight),
        &&LBL(Tile_Bottom2),
        &&LBL(Tile_Bottom),
        &&LBL(Tile_BottomLeft),
        &&LBL(Tile_BottomRight),
        &&LBL(Tile_Left2),
        &&LBL(Tile_Left),
        &&LBL(Tile_Right2),
        &&LBL(Tile_Right),
        &&LBL(Tile_MiddleLeftTop),
        &&LBL(Tile_MiddleLeft),
        &&LBL(Tile_MiddleRight),
        &&LBL(Tile_Middle),
        &&LBL(Tile_MiddleBottom),
        &&LBL(Tile_MiddleRightTop),
        &&LBL(Tile_MiddleTop),
        &&LBL(Tile_Spikes),
        &&LBL(Tile_SpikedBall),
        &&LBL(Tile_Jumper),
        &&LBL(Tile_BarrierVertical),
        &&LBL(Tile_BarrierHorizontal),
        &&LBL(Tile_WaterTop),
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&end,
        &&LBL(Tile_Portal0),
        &&LBL(Tile_Portal0),
        &&LBL(Tile_GravitasUp),
        &&LBL(Tile_SpikesDown),
        &&LBL(Tile_Warning),
        &&end,
        &&LBL(Tile_GravitasDown),
    };
    
    void* next = &&horizontal;

     // The current tile
    const i16 tileX = I16(e->tile.x);
    const i16 currentX = tileX << STRIDE_BITS;
    i16 tX = currentX;
    i16 tY = I16(e->tile.y) << STRIDE_BITS;
    i8 tile = WORLD.tiles[tY + tileX];
    goto *jump[tile];

    // The horizontal tile
horizontal:;
    volatile const i8 xAfter = I8((e->position.x + e->velocity.x) >> TILE_SCALE_BITS);
    if (xAfter != e->tile.x)
    {
        tX = I16(xAfter) << TILE_SCALE_BITS;
        next = &&vertical;
        tile = WORLD.tiles[tY + xAfter];
        goto *jump[tile];
    }
    // The vertical tile
vertical:;
    volatile const i8 yAfter = I8((e->position.y - e->velocity.y) >> TILE_SCALE_BITS);
    if (yAfter != e->tile.y)
    {
        tX = currentX;
        tY = I16(yAfter) << TILE_SCALE_BITS;
        next = &&adjust;
        tile = WORLD.tiles[tY + tileX];
        goto* jump[tile];
    }
adjust: // Adjust position and flags if necessary
    if (e->velocity.x)
    {
        e->position.x += I16(e->velocity.x);
        e->tile.x     = xAfter;
        e->velocity.x = 0;
    }

    if (e->velocity.y)
    {
        e->position.y -= I16(e->velocity.y);
        e->tile.y = yAfter;
    }
    
    i8 dy = e->tile.y - CAMERA.tile.y;
    const i8 dyMask = dy >> 7;
    dy = (dy ^ dyMask) - dyMask;
    i8 dx = e->tile.x - CAMERA.tile.x;
    const i8 dxMask = dx >> 7;
    dx = ((dx ^ dxMask) - dxMask);
    
    // Check if entity is local.
    e->isLocal = dx < 4 && dy < 4;
    if (!e->isLocal)
    {
        if (dx >= 6 || dy >= 6) // Possibly remove entity if too far away
        {
            entity_set_status(e, EntityStatus_Inactive);
        }
        e->isSameTile = false;
    }
    else
    {
        e->isSameTile = e->tile.x == CAMERA.tile.x && e->tile.y == CAMERA.tile.y;
    }
    return;

LBL(Tile_Air):
    if (e->id == 0 && e->substance != Substance_Air)
    {
        e->routine = e->type == Character_Croc ? routine_croc_air : routine_doc_air;
        e->substance = Substance_Air;
    }
    goto *next;
LBL(Tile_Water):
    if (e->id == 0 && e->substance != Substance_Water)
    {
        e->routine = e->type == Character_Croc ? routine_croc_water : routine_doc_water;
        e->substance = Substance_Water;
    }
    goto *next;
LBL(Tile_Top2):
    if (relative_y(e) - e->velocity.y <= 0)
        set_ground(e);
        /* fallthrough */
LBL(Tile_Top):
    if (relative_y(e) + e->velocity.y <= 0)
        set_ground(e);
    goto *next;
LBL(Tile_TopLeft):
    if (relative_y(e) + e->velocity.y <= 0)
        set_ground(e);
    if (relative_x(e) - e->velocity.x <= 0)
        e->velocity.x = 0;
    goto *next;
LBL(Tile_TopRight):
    if (relative_y(e) + e->velocity.y <= 0)
        set_ground(e);
    if (relative_x(e) - e->velocity.x >= TILE_WIDTH)
        e->velocity.x = 0;
    goto* next;
LBL(Tile_Bottom2):
    if (relative_y(e) - e->velocity.y >= TILE_HEIGHT)
        set_ground(e);
    goto* next;
LBL(Tile_Bottom):
    if (relative_y(e) + e->velocity.y >= TILE_HEIGHT)
        set_ground(e);
    goto *next;
LBL(Tile_BottomLeft):
    if (relative_y(e) - e->velocity.y >= TILE_HEIGHT)
        set_ground(e);
    if (relative_x(e) + e->velocity.x <= 0)
        e->velocity.x = 0;
    goto *next;
LBL(Tile_BottomRight):
    if (relative_y(e) - e->velocity.y >= TILE_HEIGHT)
        set_ground(e);
    if (relative_x(e) + e->velocity.x >= TILE_WIDTH)
        e->velocity.x = 0;
    goto *next;
LBL(Tile_Left2):
    if (relative_x(e) + e->velocity.x < 0)
        e->velocity.x = 0;
    goto* next;
LBL(Tile_Left):
    if (relative_x(e) + e->velocity.x >= 0)
        e->velocity.x = 0;
    goto *next;
LBL(Tile_Right2):
    if (relative_x(e) + e->velocity.x >= TILE_WIDTH)
        e->velocity.x = 0;
    goto* next;
LBL(Tile_Right):
    if (relative_x(e) - e->velocity.x >= TILE_WIDTH)
        e->velocity.x = 0;
    goto *next;
LBL(Tile_MiddleLeftTop):
    {
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y <= 0 && relativeY < TILE_HEIGHT_2 && relative_x(e) <= (TILE_WIDTH_2 + PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_MiddleLeft):
    {
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y >= TILE_HEIGHT_2 && relativeY < TILE_HEIGHT_2 && relative_x(e) <= (TILE_WIDTH_2 + PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_MiddleRight):
    {
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y >= TILE_HEIGHT_2 && relativeY < TILE_HEIGHT_2 && relative_x(e) >= (TILE_WIDTH_2 - PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_Middle):
    {
        const i8 relativeX = relative_x(e);
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y >= TILE_HEIGHT_2 && relativeY < TILE_HEIGHT_2 && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_MiddleBottom):
    {
        const i8 relativeX = relative_x(e);
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y >= TILE_HEIGHT_2 && relativeY < TILE_HEIGHT && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_MiddleRightTop):
    {
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y >= 0 && relativeY < 0 && relative_x(e) >= (TILE_WIDTH_2 - PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_MiddleTop):
    {
        const i8 relativeX = relative_x(e);
        const i8 relativeY = relative_y(e);
        if (relativeY - e->velocity.y >= 0 && relativeY < 0 && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_Spikes):
    if (relative_y(e) - e->velocity.y >= (TILE_HEIGHT >> 1))
    {
        set_ground(e);
        if (e->id > 0)
        {
            entity_set_status(e, EntityState_Dead);
        }
        else if (!WORLD.gameIsOver)
        {
            WORLD.playerDamage();
            CAMERA.invisiblityFrames = 0;
            WORLD.playerDamage();
            WORLD.gameIsOver = true;
        }
    }
    goto *next;
LBL(Tile_SpikedBall):
    {
        i8 nextX = relative_x(e) + e->velocity.x;
        i8 nextY = relative_y(e) + e->velocity.y;
        if (e->globalId == -1 && ((nextX >= ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3))) ||
            (nextY >= ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)))))
            WORLD.playerDamage();
    }
    goto *next;
LBL(Tile_Jumper):
{
    const i8 relativeX = relative_x(e);
    const i8 relativeY = relative_y(e);
    if (relativeY - e->velocity.y >= TILE_HEIGHT_2 && relativeY < TILE_HEIGHT && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
        e->velocity.y = Velocity_Jumper;
}
    goto *next;
LBL(Tile_BarrierVertical):
    if (!TILE_FLAG(0))
    {
        if (relative_x(e) + e->velocity.x >= TILE_WIDTH - 1)
        {
            if (e->id == 0 && e->stopwatch != 0)
                TILE_FLAG(0) = true;
            else
                e->velocity.x = 0;
        }
    }
    goto *next;
LBL(Tile_BarrierHorizontal):
    if (!TILE_FLAG(1))
    {
        if (relative_y(e) - e->velocity.y >= TILE_WIDTH)
        {
            if (e->id == 0)
            {
                if (e->velocity.y < -14 || e->velocity.y > 14) // Broke the barrier
                {
                    TILE_FLAG(1) = true;
                }
                else
                {
                    set_ground(e);
                }
            }
            else
            {
                set_ground(e);
            }
        }
    }
    goto *next;
LBL(Tile_WaterTop):
    if ((relative_y(e) + e->velocity.y >= TILE_HEIGHT - 1) && e->velocity.y > 0)
        e->velocity.y = Velocity_Breaching;
    goto *next;
LBL(Tile_Portal0):
    {
        i8 nextX = relative_x(e) + e->velocity.x;
        i8 nextY = relative_y(e) + e->velocity.y;
        if ((nextX >= ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3))) ||
            (nextY >= ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3))))
            WORLD.stageEntered(WORLD.level->adjacentStages[tile - Tile_Portal0]);
    }
    goto *next;
LBL(Tile_GravitasUp):
    if (e->id == 0 && e->substance < Substance_GravitasAir)
    {
        if (e->substance == Substance_Air)
        {
            e->routine = e->type == Character_Croc ? routine_croc_gravitas_air : routine_doc_gravitas_air;
            e->substance = Substance_GravitasAir;
        }
        else // Substance_GravitasWater
        {
            e->routine = e->type == Character_Croc ? routine_croc_gravitas_water : routine_doc_gravitas_water;
            e->substance = Substance_GravitasWater;
        }
        WORLD.gravity = -1;
    }
    goto* next;
LBL(Tile_SpikesDown):
    if (relative_y(e) - e->velocity.y < (TILE_HEIGHT >> 1))
    {
        set_ground(e);
        if (e->id > 0)
        {
            entity_set_status(e, EntityState_Dead);
        }
        else if (!WORLD.gameIsOver)
        {
            WORLD.playerDamage();
            CAMERA.invisiblityFrames = 0;
            WORLD.playerDamage();
            WORLD.gameIsOver = true;
        }
    }
    goto *next;
LBL(Tile_Warning):
    goto *next;
LBL(Tile_GravitasDown):
    if (e->id == 0 && e->substance >= Substance_GravitasAir)
    {
        if (e->substance == Substance_GravitasAir)
        {
            e->routine = e->type == Character_Croc ? routine_croc_air : routine_doc_air;
            e->substance = Substance_Air;
        }
        else
        {
            e->routine = e->type == Character_Croc ? routine_croc_water : routine_doc_water;
            e->substance = Substance_Water;
        }
        WORLD.gravity = 1;
    }
    goto *next;
end:
    goto *next;
}


#if 0 // This is the old routine. I kept it to change it back if things go south. (Even though I have version control)
void world_progress(void)
{
    ++WORLD.ticks;
    //WORLD.gravity = I8(WORLD.ticks & 1);

    for (i8 id = 0; id < WORLD.entityCount; ++id)
    {
        WORLD.selectedEntity = &WORLD.entities[WORLD.entityIdxs[id]];
        check_tiles();
    }

    v2i selectedTile;
    selectedTile.x = I8((CAMERA.position.x - 128) >> TILE_SCALE_BITS);
    selectedTile.y = I8((CAMERA.position.y - 128) >> TILE_SCALE_BITS) + 1;

    i8 xMax = selectedTile.x + 9;
    xMax = MIN8(xMax, WORLD_STRIDE);
    xMax = MAX8(0, xMax);
    i8 yMax = selectedTile.y + 8;
    yMax = MIN8(yMax, WORLD_EXTENT);
    yMax = MAX8(0, yMax);
    selectedTile.x = MAX8(0, selectedTile.x);
    selectedTile.y = MAX8(0, selectedTile.y);
    
    static const void* jump[] = 
    {
        &&end, // LBL(Tile_Air),
        &&end, // LBL(Tile_Water),
        &&LBL(Tile_Top),
        &&LBL(Tile_Top),
        &&LBL(Tile_TopLeft),
        &&LBL(Tile_TopRight),
        &&LBL(Tile_Bottom), 
        &&LBL(Tile_Bottom),
        &&LBL(Tile_BottomLeft),
        &&LBL(Tile_BottomRight),
        &&LBL(Tile_Left),
        &&LBL(Tile_Left),
        &&LBL(Tile_Right),
        &&LBL(Tile_Right),
        &&LBL(Tile_MiddleLeftTop),
        &&LBL(Tile_MiddleLeft),
        &&LBL(Tile_MiddleRight),
        &&LBL(Tile_Middle),
        &&LBL(Tile_MiddleBottom),
        &&LBL(Tile_MiddleRightTop),
        &&LBL(Tile_MiddleTop),
        &&LBL(Tile_Spikes),
        &&LBL(Tile_SpikedBall),
        &&LBL(Tile_Jumper),
        &&LBL(Tile_BarrierVertical),
        &&LBL(Tile_BarrierHorizontal),
        &&LBL(Tile_WaterTop),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_Portal0),
        &&LBL(Tile_Portal0),
    };

    i8 xMin = selectedTile.x;
    i8 idxDelta  = WORLD_STRIDE - (xMax - selectedTile.x) + 1;
    
    i16 tileLeft, tileRight, tileTop; //, tileBottom;
    i8 startX, deltaX;
    Tile const* tiles = WORLD.level + (I16(selectedTile.y) << STRIDE_BITS) + selectedTile.x;
    Tile tile = *tiles; 

    i16 firstTileLeft  = (I16(selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x;
    i16 firstTileRight = firstTileLeft + TILE_WIDTH;
    tileLeft  = firstTileLeft;
    tileRight = firstTileRight;
    tileTop   = CAMERA.position.y - (I16(selectedTile.y) << TILE_SCALE_BITS); // This one inverts the y axis

    goto *jump[tile];

LBL(Tile_Top): 
    if (tileTop <= 127 && tileTop >= -128)
    {
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = TILE_WIDTH + I8(tileLeft + 128);
        }
        else
        {
            startX = I8(tileLeft);
            deltaX = TILE_WIDTH;
        }
        beam_set_position(I8(tileTop), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_TopLeft):
    if (tileTop <= 127 && tileTop >= -128)
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = TILE_WIDTH + I8(tileLeft + 128);
        }
        else
        {
            startX = I8(tileLeft);
            deltaX = TILE_WIDTH;
        }
        beam_set_position(I8(tileTop), startX);
        Draw_Line_d(0, deltaX);
    }
    if (tileLeft <= 127 && tileLeft >= -128)
    {
        i8 startY, deltaY;
        if (tileTop > (I16(127) + TILE_HEIGHT)) // clip
        {
            startY = 127;
            deltaY = -TILE_HEIGHT + I8(tileTop - 127);
        }
        else
        {
            startY = I8(tileTop);
            deltaY = -TILE_HEIGHT;
        }
        beam_set_position(startY, I8(tileLeft));
        Draw_Line_d(deltaY, 0);
    }
    goto end;
LBL(Tile_TopRight):
    if (tileTop <= 127 && tileTop >= -128)
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = TILE_WIDTH + I8(tileLeft + 128);
        }
        else
        {
            startX = I8(tileLeft);
            deltaX = TILE_WIDTH;
        }
        beam_set_position(I8(tileTop), startX);
        Draw_Line_d(0, deltaX);
    }

    if (tileRight <= 127 && tileRight >= -128)
    {
        i8 startY, deltaY;
        if (tileTop > 127) // clip
        {
            startY = 127;
            deltaY = -TILE_HEIGHT + I8(tileTop - 127);
        }
        else
        {
            startY = I8(tileTop);
            deltaY = -TILE_HEIGHT;
        }
        beam_set_position(startY, I8(tileRight));
        Draw_Line_d(deltaY, 0);
    }
    goto end;
LBL(Tile_Bottom):
    if (tileTop <= (I16(127) + TILE_HEIGHT) && tileTop >= (-128 + TILE_HEIGHT))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = TILE_WIDTH + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft);
            deltaX = TILE_WIDTH;
        }
        beam_set_position(I8(tileTop - TILE_HEIGHT), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_BottomLeft):
    if (tileTop <= (I16(127) + TILE_HEIGHT) && tileTop >= (-128 + TILE_HEIGHT))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = TILE_WIDTH + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft);
            deltaX = TILE_WIDTH;
        }
        beam_set_position(I8(tileTop - TILE_HEIGHT), startX);
        Draw_Line_d(0, deltaX);
    }
    if (tileLeft <= 127 && tileLeft >= -128)
    {
        i8 startY, deltaY;
        if (tileTop > (I16(127) + TILE_HEIGHT)) // clip
        {
            startY = 127;
            deltaY = -TILE_HEIGHT + I8(tileTop - 127);
        }
        else
        {
            startY = I8(tileTop);
            deltaY = -TILE_HEIGHT;
        }
        beam_set_position(startY, I8(tileLeft));
        Draw_Line_d(deltaY, 0);
    }
    goto end;
LBL(Tile_BottomRight):
    if (tileTop <= (I16(127) + TILE_HEIGHT) && tileTop >= (-128 + TILE_HEIGHT))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = TILE_WIDTH + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft);
            deltaX = TILE_WIDTH;
        }
        beam_set_position(I8(tileTop - TILE_HEIGHT), startX);
        Draw_Line_d(0, deltaX);
    }
    if (tileRight <= 127 && tileRight >= -128)
    {
        i8 startY, deltaY;
        if (tileTop > 127) // clip
        {
            startY = 127;
            deltaY = -TILE_HEIGHT + I8(tileTop - 127);
        }
        else
        {
            startY = I8(tileTop);
            deltaY = -TILE_HEIGHT;
        }
        beam_set_position(startY, I8(tileRight));
        Draw_Line_d(deltaY, 0);
    }
    goto end;
LBL(Tile_Left):
    if (tileLeft <= 127 && tileLeft >= -128)
    {
        i8 startY, deltaY;
        if (tileTop > 127) // clip
        {
            startY = 127;
            deltaY = -TILE_HEIGHT + I8(tileTop - 127);
        }
        else
        {
            startY = I8(tileTop);
            deltaY = -TILE_HEIGHT;
        }
        beam_set_position(startY, I8(tileLeft));
        Draw_Line_d(deltaY, 0);
    }
    goto end;
LBL(Tile_Right):
    if (tileRight <= 127 && tileRight >= -128)
    {
        i8 startY, deltaY;
        if (tileTop > 127) // clip
        {
            startY = 127;
            deltaY = -TILE_HEIGHT + I8(tileTop - 127);
        }
        else
        {
            startY = I8(tileTop);
            deltaY = -TILE_HEIGHT;
        }
        beam_set_position(startY, I8(tileRight));
        Draw_Line_d(deltaY, 0);
    }
    goto end;
LBL(Tile_MiddleLeftTop):
    if (tileTop <= 127 && tileTop >= -128)
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft);
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position(I8(tileTop), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_MiddleLeft):
    if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft);
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position(I8(tileTop - (TILE_HEIGHT >> 1)), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_MiddleRight):
    if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft + (TILE_WIDTH >> 1));
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position(I8(tileTop - (TILE_HEIGHT >> 1)), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_Middle):
    if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft + (TILE_WIDTH >> 2));
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position(I8(tileTop - (TILE_HEIGHT >> 1)), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_MiddleBottom):
    if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft + (TILE_WIDTH >> 2));
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position(I8(tileTop - TILE_HEIGHT), startX);
        Draw_Line_d(0, deltaX);
    }
    goto end;
LBL(Tile_MiddleRightTop):
    if (tileTop <= 127 && tileTop >= -128)
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft + (TILE_WIDTH >> 1));
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position((i8)tileTop, startX);
        Draw_Line_d(0, deltaX);    
    }
    goto end;
LBL(Tile_MiddleTop):
    if (tileTop <= 127 && tileTop >= -128)
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft + (TILE_WIDTH >> 2));
            deltaX = (TILE_WIDTH >> 1);
        }
        beam_set_position((i8)tileTop, startX);
        Draw_Line_d(0, deltaX);    
    }
    goto end;
LBL(Tile_Spikes):
    if (tileTop <= (I16(127) + TILE_HEIGHT) && tileTop >= (-128 + TILE_HEIGHT))
    {
        i8 startX, deltaX;
        if (tileLeft < -128) // clip horizontally
        {
            startX = -128;
            deltaX = (TILE_WIDTH >> 2) + I8(tileLeft + 128);
        }
        else 
        {
            startX = I8(tileLeft);
            deltaX = (TILE_WIDTH >> 2);
        }
        beam_set_position(I8(tileTop - TILE_HEIGHT), startX);
        Draw_Line_d(TILE_HEIGHT, deltaX);
        Draw_VLc((void* const)spikes);
    }
    else // clip vertically
    {
        i8 startX, deltaX;
        i8 dy  = I8(tileTop + TILE_HEIGHT);
        i8 dy4 = dy >> 2; // dy / 4 as the slope m = 64 / 16. The starting position and change in X is affected by m.
        if (tileLeft < -128) // clip horizontally
        {
            startX = I8(-128) - dy4;
            deltaX = (TILE_WIDTH >> 2) + I8(tileLeft + 128) + dy4;
        }
        else 
        {
            startX = I8(tileLeft) - dy4;
            deltaX = (TILE_WIDTH >> 2) + dy4;
        }
        beam_set_position(-128, startX);
        Draw_Line_d(TILE_HEIGHT + dy, deltaX);
        Draw_VLc((void* const)spikes);
    }
    goto end;
LBL(Tile_SpikedBall):
    {
        beam_set_position(I8(tileTop - (TILE_HEIGHT >> 1)), I8(tileLeft + (TILE_HEIGHT >> 2)));
        Draw_VLc((void* const)spikedBall);
    }
    goto end;
LBL(Tile_Jumper):
    if (tileTop <= (I16(127) + TILE_HEIGHT) && tileTop >= (-128 + TILE_HEIGHT))
    {
        beam_set_position(I8(tileTop - TILE_HEIGHT), I8(tileLeft + (TILE_WIDTH >> 2)));
        Draw_VLc((void* const)jumper);
    }
    goto end;
LBL(Tile_BarrierVertical):
    if (!TILE_FLAG(0) && tileTop <= 127 && tileRight >= -128 && tileRight <= 127)
    {
        beam_set_position(I8(tileTop), I8(tileRight));
        Draw_VLc((void* const)barrierVertical);
    }
    goto end;
LBL(Tile_BarrierHorizontal):
    if (!TILE_FLAG(1) && tileTop <= (I16(127) + TILE_HEIGHT) && tileTop >= (-128 + TILE_HEIGHT) && tileRight >= -128 && tileRight <= 127)
    {
        beam_set_position(I8(tileTop - TILE_HEIGHT), I8(tileRight));
        Draw_VLc((void* const)barrierHorizontal);
    }
    goto end;
LBL(Tile_WaterTop):
    if (tileTop <= 127 && tileLeft >= -128 && tileLeft <= 127)
    {
        beam_set_position(I8(tileTop), I8(tileLeft));
        Draw_VLc((void* const)watertop[WORLD.freq8_8]);
    }
    goto end;
LBL(Tile_E0):
    tile -= Tile_E0;
    last_sighting last = &WORLD.lastSeen[tile];
    if (last->status == EntityStatus_Inactive)
    {
        last->status = EntityStatus_Active;
        entity_create_named(tile, selectedTile);
    }
    goto end;
LBL(Tile_Portal0):
{
    i8 tick = I8(WORLD.freq8_8);
    const i8* p = portal[tick];
    i16 tileCenterY = tileTop - (TILE_HEIGHT >> 1) + p[1];
    i16 tileCenterX = tileLeft + (TILE_WIDTH >> 1) + p[2];
    if (tileCenterY <= 127 && tileCenterY >= -128 && tileCenterX <= 127 && tileCenterX >= -128)
    {
        beam_set_position(I8(tileCenterY), I8(tileCenterX));
        Draw_VLc((void* const)p);
    }
}

end:
    if (++selectedTile.x < xMax)
    {
        tile = *++tiles;
        tileLeft   = tileRight;
        tileRight  = tileLeft + TILE_WIDTH;
        goto *jump[tile];
    }
    else if (++selectedTile.y < yMax)
    {
        selectedTile.x = xMin; // Reset to start of row
        tile = *(tiles += idxDelta);
        tileLeft   = firstTileLeft;
        tileRight  = firstTileRight;
        tileTop    -= TILE_HEIGHT; 
        goto *jump[tile];
    }
}


#else

#define draw_top() \
{ \
    if (tt > tb) \
    { \
        if (tr > tl) \
        { \
            startX = tl; \
            deltaX = TILE_WIDTH; \
        } \
        else \
        { \
            startX = -128; \
            deltaX = I8(-128) - tr; \
        } \
        beam_set_position(tt, startX); \
        Draw_Line_d(0, deltaX); \
    }\
}

#define draw_bottom() \
{ \
    if (tt > tb) \
    { \
        if (tr > tl) \
        { \
            startX = tl; \
            deltaX = TILE_WIDTH; \
        } \
        else \
        { \
            startX = -128; \
            deltaX = I8(-128) - tr; \
        } \
        beam_set_position(tb, startX); \
        Draw_Line_d(0, deltaX); \
    } \
}

#define draw_left() \
{ \
    if (tr > I8(U8(tr) - TILE_WIDTH)) \
    { \
        if (96 > I8(U8(tt) - TILE_HEIGHT)) \
        { \
            startY = I8(tt); \
            deltaY = -TILE_HEIGHT; \
        } \
        else \
        { \
            startY = 127; \
            deltaY = -(I8(-128) - tt); \
        } \
        beam_set_position(startY, I8(tl)); \
        Draw_Line_d(deltaY, 0); \
    } \
}

#define draw_right() \
{ \
    if (tr > tl) \
    { \
        if (tt > tb) \
        { \
            startY = I8(tt); \
            deltaY = -TILE_HEIGHT; \
        } \
        else \
        { \
            startY = 127; \
            deltaY = -(I8(-128) - tt); \
        } \
        beam_set_position(startY, I8(tr)); \
        Draw_Line_d(deltaY, 0); \
    } \
}

#define draw_middle_left_top() \
{ \
    if (tt > tb) \
    { \
        if (tr > tl) \
        { \
            startX = tl; \
            deltaX = (TILE_WIDTH >> 1); \
        } \
        else \
        { \
            startX = -128; \
            deltaX = I8(-128) - tr; \
        } \
        beam_set_position(tt, startX); \
        Draw_Line_d(0, deltaX); \
    } \
} 


#define draw_spikes() \
{ \
    if (tr > tl && tt > tb) \
    { \
        beam_set_position(tb, tl); \
        Draw_VLc((void* const)spikes); \
    } \
}

//#define draw_spikes() 
//{ 
//    i8 clippedTL = I8(U8(tr) - TILE_WIDTH_3_4); 
//    if (tr > clippedTL) 
//    { 
//        i8 clippedTB = I8(U8(tt) - TILE_HEIGHT); 
//        if (tt > clippedTB) 
//        { 
//            startX = tl; 
//            deltaX = TILE_WIDTH_4; 
//            startY = tb; 
//            deltaY = TILE_HEIGHT; 
//        } 
//        else 
//        { 
//            clippedTB += I8(-128); 
//            deltaY = TILE_HEIGHT + clippedTB; 
//            startY = I8(-128); 
//            clippedTB >>= 2; 
//            deltaX = TILE_WIDTH_4 + clippedTB; 
//            startX = tl - clippedTB; 
//        } 
//        beam_set_position(startY, startX); 
//        Draw_Line_d(deltaY, deltaX); 
//        Draw_VLc((void* const)spikes); 
//    } 
//    else 
//    {
//        i8 clippedTB = I8(U8(tt) - TILE_HEIGHT);
//        if (tt > clippedTB)
//        {
//            clippedTL += I8(-128);
//            startX = -128;
//            deltaX = tr - clippedTL;
//            clippedTL >>= 2;
//            startY = tb - clippedTL;
//            deltaY = TILE_HEIGHT + clippedTL;
//        }
//        else 
//        {
//            
//        }
//        beam_set_position(startY, startX);
//        Draw_Line_d(deltaY, deltaX);
//        Draw_VLc((void* const)spikes);
//    }
//}

#define draw_middle_left() \
{ \
    if (tr > tl) \
    { \
        i8 centerY = I8(U8(tt) - TILE_HEIGHT_2); \
        if (tt > centerY) \
        { \
            startX = tl; \
            deltaX = TILE_WIDTH_2; \
            beam_set_position(centerY, startX); \
            Draw_Line_d(0, deltaX); \
        } \
    } \
}

#define draw_middle_right() \
{ \
    i8 centerX = I8(U8(tl) + TILE_WIDTH_2); \
    if (tr > centerX) \
    { \
        i8 centerY = I8(U8(tt) - TILE_HEIGHT_2); \
        if (tt > centerY) \
        { \
            beam_set_position(centerY, centerX); \
            Draw_Line_d(0, TILE_WIDTH_2); \
        } \
    }\
}

#define draw_middle() \
{ \
    i8 centerX = I8(U8(tl) + TILE_WIDTH_4); \
    if (tr > centerX) \
    { \
        i8 centerY = I8(U8(tt) - TILE_HEIGHT_2); \
        if (tt > centerY) \
        { \
            beam_set_position(centerY, centerX); \
            Draw_Line_d(0, TILE_WIDTH_2); \
        } \
    }\
}

#define draw_middle_bottom() \
{ \
    i8 centerX = I8(U8(tl) + TILE_WIDTH_4); \
    if (tr > centerX) \
    { \
        if (tt > tb) \
        { \
            beam_set_position(tb, centerX); \
            Draw_Line_d(0, TILE_WIDTH_2); \
        } \
    }\
}

#define draw_middle_left_bottom() \
{ \
    if (tt > tb) \
    { \
        if (tr > tl) \
        { \
            startX = tl; \
            deltaX = (TILE_WIDTH >> 1); \
        } \
        else  \
        { \
            startX = -128; \
            deltaX = I8(-128) - tr; \
        } \
        beam_set_position(tb, startX); \
        Draw_Line_d(0, deltaX); \
    } \
}

#define draw_middle_right_top() \
{ \
    if (tt > tb) \
    { \
        if (tr > tl) \
        { \
            startX = tr - (TILE_WIDTH >> 1); \
            deltaX = TILE_WIDTH >> 1; \
        } \
        else \
        { \
            startX = -128; \
            deltaX = I8(-128) - tr; \
        } \
        beam_set_position(tt, startX); \
        Draw_Line_d(0, deltaX); \
    } \
}

#define draw_middle_top() \
{ \
    if (tt > tb) \
    { \
        if (tr > tl) \
        { \
            startX = tl + (TILE_WIDTH >> 2); \
            deltaX = TILE_WIDTH >> 1; \
        } \
        else \
        { \
            startX = -128; \
            deltaX = I8(-128) - tr; \
        } \
        beam_set_position(tt, startX); \
        Draw_Line_d(0, deltaX); \
    } \
}

#define draw_spiked_ball() \
{ \
    i8 centerY = tb + (TILE_HEIGHT >> 1); \
    i8 centerX = tl + (TILE_WIDTH >> 1); \
    if (centerY < tt && centerX < tr) \
    { \
        beam_set_position(centerY, centerX); \
        Draw_VLc((void* const)spikedBall); \
    } \
}

#define draw_jumper() \
{ \
    i8 x = tl + (TILE_WIDTH >> 2); \
    if (tb < tt && x < tr) \
    { \
        beam_set_position(tb, x); \
        Draw_VLc((void* const)jumper); \
    } \
}

#define draw_barrier_vertical() \
{ \
    if (!TILE_FLAG(0) && tt > tb && tr > tl) \
    { \
        beam_set_position(tt, tr); \
        Draw_VLc((void* const)barrierVertical); \
    } \
}

#define draw_barrier_horizontal() \
{ \
    if (!TILE_FLAG(1) && tt > tb && tr > tl) \
    { \
        beam_set_position(tb, tr); \
        Draw_VLc((void* const)barrierHorizontal); \
    } \
}

#define draw_water_top() \
{ \
    if (tt > tb) \
    { \
        beam_set_position(tt, tl); \
        Draw_VLc((void* const)watertop[WORLD.freq8_8]); \
    } \
}

#define draw_portal() \
{ \
    i8 centerY = tb + (TILE_HEIGHT >> 1); \
    i8 centerX = tl + (TILE_WIDTH >> 1); \
    if (centerY < tt && centerX < tr) \
    { \
        beam_set_position(centerY, centerX); \
        Draw_VLc((void* const)portal[WORLD.freq8_8]); \
    } \
}

#define draw_gravitas_up() \
{ \
    if (tt > tb) \
    { \
        beam_set_position(tb, tl); \
        Draw_VLc((void* const)gravitasUp); \
    } \
}

#define draw_gravitas_down() \
{ \
    if (tt > tb) \
    { \
        beam_set_position(tt, tl); \
        Draw_VLc((void* const)gravitasDown); \
    } \
}

#define draw_spikes_down() \
{ \
    if (tt > tb) \
    { \
        beam_set_position(tt, tl); \
        Draw_VLc((void* const)spikesDown); \
    } \
}

void world_progress(void)
{
    ++WORLD.ticks;
    WORLD.freq2   = I8(WORLD.ticks & 1);
    WORLD.freq16  = I8((WORLD.ticks >> 4) & 1);
    WORLD.freq8_8 = I8((WORLD.ticks >> 3) & 7);

    WORLD.entities[0].collision(&WORLD.entities[0]);
    WORLD.entities[1].collision(&WORLD.entities[1]);
    WORLD.entities[2].collision(&WORLD.entities[2]);
    WORLD.entities[3].collision(&WORLD.entities[3]);
    /*WORLD.entities[4].collision(&WORLD.entities[4]);
    WORLD.entities[5].collision(&WORLD.entities[5]);*/


   /* for (i8 id = 0; id < ENTITIES_ACTIVE_MAX; ++id)
    {
        WORLD.selectedEntity = &WORLD.entities[WORLD.entityIdxs[id]];
        
    }
    print_signed_int(100, 50, WORLD.entityCount);*/

    /*i8 overX = I8(CAMERA.position.x & (TILE_WIDTH - 1));
    i8 overY = I8(CAMERA.position.y & (TILE_HEIGHT - 1));*/

    v2i selectedTile;
    i8 xMin = I8(CAMERA.position.x >> TILE_SCALE_BITS);
    xMin -= 4;
    selectedTile.x = xMin;
    i8 yMin = I8(CAMERA.position.y >> TILE_SCALE_BITS);
    yMin -= 4;
    selectedTile.y = yMin + 8;
    yMin = MAX8(0, yMin);
    selectedTile.y = MIN8(selectedTile.y, (WORLD_EXTENT - 1));

    xMin = MAX8(0, xMin);

    i8 xMax = selectedTile.x + 8;
    xMax = MIN8(xMax, (WORLD_STRIDE));
    xMax = MAX8(0, xMax);

    selectedTile.x = MAX8(0, selectedTile.x);

    static const void* jump[] =
    {
        &&end, // Tile_Air
        &&end, // Tile_Water
        &&LBL(Tile_Top),
        &&LBL(Tile_Top),
        &&LBL(Tile_TopLeft),
        &&LBL(Tile_TopRight),
        &&LBL(Tile_Bottom),
        &&LBL(Tile_Bottom),
        &&LBL(Tile_BottomLeft),
        &&LBL(Tile_BottomRight),
        &&LBL(Tile_Left),
        &&LBL(Tile_Left),
        &&LBL(Tile_Right),
        &&LBL(Tile_Right),
        &&LBL(Tile_MiddleLeftTop),
        &&LBL(Tile_MiddleLeft),
        &&LBL(Tile_MiddleRight),
        &&LBL(Tile_Middle),
        &&LBL(Tile_MiddleBottom),
        &&LBL(Tile_MiddleRightTop),
        &&LBL(Tile_MiddleTop),
        &&LBL(Tile_Spikes),
        &&LBL(Tile_SpikedBall),
        &&LBL(Tile_Jumper),
        &&LBL(Tile_BarrierVertical),
        &&LBL(Tile_BarrierHorizontal),
        &&LBL(Tile_WaterTop),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_E0),
        &&LBL(Tile_Portal0),
        &&LBL(Tile_Portal0),
        &&LBL(Tile_GravitasUp),
        &&LBL(Tile_SpikesDown),
        &&LBL(Tile_Warning),
        &&end,
        &&end,
        &&LBL(Tile_GravitasDown),
    };

    i8 idxDelta = (1 << STRIDE_BITS) + (xMax - xMin) - 1;

    Tile const* tiles = WORLD.tiles + (I16(selectedTile.y) << STRIDE_BITS) + selectedTile.x;
    Tile tile = *tiles;

    i8 tt, tb, tl, tr;
    i8 startX = 0, deltaX = 0;
    i8 startY = 0, deltaY = 0;

    tr = I8((I16(selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x + TILE_WIDTH);
    tl = tr - TILE_WIDTH;
    tt = I8(CAMERA.position.y - (I16(selectedTile.y) << TILE_SCALE_BITS));
    tb = tt - TILE_HEIGHT;

    i8 firstTL = tl;
    i8 firstTR = tr;
    //assert(tt < 0 && tb > 0)



    goto *jump[tile];

LBL(Tile_Top):
    draw_top();
    goto end;
LBL(Tile_TopLeft):
    draw_top();
    draw_left();
    goto end;
LBL(Tile_TopRight):
    draw_top();
    draw_right();
    goto end;
LBL(Tile_Bottom):
    draw_bottom();
    goto end;
LBL(Tile_BottomLeft):
    draw_bottom();
    draw_left();
    goto end;
LBL(Tile_BottomRight):
    draw_bottom();
    draw_right();
    goto end;
LBL(Tile_Left):
    draw_left();
    goto end;
LBL(Tile_Right):
    draw_right();
    goto end;
LBL(Tile_MiddleLeft):
    draw_middle_left();
    goto end;
LBL(Tile_MiddleRight):
    draw_middle_right();
    goto end;
LBL(Tile_Middle):
    draw_middle();
    goto end;
LBL(Tile_MiddleBottom) :
    draw_middle_bottom();
    goto end;
LBL(Tile_MiddleRightTop) :
    draw_middle_right_top();
    goto end;
LBL(Tile_MiddleTop) :
    draw_middle_top();
    goto end;
LBL(Tile_MiddleLeftTop):
    draw_middle_left_top();
    goto end;
LBL(Tile_Spikes):
    draw_spikes();
    goto end;
LBL(Tile_SpikedBall):
    draw_spiked_ball();
    goto end;
LBL(Tile_Jumper):
    draw_jumper();
    goto end;
LBL(Tile_BarrierVertical):
    draw_barrier_vertical();
    goto end;
LBL(Tile_BarrierHorizontal):
    draw_barrier_horizontal();
    goto end;
LBL(Tile_WaterTop):
    draw_water_top();
    goto end;
LBL(Tile_E0):
if (WORLD.entityCount < ENTITIES_ACTIVE_MAX)
{
    tile -= Tile_E0;
    last_sighting last = &WORLD.lastSeen[tile];
    if (last->status == EntityStatus_Inactive)
    {
        last->status = EntityStatus_Active;
        entity_create_named(tile, selectedTile);
    }
}
    goto end;
LBL(Tile_Portal0):
    draw_portal();
    goto end;
LBL(Tile_GravitasUp):
    draw_gravitas_up()
    goto end;
LBL(Tile_SpikesDown) :
    draw_spikes_down();
    goto end;
LBL(Tile_Warning):
    goto end;
LBL(Tile_GravitasDown):
    draw_gravitas_down();
    goto end;
end:
    if ((++selectedTile.x) < xMax)
    {
        tile = *(++tiles);
        tl = tr;
        tr += TILE_WIDTH;
        goto *jump[tile];
    }
    else if ((--selectedTile.y) >= yMin)
    {
        selectedTile.x = xMin; // Reset to start of row
        tile = *(tiles -= idxDelta);
        tl = firstTL;
        tr = firstTR;
        tb = tt;
        tt += TILE_HEIGHT;
        goto* jump[tile];
    }
}

#endif

void __routine_stub(entity e) { (void)e; }

void entity_set_status(entity e, EntityStatus status)
{
    if (e->globalId >= 0)
    {
        WORLD.lastSeen[e->globalId].status = status;
    } // else Entity is unknown, so just destroy it. (e.g. projectiles)

    const idx_t last = WORLD.entityCount - 1;
    if (last != e->id) // Swap with last if not last
    {
        i8 tmp = WORLD.entityIdxs[e->id];
        WORLD.entityIdxs[e->id] = WORLD.entityIdxs[last];
        WORLD.entityIdxs[last] = tmp;
    }
    WORLD.entityCount = last;
    e->routine   = __routine_stub;
    e->collision = __routine_stub;
    e->state = EntityState_Dead;
}

void entity_create_named(idx_t const globalId, v2i const tile)
{
    entity e      = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount]];
    e->id         = WORLD.entityCount++;
    e->globalId   = globalId;
    e->tile.y     = tile.y;
    e->tile.x     = tile.x;
    e->position.y = (I16(tile.y) << TILE_SCALE_BITS) - (TILE_HEIGHT >> 1);
    e->position.x = (I16(tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1);
    e->velocity.x = 0, e->velocity.y = 0;
    e->type       = WORLD.level->entities[globalId];
    e->isEnemy    = e->type > Prop_Max;
    e->isLocal    = true;
    e->isSameTile = false;
    e->isGrounded = false;
    e->transform  = 0;
    e->collision  = check_tiles;
    WORLD.entityAdded(e);
}

void entity_create_anonymous(EntityType const type, v2i const tile)
{
    if (WORLD.entityCount < ENTITIES_ACTIVE_MAX)
    {
        entity e      = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount]];
        e->id         = WORLD.entityCount++;
        e->globalId   = -1;
        e->tile.y     = tile.y;
        e->tile.x     = tile.x;
        e->position.y = (I16(tile.y) << TILE_SCALE_BITS) - (TILE_HEIGHT >> 1);
        e->position.x = (I16(tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1);
        e->velocity.x = 0, e->velocity.y = 0;
        e->type       = type;
        e->isEnemy    = e->type > Prop_Max;
        e->isLocal    = true;
        e->isSameTile = false;
        e->isGrounded = false;
        e->transform  = 0;
        e->collision  = check_tiles;
        WORLD.entityAdded(e);
    }
}

void world_create(Stage const stage)
{
    MEMZERO(WORLD);
    WORLD.level       = &levels[stage];
    WORLD.tiles       = WORLD.level->level;
    WORLD.entityCount = 1; // 1 for the player
    for (idx_t idx = 0; idx < ENTITIES_ACTIVE_MAX; ++idx)
    {
        WORLD.entities[idx].id        = idx;
        WORLD.entities[idx].globalId  = -1;
        WORLD.entities[idx].routine   = __routine_stub;
        WORLD.entities[idx].collision = __routine_stub;
        WORLD.entityIdxs[idx]         = idx;
    }

    CAMERA.tile.y     = WORLD.level->startingTile.y;
    CAMERA.tile.x     = WORLD.level->startingTile.x;
    CAMERA.position.y = (I16(CAMERA.tile.y) << TILE_SCALE_BITS)  - (TILE_HEIGHT >> 1);
    CAMERA.position.x = (I16(CAMERA.tile.x) << TILE_SCALE_BITS)  + (TILE_WIDTH >> 1);
    CAMERA.id         = 0; // Player is always at 0
    CAMERA.globalId   = -1; // Player does not have an id
    CAMERA.collision = check_tiles;
}
