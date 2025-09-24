/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void update_kill_revive(entity e)
{
    PLAYER.isOtherCharacterDead = false; // Gets the other character back
    entity_set_status(e, EntityStatus_Dead);
}

void update_kill(entity e)
{
    entity_set_status(e, EntityStatus_Dead);
}

void update_death(entity e)
{
    if (--e->animation.remainder != 0) // Play Animation
    {
        e->velocity.y = 0;
        e->velocity.x = 0;
        if (WORLD.freq8_8)
        {
            if (e->inLocalSpace)
            {
                const i16 dy = CAMERA.position.y - e->position.y;
                const i16 dx = e->position.x - CAMERA.position.x;
                draw_queue_push(e->animation.keyframe, (i8)dy, (i8)dx);
            }
            e->animation.keyframe += e->animation.size;
        }
    }
    else 
    {
        e->kill(e);
    }
}

void update_stub(entity e)
{
    (void)e;
}

void add_score(u8 score)
{
    PLAYER.score      += score;
    PLAYER.scoreGained = score;
    GAME.ticksScoreGainedVisible = 16;
}

void entity_set_status(entity e, EntityStatus status)
{
    if (e->globalId >= 0) // Is named entity
    {
        WORLD.lastSeen[e->globalId].status = status;
    } // else Entity is unknown, so just destroy it. (e.g. projectiles)

    const idx_t last = WORLD.entityCount - 1;
    if (last != e->id) // Swap with last if not last
    {
        i8 tmp = WORLD.entityIdxs[e->id];
        WORLD.entityIdxs[e->id] = WORLD.entityIdxs[last];
        WORLD.entityIdxs[last] = tmp;
    }
    WORLD.entityCount = last;
    e->update   = update_stub;
    e->state    = EntityState_Dead;
}

void entity_create_named(idx_t const globalId, v2i const tile)
{
    entity e      = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount]];
    e->id         = WORLD.entityCount++;
    e->globalId   = globalId;
    e->tile.y     = tile.y;
    e->tile.x     = tile.x;
    e->position.y = (I16(tile.y) << TILE_SCALE_BITS) - (TILE_HEIGHT >> 1);
    e->position.x = (I16(tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1);
    e->velocity.x = 0, e->velocity.y = 0;
    e->type       = WORLD.level->entities[globalId];
    e->isEnemy    = e->type > Prop_Max;
    e->inLocalSpace    = true; // Has to be local in order to spawn
    e->isSameTile = false; // Unknown
    e->isGrounded = false; // Unknown
    e->transform  = 0;
    prefabs[e->type](e);
}

void entity_create_anonymous(EntityType const type, v2i const tile)
{
    if (WORLD.entityCount < ENTITIES_ACTIVE_MAX)
    {
        entity e        = &WORLD.entities[WORLD.entityIdxs[WORLD.entityCount]];
        e->id           = WORLD.entityCount++;
        e->globalId     = ID_INVALID; // Anonymous entities have no global id
        e->tile.y       = tile.y, e->tile.x = tile.x; // Tile position
        e->position.y   = (I16(tile.y) << TILE_SCALE_BITS) - (TILE_HEIGHT >> 1); // Starts in middle of tile and Y inverted
        e->position.x   = (I16(tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1); // Starts in middle of tile
        e->velocity.x   = 0, e->velocity.y = 0; // Starts with no velocity by defautlt
        e->type         = type;
        e->isEnemy      = e->type > Prop_Max;
        e->inLocalSpace = true; // Has to be local in order to spawn
        e->isSameTile   = false; // Unknown
        e->isGrounded   = false; // Unknown
        e->transform    = 0;
        prefabs[type](e);
    }
}

void entity_set_animation(entity e, void const* keyframes, i8 keyframeSize, i8 keyframeCount)
{
    e->animation.keyframe  = (i8*)keyframes; 
    e->animation.size      = keyframeSize;
    e->animation.remainder = keyframeCount;
}

/////////////////////////////////////////////////////////////////////////
//	Function Table
//
const prefab_t prefabs[EntityType_Max] =
{
    prefab_crate, 
    prefab_barrel,
    prefab_coin,
    prefab_tunichtgut,
    prefab_halunke,
    prefab_gauner,
    prefab_schuft,
    prefab_schelm,
    prefab_bandit,
};