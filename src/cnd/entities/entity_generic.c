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
            if (GRAVITY_DOWN())
            {
                draw_stack_push(e->animation.keyframe, (i8)dy, (i8)dx);
            }
            else
            {
                draw_stack_push(e->animation.keyframe, (i8)dy - 10, (i8)dx);
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

void entity_list_clear()
{
    WORLD.list.aliveCount = 0;
    WORLD.list.freeCount  = ARRAY_SIZE(WORLD.list.free);
    i8 idx = WORLD.list.freeCount - 1;
    for (i8 i = 0; i < WORLD.list.freeCount; ++i, --idx)
    {
        handle h       = HANDLE_CREATE(idx, 0);
        entity e       = &WORLD.list.entities[idx];
        e->handle      = h;
        e->isAllocated = false;
        e->globalId    = ID_INVALID;
        e->update      = update_stub;
        e->kill        = update_stub;
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
        if (GRAVITY_DOWN())
        {
            if (CAMERA.velocity.y <= -2)
                entity_set_death(e);
            else
                character_damage();
        }
        else // GRAVITY_UP()
        {
            if (CAMERA.velocity.y >= 2)
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
        e->tileAbsDelta = (v2i){3, 3}; // Unknown
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
        e->tileAbsDelta = (v2i){ 3, 3 }; // Unknown
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

#ifdef NDEBUG
static i8 entityHitbox[9] =
{
    LENGTH(4),
    0, 0,
    0, 0,
    0, 0,
    0, 0,
};

static i8 cameraHitbox[9] =
{
    LENGTH(4),
    0, 0,
    0, 0,
    0, 0,
    0, 0,
};
#endif

bool entity_intersects_camera(entity e, i8 dy, i8 dx)
{
    i8 l1 = dx, r1 = dx + (dx >= 0 ? e->hitbox.x : -e->hitbox.x), b1 = dy;
    i8 l2 = 0, r2 = (CAMERA.transform >= 0 ? CAMERA.hitbox.x : -CAMERA.hitbox.x), b2 = 0;
    i8 t1 = dy, t2;
    if (GRAVITY_DOWN())
    {
        t1 += e->hitbox.y;
        t2 = CAMERA.hitbox.y;
    }
    else
    {
        t1 -= e->hitbox.y;
        t2 = -CAMERA.hitbox.y;
    }

    i8 tmp;
    SWAP_MAX(tmp, l1, r1);
    SWAP_MAX(tmp, l2, r2);
    SWAP_MAX(tmp, b1, t1);
    SWAP_MAX(tmp, b2, t2);

#ifdef NDEBUG
    entityHitbox[1] = t1 - b1;
    entityHitbox[4] = r1 - l1;
    entityHitbox[5] = -entityHitbox[1];
    entityHitbox[8] = -entityHitbox[4];

    cameraHitbox[1] = t2 - b2;
    cameraHitbox[4] = r2 - l2;
    cameraHitbox[5] = -cameraHitbox[1];
    cameraHitbox[8] = -cameraHitbox[4];

    draw_stack_push(entityHitbox, b1, l1);
    draw_stack_push(cameraHitbox, b2, l2);
#endif

    if ((r1 >= l2) && (l1 <= r2) &&
        (t1 >= b2) && (b1 <= t2))
    {
        return true;
    }
    return false;
}

void entity_exchange_blows(entity e, i8 dy)
{
    if (CAMERA.isAttacking || (GRAVITY_DOWN() && (dy < -4)) || (GRAVITY_UP() && (dy > 4)))
    {
        entity_set_death(e);
    }
    else
    {
        character_damage();
    }
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