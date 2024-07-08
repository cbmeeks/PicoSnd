#include <stdbool.h>
#include "audio.h"

#define BUFFER_LENGTH					64

/*
	VGM COMMANDS
*/
#define VGM_HEADER                      "Vgm "
#define VGM_CMD_GG_STEREO               0x4F
#define VGM_CMD_PSG                     0x50
#define VGM_CMD_YM2413                  0x51
#define VGM_CMD_YM2612_0                0x52
#define VGM_CMD_YM2612_1                0x53
#define VGM_CMD_2151                    0x54
#define VGM_CMD_WAIT                    0x61
#define VGM_CMD_WAIT_735                0x62
#define VGM_CMD_WAIT_882                0x63
#define VGM_PAUSE_BYTE                  0x64
#define VGM_CMD_EOF                     0x66
#define VGM_CMD_DATA_BLOCK              0x67
#define VGM_CMD_AY8910					0xA0
#define VGM_CMD_YM2612_PCM_SEEK         0xE0


extern bool musicPaused;
extern bool musicStopped;
extern uint32_t startTime;        // keep track of elapsedTime for pause command
extern uint32_t duration;
extern uint8_t bufferCounter;            // how many bytes have we chewed off buffer
extern uint8_t buffer[];      // VGM instruction buffer
extern uint32_t cmd_counter;

typedef struct {
    char VGMIdent[4];           // "Vgm "
    uint32_t EoFOffset;             // relative offset (from this point, 0x04) of the end of file
    uint32_t Version;               // 0x00000101 for 1.01
    uint32_t PSGClock;              // typically 3579545, 0 for no PSG
    uint32_t FMClock;               // typically , 0 for no FM
    uint32_t YM2413Clock;           // typically 3579545, 0 for no FM
    uint32_t YM2151Clock;           // typically 3579545, 0 for no FM
    uint32_t YM2612Clock;           // typically 3579545, 0 for no FM
    uint32_t GD3Offset;             // relative offset (from this point,0x14) of Gd3 tag,0 if not present
    uint32_t TotalLength;           // in samples
    uint32_t LoopOffset;            // relative again (to 0x1c), 0 if no loop
    uint32_t LoopLength;            // in samples, 0 if no loop
    uint32_t RecordingRate;
    uint32_t SNFB;
    uint32_t SNW;
    uint32_t DataOffset;
    uint32_t AY8910Clock;               // I've seen 1/2 of NTSC frequency used (1,789,773)

} VGMHeader;

void readVGMHeader();
uint8_t getByte();
uint16_t read16();
uint32_t read32();

void writeRegister(uint8_t address, uint8_t data);
char toHex(int h);

void initAudio();
void play();
void pause(long samples);
void vgm();
