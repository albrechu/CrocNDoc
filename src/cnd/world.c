#include <cnd/world.h>

#include <cnd/xutils.h>
#include <lib/print/print.h>
#include <vectrex.h>
#include <lib/assert/assert.h>
#include <lib/debug/static_assert.h>
#include <lib/debug/monitor.h>

#define POS(y, x) y, x // Starting position
#define TILE(y, x) y, x
#define DANGER_HEIGHT(y) I8(y >> 8), I8(y)
#define METADATA_BYTES 4

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

#define TE(id) (id << 3) | Tile_Enemy
#define TP(id) (id << 3) | Tile_Prop

static const Tile g_stageSewers[] = 
{
    TW, TC, TC, TC, TW,  E,  E,  E, TW, TC, TC, TC, TC, TC, TC, TC, TC, TC, TC,  E,  E,  E,  E,  E,
    TW,  E, TW,  E, TW,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,
    TW,  E, TE(0),  E, TW,  E,  E,  E, TW,  E, TG, TG, TG, TG, TG,  E,  E,  E, TW,  E,  E,  E,  E,  E,
    TW, TG, TG,  E, TW,  E,  E,  E, TW,  E, TW,  E,  E,  E, TW,  E,  E,  E, TW,  E,  E,  E,  E,  E,
     E,  E, TW,  P, TW,  E,  E,  E, TW,  E, TW,  E,  E,  E, TW,  E,  E,  E, TW,  E,  E,  E,  E,  E,
     E,  E, TW,  E, TW,  E,  E,  E, TW,  E, TW,  E,  E,  E, TW,  E,  E,  E, TW,  E,  E,  E,  E,  E,
     E,  E, TW,  E, TW, TC, TC, TC, TW,  E, TW, TC, TC, TC, TC,  E,  E,  E, TW,  E,  E,  E,  E,  E,
     E,  E, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,
     E,  E, TW,  P, TW,  E,  E,  E,  E,  E,  E,  E, TE(2),  E, TE(3),  E, TP(0),  E, TW,  E,  E,  E,  E,  E,
     E,  E, TW,  E,  E,  E,  E, TW, TG, TE(1), TG, TG, TG, TG, TG, TG, TG, TG, TW, TG, TG, TG, TG, TG,
     E,  E, TW,  E,  E,  E,  P, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,
     E,  E, TW,  E,  E,  E,  E, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,
     E,  E, TW, TG, TG,  E,  E, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TP(3),  E, TW,
     E,  E,  E,  E, TW,  E,  E, TW, TW,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,
     E,  E,  E,  E, TW, TG, TG, TW, TW,  E,  E,  E,  E,  E,  P,  E,  E,  E,  E,  E,  E,  E,  E, TW,
     E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,
     E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E, TP(1),  E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,
     E,  E,  E,  E,  E,  E,  E,  E, TW, TG, TG,  E,  P,  E,  E,  E,  E,  E,  E,  E,  E, TP(2),  E, TW,
     E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  P,  E, TG, TG, TG, TG, TG, TG,
     E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,  E,  E, TW,  E,  E,  E,  E,  E,
     E,  E,  E,  E,  E,  E,  E,  E,  E,  E, TW, TS, TS, TS, TS, TS, TS, TS, TW,  E,  E,  E,  E,  E,
     E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
     E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
     E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
};
static_assert(sizeof(g_stageSewers) == ((256u >> TILE_SCALE_BITS) * WORLD_HEIGHT) * ((256u >> TILE_SCALE_BITS) * WORLD_WIDTH));

const EntityType g_sewersEnemies[] = 
{
    Enemy_Wegelagerer,
    Enemy_Tunichtgut,
    Enemy_Halunke,
    Enemy_Halunke,
};

const EntityType g_sewersProps[] = 
{
    Prop_Barrel,
};

const v2i g_stageStartPts[] = 
{
    { 4, 2 }, // Stage_Sewers
    // { 0, 0 }, // Stage_WaErways
    // { 0, 0 }, // Stage_MolEnlands
    // { 0, 0 }, // Stage_Bonus
};

const Tile* const g_tilesets[] = 
{
    g_stageSewers,
    // g_stageWaErways,
    // g_stageMolEnlands,
    // g_stageBonus,
};

// static const Tile g_blank[(8 * WORLD_HEIGHT) * (8 * WORLD_WIDTH)] = 
// {
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
//      E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,  E,
// };

void check_collision()
{
    entity e = &ENTITY(WORLD.selectedEntity);
    // e->velocity.x += Velocity_Friction * (e->velocity.x != 0 && e->velocity.y == 0) * e->transform;
	// e->velocity.y += Velocity_Gravity;
    // monitor("VX", e->velocity.x);
    // monitor("VY", e->velocity.y);

    v2l tiles[2]; // tiles to check
    tiles[0].x = e->position.x >> TILE_SCALE_BITS;
    tiles[0].y = e->position.y >> TILE_SCALE_BITS;
    tiles[1].x = (e->position.x + e->velocity.x) >> TILE_SCALE_BITS;
    tiles[1].y = (e->position.y + e->velocity.y) >> TILE_SCALE_BITS;

    i8 tileMax = tiles[0].x != tiles[1].x && tiles[0].y != tiles[1].y;
    // monitor("TILE MAX", tileMax);
    if (false)
    for (i8 t = 0; t <= tileMax; ++t)
    {
        i16 tileIdx = tiles[t].y * WORLD_WIDTH + tiles[t].x;

        switch (WORLD.tileset[tileIdx])
        {
        case Tile_Ground:
        {   
            i8 relative = I8(e->position.y - (tiles[t].x << TILE_SCALE_BITS));
            if (relative + e->velocity.y >= 32)
            {
                e->velocity.y = 0;
            }
        }
            break;
        case Tile_Wall:
            break;
        case Tile_Ceiling:
        {   
            i8 relative = I8(e->position.y - (tiles[t].x << TILE_SCALE_BITS));
            if (relative + e->velocity.y < 0)
            {
                e->velocity.y = 0;
            }
        }
            break;
        case Tile_Platform:
            break;
        case Tile_Spikes:

            break;
        case Tile_Empty:

            break;
        default:
            break;
        }    
    }
    e->position.x += e->velocity.x;
    e->position.y += e->velocity.y;
}

void world_progress(void)
{
    WORLD.selectedEntity = 0;
    check_collision();
    ++WORLD.selectedEntity;
    i8 e;
    assert(WORLD.enemies == 0);
    for (e = 0; e < WORLD.enemies; ++e, ++WORLD.selectedEntity)
    {
        check_collision();
    }
    WORLD.selectedEntity += ENEMIES_MAX - e;
    assert(WORLD.props == 0);
    for (e = 0; e < WORLD.props; ++e, ++WORLD.selectedEntity)
    {
        check_collision();
    }
    
    //if ((CAMERA.velocity.x | CAMERA.velocity.y) == 0) // camera is static
    //    return;

    LIST.count = 0;

    i8 y = I8(CAMERA.position.y >> TILE_SCALE_BITS);
    i8 x = I8(CAMERA.position.x >> TILE_SCALE_BITS);
    
    WORLD.selectedTile.x = x - 2;
    WORLD.selectedTile.x = MAX8(0, WORLD.selectedTile.x);
    WORLD.selectedTile.y = y - 2;
    WORLD.selectedTile.y = MAX8(0, WORLD.selectedTile.y);

    i8 xMax = WORLD.selectedTile.x + 4;
    xMax = MIN8(xMax, I8((256u >> TILE_SCALE_BITS) * WORLD_WIDTH));
    i8 yMax = WORLD.selectedTile.y + 4;
    yMax = MIN8(yMax, I8((256u >> TILE_SCALE_BITS) * WORLD_HEIGHT));

    i8 tdx = xMax - WORLD.selectedTile.x;

    i16 left   = CAMERA.position.x - 128;
    i16 right  = CAMERA.position.x + 127;
    i16 top    = CAMERA.position.y + 127;
    i16 bottom = CAMERA.position.y - 128;

    i16 tileIdx    = WORLD.selectedTile.y * WORLD_WIDTH + WORLD.selectedTile.x;    
    // i16 tileIdxMax = yMax * WORLD_WIDTH + xMax;
    // monitor("TILE Y", y);
    // monitor("TILE X", x);
    while (true)
    {
        i16  tileLeft   = I16(WORLD.selectedTile.x) << TILE_SCALE_BITS;
        i16  tileRight  = tileLeft + (1 << TILE_SCALE_BITS);
        i16  tileTop    = I16(WORLD.selectedTile.y) << TILE_SCALE_BITS;
        i16  tileBottom = tileTop + (1 << TILE_SCALE_BITS);
        bool clip       = !(tileRight <= left || tileLeft >= right || tileBottom <= top || tileTop >= bottom);

        // monitor("TILE IDX", tileIdx);
        if (!clip)
        {
            i16 offsetX = tileLeft - left;
            i16 offsetY = tileTop  - top;
            const Tile tile = WORLD.tileset[tileIdx];
            assert(LIST.count <= DRAW_STRIPS_MAX);
            line_strip_t* strip = &LIST.strips[LIST.count];
            switch (tile & Tile_TypeMax)
            {
            case Tile_Ground:
                if (tileBottom >= bottom && tileTop <= top)
                {
                    strip->pos.x = I8(offsetX);
                    strip->pos.y = I8(offsetY);
                    strip->data = mesh_get(Mesh_Ground);
                    ++LIST.count;
                }
                break;
            case Tile_Wall:
                if (tileRight <= right && tileLeft >= left)
                {
                    strip->pos.x = I8(offsetX);
                    strip->pos.y = I8(offsetY);
                    strip->data = mesh_get(Mesh_Wall);
                    ++LIST.count;
                }
                break;
            case Tile_Ceiling:
                strip->pos.x = I8(offsetX);
                strip->pos.y = I8(offsetY);
                strip->data = mesh_get(Mesh_Ceiling);
                ++LIST.count;
                break;
            case Tile_Platform:
                strip->pos.x = I8(offsetX);
                strip->pos.y = I8(offsetY);
                strip->data = mesh_get(Mesh_Platform);
                ++LIST.count;
                break;
            default:
                break;
            }
        }

        if (WORLD.selectedTile.x < xMax)
        {
            ++WORLD.selectedTile.x;
            ++tileIdx;
        }
        else
        {
            // monitor("Y",    WORLD.selectedTile.y);
            // monitor("YMAX", yMax);
            if (++WORLD.selectedTile.y < yMax) // Next row
            {
                tileIdx              += I16((256u >> TILE_SCALE_BITS) * WORLD_WIDTH) - I16(tdx);
                WORLD.selectedTile.x -= tdx; // Reset to start of row
                continue;
            }
            break;
        }
    }
    

    // for (; WORLD.selectedTile.y < yMax; ++WORLD.selectedTile.y, tileIdx += WORLD_WIDTH - tdx)
    // {
    //     for (; WORLD.selectedTile.x < xMax; ++WORLD.selectedTile.x, ++tileIdx)
    //     {
            
            

    //     }
    //     WORLD.selectedTile.x -= I8(tdx);
    // }
    
}


world_t g_world;

void world_create(Stage const stage)
{
    MEMZERO(WORLD);
    CAMERA.position.x = I16(g_stageStartPts[stage].x) << TILE_SCALE_BITS;
    CAMERA.position.y = I16(g_stageStartPts[stage].y) << TILE_SCALE_BITS;
    WORLD.tileset     = g_tilesets[stage];
}

void world_draw(void) 
{
    for (i8 i = 0; i < WORLD.drawList.count; ++i)
    {    
        beam_set_position(WORLD.drawList.strips[i].pos.y, WORLD.drawList.strips[i].pos.x);
        Draw_VLc((void* const)WORLD.drawList.strips[i].data);
    }
}


