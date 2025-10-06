#include <cnd/xutils.h>

u8 manhattan(i8 const a, i8 const b)
{
    const i8 aMask = a >> 7;
    const i8 bMask = b >> 7;
    return U8((a ^ aMask) - aMask) + U8((b ^ bMask) - bMask);
}

u16 g_seed = 0xBEEF;
void random_mix_seed(u8 value)
{
    g_seed = (g_seed << 5) | (g_seed >> 11);
    g_seed ^= (U16(value) << 8) | value;
}

u16 random()
{
    g_seed ^= g_seed << 7;
    g_seed ^= g_seed >> 9;
    g_seed ^= g_seed << 8;
    return g_seed;
}
