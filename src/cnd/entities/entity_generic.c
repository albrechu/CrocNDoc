/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>
#include <cnd/draw_queue.h>
#include <lib/monitor.h>

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
        if (e->inLocalSpace)
        {
            const i16 dy = CAMERA.position.y - e->position.y;
            const i16 dx = e->position.x - CAMERA.position.x;
            if (WORLD.gravity > 0)
            {
                draw_queue_push(e->animation.keyframe, (i8)dy, (i8)dx);
            }
            else
            {
                draw_queue_push(e->animation.keyframe, (i8)dy - 10, (i8)dx);
            }
        }
        e->animation.keyframe += e->animation.size;
    }
    else 
    {
        if (e->score)
        {
            add_score(e->score);
        }
        e->kill(e);
    }
}

void update_stub(entity e)
{
    (void)e;
}

void add_score(u8 score)
{
    PLAYER.score += score;
    PLAYER.scoreGained = score;
    GAME.ticksScoreGainedVisible = 16;
}

void entity_create_list()
{
    WORLD.list.aliveCount = 0;
    WORLD.list.freeCount  = ARRAY_SIZE(WORLD.list.free);
    i8 idx = WORLD.list.freeCount - 1;
    for (i8 i = 0; i < WORLD.list.freeCount; ++i, --idx)
    {
        handle h = HANDLE_CREATE(idx, 0);
        entity e = &WORLD.list.entities[idx];
        e->handle   = h;
        e->isAllocated = false;
        e->globalId = ID_INVALID;
        e->update   = update_stub;
        e->kill     = update_stub;
        WORLD.list.free[i] = h;
    }
}

entity entity_alloc()
{
    if (WORLD.list.freeCount == 0)
        return null;

    handle idx = WORLD.list.free[--WORLD.list.freeCount];
    WORLD.list.alive[WORLD.list.aliveCount++] = idx;
    entity e = &WORLD.list.entities[idx];
    e->isAllocated = true;
    e->handle      = HANDLE_CREATE(idx, HANDLE_VERSION(e->handle));
    return e;
}

void entity_free(entity e)
{
    const handle idx     = HANDLE_INDEX(e->handle);
    const handle version = (HANDLE_VERSION(e->handle) + 1) & ((1 << HANDLE_VERSION_BITS) - 1);
    e->handle            = HANDLE_CREATE(idx, version);
    e->isAllocated = false;
    WORLD.list.free[WORLD.list.freeCount++] = idx;

    idx_t i;
    for (i = 0; i < WORLD.list.aliveCount; ++i)
    {
        if (WORLD.list.alive[i] == idx)
            break;
    }
    idx_t last = WORLD.list.aliveCount - 1;
    if (i != last) // swap with last
    {
        WORLD.list.alive[i] = WORLD.list.alive[last];
    }
    --WORLD.list.aliveCount;
}

bool handle_valid(handle h)
{
    const handle idx     = HANDLE_INDEX(h);
    const handle version = HANDLE_VERSION(h);

    if (idx >= ENTITIES_MAX) 
        return false;

    entity e              = &WORLD.list.entities[idx];
    handle currentVersion = HANDLE_VERSION(e->handle);
    return currentVersion == version;
}

entity entity_get(handle h)
{
    if (!handle_valid(h))
        return null;
    return &WORLD.list.entities[HANDLE_INDEX(h)];
}

void entity_set_death(entity e)
{
    e->update = update_death;
    if (WORLD.gravity < 0)
        CAMERA.velocity.y = -Velocity_KillUpWind;
    else
        CAMERA.velocity.y = Velocity_KillUpWind;
}

void entity_camera_hit_detection(entity e, i8 localDx)
{
    const i8 localDxMask = localDx >> 7;
    const i8 localDxAbs = (localDx ^ localDxMask) - localDxMask;
    if (CAMERA.isAttacking)
    {
        if ((CAMERA.velocity.x ^ localDx) >= 0) // Only if they look in the opposite direction upon attack.
            entity_set_death(e);
    }
    else if (localDxAbs < 6)
    {
        if (WORLD.gravity < 0)
        {
            if (CAMERA.velocity.y >= 2)
                entity_set_death(e);
            else
                character_damage();
        }
        else // WORLD.gravity > 0
        {
            if (CAMERA.velocity.y <= -2)
                entity_set_death(e);
            else
                character_damage();
        }
    }
    else
    {
        //character_damage();
    }
}

void entity_set_status(entity e, EntityStatus status)
{
    if (e->globalId >= 0) // Is named entity
    {
        WORLD.lastSeen[e->globalId].status = status;
    } // else Entity is unknown, so just destroy it. (e.g. projectiles)

    e->update = update_stub;
    e->state  = EntityState_Dead;
    entity_free(e);    
}

void entity_create_named(idx_t const globalId, v2i const tile)
{
    entity e = entity_alloc();
    if (e)
    {
        e->reference    = HANDLE_INVALID;
        e->isAllocated = true;
        e->globalId     = globalId;
        e->tile.y       = tile.y;
        e->tile.x       = tile.x;
        e->position.y   = (I16(tile.y) << TILE_SCALE_BITS) - (TILE_HEIGHT >> 1);
        e->position.x   = (I16(tile.x) << TILE_SCALE_BITS) + (TILE_WIDTH >> 1);
        e->velocity.x   = 0, e->velocity.y = 0;
        e->type         = WORLD.level->entities[globalId];
        e->isEnemy      = e->type > Prop_Max;
        e->inLocalSpace = true; // Has to be local in order to spawn
        e->isSameTile   = false; // Unknown
        e->isGrounded   = false; // Unknown
        e->transform    = 0;
        prefabs[e->type](e);
    }
}

void entity_create_anonymous(EntityType const type, v2i const tile)
{
    entity e = entity_alloc();
    if (e)
    {
        e->reference    = HANDLE_INVALID;
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
    prefab_croc_prepare,
    prefab_croc,
    prefab_barrel,
    prefab_coin,
    prefab_halunke,
    prefab_gauner,
    prefab_schuft,
    prefab_schelm,
    prefab_bandit,
};