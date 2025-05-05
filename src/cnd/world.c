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
    E   = Tile_Empty,
    T   = Tile_Top,
    TL  = Tile_TopLeft,
    TR  = Tile_TopRight,
    B   = Tile_Bottom,
    BL  = Tile_BottomLeft,
    BR  = Tile_BottomRight,
    L   = Tile_Left,
    R   = Tile_Right,
    M   = Tile_Middle,
    S   = Tile_Spikes,
    J   = Tile_Jumper,
    B1  = Tile_BarrierVertical,
    B2  = Tile_BarrierHorizontal,
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
};

/** 
 * 
 * Legend: _ ground, | wall, - platform, = breakable ground, 
 *         c crate, w spikes
 *         
 * Stage_Sewers:
 *                  ________
 *  _____           |   __ -|
 * |____ |    ______|  |__|-|           
 *     | |___| ______ _____-|
 *     |_____ -|    |=|
 *          |__|    | |_______________
 *                  |___c__ _         |____
 *                        |       _   _____=>
 *                        |wwwwwwwwww|
 */
// static const Tile g_stageSewers[] = 
// {
//     TW, TC, TC, TC, TC, TC, TC, TW,  E,  E,  E,  E, TW, TC, TC, TC, TC, TC,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//     TW,  E,  E,  E, TW,  E,  E, TW,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//     TW,  E,  E,  E, TW,  E,  E, TW,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//     TW,  E,  E,  E, TE,  E,  E, TW,  E,  E,  E,  E, TW,  E, TG, TG, TG,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//     TW, TG, TG, TG, TG,  E,  E, TW,  E,  E,  E,  E, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E,  E, TW,  E,  E,  E,  E, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  P,  E, TW,  E,  E,  E,  E, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E,  E, TW,  E,  E,  E,  E, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E,  P, TW, TC, TC, TC, TC, TW,  E, TW, TC, TC,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  P,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E, TW, TG, TE, TG, TG, TG, TG, TG, TG,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  P, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW,  E, TP,  E,  E,  E,  E, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E, TW, TG, TG, TG,  E,  E,  E, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E, TW, TG, TG, TG, TW, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW, TG, TG,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW, TS, TS, TS, TS, TS,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
// };

static const Tile g_stageSewers[] = 
{
      E,   B,   B,   B,   E,   E,   E,   E,   E,   B,   B,   B,   B,   B,   B,   B,   B,   B,   E,   E,   E,   E,   E,   E,
      R,   E,   R,   E,  TR,   E,   E,   E,   R,   E,   E,   E,   E,   E,   E,   E,   E,   E,   L,   E,   E,   E,   E,   E,
      R,   E,  B1,   E,   R,   E,   E,   E,   R,   E,   B,   B,   B,   B,   B,   E,   E,   E,   L,   E,   E,   E,   E,   E,
      R,   T,  TR,   E,   R,   E,   E,   E,   R,   E,   L,   E,   E,   E,   R,   E,   E,   E,   L,   E,   E,   E,   E,   E,
      R,   E,   R,   M,   R,   E,   E,   E,   R,   E,   L,   E,   E,   E,   R,   E,   E,   E,   L,   E,   E,   E,   E,   E,
      R,   E,   R,   E,   R,   E,   E,   E,   R,   E,   L,   E,   E,   E,   R,   E,   E,   E,   L,   E,   E,   E,   E,   E,
      R,   E,   R,   E,   R,   E,   E,   E,   R,   E,   T,   T,   T,   T,   T,   T,   E,   B,   L,   E,   E,   E,   E,   E,
      R,   E,   R,   E,   R,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   T,   E,   L,   E,   E,   E,   E,   E,
      R,   E,   R,   M,   E,   E,   E,   E,   E,   E,   E,   E,  E2,   E,  E3,   E,  E8,   B,   L,   E,   E,   E,   E,   E,
      R,   E,   R,   E,   E,   E,   E,   E,   E,   E,   B,   B,   B,   B,   B,   B,   B,   B,   L,   B,   B,   B,   B,   B,
      R,   E,   R,   E,   E,   E,   M,  TL,  TR,  B2,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,  BL,   E,   E,   E,   L,   R,   E,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   T,   L,  E6,   L,   R,   E,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  E9,   R,   E, 
      R,   E,   E,   E,   E,   L,   M,   L,   R,   E,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   T,   T,   E,   R,   E,   E,   E,   E,   E,   M,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   E,   E,   E,   R,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   E,   E,   E,   R,   E,   E,   E,  E7,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   E,   E,   E,   E,   T,   T,   E,   M,   E,   E,   E,   E,   E,   E,   E,   E,   J,  BR,   E,
      R,   E,   E,   M,   E,   M,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   M,   E,  TL,   T,   T,   T,   E,   E,
      R,   E,   E,   E,   E,   E,   M,   E,   M,   E,   M,   E,   S,   S,   S,   S,   S,   S,   L,   E,   E,   E,   E,   E,
      R,   E,   E,   E,   S,   S,   S,   S,   S,   S,   S,   S,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   T,   T,   T,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
};
static_assert(sizeof(g_stageSewers) == (I16(WORLD_EXTENT) * I16(WORLD_STRIDE)));

const EntityType g_sewersEntities[16] = 
{
    Enemy_Tunichtgut,
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

const v2i g_stageStartPts[] = 
{
    { 22,10 }, // Stage_Sewers
    { 0, 0 }, // Stage_WaErways
    { 0, 0 }, // Stage_MolEnlands
    { 0, 0 }, // Stage_Bonus
};

const Tile* const g_tilesets[] = 
{
    g_stageSewers,
    // g_stageWaErways,
    // g_stageMolEnlands,
    // g_stageBonus,
};

const EntityType* const g_stageEntities[] = 
{
    g_sewersEntities,
    // g_waErwaysEnemies,
    // g_molEnlandsEnemies,
    // g_bonusEnemies,
};
static bool g_tileFlags[4];
world_t g_world;

void check_collision()
{
    entity e = &WORLD.entities[WORLD.selectedEntity];
    e->velocity.x -= WORLD.drag * (e->velocity.x != 0 && e->velocity.y == 0) * e->transform;
	e->velocity.y -= WORLD.drag;
    // e->velocity.y  = MAX8(-8, e->velocity.y);
    // print_signed_int(-100, -50, e->tile.y);
    // print_signed_int(-100, -20, e->tile.x);

    
    v2i tiles[3]; // tiles to check
    tiles[0].y = e->tile.y;
    tiles[0].x = e->tile.x;
    tiles[1].y = I8((e->position.y - e->velocity.y) >> TILE_SCALE_BITS);
    tiles[1].x = e->tile.x;
    tiles[2].y = e->tile.y;
    tiles[2].x = I8((e->position.x + e->velocity.x) >> TILE_SCALE_BITS);

    for (i8 i = 0; i < 3; ++i)
    {
        v2i* tile = &tiles[i];
        // if (tile->y != tile->y || tile->x != tile->x)
        {
            switch (WORLD.tileset[I16(tile->y) * WORLD_STRIDE + tile->x])
            {
            case Tile_Top:
            {
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y <= 0)
                    e->velocity.y = 0;
            }
            break;
            case Tile_TopLeft:
            {
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y <= 0)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x <= 0)
                    e->velocity.x = 0;
            }
                break;
            case Tile_TopRight:
            {
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) - e->velocity.y <= 0)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x >= TILE_WIDTH)
                    e->velocity.x = 0;
            }
                break;
            case Tile_Bottom:
            {
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y >= TILE_HEIGHT)
                    e->velocity.y = 0;
            }
                break;
            case Tile_BottomLeft:
            {
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y >= TILE_HEIGHT)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x <= 0)
                    e->velocity.x = 0;
            }
                break;
            case Tile_BottomRight:
            {
                i8 relY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if (relY - e->velocity.y >= TILE_HEIGHT - 1)
                    e->velocity.y = 0;
                i8 relX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                if (relX + e->velocity.x >= TILE_WIDTH - 2)
                    e->velocity.x = 0;
            }
            
                break;
            case Tile_Left:
            {
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x <= 0)
                    e->velocity.x = 0;
            }
                break;
            case Tile_Right:
            if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + (i8)Abs_b(e->velocity.x) >= TILE_WIDTH)
                    e->velocity.x = 0;
                break;
            case Tile_MiddleLeft:
            
                break;
            case Tile_MiddleRight:
            
                break;
            case Tile_Middle:
            {
                const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                if (relativeY + e->velocity.y >= TILE_HEIGHT >> 1 && relativeX + e->velocity.x <= TILE_WIDTH >> 1 && relativeX + e->velocity.x >= 0)
                    e->velocity.y = 0;
            }
                break;
            case Tile_Spikes:
            {
                const i8 relative = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if (relative - e->velocity.y >= TILE_HEIGHT)
                {
                    e->velocity.y = 0;
                    e->state      = EntityState_Dead;
                }
            }
                break;
            case Tile_Jumper:
            {
                const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                if (relativeY + e->velocity.y >= (TILE_HEIGHT >> 1) && relativeX + e->velocity.x <= (TILE_WIDTH - (TILE_WIDTH >> 2)) && relativeX + e->velocity.x >= (TILE_WIDTH >> 2))
                {
                    e->velocity.y = Velocity_Jumper;
                }
            }
                break; 
            case Tile_BarrierVertical:
            if (!TILE_FLAG(0))
            {
                const i8 relative = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                i8 velocity = (i8)Abs_b(e->velocity.x);
                if (relative + velocity >= TILE_WIDTH)
                {                   
                    if (velocity > 6) // Broke the barrier
                        TILE_FLAG(0) = true;
                    else
                        e->velocity.x = 0;
                }
            }
                break;
            case Tile_BarrierHorizontal:
                if (!TILE_FLAG(1))
                {
                    if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) - e->velocity.y >= TILE_WIDTH)
                    {                   
                        if (e->velocity.y > 9) // Broke the barrier
                            TILE_FLAG(1) = true;
                        else
                            e->velocity.y = 0;
                    }
                }
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
            default:
                break;
            }
        }
    }
    
    // tiles[1].x = I8((e->position.x + e->velocity.x) >> TILE_SCALE_BITS);
    // tiles[1].y = I8((e->position.y - e->velocity.y) >> TILE_SCALE_BITS);
    
    // if (WORLD.selectedEntity == 0)
    // {
    //     print_signed_int(-50, -40, e->velocity.y);
    //     print_signed_int(-50, 0, e->velocity.x);
    // }
    e->position.x += e->velocity.x;
    e->position.y -= e->velocity.y;
    e->tile.x = I8(e->position.x >> TILE_SCALE_BITS);
    e->tile.y = I8(e->position.y >> TILE_SCALE_BITS);

    // print_signed_int(-50, -40, e->isLocal);
    // print_signed_int(-50, -40, e->tile.y);
    // print_signed_int(-50, 0, e->tile.x);
}

void world_progress(void)
{
    ++WORLD.ticks;
    WORLD.drag = WORLD.ticks & 1;

    for (idx_t i = 0; i < WORLD.entityCount; ++i)
    {
        WORLD.selectedEntity = WORLD.entityIdxs[WORLD.entityIdxs[i]];
        print_signed_int(-50, -40, WORLD.selectedEntity);
        check_collision();
    }
        
    //if ((CAMERA.velocity.x | CAMERA.velocity.y) == 0) // camera is static
    //    return;

    LIST.lines  = 0;
    LIST.strips = 0;

    // i8 y = I8(CAMERA.position.y >> TILE_SCALE_BITS);
    // i8 x = I8(CAMERA.position.x >> TILE_SCALE_BITS);
    
    i16 left   = CAMERA.position.x - 128;
    // i16 right  = CAMERA.position.x + 127;
    // i16 top    = CAMERA.position.y + 127;
    i16 bottom = CAMERA.position.y - 128;

    WORLD.selectedTile.x = I8(left   >> TILE_SCALE_BITS);
    WORLD.selectedTile.y = I8(bottom >> TILE_SCALE_BITS);

    i8 xMax = WORLD.selectedTile.x + 5;
    xMax = MIN8(xMax, WORLD_STRIDE);
    xMax = MAX8(0, xMax);
    i8 yMax = WORLD.selectedTile.y + 5;
    yMax = MIN8(yMax, WORLD_EXTENT);
    yMax = MAX8(0, yMax);
    WORLD.selectedTile.x = MAX8(0, WORLD.selectedTile.x);
    WORLD.selectedTile.y = MAX8(0, WORLD.selectedTile.y);
    
    i8 idxStride = xMax - WORLD.selectedTile.x;
    i8 idxDelta  = WORLD_STRIDE - idxStride + 1;
    i16 tileIdx    = WORLD.selectedTile.y * I16(WORLD_STRIDE) + WORLD.selectedTile.x;    
    // i16 tileIdxMax = yMax * WORLD_WIDTH + xMax;
    while (true)
    {
        i16 tileLeft   = (I16(WORLD.selectedTile.x) << TILE_SCALE_BITS) - CAMERA.position.x;
        i16 tileRight  = tileLeft          + TILE_WIDTH;
        i16 tileTop    = CAMERA.position.y - (I16(WORLD.selectedTile.y) << TILE_SCALE_BITS); // This one inverts the y axis
        i16 tileBottom = tileTop           - TILE_HEIGHT;
        
        // monitor("TILE IDX", tileIdx);
        // if (tileLeft >= I16(-128) && tileRight <= I16(127) && tileTop <= I16(127) && tileBottom >= I16(-128))
        // bool clip       = !(tileRight <= left || tileLeft >= right || tileBottom <= top || tileTop >= bottom);
        // if (tileLeft >= -128 && tileLeft <= 127 && tileTop <= 127 && tileTop >= -128)
        {
            // assert(LIST.count <= DRAW_STRIPS_MAX);
            // i16 offsetX = tileLeft - left;
            // i16 offsetY = tileTop  - top;
            const Tile tile = WORLD.tileset[tileIdx];

            switch (tile)
            {
            case Tile_Top:
            if (tileTop <= 127 && tileTop >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileTop);
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = TILE_WIDTH;
                }
                if (WORLD.selectedTile.x == 9 && WORLD.selectedTile.y == 17)
                {
                    // print_long_signed_int(-50, -40, tileTop);
                    // print_long_signed_int(-50, -20, tileTop.x);
                    // monitor("TILE IDX", tileIdx);
                    // monitor("TILE LEFT", tileLeft);
                    // monitor("TILE RIGHT", tileRight);
                    // monitor("TILE TOP", tileTop);
                    // monitor("TILE BOTTOM", tileBottom);
                }
            }
            break;
            case Tile_TopLeft:
            if (tileTop <= 127 && tileTop >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileTop);
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = TILE_WIDTH;
                }
            }
            if (tileLeft <= 127 && tileLeft >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.x = I8(tileLeft);
                l->delta.x = 0;
                if (tileTop > 127) // clip
                {
                    l->start.y = 127;
                    l->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    l->start.y = I8(tileTop);
                    l->delta.y = -TILE_HEIGHT;
                }
            }
            break;
            case Tile_TopRight:
            if (tileTop <= 127 && tileTop >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileTop);
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = TILE_WIDTH;
                }
            }
            if (tileRight <= 127 && tileRight >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.x = I8(tileRight);
                l->delta.x = 0;
                if (tileTop > 127) // clip
                {
                    l->start.y = 127;
                    l->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    l->start.y = I8(tileTop);
                    l->delta.y = -TILE_HEIGHT;
                }
            }
                break;
            case Tile_Bottom:
            if (tileBottom <= 127 && tileBottom >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileBottom);
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = TILE_WIDTH;
                }
            }
                break;
            case Tile_BottomLeft:
            if (tileBottom <= 127 && tileBottom >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileBottom);
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = TILE_WIDTH;
                }
            }
            if (tileLeft <= 127 && tileLeft >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.x = I8(tileLeft);
                l->delta.x = 0;
                if (tileBottom > 127) // clip
                {
                    l->start.y = 127;
                    l->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    l->start.y = I8(tileTop);
                    l->delta.y = -TILE_HEIGHT;
                }
            }
                break;
            case Tile_BottomRight:
            if (tileBottom <= 127 && tileBottom >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileBottom);
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = TILE_WIDTH;
                }
            }
            if (tileRight <= 127 && tileRight >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.x = I8(tileRight);
                l->delta.x = 0;
                if (tileBottom > 127) // clip
                {
                    l->start.y = 127;
                    l->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    l->start.y = I8(tileTop);
                    l->delta.y = -TILE_HEIGHT;
                }
            }
                break;
            case Tile_Left:
            if (tileLeft <= 127 && tileLeft >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.x = I8(tileLeft);
                l->delta.x = 0;
                if (tileTop > 127) // clip
                {
                    l->start.y = 127;
                    l->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    l->start.y = I8(tileTop);
                    l->delta.y = -TILE_HEIGHT;
                }
            }
            break;
            case Tile_Right:
            if (tileRight <= 127 && tileRight >= -128)
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.x = I8(tileRight);
                l->delta.x = 0;
                if (tileTop > 127) // clip
                {
                    l->start.y = 127;
                    l->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    l->start.y = I8(tileTop);
                    l->delta.y = -TILE_HEIGHT;
                }
            }
            break;
            case Tile_MiddleLeft:
            if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileTop - (TILE_HEIGHT >> 1));
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft);
                    l->delta.x = (TILE_WIDTH >> 1);
                }
            }
            break;
            case Tile_MiddleRight:
            if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileTop - (TILE_HEIGHT >> 1));
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft + (TILE_WIDTH >> 1));
                    l->delta.x = (TILE_WIDTH >> 1);
                }
            }
            break;
            case Tile_Middle:
            if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
            {
                line_t* l = &LIST.line[LIST.lines++];
                l->start.y = I8(tileTop - (TILE_HEIGHT >> 1));
                l->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    l->start.x = -128;
                    l->delta.x = (TILE_WIDTH >> 1) + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    l->start.x = I8(tileLeft + (TILE_WIDTH >> 2));
                    l->delta.x = (TILE_WIDTH >> 1);
                }
            }
                break;
            case Tile_Spikes:
            {
                line_strip_t* s = &LIST.strip[LIST.strips++];
                s->start.y = I8(tileBottom);
                s->start.x = I8(tileLeft);
                s->data = mesh_get(Mesh_Spikes);
            }
            break; 
            case Tile_Jumper:
            {
                line_strip_t* s = &LIST.strip[LIST.strips++];
                s->start.y = I8(tileBottom);
                s->start.x = I8(tileLeft + (TILE_WIDTH >> 2));
                s->data = mesh_get(Mesh_Jumper);
            }
                break;
            case Tile_BarrierVertical:
            if (!TILE_FLAG(0))
            {
                line_strip_t* s = &LIST.strip[LIST.strips++];
                s->start.y = I8(tileTop);
                s->start.x = I8(tileRight);
                s->data    = mesh_get(Mesh_BarrierVertical);
            }
                break;
            case Tile_BarrierHorizontal:
            if (!TILE_FLAG(1))
            {
                line_strip_t* s = &LIST.strip[LIST.strips++];
                s->start.y = I8(tileBottom);
                s->start.x = I8(tileRight);
                s->data    = mesh_get(Mesh_BarrierHorizontal);
            }
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
            {
                idx_t idx = tile - Tile_E0;
                if (WORLD.entityStatus[idx] == EntityStatus_Alive)
                {
                    WORLD.entityStatus[idx] = EntityStatus_Active;
                    entity e      = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount]];
                    e->id         = WORLD.entityCount++;
                    e->tile.y     = WORLD.selectedTile.y;
                    e->tile.x     = WORLD.selectedTile.x;
                    e->position.y = I16(tileTop)  + (TILE_HEIGHT >> 1);
                    e->position.x = I16(tileLeft) + (TILE_WIDTH >> 1);
                    e->velocity.x = 0;
                    e->velocity.y = 0;
                    e->type       = WORLD.pentities[idx];
                    e->isLocal    = true;
                    e->isEnemy    = e->type > Prop_Max;
                    e->state      = e->isEnemy ? EnemyState_Follow : PropState_Idle;
                    e->transform  = 1;
                    switch (e->type)
                    {
                    case Prop_Crate:
                        e->mesh = Mesh_Crate;
                        break;
                    case Prop_Barrel:
                        e->mesh = Mesh_Barrel;
                        break;
                    case Enemy_Tunichtgut:
                        break;
                    case Enemy_Halunke:
                        break;
                    case Enemy_Gauner:
                        break;
                    case Enemy_Schuft:
                        break;
                    case Enemy_Strolch:
                        break;
                    case Enemy_Boesewicht:
                        break;
                    default:
                        break;
                    }
            }
                break;
            default:
                break;
            }
        }

        if (++WORLD.selectedTile.x < xMax)
        {
            ++tileIdx;
        }
        else
        {
            if (++WORLD.selectedTile.y < yMax) 
            {
                tileIdx              += idxDelta;
                WORLD.selectedTile.x -= idxStride; // Reset to start of row
            }
            else // End of row
            {
                return;
            }
        }
    }
}

}

void world_create(Stage const stage)
{
    MEMZERO(WORLD);
    WORLD.tileset     = g_tilesets[stage];
    WORLD.entityCount = 1; // 1 for the player
    WORLD.pentities   = g_stageEntities[stage];
    for (i8 i = 0; i < ENTITIES_MAX; i++)
        WORLD.entityIdxs[i] = i;

    CAMERA.tile.y     = g_stageStartPts[stage].y;
    CAMERA.tile.x     = g_stageStartPts[stage].x;
    CAMERA.position.y = (I16(CAMERA.tile.y) << TILE_SCALE_BITS)  - (TILE_HEIGHT >> 1);
    CAMERA.position.x = (I16(CAMERA.tile.x) << TILE_SCALE_BITS)  + (TILE_WIDTH >> 1);
    MEMZERO(g_tileFlags);
}

void world_draw(void) 
{
    Intensity_5F();
    for (i8 li = 0; li < LIST.lines; ++li)
    {
        beam_set_position(LIST.line[li].start.y, LIST.line[li].start.x);
        Draw_Line_d(LIST.line[li].delta.y, LIST.line[li].delta.x);
    }
    
    for (i8 si = 0; si < LIST.strips; ++si)
    {    
        beam_set_position(LIST.strip[si].start.y, LIST.strip[si].start.x);
        Draw_VLc((void* const)LIST.strip[si].data);
    }
}

