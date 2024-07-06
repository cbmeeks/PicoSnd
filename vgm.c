#include <stdbool.h>
#include "vgm.h"

struct TVGMHeader {
    char VGMIdent[4];            // "Vgm "
    long EoFOffset;                // relative offset (from this point, 0x04) of the end of file
    long Version;                // 0x00000101 for 1.01
    long PSGClock;                // typically 3579545, 0 for no PSG
    long FMClock;                // typically , 0 for no FM
    long YM2413Clock;            // typically 3579545, 0 for no FM
    long YM2151Clock;            // typically 3579545, 0 for no FM
    long YM2612Clock;            // typically 3579545, 0 for no FM
    long GD3Offset;                // relative offset (from this point,0x14) of Gd3 tag,0 if not present
    long TotalLength;            // in samples
    long LoopOffset;            // relative again (to 0x1c), 0 if no loop
    long LoopLength;            // in samples, 0 if no loop
    long RecordingRate;
    long SNFB;
    long SNW;
    long DataOffset;

    //	TODO	Add other fields here or a way to skip past them (skip probably better)

    long AY8910Clock;            // I've seen 1/2 of NTSC frequency used (1,789,773)

} VGMHeader;

bool musicPaused;
bool musicStopped;

