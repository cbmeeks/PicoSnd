/**
 *
 * Nice video explaining AY:  https://www.youtube.com/watch?v=srmNbi9yQNU
 */


#include <pico/printf.h>
#include <hardware/timer.h>
#include "vgm.h"
#include "res/vgm_music.h"

VGMHeader vgmHeader = {};
uint8_t buffer[BUFFER_LENGTH];
uint8_t bufferCounter = 0;
uint32_t cmd_counter = 0;
uint32_t startTime = 0;        // keep track of elapsedTime for pause command
uint32_t duration = 0;
bool musicPaused = false;
bool musicStopped = false;


void initAudio() {
    bufferCounter = 0;
    cmd_counter = 0;
    audioInit(3579545 / 2, 44100);
    readVGMHeader();
}


// read 64 byte header
void readVGMHeader() {
    vgmHeader.VGMIdent[0] = (char) getByte();    // V
    vgmHeader.VGMIdent[1] = (char) getByte();    // G
    vgmHeader.VGMIdent[2] = (char) getByte();    // M
    vgmHeader.VGMIdent[3] = (char) getByte();    //
    vgmHeader.EoFOffset = read32() + 4;

    uint32_t val = read32();
    unsigned char buff[4];
    buff[0] = toHex((unsigned char) val >> 16);
    buff[1] = toHex((unsigned char) (val >> 8));
    buff[2] = toHex((unsigned char) (val >> 4));
    buff[3] = toHex((unsigned char) (val));

//    vgmHeader.Version =
//        ((int(buff[0])-48) *1000) +
//        ((int(buff[1])-48) *100) +
//        ((int(buff[2])-48) *10) +
//        (int(buff[3])-48);

    vgmHeader.PSGClock = read32();
    vgmHeader.YM2413Clock = read32();
    vgmHeader.GD3Offset = read32() - 4;  // 4 = size of VGMIdent
    vgmHeader.TotalLength = read32();
    vgmHeader.LoopOffset = read32();
    vgmHeader.LoopLength = read32();
    vgmHeader.RecordingRate = read32();
    vgmHeader.SNFB = read16();
    vgmHeader.SNW = (int) getByte();
    getByte();
    vgmHeader.YM2612Clock = read32();
    vgmHeader.YM2151Clock = read32();
    vgmHeader.DataOffset = read32();
    read32();
    read32();
}


uint8_t getByte() {
    if (cmd_counter >= sizeof(music)) {
        musicStopped = true;
        return 0;
    }
    return music[cmd_counter++];
}

//	read a 16-bit number from 2 bytes
uint16_t read16() {
    return getByte() + (getByte() << 8);
}

//	read a 32-bit number from 4 bytes
uint32_t read32() {
    long v0 = (long) getByte();
    long v1 = (long) getByte();
    long v2 = (long) getByte();
    long v3 = (long) getByte();

    return v0 + (v1 << 8) + (v2 << 16) + (v3 << 24);
}


void pause(long samples) {
    duration = ((1000.0 / (44100.0 / (float) samples)) * 1000);
    startTime = time_us_32();
//    startTime = micros();
}


void silence() {
//    ay389x.disable();
}


void play() {
    vgm();
}

// convert number to hexadecimal 255 = FF
char toHex(int h) {
    h &= 0xf;
    if (h < 10) return (h + '0');
    return (h - 10 + 'A');
}


void writeRegister(uint8_t address, uint8_t data) {
    audioWritePsg0(0, address);     // address first
    audioWritePsg0(1, data);        // then data
}

void vgm() {
    //	do nothing if music paused
    if (musicPaused || musicStopped) return;

    // if sound needs a delay skip VGM processing instead of
    // waiting to do other work
    if ((time_us_32() - startTime) <= duration) return;

    uint8_t command = getByte();

    switch (command) {

        /*
            AY8910
            0xA0 aa dd : AY8910, write value dd to register aa
            Spec: 1.70
        */
        case VGM_CMD_AY8910:
            writeRegister(getByte(), getByte());
            break;


            /*
                Wait n samples
                  0x61 nn nn : Wait n samples, n can range from 0 to 65535
                              (approx 1.49 seconds).  Longer pauses than this are
                              represented by multiple wait commands.
                Spec: 1.70
            */
        case VGM_CMD_WAIT:
            pause(read16());
            break;


            /*
                Wait 735 samples
                0x62 : wait 735 samples (60th of sec), shortcut for 0x61 0xdf 0x02
                Spec: 1.70
            */
        case VGM_CMD_WAIT_735:
            pause(735);
            break;


            /*
                Wait 882 samples
                 0x63 : wait 882 samples (50th of sec), shortcut for 0x61 0x72 0x03
                Spec: 1.70
            */
        case VGM_CMD_WAIT_882:
            pause(882);
            break;


            /*
                Override length of 0x62/0x63
                0x64 cc nn nn : override length of 0x62/0x63
                Spec: 1.70
                ** Not fully implemented -- Just pause 882 samples **
            */
        case VGM_PAUSE_BYTE:
            getByte();
            getByte();
            getByte();
            pause(882);
            break;


            /*
                End of sound data
                0x66 : end of sound data
                Spec: 1.70
            */
        case VGM_CMD_EOF:
            silence();
            musicStopped = true;
            break;


            /*
                Data block
                  0x67 ... : data block: see Data blocks from 1.70 specs
                  Spec: 1.70
                  ** Not fully implemented **
            */
        case VGM_CMD_DATA_BLOCK:
            getByte(); // compatibility command (0x67?)
            getByte(); // datatype
            break;


            /*
                Wait n+1 samples
                0x7n : wait n+1 samples, n can range from 0 to 15.
                Spec: 1.70
                ** Not sure if this is implemented correctly. **
                TODO Check on this spec
            */
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
            pause(command & 0xF);
            break;


            /*
                PCM data bank write then short wait
                0x8n : YM2612 port 0 address 2A write from the data bank, then wait
                    n samples; n can range from 0 to 15. Note that the wait is n,
                    NOT n+1. (Note: Written to first chip instance only.)
                Spec: 1.70
                ** Not sure if this is implemented correctly **
                TODO Check on this spec
            */
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
            pause(command & 0xF);
            break;

            /*
                Seek to offset
                0xE0 dddddddd : seek to offset dddddddd (Intel byte order)
                                in PCM data bank
                Spec: 1.70
                TODO	Not implemented correctly
            */
        case VGM_CMD_YM2612_PCM_SEEK:
            read32();
            break;


            /*
                Nothing matches...
            */
        default:
            break;
    }
}