#pragma once

#include <stdint-gcc.h>

/*
 * Info:    https://www.nesdev.org/wiki/NSF
 *
 * Great Example:   https://www.zophar.net/music/nintendo-nes-nsf/metroid

 * offset  # of bytes   Function
----------------------------
$000    5   STRING  'N','E','S','M',$1A (denotes an NES sound format file)
$005    1   BYTE    Version number $01 (or $02 for NSF2)
$006    1   BYTE    Total songs   (1=1 song, 2=2 songs, etc)
$007    1   BYTE    Starting song (1=1st song, 2=2nd song, etc)
$008    2   WORD    (lo, hi) load address of data ($8000-FFFF)
$00A    2   WORD    (lo, hi) init address of data ($8000-FFFF)
$00C    2   WORD    (lo, hi) play address of data ($8000-FFFF)
$00E    32  STRING  The name of the song, null terminated
$02E    32  STRING  The artist, if known, null terminated
$04E    32  STRING  The copyright holder, null terminated
$06E    2   WORD    (lo, hi) Play speed, in 1/1000000th sec ticks, NTSC (see text)
$070    8   BYTE    Bankswitch init values (see text, and FDS section)
$078    2   WORD    (lo, hi) Play speed, in 1/1000000th sec ticks, PAL (see text)
$07A    1   BYTE    PAL/NTSC bits
                bit 0: if clear, this is an NTSC tune
                bit 0: if set, this is a PAL tune
                bit 1: if set, this is a dual PAL/NTSC tune
                bits 2-7: reserved, must be 0
$07B    1   BYTE    Extra Sound Chip Support
                bit 0: if set, this song uses VRC6 audio
                bit 1: if set, this song uses VRC7 audio
                bit 2: if set, this song uses FDS audio
                bit 3: if set, this song uses MMC5 audio
                bit 4: if set, this song uses Namco 163 audio
                bit 5: if set, this song uses Sunsoft 5B audio
                bit 6: if set, this song uses VT02+ audio
                bit 7: reserved, must be zero
$07C    1   BYTE    Reserved for NSF2
$07D    3   BYTES   24-bit length of contained program data.
                If 0, all data until end of file is part of the program.
                If used, can be used to provide NSF2 metadata
                in a backward compatible way.
$080    nnn ----    The music program/data follows
 */

typedef struct {
    char header[5];             // 'N','E','S','M',$1A (denotes an NES sound format file)
    uint8_t version;
    uint8_t total_songs;
    uint8_t starting_song;
    uint16_t load_addr_data;    // (lo, hi) load address of data ($8000-FFFF)
    uint16_t init_addr_data;    // (lo, hi) init address of data ($8000-FFFF)
    uint16_t play_addr_data;    // (lo, hi) play address of data ($8000-FFFF)
    char name_of_song[32];      // The name of the song, null terminated
    char artist[32];            // The artist, if known, null terminated
    char copyright[32];         // The copyright holder, null terminated
    uint16_t play_speed_ntsc;   // (lo, hi) Play speed, in 1/1000000th sec ticks, NTSC (see text)
    uint8_t bankswitch[8];      // Bankswitch init values (see text, and FDS section)
    uint16_t play_speed_pal;    // (lo, hi) Play speed, in 1/1000000th sec ticks, PAL (see text)
    uint8_t pal_ntsc_bits;      // PAL/NTSC bits
    uint8_t extra_snd_chip;     // Extra Sound Chip Support
    uint8_t nsf2_reserved;      // Reserved for NSF2
    uint8_t length_prg_data[3]; // 24-bit length of contained program data.
} NSF;