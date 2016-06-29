#include "ports.h"
#include "video_mode.h"

#define VBE_DISPI_INDEX_ID 0
#define VBE_DISPI_INDEX_XRES 1
#define VBE_DISPI_INDEX_YRES 2
#define VBE_DISPI_INDEX_BPP 3
#define VBE_DISPI_INDEX_ENABLE 4
#define VBE_DISPI_INDEX_BANK 5
#define VBE_DISPI_INDEX_VIRT_WIDTH 6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 7
#define VBE_DISPI_INDEX_X_OFFSET 8
#define VBE_DISPI_INDEX_Y_OFFSET 9

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA 0x01CF

//Clearing Display Memory LFB parameter
#define VBE_DISPI_NOCLEARMEM 0x80
//#define VBE_DISPI_ENABLED 0x81

#define VBE_DISPI_BPP_4 0x04
#define VBE_DISPI_BPP_8 0x08
#define VBE_DISPI_BPP_15 0x0F
#define VBE_DISPI_BPP_16 0x10
#define VBE_DISPI_BPP_24 0x18
#define VBE_DISPI_BPP_32 0x20

#define VBE_DISPI_ID0 0xB0C0
#define VBE_DISPI_ID1 0xB0C1
#define VBE_DISPI_ID2 0xB0C2
#define VBE_DISPI_ID3 0xB0C3
#define VBE_DISPI_ID4 0xB0C4
#define VBE_DISPI_ID5 0xB0C5
 
#define VBE_DISPI_DISABLED 0x00
#define VBE_DISPI_ENABLED 0x01

#define VBE_DISPI_LFB_ENABLED 0x40


#define VIDEO_ADDRESS_START 0xFD000000
#define RESOLUTION 2359296
#define VIDEO_RESOLUTION 786432
#define END_OF_SCREEN ((char*)0xFD000000) + 2359296
#define WIDTH 1024
#define HEIGHT 768
#define BPP_32_FACTOR 4
#define DATA_MODULE_ADDRESS 0x500000



void outpw(uint16_t port, uint16_t value);
uint16_t inpw(uint16_t port);



void BgaWriteRegister(unsigned short IndexValue, unsigned short DataValue)
{
    outpw(VBE_DISPI_IOPORT_INDEX, IndexValue);
    outpw(VBE_DISPI_IOPORT_DATA, DataValue);
}

unsigned short BgaReadRegister(unsigned short IndexValue)
{
    outpw(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return inpw(VBE_DISPI_IOPORT_DATA);
}

int BgaIsAvailable(void)
{
    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
}

void BgaSetVideoMode(unsigned int Width, unsigned int Height, unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory)
{


    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, (short unsigned int)VBE_DISPI_DISABLED);
    BgaWriteRegister(VBE_DISPI_INDEX_XRES, (short unsigned int) Width);
    BgaWriteRegister(VBE_DISPI_INDEX_YRES, (short unsigned int) Height);
    BgaWriteRegister(VBE_DISPI_INDEX_BPP, (short unsigned int) BitDepth);
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, (short unsigned int) VBE_DISPI_ENABLED |
        (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) |
        (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));

}

void BgaSetBank(unsigned short BankNumber)
{
    BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
}




void paintPixel(int x, int y, uint8_t blue, uint8_t green, uint8_t red) {
    uint8_t * videoPointer =  (uint8_t *) VIDEO_ADDRESS_START;
	log("1 VIDEO POINTER: %h\n", ((intptr_t) videoPointer) >> 4);
    int offset = (y * WIDTH + x) * BPP_32_FACTOR;
	log("2 VIDEO POINTER: %h\n", ((intptr_t) videoPointer) >> 4);
    videoPointer += offset;
	log("3 VIDEO POINTER: %h\n", ((intptr_t) videoPointer) >> 4);
    *videoPointer = blue;
    videoPointer++;
    *videoPointer = green;
    videoPointer++;
    *videoPointer = red;

}


int SetVideoMode(void){
	BgaSetVideoMode(WIDTH, HEIGHT, VBE_DISPI_BPP_32, 1,1);
   
   /*	int i, j;
	for(i = 0; i < WIDTH; i++) {
		for(j = 0; j < HEIGHT; j++) {
			paintPixel(i, j, 255, 255, 255);
		}
	}*/
   return 0;
}


//http://wiki.osdev.org/Bochs_VBE_Extensions
//http://lxr.free-electrons.com/source/drivers/gpu/drm/bochs/bochs.h#L36