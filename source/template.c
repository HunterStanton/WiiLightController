#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

// Define colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define WHITE   "\x1b[39m"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void ResetScreen()
{
    // Clears the screen and prints basic info about usage
    printf("\x1b[2J");
    printf(YELLOW "Wii Light Controller\nAllows you to turn on the Wii DVD drive light and control it's brightness.\n2017 Hunter Stanton\n------------------------------------\nControls:\nPress 1 to Turn on the DVD Light\nPress 2 to get current status of DVD light\nPress DPAD UP to turn brightness of DVD light up\nPress DPAD DOWN to turn brightness of DVD light down\nPress B to clear the screen\nPress HOME to return to loader\n" WHITE);
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();
	
	// This function initialises the attached controllers
	WPAD_Init();
	
	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	
	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);
	
	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
    
    // Initialize Wii Light stuff
    void WIILIGHT_Init();
    
    WIILIGHT_SetLevel(0);
    WIILIGHT_TurnOn();
    
    // Variables to store current Wii DVD light state
    bool wiiLightIsOn = true;
    int wiiLightBrightness = 0;


    ResetScreen();
	
    

    // Run this code every single frame/vertical sync
	while(1) {

		// Scan the user input
		WPAD_ScanPads();

		// Check which buttons are actually being pressed
		u32 pressed = WPAD_ButtonsDown(0);

		// Return to HBC
        if ( pressed & WPAD_BUTTON_HOME )
        {
            // Notify the user their session is about to end
            printf(WHITE "Returning to launcher!\n");
            printf(WHITE "Goodbye!\n");
            
            // Turn off the DVD light to make certain that it doesn't somehow stay on when back at the HBC
            WIILIGHT_TurnOff();
            
            // Sleep for 3 seconds
            sleep(3);
            
            // Exit back to loader
            exit(0);
        }
        
        if (pressed & WPAD_BUTTON_B )
        {
            // Clear the screen and print basic usage info
            ResetScreen();
        }
        
        if ( pressed & WPAD_BUTTON_1)
        {
            if(!wiiLightIsOn)
            {
                // Toggle the light
                WIILIGHT_Toggle();
                
                wiiLightIsOn = true;
                printf(GREEN "Turned on Wii Light!\n" WHITE);
            }
            else
            {
                // Toggle the light
                WIILIGHT_Toggle();
                
                wiiLightIsOn = false;
                printf(RED "Turned off Wii Light!\n" WHITE);
            }
        }
        
        if (pressed & WPAD_BUTTON_2)
        {
            if(wiiLightIsOn)
            {
                // Inform the user about the current brightness
                printf(WHITE "Wii Light is " GREEN "ON" WHITE " and it's brightness is %i\n", wiiLightBrightness);
            }
            else
            {
                printf(WHITE "Wii Light is " RED "OFF\n");
            }
        }
        
        if (pressed & WPAD_BUTTON_UP)
        {
            // Make sure brightness doesn't exceed 255
            if(wiiLightBrightness != 255)
            {
                // Increment brightness variable and also set the brightness
                wiiLightBrightness++;
                WIILIGHT_SetLevel(wiiLightBrightness);
            }
        }
        
        if (pressed & WPAD_BUTTON_DOWN)
        {
            // Make sure brightness can't go under 0
            if(wiiLightBrightness != 0)
            {
                // Decrement brightness variable and also set the brightness
                wiiLightBrightness--;
                WIILIGHT_SetLevel(wiiLightBrightness);
            }
        }

		// Wait for vertical sync, ensuring that the loop only runs once per frame
		VIDEO_WaitVSync();
	}

	return 0;
}
