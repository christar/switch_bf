#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <switch.h>

#include "bf.h"

int main(int argc, char* argv[])
{
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    // Other initialization goes here. As a demonstration, we print hello world.
    printf("Welcome to Brainfuck!\n");
    printf("Press A to evalute hello.bf\n");
    printf("Press B to print hello message\n");

	Result rc = romfsInit();
	if (R_FAILED(rc)) {
		printf("romfsInit failed: %08X\n", rc);
		return 1;
	}
	
    // Main loop
    while (appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

        // Your code goes here
		if (kDown & HidNpadButton_A) {
			// FILE *file = fopen("romfs:/print.bf", "r");
			FILE *file = fopen("romfs:/hello.bf", "r");
			printf("Result is %d\n", bf_evaluate(file));
			fclose(file);
		}
		
		if (kDown & HidNpadButton_B) {
			bf_hello();
		}

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
