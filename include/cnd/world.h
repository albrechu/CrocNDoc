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
#define CAMERA            WORLD.entities[0]
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
// #define ENEMIES_MAX     3
#define ENTITIES_MAX    8
// #define PROPS_MAX       3

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
    Tile_MiddleLeft,
    Tile_MiddleRight,
    Tile_Middle,
    Tile_Spikes,
    Tile_Jumper,
    Tile_BarrierVertical,
    Tile_BarrierHorizontal,
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

    Tile_Empty,
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
    entity_t     entities[1 + ENTITIES_MAX];
    idx_t        entityIdxs[1 + ENTITIES_MAX];
    i8           entityCount;
    // i8           enemies, props;
    // entity_t     enemy[ENEMIES_MAX];
    // entity_t     prop[PROPS_MAX];
    EntityStatus entityStatus[16];
    v2i          selectedTile;
    idx_t        selectedEntity;
    const Tile* tileset;
    const EntityType* pentities;
    i8 ticks;
    i8 drag;
} world_t;

extern world_t g_world;

void world_create(Stage const stage);
void world_draw(void);
void world_progress(void);
void game_enter_stage(Stage stage);

// void world_load_stage(Stage const stage);
void tile_write_list();

#endif /* WORLD_H */