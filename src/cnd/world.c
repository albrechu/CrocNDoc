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
    MT  = Tile_MiddleTop,
    ML  = Tile_MiddleLeft,
    MR  = Tile_MiddleRight,
    S   = Tile_Spikes,
    J   = Tile_Jumper,
    BV  = Tile_BarrierVertical,
    BH  = Tile_BarrierHorizontal,
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
      E,   B,   B,   B,   B,   E,   E,   E,   E,   B,   B,   B,   B,   B,   B,   B,   B,   B,   B,   E,   E,   E,   E,   E,
      R,   E,   R,   E,   R,   E,   E,   E,   R,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,   E,  BV,   E,   R,   E,   E,   E,   R,   E,  TL,   T,   T,   T,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,  T2,  TR,   E,   R,   E,   E,   E,   R,   E,  L2,   E,   E,   E,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,  P0,  R2,   M,   R,   E,   E,   E,   R,   E,  L2,   E,   E,   E,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,   E,  R2,   E,   R,   E,   E,   E,   R,   E,  L2,   E,   E,   E,   L,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,   E,  R2,   E,   R,   E,   E,   E,   R,   E,   T,   T,   T,   T,   T,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,   J,  R2,   E,   R,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,   E,   E,   E,   E,
      R,   E,  R2,   M,   E,   E,   E,   E,   E,   E,   E,   E,  E2,   E,  E3,   E,  E8,   E,   R,   E,   E,   E,   E,   E,
      R,   J,  R2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   B,   B,   B,   B,   B,
      R,   E,  R2,  MB,   E,   E,  MT,  TL,  TR,  BH,  TL,   T,   T,   T,   T,   T,   T,   T,   T,   E,   E,   E,   R,   E,
      R,   E,  R2,   E,   E,   E,  MB,  L2,  R2,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   J,   T,   T,   L,  E0,  L2,  R2,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,  P1,   R,   E, 
      R,   E,   E,   E,   E,   L,  MT,  L2,  R2,   E,  L2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   T,   T,   E,  R2,   E,   E,   E,   E,   E,   M,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   E,   E,   E,  R2,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   J,   E,   E,   E,   E,   E,  R2,   E,   E,   E,  E7,   E,   E,   E,   E,   E,   E,   E,   E,   E,   R,   E,
      R,   E,   E,   E,   E,   E,   E,   E,   E,   T,   T,   E,   M,   E,   E,   E,   E,   E,   E,   E,   E,   J,  BR,   E,
      R,   E,   E,   M,   E,   M,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   M,   E,  TL,   T,   T,   T,   E,   E,
      R,   E,   E,   E,   E,   E,   M,   E,   M,   E,   M,   E,   S,   S,   S,   S,   S,   S,   L,   E,   E,   E,   E,   E,
      R,   J,   E,   E,   S,   S,   S,   S,   S,   S,   S,   S,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   T,   T,   T,   L,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
      E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,   E,
};
static_assert(sizeof(g_stageSewers) == (I16(WORLD_EXTENT) * I16(WORLD_STRIDE)));

const EntityType g_sewersEntities[] = 
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
static_assert(sizeof(g_sewersEntities) <= ENTITIES_MAX);

const stage_metadata_t g_stagesMetadata[] = 
{
    // Stage_Sewers
    {
        .startingTile   = { 2, 1 },
        .adjacentStages = { Stage_Sewers, Stage_Sewers },
        .pentities      = g_sewersEntities,
    },
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
    e->velocity.x -= WORLD.gravity * (e->velocity.x != 0 && e->velocity.y == 0) * e->transform;
	e->velocity.y -= WORLD.gravity;
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

    if ((Vec_Btn_State & Input_Button3) != Input_Button3)
    for (i8 i = 0; i < 3; ++i)
    {
        v2i* tile = &tiles[i];
        // if (tile->y != tile->y || tile->x != tile->x)
        {
            const Tile type = WORLD.tileset[I16(tile->y) * WORLD_STRIDE + tile->x];
            switch (type)
            {
            case Tile_Top2:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) - e->velocity.y <= 0)
                    e->velocity.y = 0;
                /* fallthrough */
            case Tile_Top:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y <= 0)
                    e->velocity.y = 0;
            break;
            case Tile_TopLeft:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y <= 0)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) - e->velocity.x <= 0)
                    e->velocity.x = 0;
                break;
            case Tile_TopRight:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y <= 0)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) - e->velocity.x >= TILE_WIDTH - 1)
                    e->velocity.x = 0;
                break;
            case Tile_Bottom2:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) - e->velocity.y >= TILE_HEIGHT - 1)
                    e->velocity.y = 0;
                /* fallthrough */
            case Tile_Bottom:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) + e->velocity.y >= TILE_HEIGHT - 1)
                    e->velocity.y = 0;
                break;
            case Tile_BottomLeft:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) - e->velocity.y >= TILE_HEIGHT - 1)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x <= 0)
                    e->velocity.x = 0;
                break;
            case Tile_BottomRight:
                if (I8(e->position.y - (tile->y << TILE_SCALE_BITS)) - e->velocity.y >= TILE_HEIGHT - 1)
                    e->velocity.y = 0;
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x >= TILE_WIDTH - 1)
                    e->velocity.x = 0;
                break;
            case Tile_Left2:
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) - e->velocity.x <= 0)
                    e->velocity.x = 0;
                /* fallthrough */
            case Tile_Left:
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x <= 0)
                    e->velocity.x = 0;
                break;
            case Tile_Right2:
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) - e->velocity.x >= TILE_WIDTH - 1)
                    e->velocity.x = 0;
                /* fallthrough */
            case Tile_Right:
                if (I8(e->position.x - (tile->x << TILE_SCALE_BITS)) + e->velocity.x >= TILE_WIDTH - 1)
                    e->velocity.x = 0;
                break;
            case Tile_MiddleLeft:
            {
                const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - e->velocity.y >= (TILE_HEIGHT >> 1) - 1 && relativeY < (TILE_HEIGHT >> 1) && relativeX <= TILE_WIDTH >> 1)
                    e->velocity.y = 0;
            }
                break;
            case Tile_MiddleRight:
            {
                const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - e->velocity.y >= (TILE_HEIGHT >> 1) - 1 && relativeY < (TILE_HEIGHT >> 1) && relativeX >= TILE_WIDTH >> 1)
                    e->velocity.y = 0;
            }
                break;
            case Tile_Middle:
            {
                const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - e->velocity.y >= (TILE_HEIGHT >> 1) - 1 && relativeY < (TILE_HEIGHT >> 1) && relativeX >= TILE_WIDTH >> 2 && relativeX <= (TILE_WIDTH- (TILE_WIDTH >> 2)))
                    e->velocity.y = 0;
            }
                break;
            case Tile_MiddleBottom:
            {
                const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if (relativeY - e->velocity.y >= TILE_HEIGHT - 1 && relativeY < TILE_HEIGHT && relativeX >= TILE_WIDTH >> 2 && relativeX <= (TILE_WIDTH- (TILE_WIDTH >> 2)))
                    e->velocity.y = 0;
            }
                break;
            case Tile_MiddleTop:
                {
                    const i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                    const i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                    if (relativeY - e->velocity.y >= 0 && relativeY < 0 && relativeX >= TILE_WIDTH >> 2 && relativeX <= (TILE_WIDTH- (TILE_WIDTH >> 2)))
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
                    if (velocity > 4) // Broke the barrier
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
                        if (e->velocity.y < -14) // Broke the barrier
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
            case Tile_Portal0:
            case Tile_Portal1:
            {
                i8 relativeX = I8(e->position.x - (tile->x << TILE_SCALE_BITS));
                i8 relativeY = I8(e->position.y - (tile->y << TILE_SCALE_BITS));
                if ((relativeX + e->velocity.x >= (TILE_WIDTH >> 2) && relativeX + e->velocity.x <= TILE_WIDTH - (TILE_WIDTH >> 2)) || 
                    (relativeY + e->velocity.y >= (TILE_WIDTH >> 2) && relativeY + e->velocity.y <= TILE_WIDTH - (TILE_WIDTH >> 2)))
                    WORLD.stageEntered(WORLD.metadata->adjacentStages[type - Tile_Portal0]);
            }
                break;
            default:
                break;
            }
        }
    }
    e->position.x += e->velocity.x;
    e->position.y -= e->velocity.y;
    e->tile.x = I8(e->position.x >> TILE_SCALE_BITS);
    e->tile.y = I8(e->position.y >> TILE_SCALE_BITS);
}

void world_progress(void)
{
    ++WORLD.ticks;
    WORLD.gravity = WORLD.ticks & 1;

    for (idx_t i = 0; i < WORLD.entityCount; ++i)
    {
        WORLD.selectedEntity = WORLD.entityIdxs[WORLD.entityIdxs[i]];
        check_collision();
    }
        
    //if ((CAMERA.velocity.x | CAMERA.velocity.y) == 0) // camera is static
    //    return;

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
    Intensity_5F();
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
            case Tile_Top2:
            case Tile_Top:
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
            break;
            case Tile_TopLeft:
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
                if (tileBottom > 127) // clip
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
            break;
            case Tile_TopRight:
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
                break;
            case Tile_Bottom2:
            case Tile_Bottom:
            if (tileBottom <= 127 && tileBottom >= -128)
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
                beam_set_position(I8(tileBottom), startX);
                Draw_Line_d(0, deltaX);
            }
                break;
            case Tile_BottomLeft:
            if (tileBottom <= 127 && tileBottom >= -128)
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
                beam_set_position(I8(tileBottom), startX);
                Draw_Line_d(0, deltaX);
            }
            if (tileLeft <= 127 && tileLeft >= -128)
            {
                i8 startY, deltaY;
                if (tileBottom > 127) // clip
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
                break;
            case Tile_BottomRight:
            if (tileBottom <= 127 && tileBottom >= -128)
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
                beam_set_position(I8(tileBottom), startX);
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
                break;
            case Tile_Left2:
            case Tile_Left:
            if (tileLeft <= 127 && tileLeft >= -128)
            {
                i8 startY, deltaY;
                if (tileBottom > 127) // clip
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
                break;
            case Tile_Right2:
            case Tile_Right:
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
            break;
            case Tile_MiddleLeft:
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
            break;
            case Tile_MiddleRight:
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
            break;
            case Tile_Middle:
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
                break;
        case Tile_MiddleBottom:
            if (tileBottom <= 127 && tileBottom >= -128)
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
                beam_set_position((i8)tileBottom, startX);
                Draw_Line_d(0, deltaX);
            }
                break;
            case Tile_MiddleTop:
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
                break;
            case Tile_Spikes:
            if (tileBottom <= 127 && tileBottom >= -128)
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
                beam_set_position(I8(tileBottom), startX);
                Draw_Line_d(TILE_HEIGHT, deltaX);
                Draw_VLc(mesh_get(Mesh_Spikes));
            }
            else // clip vertically
            {
                i8 startX, deltaX;
                i8 dy  = I8(tileBottom + 128);
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
                Draw_VLc(mesh_get(Mesh_Spikes));
            }
            break; 
            case Tile_Jumper:
            if (tileBottom >= -128 && tileBottom < 127)
            {
                beam_set_position(I8(tileBottom), I8(tileLeft + (TILE_WIDTH >> 2)));
                Draw_VLc(mesh_get(Mesh_Jumper));
            }
                break;
            case Tile_BarrierVertical:
            if (!TILE_FLAG(0))
            {
                beam_set_position(I8(tileTop), I8(tileRight));
                Draw_VLc(mesh_get(Mesh_BarrierVertical));
            }
                break;
            case Tile_BarrierHorizontal:
            if (!TILE_FLAG(1))
            {
                beam_set_position(I8(tileBottom), I8(tileRight));
                Draw_VLc(mesh_get(Mesh_BarrierHorizontal));
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
                last_sighting last = &WORLD.lastSeen[idx];
                if (last->status == EntityStatus_Inactive)
                {
                    last->status = EntityStatus_Active;
                    entity e      = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount++]];
                    e->id         = idx;
                    e->tile.y     = WORLD.selectedTile.y;
                    e->tile.x     = WORLD.selectedTile.x;
                    e->position.y = -(I16(tileTop)  - (TILE_HEIGHT >> 1));
                    e->position.x = I16(tileLeft) + (TILE_WIDTH >> 1);
                    e->velocity.x = 0, e->velocity.y = 0;
                    e->type       = WORLD.metadata->pentities[idx];
                    e->isLocal    = true;
                    e->isEnemy    = e->type > Prop_Max;
                    e->transform  = 0;
                    WORLD.entityAdded(e);
                }
                break;
                case Tile_Portal0:
                case Tile_Portal1:
                {
                    i16 tileCenterY = tileTop + (TILE_WIDTH >> 1);
                    if (tileCenterY <= 127 && tileCenterY >= -128)
                    {
                        beam_set_position(I8(tileCenterY), I8(tileLeft));

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

void __routine_stub(entity e) { (void)e; }

void world_entity_set_status(entity e, idx_t idxInIdxs, EntityStatus status)
{
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
        if (idxInIdxs < 0) // Find idx
        {
            idx_t* idx = &WORLD.entityIdxs[0];
            for (idxInIdxs = 1; idxInIdxs < WORLD.entityCount; ++idxInIdxs, ++idx)
            {
                if (e == (WORLD.entities + WORLD.entityIdxs[*idx]))
                    break;
            }
        }
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
