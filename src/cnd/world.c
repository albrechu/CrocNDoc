#include <cnd/world.h>

#include <cnd/xutils.h>
#include <lib/print.h>
#include <vectrex.h>
#include <lib/assert.h>
#include <lib/static_assert.h>
#include <lib/monitor.h>

#define POS(y, x) y, x // Starting position
#define TILE(y, x) y, x
#define METADATA_BYTES 4
#define TILE_FLAG(id) g_tileFlags[id]

enum TileAlias
{
    E   = Tile_Air,
    W   = Tile_Water,
    T2  = Tile_Top2,
    T   = Tile_Top,
    TL  = Tile_TopLeft,
    TR  = Tile_TopRight,
    B2   = Tile_Bottom2,
    B   = Tile_Bottom,
    BL  = Tile_BottomLeft,
    BR  = Tile_BottomRight,
    L2   = Tile_Left2,
    L   = Tile_Left,
    R2   = Tile_Right2,
    R   = Tile_Right,
    M   = Tile_Middle,
    MB  = Tile_MiddleBottom,
    MRT = Tile_MiddleRightTop,
    MT  = Tile_MiddleTop,
    MLT = Tile_MiddleLeftTop,
    ML  = Tile_MiddleLeft,
    MR  = Tile_MiddleRight,
    S   = Tile_Spikes,
    SB  = Tile_SpikedBall,
    J   = Tile_Jumper,
    BV  = Tile_BarrierVertical,
    BH  = Tile_BarrierHorizontal,
    WT  = Tile_WaterTop,
    // Entities
    E0  = Tile_E0,
    E1  = Tile_E1,
    E2  = Tile_E2,
    E3  = Tile_E3,
    E4  = Tile_E4,
    E5  = Tile_E5,
    E6  = Tile_E6,
    E7  = Tile_E7,
    E8  = Tile_E8,
    E9  = Tile_E9,
    E10 = Tile_E10,
    E11 = Tile_E11,
    E12 = Tile_E12,
    E13 = Tile_E13,
    E14 = Tile_E14,
    E15 = Tile_E15,
    // Portals
    P0  = Tile_Portal0,
    P1  = Tile_Portal1,
};

/** 
 * 
 * Legend: _ ground, | wall, - platform, = breakable ground, 
 *         c crate, w spikes
 *         
 * Stage_Sewers:
 *                  ________
 *  _____           |   __ -|
 * |___| |    ______|  |__|-|           
 *     | |___| ______ _____-|
 *     |_____ -|    |=|
 *          |__|    | |_______________
 *                  |___c__ _         |____
 *                        |       _   _____=>
 *                        |wwwwwwwwww|
 */
static const Tile g_stageSewers[] = 
{
      B,   B,   B,   B,   B,   E,   E,   E,   E,   B,   B,   B,   B,   B,   B,   B,   B,   B,   B,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   E,   R,   E,   E,  L2,   E,   E,   R,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   E,  BV,   E,   E,  L2,   E,   E,   R,   E,  TL,   T,   T,   T,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
     TL,  T2,  TR,   T,   E,  L2,   E,   E,   R,   E,  L2,   E,   E,   E,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,  P0,  R2, MRT,   T,  L2,   E,   E,   R,   E,  L2,   E,   E,   E,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   E,  R2,  E1,   E,  L2,   E,   E,   R,   E,  L2,   E,   E,   E,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   E,  R2,   T, MLT,  L2,   E,   E,   R,   E,   T,   T,   T,   T,   T,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   J,  R2,   E,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   E,  R2,   M,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  E3,   E,  E8,   E,   R,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   J,  R2,   E,   E,   E,   E,   E,   E,   E,   E,   E,  E2,   E,   E,   E,   E,   E,   R,   B,   B,   B,   B,   B, E, E, E, E, E, E, E, E,
      L,   E,  R2,  MB,   E,   E,  MT,  TL,  TR,  BH,  TL,   T,   T,   T,   T,   T,   T,   T,   T,   E,   E,   E,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,  R2,   E,   E,   E,  MB,  L2,  R2,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,   J,   T,   T,   L,  E0,  L2,  R2,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  P1,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,   E,   E,   E,   L,  MT,  L2,  R2,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,   E,   E,   E,   T,   T,   E,  R2,   E,   E,   E,   E,   E,   M,   E,   E,   E,   E,   E,   E,   E,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,   E,   E,   E,   E,   E,   E,  R2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,   J,   E,   E,   E,   E,   E,  R2,   E,   E,   E,  E7,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E, E, E, E, E, E, E, E, E,
      L,   E,   E,   E,   E,   E,   E,   E,   E,   T,   T,   E,   M,   E,   E,   E,   E,   E,   E,   E,   E,   J,  BR,   E, E, E, E, E, E, E, E, E,
      L,   E,   E,  MR,   E,   M,   E,   E,  MR,   T,   E,   E,   E,   E,   E,   E,   M,   E,  TL,   T,   T,   T,   E,   E, E, E, E, E, E, E, E, E,
      L,   E,   E,   E,   E,   E,   MR,  E,   E,   E,   M,   E,   S,   S,   S,   S,   S,   S,   L,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      L,   J,   E,   E,   S,   S,   S,   S,   S,   S,   S,   S,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      T,   T,   T,   T,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E, E, E, E, E, E, E, E, E,
};
static_assert(sizeof(g_stageSewers) == (I16(WORLD_EXTENT) * I16(WORLD_STRIDE)));

const EntityType g_sewersEntities[] = 
{
    Prop_Barrel,
    Enemy_Schuft,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Prop_Barrel,
};
static_assert(sizeof(g_sewersEntities) <= ENTITIES_MAX);

static const Tile g_waterways[] = 
{
      R,   E,   E,   E,   R,   W,   W,   L,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  R,   E,   E,   L,
      R,   E,   E,   E,   R,  P0,   W,   L,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  R,  P1,   E,   L,
      R,   T,   T,  WT,  WT,   T,   T,  WT,  WT,  WT,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  R,   E,   E,   L,
      R,   W,   W,   W,   W,   L,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   T,  WT,   L,
      R,   W,   S,   S,   S,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   L,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      R,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W,   W, W, W, W, W, W, W, W, W,  W,   W,   W,   L,
      W,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T, T, T, T, T, T, T, T, T,  T,   T,   T,   W,
};
static_assert(sizeof(g_waterways) == (I16(WORLD_EXTENT) * I16(WORLD_STRIDE)));

const EntityType g_waterwaysEntities[] = 
{
    Prop_Barrel,
    Enemy_Tunichtgut,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
};
static_assert(sizeof(g_waterwaysEntities) <= ENTITIES_MAX);


const stage_metadata_t g_stagesMetadata[] = 
{
    // Stage_Sewers
    {
        .startingTile   = { 2, 1 },
        .adjacentStages = { Stage_Waterways, Stage_Waterways },
        .pentities      = g_sewersEntities,
    },
    // Stage_Waterways
    {
        .startingTile   = { 2, 1 },
        .adjacentStages = { Stage_Sewers, Stage_Sewers },
        .pentities      = g_waterwaysEntities,
    }
};

const Tile* const g_tilesets[] = 
{
    g_stageSewers,
    g_waterways,
    // g_stageMolEnlands,
    // g_stageBonus,
};

static bool g_tileFlags[4];
world_t g_world;

#define ENT WORLD.selectedEntity
void check_collision()
{
    ENT->isGrounded = ENT->velocity.y < 0;
    ENT->velocity.x -= (WORLD.gravity | ENT->transform) * (!!ENT->velocity.x & !ENT->velocity.y);
	ENT->velocity.y -= WORLD.gravity;
    // WORLD.selectedEntity->velocity.y  = MAX8(-8, WORLD.selectedEntity->velocity.y);
    // print_signed_int(-100, -50, WORLD.selectedEntity->tile.y);
    // print_signed_int(-100, -20, WORLD.selectedEntity->tile.x);
    
    v2i tiles[3]; // tiles to check
    tiles[0].y = ENT->tile.y;
    tiles[0].x = ENT->tile.x;
    tiles[1].y = I8((ENT->position.y - ENT->velocity.y) >> TILE_SCALE_BITS);
    tiles[1].x = ENT->tile.x;
    tiles[2].y = ENT->tile.y;
    tiles[2].x = I8((ENT->position.x + ENT->velocity.x) >> TILE_SCALE_BITS);

    for (i8 i = 0; i < 3; ++i)
    {
        v2i* tile = &tiles[i];
        {
            const Tile type = WORLD.tileset[(I16(tile->y) << STRIDE_BITS) + tile->x];
            switch (type)
            {
            case Tile_Air:
                ENT->substance = Substance_Air;
                break;
            case Tile_Water:
                ENT->substance = Substance_Water;
                break;
            case Tile_Top2:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) - ENT->velocity.y <= 0)
                    ENT->velocity.y = 0;
                /* fallthrough */
            case Tile_Top:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y <= 0)
                    ENT->velocity.y = 0;
            break;
            case Tile_TopLeft:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y <= 0)
                    ENT->velocity.y = 0;
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) - ENT->velocity.x <= 0)
                    ENT->velocity.x = 0;
                break;
            case Tile_TopRight:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y <= 0)
                    ENT->velocity.y = 0;
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) - ENT->velocity.x >= TILE_WIDTH - 1)
                    ENT->velocity.x = 0;
                break;
            case Tile_Bottom2:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) - ENT->velocity.y >= TILE_HEIGHT - 1)
                    ENT->velocity.y = 0;
                /* fallthrough */
            case Tile_Bottom:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y >= TILE_HEIGHT - 1)
                    ENT->velocity.y = 0;
                break;
            case Tile_BottomLeft:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) - ENT->velocity.y >= TILE_HEIGHT - 1)
                    ENT->velocity.y = 0;
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x <= 0)
                    ENT->velocity.x = 0;
                break;
            case Tile_BottomRight:
                if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) - ENT->velocity.y >= TILE_HEIGHT - 1)
                    ENT->velocity.y = 0;
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x >= TILE_WIDTH - 1)
                    ENT->velocity.x = 0;
                break;
            case Tile_Left2:
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) - ENT->velocity.x <= 0)
                    ENT->velocity.x = 0;
                /* fallthrough */
            case Tile_Left:
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x <= 0)
                    ENT->velocity.x = 0;
                break;
            case Tile_Right2:
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) - ENT->velocity.x >= TILE_WIDTH - 1)
                    ENT->velocity.x = 0;
                /* fallthrough */
            case Tile_Right:
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x >= TILE_WIDTH - 1)
                    ENT->velocity.x = 0;
                break;
            case Tile_MiddleLeftTop:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y <= 0 && relativeY < (TILE_HEIGHT >> 1) && relativeX <= TILE_WIDTH >> 1)
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_MiddleLeft:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y >= (TILE_HEIGHT >> 1) - 1 && relativeY < (TILE_HEIGHT >> 1) && relativeX <= TILE_WIDTH >> 1)
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_MiddleRight:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y >= (TILE_HEIGHT >> 1) - 1 && relativeY < (TILE_HEIGHT >> 1) && relativeX >= TILE_WIDTH >> 1)
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_Middle:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y >= (TILE_HEIGHT >> 1) - 1 && relativeY < (TILE_HEIGHT >> 1) && relativeX >= TILE_WIDTH >> 2 && relativeX <= (TILE_WIDTH- (TILE_WIDTH >> 2)))
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_MiddleBottom:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y >= TILE_HEIGHT - 1 && relativeY < TILE_HEIGHT && relativeX >= TILE_WIDTH >> 2 && relativeX <= (TILE_WIDTH- (TILE_WIDTH >> 2)))
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_MiddleRightTop:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y >= 0 && relativeY < 0 && relativeX >= TILE_WIDTH >> 1)
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_MiddleTop:
            {
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - ENT->velocity.y >= 0 && relativeY < 0 && relativeX >= TILE_WIDTH >> 2 && relativeX <= (TILE_WIDTH- (TILE_WIDTH >> 2)))
                    ENT->velocity.y = 0;
            }
                break;
            case Tile_Spikes:
            {
                const i8 relative = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                if (relative - ENT->velocity.y >= (TILE_HEIGHT >> 1))
                {
                    ENT->velocity.y = 0;
                    if (ENT->id >= 0)
                    {
                        world_entity_set_status(ENT, WORLD.selectedEntityIdxIdx, EntityState_Dead);
                    }
                    else
                    {
                        WORLD.playerDamage();
                        WORLD.playerDamage();
                    }
                }
            }
                break;
            case Tile_SpikedBall:
            {
                i8 nextX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x;
                i8 nextY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y;
                if (ENT->id == -1 && ((nextX >= ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3))) || 
                    (nextY >= ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)))))
                    WORLD.playerDamage();
            }
                break;
            case Tile_Jumper:
            {
                const i8 relativeY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS));
                const i8 relativeX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS));
                if (relativeY + ENT->velocity.y >= (TILE_HEIGHT >> 1) && relativeX + ENT->velocity.x <= (TILE_WIDTH - (TILE_WIDTH >> 2)) && relativeX + ENT->velocity.x >= (TILE_WIDTH >> 2))
                {
                    ENT->velocity.y = Velocity_Jumper;
                }
            }
                break; 
            case Tile_BarrierVertical:
            if (!TILE_FLAG(0))
            {
                if (I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x >= TILE_WIDTH - 1)
                {
                    if (ENT->id == -1 && ENT->stopwatch != 0)
                        TILE_FLAG(0) = true;
                    else
                        ENT->velocity.x = 0;
                }
            }
                break;
            case Tile_BarrierHorizontal:
                if (!TILE_FLAG(1))
                {
                    if (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) - ENT->velocity.y >= TILE_WIDTH)
                    {                   
                        if (ENT->velocity.y < -14) // Broke the barrier
                            TILE_FLAG(1) = true;
                        else
                            ENT->velocity.y = 0;
                    }
                }
                break;
            case Tile_WaterTop:
                if (ENT->id == -1 && (I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y >= TILE_HEIGHT - 1) && ENT->velocity.y > 0)
                    ENT->velocity.y += Velocity_Jump;
                break;
            case Tile_E0:
            case Tile_E1:
            case Tile_E2:
            case Tile_E3:
            case Tile_E4:
            case Tile_E5:
            case Tile_E6:
            case Tile_E7:
            case Tile_E8:
            case Tile_E9:
            case Tile_E10:
            case Tile_E11:
            case Tile_E12:
            case Tile_E13:
            case Tile_E14:
            case Tile_E15:
                break;
            case Tile_Portal0:
            case Tile_Portal1:
            {
                i8 nextX = I8(ENT->position.x - (tile->x << TILE_SCALE_BITS)) + ENT->velocity.x;
                i8 nextY = I8(ENT->position.y - (tile->y << TILE_SCALE_BITS)) + ENT->velocity.y;
                if ((nextX >= ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3)) && nextX <= TILE_WIDTH - ((TILE_WIDTH >> 2) + (TILE_WIDTH >> 3))) || 
                    (nextY >= ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3)) && nextY <= TILE_HEIGHT - ((TILE_HEIGHT >> 2) + (TILE_HEIGHT >> 3))))
                    WORLD.stageEntered(WORLD.metadata->adjacentStages[type - Tile_Portal0]);
            }
                break;
            default:
                break;
            }
        }
    }
    ENT->position.x += ENT->velocity.x;
    ENT->position.y -= I16(ENT->velocity.y);
    ENT->tile.x = I8(ENT->position.x >> TILE_SCALE_BITS);
    ENT->tile.y = I8(ENT->position.y >> TILE_SCALE_BITS);
    
    // Check if entity is local.
    u8 distX = Abs_b(ENT->tile.x - CAMERA.tile.x);
    u8 distY = Abs_b(ENT->tile.y - CAMERA.tile.y);
    ENT->isLocal = distX < 3 && distY < 3;
    ENT->isSameTile = ENT->tile.x == CAMERA.tile.x && ENT->tile.y == CAMERA.tile.y;
    //if (!ENT->isLocal)
    //{
    //    if (distX >= 12 || distY >= 12) // Possibly remove entity if too far away
    //    {
    //        world_entity_set_status(ENT, WORLD.selectedEntityIdxIdx, EntityStatus_Inactive);
    //    }
    //}
    ENT->isGrounded = ENT->velocity.y == 0;
}

#define LBL(x) lbl_##x

void world_progress(void)
{
    ++WORLD.ticks;
    WORLD.gravity = WORLD.ticks & 1;

    for (WORLD.selectedEntityIdxIdx = 0; WORLD.selectedEntityIdxIdx < WORLD.entityCount; ++WORLD.selectedEntityIdxIdx)
    {
        WORLD.selectedEntity = &WORLD.entities[WORLD.entityIdxs[WORLD.selectedEntityIdxIdx]];
        check_collision();
    }

    WORLD.selectedTile.x = I8((CAMERA.position.x - 128) >> TILE_SCALE_BITS);
    WORLD.selectedTile.y = I8((CAMERA.position.y - 128) >> TILE_SCALE_BITS);

    i8 xMax = WORLD.selectedTile.x + 4;
    xMax = MIN8(xMax, WORLD_STRIDE);
    xMax = MAX8(0, xMax);
    i8 yMax = WORLD.selectedTile.y + 5;
    yMax = MIN8(yMax, WORLD_EXTENT);
    yMax = MAX8(0, yMax);
    WORLD.selectedTile.x = MAX8(0, WORLD.selectedTile.x);
    WORLD.selectedTile.y = MAX8(0, WORLD.selectedTile.y);
    
    
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

    i8 idxStride = xMax - WORLD.selectedTile.x;
    i8 idxDelta  = WORLD_STRIDE - idxStride + 1;
    
    i16 tileLeft, tileRight, tileTop; //, tileBottom;
    i8 startX, deltaX;
    Tile const* tiles = WORLD.tileset + (I16(WORLD.selectedTile.y) << STRIDE_BITS) + WORLD.selectedTile.x;
    Tile tile = *tiles; 

    tileLeft   = (I16(WORLD.selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x;
    tileRight  = tileLeft          + TILE_WIDTH;
    tileTop    = CAMERA.position.y - (I16(WORLD.selectedTile.y) << TILE_SCALE_BITS); // This one inverts the y axis
    // tileBottom = tileTop           - TILE_HEIGHT;

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
        Draw_VLc((void* const)watertop[(WORLD.ticks >> 3) & 0x7]);
    }
    goto end;
LBL(Tile_E0):
    tile -= Tile_E0;
    last_sighting last = &WORLD.lastSeen[tile];
    if (last->status == EntityStatus_Inactive)
    {
        last->status = EntityStatus_Active;
        entity e      = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount++]];
        e->id         = tile;
        e->tile.y     = WORLD.selectedTile.y;
        e->tile.x     = WORLD.selectedTile.x;
        e->position.y = -(I16(e->tile.y) << TILE_SCALE_BITS) - (TILE_HEIGHT >> 1);
        e->position.x = (I16(e->tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1);
        e->velocity.x = 0, e->velocity.y = 0;
        e->type       = WORLD.metadata->pentities[tile];
        e->isLocal    = true;
        e->isSameTile = false;
        e->isGrounded = false;
        e->isEnemy    = e->type > Prop_Max;
        e->transform  = 0;
        WORLD.entityAdded(e);
    }
    goto end;
LBL(Tile_Portal0):
{
    i8 tick = (WORLD.ticks >> 3) & 0x7;
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
    if (++WORLD.selectedTile.x < xMax)
    {
        tile = *++tiles;
        tileLeft   = (I16(WORLD.selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x;
        tileRight  = tileLeft          + TILE_WIDTH;
        tileTop    = CAMERA.position.y - (I16(WORLD.selectedTile.y) << TILE_SCALE_BITS); 
        goto *jump[tile];
    }
    else if (++WORLD.selectedTile.y < yMax)
    {
        WORLD.selectedTile.x -= idxStride; // Reset to start of row
        tile = *(tiles += idxDelta);
        tileLeft   = (I16(WORLD.selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x;
        tileRight  = tileLeft          + TILE_WIDTH;
        tileTop    = CAMERA.position.y - (I16(WORLD.selectedTile.y) << TILE_SCALE_BITS); 
        goto *jump[tile];
    }
    
}

void __routine_stub(entity e) { (void)e; }

void world_entity_set_status(entity e, idx_t idxInIdxs, EntityStatus status)
{
    if (idxInIdxs < 0) // Find idx
    {
        idx_t* idx = &WORLD.entityIdxs[0];
        for (idxInIdxs = 1; idxInIdxs < WORLD.entityCount; ++idxInIdxs, ++idx)
        {
            if (e == (WORLD.entities + WORLD.entityIdxs[*idx]))
                break;
        }
    }

    switch (status)
    {
    case EntityStatus_Inactive:
    deactivate:
    {
        const idx_t last = WORLD.entityCount - 1;
        if (last != idxInIdxs) // Swap with last
        {
            WORLD.lastSeen[e->id].status = status;
            i8 tmp = WORLD.entityIdxs[idxInIdxs];
            WORLD.entityIdxs[idxInIdxs] = WORLD.entityIdxs[last];
            WORLD.entityIdxs[last]      = tmp;
        }
        --WORLD.entityCount;
        e->routine = __routine_stub;
    }
        break;
    case EntityStatus_Dead:
        goto deactivate;
    default: // not responsible for: EntityStatus_Active
        break;
    }
}


void world_create(Stage const stage)
{
    MEMZERO(WORLD);
    WORLD.tileset     = g_tilesets[stage];
    WORLD.metadata    = g_stagesMetadata + stage;
    WORLD.entityCount = 1; // 1 for the player
    for (idx_t idx = 0; idx < ENTITIES_ACTIVE_MAX; ++idx)
    {
        WORLD.entities[idx].id = idx;
        WORLD.entities[idx].routine = __routine_stub;
        WORLD.entityIdxs[idx] = idx;
    }

    CAMERA.id         = -1;
    CAMERA.tile.y     = WORLD.metadata->startingTile.y;
    CAMERA.tile.x     = WORLD.metadata->startingTile.x;
    CAMERA.position.y = (I16(CAMERA.tile.y) << TILE_SCALE_BITS)  - (TILE_HEIGHT >> 1);
    CAMERA.position.x = (I16(CAMERA.tile.x) << TILE_SCALE_BITS)  + (TILE_WIDTH >> 1);
    MEMZERO(g_tileFlags);
}
