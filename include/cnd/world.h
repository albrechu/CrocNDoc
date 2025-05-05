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

#ifndef WORLD_H
#define WORLD_H

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/mesh.h>


#define WORLD             g_world
#define LIST              WORLD.drawList
#define CAMERA            WORLD.player
#define ENTITY(idx)       (&WORLD.player)[idx]
#define ENEMY(idx)        WORLD.enemy[idx]
#define PROP(idx)         WORLD.prop[idx]
#define TILE_SCALE_BITS   6
#define TILES_VIEW_WIDTH  I8(256u >> TILE_SCALE_BITS)
#define TILES_VIEW_HEIGHT I8(256u >> TILE_SCALE_BITS)
#define WORLD_WIDTH       6
#define WORLD_HEIGHT      6
#define WORLD_STRIDE      I8(TILES_VIEW_WIDTH * WORLD_WIDTH)
#define WORLD_EXTENT      I8(TILES_VIEW_HEIGHT * WORLD_HEIGHT)
#define TILE_WIDTH        I8(1 << TILE_SCALE_BITS)
#define TILE_HEIGHT       I8(1 << TILE_SCALE_BITS)
// #define DRAW_STRIPS_MAX 8
#define DRAW_STRIPS_MAX 20
#define DRAW_LINES_MAX  40
#define ENEMIES_MAX     3
#define PROPS_MAX       3

/////////////////////////////////////////////////////////////////////////
//	Types
//
// typedef struct line_strip_t
// {
//     i8  y, x;
//     i8  vectors;
//     v2i data[];
// } line_strip_t, *line_strip;

// typedef struct drawable_strip_t
// {
//     v2i        tile;
//     v2i        start;
//     i8         vectors;
//     v2i const* strip; 
// } drawable_strip_t, *drawable_strip;

// typedef struct tile_t
// {
//     line_strip_t const* lists[3];
//     i8                  count;
// } tile_t, *tile;

// typedef struct draw_list_t
// {
//     drawable_strip_t strips[DRAW_STRIPS_MAX];
//     i8 count;
//     i8 camTileOffset;
//     i8 camTileCount;
// } draw_list_t;

enum Tile_
{
    Tile_Top,
    Tile_Bottom,
    Tile_Left,
    Tile_Right,
    Tile_MiddleLeft,
    Tile_MiddleRight,
    Tile_Middle,
    Tile_Spikes,
    // Enemies
    Tile_E0,
    Tile_E1,
    Tile_E2,
    Tile_E3,
    Tile_E4,
    Tile_E5,
    Tile_E6,
    Tile_E7,
    // Props
    Tile_P0,
    Tile_P1,
    Tile_P2,
    Tile_P3,
    Tile_P4,
    Tile_P5,
    Tile_P6,
    Tile_P7,

    Tile_Empty,

    E = Tile_Empty,


    // // Type
    // Tile_Empty,
    // Tile_Ground,
    // Tile_LeftWall,
    // Tile_RightWall,
    // Tile_Ceiling,
    // Tile_Platform,
    // Tile_Enemy,
    // Tile_Prop,
    // Tile_Spikes,
    // Tile_TypeMax = 0x0F,
    
    // // Tile_Crate = Tile_Prop | 0x10,
    // // Tile_Jumper,
    // // Tile_Barrel,
    // // Tile_Halunke,
    // // Tile_Gauner,
    // // Tile_Schuft,
    // // Tile_Strolch,
    // // Tile_Boesewicht,

    // E = Tile_Empty,
    // P = Tile_Platform,
    // TG = Tile_Ground,
    // TL = Tile_LeftWall,
    // TR = Tile_RightWall,
    // TC = Tile_Ceiling,
    // TS = Tile_Spikes,
    
};
typedef i8 Tile;

enum EntityStatus_
{
    EntityStatus_Alive,
    EntityStatus_Active,
    EntityStatus_Dead,
};
typedef i8 EntityStatus;

typedef struct line_t
{
    v2i start;
    v2i delta;
} line_t, *line;

typedef struct line_strip_t
{
    v2i start;
    i8 const* data; // First is the vector
} line_strip_t, *line_strip;

typedef struct draw_list_t
{
    line_t       line[DRAW_LINES_MAX];
    line_strip_t strip[DRAW_STRIPS_MAX];
    i8 lines, strips;
} draw_list_t, *draw_list;

typedef struct world_t
{
    // tile_t      tiles[WORLD_HEIGHT * WORLD_WIDTH];
    draw_list_t  drawList;
    // i16         hazard; // Hazard Height 
    entity_t     player;
    entity_t     enemy[ENEMIES_MAX];
    entity_t     prop[PROPS_MAX];
    i8           enemies, props;
    EntityStatus enemyStatus[16];
    EntityStatus propStatus[16];
    v2i          selectedTile;
    idx_t        selectedEntity;
    // Stage       stage;
    const Tile* tileset;
    i8 ticks;
    i8 drag;
} world_t;

extern world_t g_world;

void world_create(Stage const stage);
void world_draw(void);
void world_progress(void);

// void world_load_stage(Stage const stage);
void tile_write_list();

#endif /* WORLD_H */