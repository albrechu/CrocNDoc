#include <cnd/world.h>

#include <cnd/xutils.h>
#include <lib/print.h>
#include <vectrex.h>
#include <lib/assert.h>
#include <lib/static_assert.h>
#include <lib/monitor.h>

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

#define TE(id) (id << 4) | Tile_Enemy
#define TP(id) (id << 4) | Tile_Prop
#define E0     TE(0)
#define E1     TE(1)
#define E2     TE(2)
#define E3     TE(3)
#define E4     TE(4)
#define E5     TE(5)
#define E6     TE(6)
#define E7     TE(7)
#define P0     TP(0)
#define P1     TP(1)
#define P2     TP(2)
#define P3     TP(3)
#define P4     TP(4)
#define P5     TP(5)
#define P6     TP(6)
#define P7     TP(7)

static const Tile g_stageSewers[] = 
{
      E,  TG,  TG,  TG,  TG,   E,   E,   E,   E,  TG,  TG,  TG,  TG,  TG,  TG,  TG,  TG,  TG,   E,   E,   E,   E,   E,   E,
     TR,   E,  TR,   E,  TR,   E,   E,   E,  TR,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TL,   E,   E,   E,   E,   E,
     TR,   E,   E,   E,  TR,   E,   E,   E,  TR,   E,  TG,  TG,  TG,  TG,  TG,   E,   E,   E,  TL,   E,   E,   E,   E,   E,
     TR,  TC,  TC,  TL,  TR,   E,   E,   E,  TR,   E,  TL,   E,   E,   E,  TR,   E,   E,   E,  TL,   E,   E,   E,   E,   E,
     TR,   E,  TR,   P,  TR,   E,   E,   E,  TR,   E,  TL,   E,   E,   E,  TR,   E,   E,   E,  TL,   E,   E,   E,   E,   E,
     TR,   E,  TR,   E,  TR,   E,   E,   E,  TR,   E,  TL,   E,   E,   E,  TR,   E,   E,   E,  TL,   E,   E,   E,   E,   E,
     TR,   E,  TR,   E,  TR,   E,   E,   E,  TR,   E,  TC,  TC,  TC,  TC,  TC,  TC,   E,  TG,  TL,   E,   E,   E,   E,   E,
     TR,   E,  TR,   E,  TR,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TC,   E,  TL,   E,   E,   E,   E,   E,
     TR,   E,  TR,   P,   E,   E,   E,   E,   E,   E,   E,   E,  E2,   E,  E3,   E,  P1,  TG,  TL,   E,   E,   E,   E,   E,
     TR,   E,  TR,   E,   E,   E,   E,  TG,  TG,  E1,  TG,  TG,  TG,  TG,  TG,  TG,  TG,  TG,  TL,  TG,  TG,  TG,  TG,  TG,
     TR,   E,  TR,   E,   E,   E,   P,  TL,  TR,   E,  TL,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TL,
     TR,   E,  TR,   E,   E,   E,   E,  TL,  TR,   E,  TL,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TL,
     TR,   E,   E,  TC,  TC,  TL,  P0,  TL,  TR,   E,  TL,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  P4,   E,  TL, 
     TR,   E,   E,   E,   E,  TL,   P,  TL,  TR,   E,  TL,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TL,
     TR,   E,   E,   E,   E,  TC,  TC,   E,  TR,   E,   E,   E,   E,   E,   P,   E,   E,   E,   E,   E,   E,   E,   E,  TL,
     TR,   E,   E,   E,   E,   E,   E,   E,  TR,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TL,
     TR,   E,   E,   E,   E,   E,   E,   E,  TR,   E,   E,   E,  P2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  TL,
     TR,   E,   E,   E,   E,   E,   E,   E,   E,  TC,  TC,   E,   P,   E,   E,   E,   E,   E,   E,   E,   E,  P3,   E,  TL,
     TR,   E,   E,   P,   E,   P,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   P,   E,  TG,  TG,  TG,  TG,  TG,  TL,
     TR,   E,   E,   E,   E,   E,   P,   E,   P,   E,   P,   E,   E,   E,   E,   E,   E,   E,  TL,   E,   E,   E,   E,   E,
     TR,  P5,   E,   E,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,  TS,   E,   E,   E,   E,   E,   E,
      E,  TC,  TC,  TC,  TL,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
};
static_assert(sizeof(g_stageSewers) == (I16(WORLD_EXTENT) * I16(WORLD_STRIDE)));

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
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
};

const v2i g_stageStartPts[] = 
{
    { 11, 7 }, // Stage_Sewers
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
    e->velocity.x -= WORLD.drag * (e->velocity.x != 0 && e->velocity.y == 0) * e->transform;
	e->velocity.y -= WORLD.drag;
    // e->velocity.y  = MAX8(-8, e->velocity.y);
    // print_signed_int(-100, -50, e->tile.y);
    // print_signed_int(-100, -20, e->tile.x);

    
    v2i tiles[2]; // tiles to check
    tiles[0].y = e->tile.y;
    tiles[0].x = e->tile.x;
    tiles[1].y = I8((e->position.y - e->velocity.y) >> TILE_SCALE_BITS);
    tiles[1].x = I8((e->position.x + e->velocity.x) >> TILE_SCALE_BITS);
    const i8 idY[3] = { 0, 1, 0 };
    const i8 idX[3] = { 0, 0, 1 };

    for (i8 i = 0; i < 3; ++i)
    {
        v2i* tile1 = &tiles[idY[i]];
        v2i* tile2 = &tiles[idX[i]];
        if (tile1->y != tile2->y || tile1->x != tile2->x)
        {
            switch (WORLD.tileset[I16(tile1->y) * WORLD_STRIDE + tile2->x])
            {
            case TG: // Bottom Line  _
            {   
                const i8 relative = I8(e->position.y - (tile1->y << TILE_SCALE_BITS));
                if (relative + (i8)Abs_b(e->velocity.y) >= TILE_HEIGHT)
                {
                    e->velocity.y = 0;
                }
            }
            break;
            case TL: // Left Line |
            {
                const i8 relative = I8(e->position.x - (tile2->x << TILE_SCALE_BITS));
                if (relative + e->velocity.x <= 0)
                {
                    e->velocity.x = 0;
                }
            }
            break;
            case TR: // Right Line |
            {
                const i8 relative = I8(e->position.x - (tile2->x << TILE_SCALE_BITS));
                // monitor("RELATIVE", relative);
                // monitor("I", i);
                if (relative + (i8)Abs_b(e->velocity.x) >= TILE_WIDTH)
                {
                    e->velocity.x = 0;
                }
            }
            break;
            case TC: // Top Line
            {   
                const i8 relative = I8(e->position.y - (tile1->y << TILE_SCALE_BITS));
                if (relative - (i8)Abs_b(e->velocity.y) <= 0)
                {
                    e->velocity.y = 0;
                }
            }
            break;
            case TS: // Spike
            {
                const i8 relative = I8(e->position.y - (tile1->y << TILE_SCALE_BITS));
                if (relative - e->velocity.y >= TILE_HEIGHT)
                {
                    e->velocity.y = 0;
                    e->state      = EntityState_Dead;
                }
            }
            break;
            case P: // Middle Half Line
            {
                const i8 relativeY = I8(e->position.y - (tile1->y << TILE_SCALE_BITS));
                const i8 relativeX = I8(e->position.x - (tile2->x << TILE_SCALE_BITS));
                if (relativeY + (i8)Abs_b(e->velocity.y) >= TILE_HEIGHT >> 1 && relativeX + e->velocity.x <= TILE_WIDTH >> 1 && relativeX + e->velocity.x >= 0)
                {
                    e->velocity.y = 0;
                }
            }
                break;
            default: // else Tile_Enemy, Tile_Prop, Tile_Empty
                break;
            }
        }
    }
    
    // tiles[1].x = I8((e->position.x + e->velocity.x) >> TILE_SCALE_BITS);
    // tiles[1].y = I8((e->position.y - e->velocity.y) >> TILE_SCALE_BITS);
    if(false)
    if ((tiles[0].x ^ tiles[1].x || tiles[0].y ^ tiles[1].y))
    {
        switch (WORLD.tileset[I16(tiles[1].y) * WORLD_STRIDE + tiles[1].x])
        {
        case TG: // Bottom Line  _
        {   
            const i8 relative = I8(e->position.y - (tiles[0].y << TILE_SCALE_BITS));
            if (relative - e->velocity.y >= TILE_HEIGHT)
            {
                e->velocity.y = 0;
            }
        }
        break;
        case TL: // Left Line |
        {
            const i8 relative = I8(e->position.x - (tiles[1].x << TILE_SCALE_BITS));
             if (relative + e->velocity.x <= TILE_WIDTH)
             {
                 e->velocity.x = 0;
             }
         }
             break;
        case TR: // Right Line |
        {
            const i8 relative = I8(e->position.x - (tiles[1].x << TILE_SCALE_BITS));
            // monitor("RELATIVE", relative);
            if (relative + e->velocity.x >= 0)
            {
                e->velocity.x = 0;
            }
        }
            break;
        case TC: // Top Line
        {   
            const i8 relative = I8(e->position.y - (tiles[1].y << TILE_SCALE_BITS));
            if ((relative - e->velocity.y) <= 0)
            {
                e->velocity.y = 0;
            }
        }
            break;
        case TS: // Spike
        {
            const i8 relative = I8(e->position.y - (tiles[0].y << TILE_SCALE_BITS));
            if (relative + e->velocity.y >= 0)
            {
                e->velocity.y = 0;
            }
        }
        break;
        case P: // Middle Half Line
        {
            const i8 relativeY = I8(e->position.y - (tiles[0].y << TILE_SCALE_BITS));
            const i8 relativeX = I8(e->position.x - (tiles[0].x << TILE_SCALE_BITS));
            if (relativeY - e->velocity.y >= TILE_HEIGHT >> 1 && relativeX + e->velocity.x <= TILE_WIDTH >> 1 && relativeX + e->velocity.x >= 0)
            {
                e->velocity.y = 0;
            }
        }
            break;
        default: // else Tile_Enemy, Tile_Prop, Tile_Empty
            break;
        }
    }
    
    e->position.x += e->velocity.x;
    e->position.y -= e->velocity.y;
    e->tile.x = I8(e->position.x >> TILE_SCALE_BITS);
    e->tile.y = I8(e->position.y >> TILE_SCALE_BITS);
    e->isLocal = Abs_b(e->tile.x - CAMERA.tile.x) <= 4 && Abs_b(e->tile.y - CAMERA.tile.y) <= 4;

    if (e->velocity.x | e->velocity.y) // If 
    {
    }
    
}

void world_progress(void)
{
    ++WORLD.ticks;
    WORLD.drag = WORLD.ticks & 1;
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
    for (e = 0; e < WORLD.props; ++e, ++WORLD.selectedEntity)
    {
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

    i8 xMax = WORLD.selectedTile.x + 4;
    xMax = MIN8(xMax, WORLD_STRIDE);
    xMax = MAX8(0, xMax);
    i8 yMax = WORLD.selectedTile.y + 4;
    yMax = MIN8(yMax, WORLD_EXTENT);
    yMax = MAX8(0, yMax);
    WORLD.selectedTile.x = MAX8(0, WORLD.selectedTile.x);
    WORLD.selectedTile.y = MAX8(0, WORLD.selectedTile.y);
    
    i8 idxStride = xMax - WORLD.selectedTile.x;
    i8 idxDelta  = WORLD_STRIDE - idxStride + 1;
    // monitor("IDX DELTA", idxDelta);
    // monitor("IDX STRIDE", idxStride);
    i16 tileIdx    = WORLD.selectedTile.y * I16(WORLD_STRIDE) + WORLD.selectedTile.x;    
    // i16 tileIdxMax = yMax * WORLD_WIDTH + xMax;
    //print_signed_int(-90, -50, WORLD.selectedTile.y);
    //print_signed_int(-90, -20, WORLD.selectedTile.x);
    //print_signed_int(-110, -50, yMax);
    //print_signed_int(-110, -20, xMax);
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
            switch (tile & Tile_TypeMax)
            {
            case TG:
            if (tileBottom <= 127 && tileBottom >= -128)
            {
                line_t* line = &LIST.line[LIST.lines++];
                line->start.y = I8(tileBottom);
                line->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    line->start.x = -128;
                    line->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    line->start.x = I8(tileLeft);
                    line->delta.x = TILE_WIDTH;
                }
            }
                break;
                case TC:
            if (tileTop <= 127 && tileTop >= -128)
            {
                line_t* line = &LIST.line[LIST.lines++];
                line->start.y = I8(tileTop);
                line->delta.y = 0;
                if (tileLeft < -128) // clip
                {
                    line->start.x = -128;
                    line->delta.x = TILE_WIDTH + I8(tileLeft + 128);
                }
                else if (tileLeft <= 127)
                {
                    line->start.x = I8(tileLeft);
                    line->delta.x = TILE_WIDTH;
                }
            }
                break;
            case TR:
            if (tileRight <= 127 && tileRight >= -128)
            {
                line_t* line = &LIST.line[LIST.lines++];
                line->start.x = I8(tileRight);
                line->delta.x = 0;
                if (tileTop > 127) // clip
                {
                    line->start.y = 127;
                    line->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    line->start.y = I8(tileTop);
                    line->delta.y = -TILE_HEIGHT;
                }
            }
                break;
            case TL:
            if (tileLeft <= 127 && tileLeft >= -128)
            {
                line_t* line = &LIST.line[LIST.lines++];
                line->start.x = I8(tileLeft);
                line->delta.x = 0;
                if (tileTop > 127) // clip
                {
                    line->start.y = 127;
                    line->delta.y = -TILE_HEIGHT + I8(tileTop - 127);
                }
                else
                {
                    line->start.y = I8(tileTop);
                    line->delta.y = -TILE_HEIGHT;
                }
            }
                break;
            case P:
            if (tileTop <= (I16(127) + (TILE_HEIGHT >> 1)) && tileTop >= (-128 + (TILE_HEIGHT >> 1)))
            {
                line_t* line = &LIST.line[LIST.lines++];
                line->start.y = I8(tileTop - (TILE_HEIGHT >> 1));
                line->start.x = I8(tileLeft + (TILE_WIDTH >> 1));
                line->delta.y = 0;
                line->delta.x = TILE_WIDTH;
            }
            // tileLeft += TILE_WIDTH >> 1;
            // tileTop  += TILE_HEIGHT >> 1;
            // //if (tileLeft >= -128 && tileLeft <= 127 && tileTop <= 127 && tileTop >= -128)
            // {
            //     line_t* line = &LIST.line[LIST.lines++];
            //     line->start.y = I8(tileTop);
            //     line->start.x = I8(tileLeft);
            //     line->delta.y = 0;
            //     line->delta.x = TILE_WIDTH >> 1;
            // }
                break;
            case TE(0):
                break;
            case TP(0):
            {
                idx_t propIdx = tile >> 4;
                if (WORLD.propStatus[propIdx] == EntityStatus_Alive)
                {
                    WORLD.propStatus[propIdx] = EntityStatus_Active;
                    entity e = &WORLD.prop[WORLD.props];
                    e->id         = WORLD.props++;
                    e->tile.x     = WORLD.selectedTile.x;
                    e->tile.y     = WORLD.selectedTile.y;
                    e->position.y = I16(tileTop)  + (TILE_HEIGHT >> 1);
                    e->position.x = I16(tileLeft) + (TILE_WIDTH >> 1);
                    e->velocity.x = 0;
                    e->velocity.y = 0;
                    e->isLocal    = true;
                    e->transform  = 0;
                    e->type       = g_sewersProps[propIdx];
                    switch (e->type)
                    {
                    case Prop_Barrel:
                        e->mesh = Mesh_Barrel;
                        break;
                    default:
                        break;
                    }
                    e->state = PropState_Idle;
                    e->type = g_sewersProps[propIdx];
                }
            }
                break;
            case TS:
            {
                line_strip_t* strip = &LIST.strip[LIST.strips++];
                strip->start.y = I8(tileBottom);
                strip->start.x = I8(tileLeft);
                strip->data = mesh_get(Mesh_Spikes);
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

void world_create(Stage const stage)
{
    MEMZERO(WORLD);
    WORLD.tileset     = g_tilesets[stage];
    CAMERA.tile.y     = g_stageStartPts[stage].y;
    CAMERA.tile.x     = g_stageStartPts[stage].x;
    CAMERA.position.y = (I16(g_stageStartPts[stage].y) << TILE_SCALE_BITS) - 1;
    CAMERA.position.x = (I16(g_stageStartPts[stage].x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 2);
}

void world_draw(void) 
{
    Intensity_5F();
    for (i8 line = 0; line < LIST.lines; ++line)
    {
        beam_set_position(LIST.line[line].start.y, LIST.line[line].start.x);
        Draw_Line_d(LIST.line[line].delta.y, LIST.line[line].delta.x);
    }
    
    for (i8 strip = 0; strip < LIST.strips; ++strip)
    {    
        beam_set_position(LIST.strip[strip].start.y, LIST.strip[strip].start.x);
        Draw_VLc((void* const)LIST.strip[strip].data);
    }
}


world_t g_world;