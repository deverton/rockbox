/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by Daniel Stenberg
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#ifndef ID3_H
#define ID3_H

#include <stdbool.h>
#include "config.h"
#include "file.h"

/* Audio file types. */
enum {
    AFMT_UNKNOWN = 0,  /* Unknown file format */

    AFMT_MPA_L1,       /* MPEG Audio layer 1 */
    AFMT_MPA_L2,       /* MPEG Audio layer 2 */
    AFMT_MPA_L3,       /* MPEG Audio layer 3 */

    AFMT_PCM_WAV,      /* Uncompressed PCM in a WAV file */
    AFMT_OGG_VORBIS,   /* Ogg Vorbis */
    AFMT_FLAC,         /* FLAC */
    AFMT_MPC,          /* Musepack */
    AFMT_A52,          /* A/52 (aka AC3) audio */
    AFMT_WAVPACK,      /* WavPack */
    AFMT_ALAC,         /* Apple Lossless Audio Codec */
    AFMT_AAC,          /* Advanced Audio Coding (AAC) in M4A container */
    AFMT_SHN,          /* Shorten */
    AFMT_AIFF,         /* Audio Interchange File Format */
    AFMT_SID,          /* SID File Format */

    /* New formats must be added to the end of this list */

    AFMT_NUM_CODECS,

#if CONFIG_CODEC == SWCODEC
    /* masks to decompose parts */
    CODEC_AFMT_MASK    = 0x0fff,
    CODEC_TYPE_MASK    = 0x7000,

    /* switch for specifying codec type when requesting a filename */
    CODEC_TYPE_DECODER = (0 << 12), /* default */
    CODEC_TYPE_ENCODER = (1 << 12)
#endif
};

#if CONFIG_CODEC == SWCODEC
#define AFMT_ENTRY(label, codec_fname, codec_enc_fname, enc_ext) \
    { label, codec_fname, codec_enc_fname, enc_ext }
#else
#define AFMT_ENTRY(label, codec_fname, codec_enc_fname, enc_ext) \
    { label }
#endif

/* record describing the audio format */
struct afmt_entry
{
#if CONFIG_CODEC == SWCODEC
    char label[8];      /* format label */
    char *codec_fn;     /* filename of decoder codec */
    char *codec_enc_fn; /* filename of encoder codec */
    char *ext;          /* default extension for file (enc only for now) */
#else
    char label[4];
#endif
};

/* database of labels and codecs. add formats per above enum */
extern const struct afmt_entry audio_formats[AFMT_NUM_CODECS];

#if CONFIG_CODEC == SWCODEC
/* recording quality to AFMT_* */
extern const int rec_quality_info_afmt[9];
#endif

struct mp3entry {
    char path[MAX_PATH];
    char* title;
    char* artist;
    char* album;
    char* genre_string;
    char* track_string;
    char* year_string;
    char* composer;
    int tracknum;
    int version;
    int layer;
    int year;
    unsigned char id3version;
    unsigned char genre;
    unsigned int codectype;
    unsigned int bitrate;
    unsigned long frequency;
    unsigned long id3v2len;
    unsigned long id3v1len;
    unsigned long first_frame_offset; /* Byte offset to first real MP3 frame.
                                         Used for skipping leading garbage to
                                         avoid gaps between tracks. */
    unsigned long vbr_header_pos;
    unsigned long filesize; /* without headers; in bytes */
    unsigned long length;   /* song length in ms */
    unsigned long elapsed;  /* ms played */

    int lead_trim;          /* Number of samples to skip at the beginning */
    int tail_trim;          /* Number of samples to remove from the end */

    /* Added for Vorbis */
    unsigned long samples;  /* number of samples in track */

    /* MP3 stream specific info */
    unsigned long frame_count; /* number of frames in the file (if VBR) */

    /* Used for A52/AC3 */
    unsigned long bytesperframe; /* number of bytes per frame (if CBR) */

    /* Xing VBR fields */
    bool vbr;
    bool has_toc;           /* True if there is a VBR header in the file */
    unsigned char toc[100]; /* table of contents */

    /* these following two fields are used for local buffering */
    char id3v2buf[300];
    char id3v1buf[3][92];

    /* resume related */
    unsigned long offset;  /* bytes played */
    int index;             /* playlist index */

    /* FileEntry fields */
    long fileentryoffset;
    long filehash;
    long songentryoffset;
    long rundbentryoffset;

    /* runtime database fields */
    short rating;
    long playcount;
    long lastplayed;
    
    /* replaygain support */
    
#if CONFIG_CODEC == SWCODEC
    char* track_gain_string;
    char* album_gain_string;
    long track_gain;    /* 7.24 signed fixed point. 0 for no gain. */
    long album_gain;
    long track_peak;    /* 7.24 signed fixed point. 0 for no peak. */
    long album_peak;
#endif
};

enum {
    ID3_VER_1_0 = 1,
    ID3_VER_1_1,
    ID3_VER_2_2,
    ID3_VER_2_3,
    ID3_VER_2_4
};

bool mp3info(struct mp3entry *entry, const char *filename, bool v1first);
char* id3_get_genre(const struct mp3entry* id3);
char* id3_get_codec(const struct mp3entry* id3);
int getid3v2len(int fd);
void adjust_mp3entry(struct mp3entry *entry, void *dest, void *orig);
void copy_mp3entry(struct mp3entry *dest, struct mp3entry *orig);

#endif
