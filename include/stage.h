#ifndef STAGE_H
#define STAGE_H

#include <types.h>
#include <defines.h>

enum Stage_
{
    Stage_Sewers,
    Stage_Waterways,
    Stage_Moltenlands,
    Stage_Bonus,
};
typedef i8 Stage;

typedef struct line_strip_t
{
    i8  y, x;
    i8  vectors;
    v2i data[];
} line_strip_t, *line_strip;

typedef struct tile_t
{
    line_strip_t const* lists[3];
    i8                  count;
} tile_t, *tile;


typedef struct drawable_strip_t
{
    v2i        start;
    u8         vectors;
    v2i const* strip; 
} drawable_strip_t, *drawable_strip;

typedef struct draw_list_t
{
    i8 count;
    drawable_strip_t strips[4];
} draw_list_t;

void draw_list_draw(void);
void draw_list_refresh(vec2 const camera);

void stage_load_tile_set(Stage const stage);
void tile_write_list(i8 const y, i8 const x, vec2 const camera);
force_inline v2i tile_calculate(vec2 const pos);

#endif /* STAGE_H */