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
#include <cnd/game.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define POS(y, x) y, x // Starting position
#define TILE(y, x) y, x
#define METADATA_BYTES 4
#define TILE_FLAG(id) WORLD.tileFlags[id]
#define LBL(x) lbl_##x
#define W WORLD
#define draw_line(y, x) Draw_Line_d2(y, x)

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
        &&LBL(Tile_Portal),
        &&LBL(Tile_Coin),
        &&LBL(Tile_GravitasUp),
        &&LBL(Tile_SpikesDown),
        &&LBL(Tile_Warning),
        &&end,
        &&LBL(Tile_GravitasDown),
        &&LBL(Tile_Hole0),
        &&LBL(Tile_Hole0),
        &&LBL(Tile_Hole0),
        &&LBL(Tile_Hole0),
    };
    
    void* next = &&horizontal;

     // The current tile
    const i16 tileX = I16(e->tile.x);
    const i16 currentX = tileX << STRIDE_BITS;
    i16 tX = currentX;
    i16 tY = I16(e->tile.y) << STRIDE_BITS;
    i8 tile = WORLD.tiles[tY + tileX];
    goto *jump[tile];

horizontal:; // The horizontal tile
    volatile const i8 xAfter = I8((e->position.x + e->velocity.x) >> TILE_SCALE_BITS);
    if (xAfter != e->tile.x)
    {
        tX = I16(xAfter) << TILE_SCALE_BITS;
        next = &&vertical;
        tile = WORLD.tiles[tY + xAfter];
        goto *jump[tile];
    }
vertical:; // The vertical tile
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
    e->inLocalSpace = dx < 4 && dy < 4;
    if (!e->inLocalSpace)
    {
        if (dx > 5 || dy > 5) // Possibly remove entity if too far away
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
    if (HANDLE_IS_CAMERA(e->handle) && (e->substance & Substance_Water) == Substance_Water)
    {
        switch (e->substance)
        {
        case Substance_Water:
            e->substance   = Substance_Air;
            e->isAttacking = false;
            e->update      = e->type == Character_Croc ? update_croc_air : update_doc_air;
            break;
        case Substance_GravitasWater:
            e->substance   = Substance_GravitasAir;
            e->isAttacking = false;
            e->update      = e->type == Character_Croc ? update_croc_gravitas_air : update_doc_gravitas_air;
            break;
        default:
            break;
        }
    }
    goto *next;
LBL(Tile_Water):
    if (HANDLE_IS_CAMERA(e->handle) && (e->substance & Substance_Water) != Substance_Water)
    {
        switch (e->substance)
        {
        case Substance_Air:
            e->substance   = Substance_Water;
            e->isAttacking = false;
            e->update      = e->type == Character_Croc ? update_croc_water : update_doc_water;
            break;
        case Substance_GravitasAir:
            e->substance   = Substance_GravitasWater;
            e->isAttacking = false;
            e->update      = e->type == Character_Croc ? update_croc_gravitas_water : update_doc_gravitas_water;
            break;
        default:
            break;
        }
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
    goto *next;
LBL(Tile_Bottom2):
    if (relative_y(e) - e->velocity.y >= TILE_HEIGHT)
        set_ground(e);
    goto *next;
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
    goto *next;
LBL(Tile_Left):
    if (relative_x(e) + e->velocity.x >= 0)
        e->velocity.x = 0;
    goto *next;
LBL(Tile_Right2):
    if (relative_x(e) + e->velocity.x >= TILE_WIDTH)
        e->velocity.x = 0;
    goto *next;
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
        if (relative_y(e) - e->velocity.y >= TILE_HEIGHT && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
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
        if (relative_y(e) + e->velocity.y <= 0 && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
            set_ground(e);
    }
    goto *next;
LBL(Tile_Spikes):
    if (relative_y(e) - e->velocity.y >= (TILE_HEIGHT >> 1))
    {
        set_ground(e);
        if (!HANDLE_IS_CAMERA(e->handle))
        {
            e->update = update_death;
        }
        else if (!WORLD.gameIsOver)
        {
            character_damage();
            CAMERA.invincibilityTicks = false;
            character_damage();
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
            character_damage();
    }
    goto *next;
LBL(Tile_Jumper):
{
    const i8 relativeX = relative_x(e);
    const i8 relativeY = relative_y(e);
    if (relativeY - e->velocity.y >= TILE_HEIGHT_2 && relativeY < TILE_HEIGHT && relativeX >= (TILE_WIDTH_4 - PLATFORM_TOLERANCE) && relativeX <= (TILE_WIDTH_3_4 + PLATFORM_TOLERANCE))
    {
        e->position.y = tY + 10;
        e->velocity.y = Velocity_Jumper;
    }
}
    goto *next;
LBL(Tile_BarrierVertical):
    if (!TILE_FLAG(0))
    {
        if (relative_x(e) + e->velocity.x >= TILE_WIDTH - 1)
        {
            if (HANDLE_IS_CAMERA(e->handle) && e->isAttacking)
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
            if (HANDLE_IS_CAMERA(e->handle))
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
    {
        e->velocity.y = Velocity_Breaching;
        e->isGrounded = false;
    }
    goto *next;
LBL(Tile_Portal):
{
    const i8 nextX = relative_x(e) + e->velocity.x;
    const i8 nextY = relative_y(e) + e->velocity.y;
    if ((nextX >= (TILE_WIDTH_4 + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - (TILE_WIDTH_4 + (TILE_WIDTH >> 3))) ||
        (nextY >= (TILE_HEIGHT_4 + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - (TILE_HEIGHT_4 + (TILE_HEIGHT >> 3))))
    {
        PLAYER.lives = 3; // Reset lives
        ++PLAYER.stagesDone;
        PLAYER.score += PLAYER.stagesDone * Score_1000;
        GAME.state = GameState_Play;
        
        GAME.stage = WORLD.level->adjacentStage;
        game_prepare_next_stage(GameState_Play);
    }
}
    goto *next;
LBL(Tile_Coin):
if (!TILE_FLAG(2))
{
    const i8 nextX = relative_x(e) + e->velocity.x;
    const i8 nextY = relative_y(e) + e->velocity.y;
    if ((nextX >= (TILE_WIDTH_4 + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - (TILE_WIDTH_4 + (TILE_WIDTH >> 3))) ||
        (nextY >= (TILE_HEIGHT_4 + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - (TILE_HEIGHT_4 + (TILE_HEIGHT >> 3))))
    {
        entity_create_anonymous(Prop_Coin, e->tile);
        TILE_FLAG(2) = true;
    }
}
    goto* next;
LBL(Tile_GravitasUp):
    if (HANDLE_IS_CAMERA(e->handle) && e->substance < Substance_GravitasAir && (relative_y(e) - e->velocity.y >= (TILE_HEIGHT >> 1)))
    {
        if (e->substance == Substance_Air)
        {
            e->update    = e->type == Character_Croc ? update_croc_gravitas_air : update_doc_gravitas_air;
            e->substance = Substance_GravitasAir;
        }
        else // Substance_GravitasWater
        {
            e->update = e->type == Character_Croc ? update_croc_gravitas_water : update_doc_gravitas_water;
            e->substance = Substance_GravitasWater;
        }
        e->velocity.y  = 0;
        if (e->isAttacking)
        {
            e->state       = CharacterState_Idle;
        }
        e->isAttacking = false;
        WORLD.gravity  = -Velocity_Gravity;
    }
    goto* next;
LBL(Tile_SpikesDown):
    if (relative_y(e) - e->velocity.y < (TILE_HEIGHT >> 1))
    {
        set_ground(e);
        if (!HANDLE_IS_CAMERA(e->handle))
        {
            entity_set_status(e, EntityState_Dead);
        }
        else if (!WORLD.gameIsOver)
        {
            character_damage();
            CAMERA.invincibilityTicks = false;
            character_damage();
            WORLD.gameIsOver = true;
        }
    }
    goto *next;
LBL(Tile_Warning):
    goto *next;
LBL(Tile_GravitasDown):
    if (HANDLE_IS_CAMERA(e->handle) && e->substance >= Substance_GravitasAir && (relative_y(e) - e->velocity.y < (TILE_HEIGHT >> 1)))
    {
        if (e->substance == Substance_GravitasAir)
        {
            e->update    = e->type == Character_Croc ? update_croc_air : update_doc_air;
            e->substance = Substance_Air;
        }
        else
        {
            e->update    = e->type == Character_Croc ? update_croc_water : update_doc_water;
            e->substance = Substance_Water;
        }
        e->isAttacking = false;
        e->velocity.y  = 0;
        e->state       = CharacterState_Idle;
        WORLD.gravity  = Velocity_Gravity;
    }
    goto *next;
LBL(Tile_Hole0):
{
    i8 nextX = relative_x(e) + e->velocity.x;
    i8 nextY = relative_y(e) + e->velocity.y;
    if ((nextX >= ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3))) ||
        (nextY >= ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3))))
    {
        Tile const* it = WORLD.tiles;
        if ((tile & 1) == 0)
            ++tile;
        else
            --tile;

        for (u16 i = 0; i < I16(WORLD_STRIDE) * I16(WORLD_EXTENT); ++i, ++it)
        {
            Tile t = *it;
            if (t == tile)
            {
                e->tile.x     = I8(U8(i % U16(WORLD_STRIDE)));
                e->tile.y     = I8(U8(i / U16(WORLD_STRIDE)) + 1u);
                e->position.y = (I16(e->tile.y) << TILE_SCALE_BITS);
                e->position.x = (I16(e->tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1);
                e->velocity.x = 0;
                e->velocity.y = 0;
                goto adjust;
            }
        }
    }
}
    goto* next;
end:
    goto *next;
}

force_inline void world_next()
{
    switch (WORLD.worldState)
    {
    case WorldState_Update:
    {
        if (WORLD.list.iterator->isAllocated)
            WORLD.list.iterator->update(WORLD.list.iterator);

        if (WORLD.list.iterator == &WORLD.list.entities[ENTITIES_ACTIVE_MAX-1])
        {
            WORLD.worldState    = WorldState_CollisionDetection;
            WORLD.list.iterator = WORLD.list.entities;
        }
        else
        {
            ++WORLD.list.iterator;
        }
    }
        break;
    case WorldState_CollisionDetection:
        if (WORLD.list.iterator->isAllocated)
        {
            check_tiles(WORLD.list.iterator);
        }

        if (WORLD.list.iterator == &WORLD.list.entities[ENTITIES_ACTIVE_MAX-1])
        {
            WORLD.worldState    = WorldState_Next;
            WORLD.list.iterator = WORLD.list.entities;
        }
        else
        {
            ++WORLD.list.iterator;
        }
        break;
    default:
        break;
    }
}

force_inline void Draw_Line_d2(i8 a, i8 b)
{
    VIA_port_a = a;
    VIA_port_b = 0;
    // LEAX 2,X
    __asm__ volatile ("nop");
    ++VIA_port_b;
    VIA_port_a = b;
    VIA_shift_reg = 0xFF; // Set solid line pattern
    VIA_t1_cnt_hi = 0x00; 
    
    world_next();
    while ((VIA_int_flags & 0x40) == 0) {} // Wait for Interrupt

    __asm__ volatile ("nop");
    VIA_shift_reg = 0x00;
    Vec_Misc_Count = 0;

    reset_0_ref();
    reset_pen();
}

void world_freeze(void)
{
    for (i8 i = 0; i < ENTITIES_ACTIVE_MAX; ++i)
        WORLD.list.entities[i].update = update_stub;
}

void world_progress(void)
{
    WORLD.list.iterator = WORLD.list.entities;

    /**
     * Handle Drawing
     */
    v2i selectedTile;
    i8 xMin        = I8(CAMERA.position.x >> TILE_SCALE_BITS) - 3;
    selectedTile.x = xMin;
    i8 yMin        = I8(CAMERA.position.y >> TILE_SCALE_BITS) - 3;
    selectedTile.y = yMin + 7;
    yMin           = MAX8(0, yMin);
    selectedTile.y = MIN8(selectedTile.y, (WORLD_EXTENT - 1));

    xMin = MAX8(0, xMin);

    i8 xMax = selectedTile.x + 7;
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
        &&LBL(Tile_Portal),
        &&LBL(Tile_Coin),
        &&LBL(Tile_GravitasUp),
        &&LBL(Tile_SpikesDown),
        &&LBL(Tile_Warning),
        &&end,
        &&LBL(Tile_GravitasDown),
        &&LBL(Tile_Hole0),
        &&LBL(Tile_Hole0),
        &&LBL(Tile_Hole0),
        &&LBL(Tile_Hole0),
    };

    i8 idxDelta = (1 << STRIDE_BITS) + (xMax - xMin) - 1;

    Tile const* tiles = WORLD.tiles + (I16(selectedTile.y) << STRIDE_BITS) + selectedTile.x;
    Tile tile = *tiles;

    i8 tt, tb, tl, tr;

    const i8 firstTL = I8((I16(selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x);
    tl = firstTL;
    tr = firstTL + TILE_WIDTH;
    tt = I8(CAMERA.position.y - (I16(selectedTile.y) << TILE_SCALE_BITS));
    tb = tt - TILE_HEIGHT;

    goto *jump[tile];

LBL(Tile_Top):
    if (tt > tb && tr > tl)
    {
        beam_set_position(tt, tl);
        draw_line(0, TILE_WIDTH);
    }
    goto end;
LBL(Tile_TopLeft):
    if (tt > tb && tr > tl)
    {
        beam_set_position(tt, tl);
        draw_line(0, TILE_WIDTH);
    }
    if (tr > tl && tt > tb)
    {
        beam_set_position(tt, tl);
        draw_line(-TILE_HEIGHT, 0);
    }
    goto end;
LBL(Tile_TopRight):
    if (tr > tl && tt > tb)
    {
        beam_set_position(tt, tl);
        draw_line(0, TILE_WIDTH);
        beam_set_position(tt, tr);
        draw_line(-TILE_HEIGHT, 0);
    }
    goto end;
LBL(Tile_Bottom):
    if (tt > tb && tr > tl) 
    {
        beam_set_position(tb, tl); 
        draw_line(0, TILE_WIDTH);
    } 
    goto end;
LBL(Tile_BottomLeft):
    if (tr > tl && tt > tb)
    {
        beam_set_position(tb, tl);
        draw_line(0, TILE_WIDTH);
        beam_set_position(tt, tl);
        draw_line(-TILE_HEIGHT, 0);
    }
    goto end;
LBL(Tile_BottomRight):
    if (tr > tl && tt > tb)
    {
        beam_set_position(tb, tl);
        draw_line(0, TILE_WIDTH);
        beam_set_position(tt, tr);
        draw_line(-TILE_HEIGHT, 0);
    }
    goto end;
LBL(Tile_Left):
    if (tr > tl && tt > tb) 
    {
        beam_set_position(tt, tl); 
        draw_line(-TILE_HEIGHT, 0);
    }
    goto end;
LBL(Tile_Right):
    if (tr > tl && tt > tb) 
    {
        beam_set_position(tt, tr); 
        draw_line(-TILE_HEIGHT, 0);
    } 
    goto end;
LBL(Tile_MiddleLeft):
    if (tr > tl) 
    {
        const i8 centerY = I8(U8(tt) - TILE_HEIGHT_2);
        if (tt > centerY) 
        {
            beam_set_position(centerY, tl); 
            draw_line(0, TILE_WIDTH_2);
        }
    }
    goto end;
LBL(Tile_MiddleRight):
{
    const i8 centerX = I8(U8(tl) + TILE_WIDTH_2); 
    if (tr > centerX) 
    { 
        const i8 centerY = I8(U8(tt) - TILE_HEIGHT_2); 
        if (tt > centerY) 
        { 
            beam_set_position(centerY, centerX); 
            draw_line(0, TILE_WIDTH_2); 
        } 
    }
}
    goto end;
LBL(Tile_Middle):
{
    const i8 centerX = I8(U8(tl) + TILE_WIDTH_4);
    if (tr > centerX)
    {
        const i8 centerY = I8(U8(tt) - TILE_HEIGHT_2);
        if (tt > centerY)
        {
            beam_set_position(centerY, centerX);
            draw_line(0, TILE_WIDTH_2);
        }
    }
}
    goto end;
LBL(Tile_MiddleBottom):
{ 
    const i8 centerX = I8(U8(tl) + TILE_WIDTH_4); 
    if (tr > centerX && tt > tb) 
    { 
        beam_set_position(tb, centerX); 
        draw_line(0, TILE_WIDTH_2); 
    }
}
    goto end;
LBL(Tile_MiddleRightTop) :
    if (tt > tb && tr > tl) 
    { 
        beam_set_position(tt, tr - TILE_WIDTH_2); 
        draw_line(0, TILE_WIDTH_2); 
    } 
    goto end;
LBL(Tile_MiddleTop) :
    if (tt > tb && tr > tl) 
    {
        beam_set_position(tt, tl + TILE_WIDTH_4); 
        draw_line(0, TILE_WIDTH_2);
    } 
    goto end;
LBL(Tile_MiddleLeftTop):
    if (tt > tb && tr > tl) 
    {
        beam_set_position(tt, tl); 
        draw_line(0, TILE_WIDTH_2);
    } 
    goto end;
LBL(Tile_Spikes):
    if (tr > tl && tt > tb) 
    {
        draw_queue_push(spikes, tb, tl)
    } 
    goto end;
LBL(Tile_SpikedBall):
    if (tb < tt && tl < tr)
    {
        draw_queue_push(spikedBall, tb + TILE_HEIGHT_2, tl + TILE_WIDTH_2)
    }
    goto end;
LBL(Tile_Jumper):
{
    if (tb < tt && tl < tr)
    {
        beam_set_position(tb, tl + TILE_WIDTH_4);
        Draw_VLc((void* const)jumper);
    }
}
    goto end;
LBL(Tile_BarrierVertical):
    if (!TILE_FLAG(0) && tt > tb && tr > tl) 
    {
        beam_set_position(tt, tr); 
        Draw_VLc((void* const)barrierVertical);
    } 
    goto end;
LBL(Tile_BarrierHorizontal):
    if (!TILE_FLAG(1) && tt > tb && tr > tl)
    {
        beam_set_position(tb, tr); 
        Draw_VLc((void* const)barrierHorizontal);
    } 
    goto end;
LBL(Tile_WaterTop):
    if (tt > tb && tr > tl)
    {
        draw_queue_push(watertop[WORLD.freq8_8], tt, tl)
    } 
    goto end;
LBL(Tile_E0):
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
LBL(Tile_Portal):
{ 
    const i8 centerY = tb + TILE_HEIGHT_2;
    const i8 centerX = tl + TILE_WIDTH_2;
    if (centerY < tt && centerX < tr)
    {
        beam_set_position(centerY, centerX); 
        Draw_VLc((void* const)portal[WORLD.freq8_8]);
    }
}
    goto end;

LBL(Tile_Coin) :
if (!TILE_FLAG(2) && tt > tb && tr > tl)
{
    draw_queue_push(diamond, tb + (TILE_HEIGHT_2 >> 1), tl + (TILE_WIDTH_2 >> 0));
}
goto end;
LBL(Tile_GravitasUp):
    if (tt > tb) 
    {
        draw_queue_push(gravitasUp, tb, tl + TILE_WIDTH_2);
    }
    goto end;
LBL(Tile_SpikesDown) :
    if (tt > tb)
    {
        draw_queue_push(spikesDown, tt, tl);
    }
    goto end;
LBL(Tile_Warning):
    goto end;
LBL(Tile_GravitasDown):
    if (tt > tb)
    {
        draw_queue_push(gravitasDown, tt, tl + TILE_WIDTH_2);
    }
    goto end;
LBL(Tile_Hole0):
{
    if (tt > tb && tr > tl)
    {
        beam_set_position(tb + TILE_HEIGHT_2, tl + TILE_WIDTH_2);
        Draw_VLc((void* const)hole);
    }
}
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
        tr = firstTL + TILE_WIDTH;
        tb = tt;
        tt += TILE_HEIGHT;
        goto* jump[tile];
    }

    while(WORLD.worldState < WorldState_Next)
    {
        world_next();
    }
    WORLD.worldState = WorldState_Update;
}

void world_create(Stage const stage)
{
    MEMZERO(WORLD);
    WORLD.level       = &levels[stage];
    WORLD.tiles       = WORLD.level->level;
    entity_create_list();
    entity_create_anonymous(Entity_Croc, WORLD.level->startingTile);

    CAMERA.position.y = (I16(CAMERA.tile.y) << TILE_SCALE_BITS)  - (TILE_HEIGHT >> 1);
    CAMERA.position.x = (I16(CAMERA.tile.x) << TILE_SCALE_BITS)  + (TILE_WIDTH  >> 1);
}
