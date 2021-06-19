#include <disp_manager.h>

int main(int argc, char* argv[])
{
    dispSystemRegister();

	selectDefaultDispDev("fb");

	defaultDispDevInit();

	dispDefaultDev(500, 500, 'B');

	DispRegion_S dispRegion;
	dispRegion.xLeftUp = 500;
	dispRegion.yLeftUp = 500;
	dispRegion.width   = 8;
	dispRegion.height  = 16;

	PDispBuffer_S pDispBuffer;
	pDispBuffer = getDispBuffer();

	flushDispRegion(&dispRegion, pDispBuffer);

    return 0;
}

