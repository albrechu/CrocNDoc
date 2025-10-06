#include <cnd/sound.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <vectrex.h>
#include <lib/print.h>

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
    SOUND.music       = &g_musicOff;
    SOUND.sfx         = &g_sfxOff;
    SOUND.reloadMusic = true;
    SOUND.reloadSfx   = true;
}

void sound_push_sfx(sfx_t const* sfx)
{
    SOUND.sfx = sfx;
    Vec_Expl_Flag = 128;
    SOUND.flags |= SoundFlags_Sfx;;
    SOUND.reloadSfx = true;
}

void sound_push_music(music_t const* music)
{
    SOUND.music = music;
    SOUND.flags |= SoundFlags_Music;
    SOUND.reloadMusic = true;
}

void sound_stop_music()
{
    Vec_Music_Flag = 0;
    Stop_Sound();
    SOUND.music = &g_musicOff;
}

force_inline void play_sfx()
{
    i8 level;

    if ((SOUND.flags & SoundFlags_Sfx) == 0)
        return;

    if (SOUND.reloadSfx)  // Init
    {
        Vec_Expl_Flag &= 0x7F;

        *((sfx_t*)&Vec_Expl_1) = *SOUND.sfx;
        
        // Extract info
        Vec_Expl_Chans = ((Vec_Expl_1 >> 3) | Vec_Expl_1) & 0x07; // Which channels are active
        Vec_Expl_ChanA = Vec_Expl_1 & 0x38;
        Vec_Expl_ChanB = Vec_Expl_1 & 0x07;

        Vec_Expl_Chan = 2;
        Vec_Expl_Timer = 0x7F;
        SOUND.reloadSfx = false;
    }
    else // Continue
    {
        if (Vec_Expl_Timer == 0)  // Nothing to do.
        {
            Vec_Expl_Flag = 0;
            SOUND.flags &= ~SoundFlags_Sfx;
            //Stop_Sound();
            //Clear_Sound();
            return;
        }

        Vec_Expl_Timer -= Vec_Expl_4; // Reduce timer by duration
        if (Vec_Expl_Timer < 0) // Also set level to 0?
        {
            level = 0;
            Vec_Expl_Timer = 0;
            goto set_level;
        }

    }

    // Adjust frequency offset and signal level
    {
        i8 time = Vec_Expl_Timer >> 2;

        // Set noise frequency offset (over time) if noise bits set.
        if (Vec_Expl_ChanA != 0)
        {
            if (Vec_Expl_2 < 0) // Constant noise
            {
                Vec_Music_Wk_6 = Vec_Expl_2;
            }
            else if (Vec_Expl_2 > 0) // Grow
            {
                Vec_Music_Wk_6 = ~time;
            }
            else // Decay
            {
                Vec_Music_Wk_6 = time;
            }
        }

        time >>= 1;

        if (time > 7 && time != 0x0F)
            ++time;

        // Set signal level
        if (Vec_Expl_3 < 0) // Constant
        {
            level = 0x0F; // This is 'time' in the original, but it would not make to call the sweep 'constant'
        }
        else if (Vec_Expl_3 > 0) // Grow
        {
            level = time ^ 0x0F;
        }
        else // Decay
        {
            level = time;
        }
    }

set_level:
    {
        // Vec_Music_Wk_7, Vec_XXX_04, Vec_XXX_03 are signal levels for each channel.
        i8* wk = &Vec_Music_Wk_7;
        for (i8 cb = Vec_Expl_Chans; cb != 0; cb >>= 1)
        {
            --wk;
            if (cb & 1)
                *wk = level;
        }
    }

    if (Vec_Expl_ChanB != 0) // Tone bits set?
    {
        do
        {
            if (--Vec_Expl_Chan < 0)
                Vec_Expl_Chan = 2;
        } while ((Bitmask_a((u8)Vec_Expl_Chan) & Vec_Expl_ChanB) == 0);

        // Add random noise
        u8 rnd = Random() & 0x0F;
        if (rnd <= 5)
            rnd = (rnd << 1) + 5;

        Vec_Music_Wk_5 = (i8)rnd;
        Vec_XXX_05     = (i8)Vec_Random_Seed1;
    }
    Vec_Music_Wk_7 &= (~Vec_Expl_1);
}


#define Music_Table_1 ((i8*)0xF9E4)
#define Music_Table_2 ((i8*)0xF9EA)
#define Freq_Table    ((i8*)0xFC8D)

/**
* @shared     Vec_XXX_05, 
* @not-shared Vec_ADSR_timers, Vec_Music_Twang, Vec_Twang_Table, Vec_Music_Chan, Vec_XXX_06
* @buggy? Vec_Music_Ptr is somehow shared (Simulation bug?)!
*/
force_inline void play_music()
{
    i8 byte;
    if (SOUND.reloadMusic)
    {
        // Init_Music:
        Vec_Freq_Table = Freq_Table;
        // Init_Music_dft:
        Vec_Music_Flag = -1;
        Vec_ADSR_Table = (u16)SOUND.music->amplitudes->values;
        Vec_Twang_Table = (i8*)SOUND.music->frequencies->values;
        SOUND.musicPtr = (i8*)SOUND.music->notes;
        //Init_Music_Buf(); // Clear work buffer
        Vec_ADSR_timers[1] = 0x1F;
        Vec_ADSR_timers[2] = 0x1F;
        Vec_Music_freq[1] = 0;
        Vec_Music_freq[2] = 0;
        Vec_Music_Chan = 0;
        SOUND.reloadMusic = false;
        goto eval_byte;
    }

    // Continue
    for (i8 i = 0; i < 3; ++i)
    {
        if (Vec_ADSR_timers[i] != 0x1F)
            ++Vec_ADSR_timers[i];
    }

    // Adjust twang (frequency offset)
    if (false) // Don't need it
    {
        i8* twang = (i8*)&Vec_Music_Twang; // Still unsure if & or not. Probably & (its then written to Vec_Expl_1). 99% of cases is the twang table 0x0 anyway and leads to problems regarding mixing.
        for (i8 limit = 7; limit <= 9; ++limit) // 7 - 9 for each channel
        {
            ++*twang;
            if (*twang < limit)
                *twang = 0;

            i8 idx = (*twang++) & 0x07;
            idx = Vec_Twang_Table[idx];
            *twang++ = idx;
        }
    }

    if (--Vec_Duration != 0)
        goto update_adsr; // Channel not done

eval_next_channel:;
    // Switch to next channel
    if (++Vec_Music_Chan > 2)
    {
        Vec_Music_Chan = 0;
    }
eval_byte:;

    byte = *SOUND.musicPtr;
    Vec_ADSR_timers[Vec_Music_Chan] = 0;
    if ((byte & 0x40) == 0) // Is noise.
    {
        byte &= 0x3F;
        Vec_Music_Wk_7 = (Vec_Music_Wk_7 & Music_Table_2[Vec_Music_Chan]) | Music_Table_2[Vec_Music_Chan + 3];
        Vec_Music_freq[Vec_Music_Chan] = ((u16*)Vec_Freq_Table)[byte]; // 16-bit frequency from note bits
    }
    else // Is track. (Melody)
    {
        byte &= 0x1F;
        Vec_Music_Wk_7 = (Vec_Music_Wk_7 & Music_Table_1[Vec_Music_Chan]) | Music_Table_1[Vec_Music_Chan + 3];
        Vec_Music_Wk_6 |= byte;
    }

    // Next music byte
    byte = *SOUND.musicPtr++; // Get first byte again
    if (byte >= 0) // Last channel?
    {
        byte = *SOUND.musicPtr; // Get Second byte
        if (byte < 0) // End of track.
        {
            Init_Music_Buf();
            Vec_Music_Flag = 0;
            SOUND.reloadMusic = true;
            return;
        }
        ++SOUND.musicPtr;
        byte &= 0x3F; // Mask off duration
        Vec_Duration = byte; // Last byte is the duration that the channel notes should be played
    }
    else // if channel (CHN) bit is set, which is 0x80.
    {
        goto eval_next_channel;
    }
update_adsr:;
    // Adjust channel timers.
    for (i8 ch = 0; ch < 3; ++ch)
    {
        u8 time = (u8)Vec_ADSR_timers[ch]; // (:O) SHOCKER, TIMER HAS TO BE UNSIGNED
        if ((time & 1) == 0)
            time = ((u8*)Vec_ADSR_Table)[time >> 1] & 0xF;
        else
            time = ((u8*)Vec_ADSR_Table)[time >> 1] >> 4;

        (&Vec_XXX_04)[-ch] = (i8)time;
    }

    // Write frequency adjusted by twang (frequency offset) to work buffer [Vec_Music_Wk_5, Vec_XXX_08]. 
    for (i8 ch = 0; ch < 3; ++ch)
    {
        const u16 f = Vec_Music_freq[2 - ch]; // Get next base frequency.
        // const i8 twang = (&Vec_Expl_1)[ch]; // Get twang from twang table
        // Using 0 instead, because the game doesn't need it.
        ((u16*)&Vec_Music_Wk_5)[ch] = f; // Write adjusted freq into music work buffer.
    }
}

/**
* Writes work buffer for sound to the corresponding register in the programmable-sound-generator(PSG)-chip.
*/
force_inline void do_sound(void)
{
    typedef union ptr_t
    {
        u8* data;
        struct { u8 page, idx; };
    } ptr_t;

    ptr_t X = (ptr_t){ .data = (u8*)&Vec_Snd_Shadow };
    ptr_t U = (ptr_t){ .data = (u8*)&Vec_Music_Work };
    
    for (i8 A = 13; A >= 0; --A)
    {
        const u8 work = *U.data;
        const u8 current = *X.data;
        if (work != current) // Register has changed.
        {
            *X.data = work;
            VIA_port_a = A;    // Select register
            VIA_port_b = 0x19; // BDIR=1, BC1=1, mux off
            VIA_port_b = 0x01; // BDIR=0, BC1=0, mux off
            (void)VIA_port_a; // Sound chip status
            VIA_port_a = (i8)work;
            VIA_port_b = 0x11; // BDIR=1, BC1=0, mux off
            VIA_port_b = 0x01; // BDIR=0, BC1=0, mux off
        }
        ++U.idx;
        ++X.idx;
    }
}

void sound_play()
{
    switch (SOUND.flags)
    {
    case SoundFlags_Music:
        break;
    case SoundFlags_Sfx:
    case SoundFlags_All:
        goto sfx;
    default:
        return;
    }
    play_music();
push:
    // Write work to PSG.
    do_sound();
    return;

sfx:
    play_sfx();
    goto push;
}