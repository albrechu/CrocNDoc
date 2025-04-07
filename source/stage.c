#include <stage.h>

#include <vectrex.h>
#include <xutility.h>

#define POS(y, x) y, x // Starting position
#define TILE(y, x) y, x

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
 * 
 * 
 * 
 */

static const i8 g_stageSewers[] = 
{
    TILE(0, 0),
    POS(-128, 64),
    LENGTH(8),
    64, 0,
    0, -64,
    0, -127,
    127, 0,
    0, 127,
    0, 127,
    -127, 0,
    -64, 0,
    TILE(1, 0),
    POS(-128, -128),
    LENGTH(3),
    0, 127,
    64, 0,
    0, 127,
    TILE(0, 1),
    POS(127, 64),
    LENGTH(2),
    -64, 0,
    0, 64,
};

static tile_t      g_tileSet[3][6];
static draw_list_t g_drawList;

void draw_list_draw(void)
{
    char s[4] = { '0', '0' + (char)g_drawList.count, '\x80', '\0'};
    beam_set_position(10, 10);
    Print_Str_d(50, -50, s);
    for (int i = 0; i < g_drawList.count; ++i)
    {
        beam_set_position(g_drawList.strips[i].start.y, g_drawList.strips[i].start.x);
        Draw_VL_a(g_drawList.strips[i].vectors, (void*)g_drawList.strips[i].strip);
    }
}

void draw_list_refresh(vec2 const camera)
{
    g_drawList.count = 0; // Clear
    v2i tile = tile_calculate(camera); // Current tile of the camera

    i8 xMin = MAX8(0, tile.x);
    i8 yMin = MAX8(0, tile.y);
    i8 xMax = MIN8(6, (tile.x + 3));
    i8 yMax = MIN8(6, (tile.y + 3));
    for (i8 i = xMin; i < xMax; ++i)
    {
        for (i8 j = yMin; j < yMax; ++j)
        {
            tile_write_list(j, i, camera);
        }
    }
}

void stage_collision_detection(vec2 const camera)
{
    (void)camera;
    // v2i tile = tile_calculate(camera);

}

void stage_load_tile_set(Stage const stage)
{
    i8 const* set;
    u16 setSize;
    switch (stage)
    {
    case Stage_Sewers:
        set     = g_stageSewers;
        setSize = sizeof g_stageSewers;
        break;
    default:
        break;
    }
    MEMZERO(g_tileSet);
    
    for (u16 i = 0; i < setSize;) // fill tile set with pointers to each tile
    {
        tile_t* list = &g_tileSet[set[0]][set[1]];
        list->lists[list->count] = (line_strip_t const*)&set[2];
        ++list->count;
        i += ((u16)(set[4] << 2)) + 7u; // (vectors + 1) * 2 + 2 pos_x/y + 1 Length + 2 tile_x/y
    }
}

void tile_write_list(i8 const y, i8 const x, vec2 const camera)
{
    if (g_tileSet[y][x].lists[0] == null)
        return;

    tile_t tile = g_tileSet[y][x];

    vec2 glob;
    glob.x = (((i16)x) << 8) - camera.x;
    glob.y = (((i16)y) << 8) - camera.y;
    // Add position of list relative to grid.
    glob.x += (i16)tile.lists[0]->x;
    glob.y += (i16)tile.lists[0]->y;

    v2i const* list = tile.lists[0]->data;
    u8 lines = (u8) tile.lists[0]->vectors + 1;
    u8 l;
    for (l = 0; l < lines; ++l)
    {
        if (glob.x < 128 && glob.y < 128 && glob.x > -129 && glob.y > -129) //(VEC_IS_LOCAL(glob))
        {
            //if (l == 0)
            //    Print_Str_d(10, 10, "HELLO\x80");
            drawable_strip strip = &g_drawList.strips[g_drawList.count];
            strip->start.y       = (i8)glob.y;
            strip->start.x       = (i8)glob.x;
            strip->vectors       = lines - l;
            strip->strip         = &list[l];
            ++g_drawList.count;
            break;
        }
        glob.x += (i16)list[l].y; 
        glob.y += (i16)list[l].x;
    }
    if (l == lines)
        Print_Str_d(10, 10, "HEY\x80");
}

force_inline v2i tile_calculate(vec2 const pos)
{
    return (v2i) { (i8) (pos.x >> 8), (i8)(pos.y >> 8), };
}
