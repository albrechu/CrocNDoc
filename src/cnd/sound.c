#include <cnd/sound.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <vectrex.h>

const music_t g_musicOff =
{
    .amplitudes  = (amplitudes_t const*)&Vec_ADSR_FADE4,
    .frequencies = (frequencies_t const*)&Vec_TWANG_VIBEHL,
    .notes       = {128, 128, 0, 128}
};

const sfx_t g_sfxOff = { 0x3F, 0, 1, 128U };



void sound_clear()
{
    Vec_Expl_Timer = 0;
    Vec_Expl_Flag  = 0;
    Vec_Music_Flag = 0;
    Stop_Sound();
    Clear_Sound();
    MEMZERO(SOUND);
    SOUND.music  = &g_musicOff;
    SOUND.sfx[0] = &g_sfxOff;
}

void sound_push_sfx(sfx_t const* sfx)
{
    if (SOUND.soundEffects < I8(ARRAY_SIZE(SOUND.sfx)))
    {
        SOUND.sfx[SOUND.soundEffects++] = sfx;
        Vec_Expl_Flag = 128;
    }
}

void sound_push_music(music_t const* music)
{
    SOUND.music = music;
}

void sound_stop_music()
{
    Vec_Music_Flag = 0;
    Stop_Sound();
    SOUND.music = &g_musicOff;
}

//if (SOUND.soundEffects > 1)
//{
//    i8 sfx = --SOUND.soundEffects;
//    while (sfx > 0)
//    {
//        i8 next = sfx - 1;
//        SOUND.sfx[next] = SOUND.sfx[sfx];
//    }
//}
//else if (SOUND.soundEffects == 1)
//{
//    --SOUND.soundEffects;
//}

void Explosion_Sound()
{
    i8 time, B;

    if (I8(Vec_Expl_Flag) < 0)  // Init
    {
        Vec_Expl_Flag &= 0x7F;

        *((sfx_t*)&Vec_Expl_1) = *SOUND.sfx[0];

        // Extract info
        Vec_Expl_Chans = ((Vec_Expl_1 >> 3) | Vec_Expl_1) & 0x07; 
        Vec_Expl_ChanA = Vec_Expl_1 & 0x38; 
        Vec_Expl_ChanB = Vec_Expl_1 & 0x07;

        Vec_Expl_Chan = 2;
        Vec_Expl_Timer = 0x7F;
        time = 0x7F;
    }
    else // Continue
    {
        time = Vec_Expl_Timer;
        if (time == 0)  // Nothing to do.
        {
            return;
        }

        time -= Vec_Expl_4;  // decrement timer by delta
        if (time < 0)
        {
            B = 0;
            Vec_Expl_Timer = 0;
            goto distribute_noise;
        }

        Vec_Expl_Timer = time;
    }

    time >>= 2;

    if (Vec_Expl_ChanA != 0)
    {
        Vec_Music_Wk_6 = time;

        if (Vec_Expl_2 < 0)
        {
            Vec_Music_Wk_6 = Vec_Expl_2;
        }
        else if (Vec_Expl_2 > 0)
        {
            Vec_Music_Wk_6 = ~time;
        }
    }

    time >>= 1;

    if (time > 7 && time != 0x0F)
        ++time;

    if (Vec_Expl_3 < 0)
    {
        B = time;
        goto distribute_noise;
    }

    if (Vec_Expl_3 > 0)
        time ^= 0x0F;

    B = time;

distribute_noise:
    {
        i8* wk = &Vec_Music_Wk_7;
        for (i8 cb = Vec_Expl_Chans; cb != 0; cb >>= 1)
        {
            --wk;
            if (cb & 1)
                *wk = B;
        }
    }

    // --- Select next active channel if ChanB mask is set ---
    if (Vec_Expl_ChanB != 0)
    {
        do
        {
            if (--Vec_Expl_Chan < 0)
                Vec_Expl_Chan = 2;
        } while ((Bitmask_a((u8)Vec_Expl_Chan) & Vec_Expl_ChanB) == 0);

        // Calculate pointer to channel registers (negative channel offset)
        u8* wk = ((u8*)&Vec_Music_Wk_1) - (Vec_Expl_Chan << 1);

        // Generate pseudo-random explosion noise
        u8 rnd = Random() & 0x0F;
        if (rnd <= 5)
            rnd = (rnd << 1) + 5;

        wk[0] = rnd;
        wk[1] = Vec_Random_Seed1;
    }
    Vec_Music_Wk_7 &= (~Vec_Expl_1);
}


#define Music_Table_1 ((i8*)0xF9E4)
#define Music_Table_2 ((i8*)0xF9EA)
#define Freq_Table    ((i8*)0xFC8D)

void Init_Sound()
{
    if (Vec_Music_Flag < 1) // Continue
    {
        i8* timers = Vec_ADSR_timers;
        for (i8 i = 2; i >= 0; --i)
        {
            if (timers[i] != 0x1F)
                ++timers[i];
        }
        i8* twang = (i8*)Vec_Music_Twang;
        for (i8 limit = 7; limit <= 9; ++limit) // 7 - 9 for each channel
        {
            ++*twang;
            if (*twang < limit)
                *twang = 0;

            i8 idx = (*twang++) & 0x07;
            idx = Vec_Twang_Table[idx];
            *twang++ = idx;
        }

        if (--Vec_Duration != 0)
            goto update_adsr; // Channel not done

        goto eval_next_channel;
    }
    else if (Vec_Music_Flag > 0) // Init
    {
        // Init_Music:
        Vec_Freq_Table = Freq_Table;
        // Init_Music_dft:
        Vec_Music_Flag = -1;
        Vec_ADSR_Table = (u16)SOUND.music->amplitudes->values;
        Vec_Twang_Table = (i8*)SOUND.music->frequencies->values;
        Vec_Music_Ptr = (i8*)SOUND.music->notes;
        Init_Music_Buf();
        Vec_ADSR_timers[1] = 0x1F;
        Vec_ADSR_timers[2] = 0x1F;
        Vec_Music_freq[1] = 0;
        Vec_Music_freq[2] = 0;
        Vec_Music_Chan = 0;
        goto eval_byte;
    }
    else // == 0 Sounds off
    {
        return;
    }
eval_next_channel:;
    // Switch to next channel
    if (--Vec_Music_Chan < 0)
        Vec_Music_Chan = 2;
eval_byte:;
    i8 byte = *Vec_Music_Ptr;
    Vec_ADSR_timers[Vec_Music_Chan] = 0;
    if ((byte & 0x40) == 0) // Is noise.
    {
        Vec_Music_Wk_7 = Music_Table_2[Vec_Music_Chan] & Vec_Music_Wk_7;
        Vec_Music_Wk_7 = Music_Table_2[Vec_Music_Chan + 3] | Vec_Music_Wk_7;
        Vec_Music_freq[Vec_Music_Chan] = ((u16*)Vec_Freq_Table)[byte & 0x3F]; // 16-bit frequency from note bits
    }
    else // Is track. (Melody)
    {
        Vec_Music_Wk_7 = Music_Table_1[Vec_Music_Chan] & Vec_Music_Wk_7;
        Vec_Music_Wk_7 = Music_Table_1[Vec_Music_Chan + 3] | Vec_Music_Wk_7;
        Vec_Music_Wk_6 = byte & 0x1F;
    }

    // Next music byte
    byte = *Vec_Music_Ptr++; // Get first byte again
    if (byte >= 0) // Last channel?
    {
        byte = *Vec_Music_Ptr; // Get Second byte
        if (byte < 0) // End of track.
        {
            Init_Music_Buf();
            Vec_Music_Flag = 0;
            return;
        }
        ++Vec_Music_Ptr;
        byte &= 0x3F; // Mask off duration
        Vec_Duration = byte; // Last byte is the duration that the channel notes should be played
    }
    else // if channel (CHN) bit is set, which is 0x80.
    {
        goto eval_next_channel;
    }
update_adsr:;
    // Adjust time for the signal to come up?
    u8* timer = (u8*)Vec_ADSR_timers; // (:O) SHOCKER, TIMER HAS TO BE UNSIGNED
    for (i8 ch = 2; ch >= 0; --ch)
    {
        u8 time = *timer++;
        if ((time & 1) == 0)
            time = ((u8*)Vec_ADSR_Table)[time >> 1] & 0xF;
        else
            time = ((u8*)Vec_ADSR_Table)[time >> 1] >> 4;
        (&Vec_Music_Wk_A)[ch] = (i8)time;
    }

    // Adjust twang of work
    u16* freqTable = &Vec_Music_freq[3];
    u16* twangs = (u16*)&Vec_Music_Wk_5;
    do
    {
        u16 freq = *--freqTable; // get next base frequency 
        i8 twang = *((i8*)(&freqTable[-4])); // Based on the ram memory layout, I don't get how this leads to twang, but it works. \_o_/
        // I don't really get why one needs a branch here too. Because they negate two times. If twang would be u8 then it would make a difference.
        //if (twang < 0) 
        //{ 
        //    const i8 neg = -twang; 
        //    freq = U16(I16(freq) - neg); 
        //} 
        //else // >= 0 
        //{ 
        //    freq = U16(I16(freq) + twang); 
        //} 
        //*twangs++ = freq; 
        // 
        // Doing just an add instead.
        *twangs++ = U16(I16(freq) + twang);
    } while (twangs != (u16*)((&Vec_Music_Work) + 14));
}

void do_sound(void)
{
    u8* X = (u8*)&Vec_Snd_Shadow; 
    u8* U = (u8*)&Vec_Music_Work; 
    
    for (i8 A = 13; A >= 0; --A)
    {
        const u8 work = *U;
        const u8 current = *X;
        if (work != current) // update register if changed
        {
            Sound_Byte_x((u8)A, work, X); // Write to PSG and indicated mirror
            *X = work;
        }
        U++;
        X++;
    }
}

void sound_play()
{
    Vec_Music_Flag |= 1;
    //Explosion_Sound();
    Init_Sound();
    do_sound();
}