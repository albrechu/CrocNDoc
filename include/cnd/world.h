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
#define ENTITIES_ACTIVE_MAX 8  // Maximum of entities that can be active.
#define ENTITIES_MAX        16 // Maximum of entities that can be in a level.
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
    Tile_MiddleBottom,
    Tile_MiddleTop,
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
    // Wormhole
    // Tile_Wormhole0,
    // Tile_Wormhole1,
    // Tile_Wormhole2,
    // Tile_Wormhole3,
    // Nothing, nothing at all.
    Tile_Empty,
};
typedef i8 Tile;

enum EntityStatus_
{
    EntityStatus_Inactive, // The entity is alive and well but takes not part in any calculations.
    EntityStatus_Active,   // The entity is currently active, takes part in the physics calculation and is possibly rendered.
    EntityStatus_Dead,     // May it rest in peace.
};
typedef i8 EntityStatus;

typedef struct last_sighting_t
{
    // v2l          position; // Last known position. Initially is I16_MIN.
    EntityStatus status;   // Last known state. Initially is EntityStatus_Alive.
} last_sighting_t, *last_sighting;

typedef struct stage_metadata_t
{
    v2i   const       startingTile;
    Stage const       adjacentStages[2];
    EntityType const* pentities;
} stage_metadata_t, *stage_metadata;

typedef struct world_t
{
    // Callbacks
    routine_t       entityAdded;
    stage_routine_t stageEntered;
    // Entities
    entity_t  entities[ENTITIES_ACTIVE_MAX];
    idx_t     entityIdxs[ENTITIES_ACTIVE_MAX];
    i8        entityCount;

    last_sighting_t lastSeen[ENTITIES_MAX];
    v2i   selectedTile;
    idx_t selectedEntity;
    // Stage information
    const Tile*             tileset;
    const stage_metadata_t* metadata;
    i8 ticks;
    // Physics
    i8 gravity; // For 
} world_t;

extern world_t g_world;

void world_create(Stage const stage);
void world_progress(void);
void world_entity_set_status(entity e, idx_t idxInIdxs, EntityStatus status);

// void world_load_stage(Stage const stage);
void tile_write_list();

#endif /* WORLD_H */