#include "global.h"
#include "input.h"
#ifdef __vita__
#include <vitasdk.h>
#endif

#ifdef __SWITCH__
int singleJoyconMode = 0;
static int rTriggerHeld = 0;
static int lTriggerHeld = 0;
static int canChangeJoyconMode = 1;
#endif

CPlayerInput::CPlayerInput()
{
	for(int iPlayer = 0; iPlayer < 4; iPlayer++)
	{
		for(int iKey = 0; iKey < NUM_KEYS; iKey++)
		{
			outputControls[iPlayer].keys[iKey].fPressed = false;
			outputControls[iPlayer].keys[iKey].fDown = false;
		}
	}

	iPressedKey = 0;

	fUsingMouse = false;
}

void CPlayerInput::CheckIfMouseUsed()
{
	fUsingMouse = false;
}

//Pass in 0 for game and 1 for menu
//Clear old button pushed states
void CPlayerInput::ClearPressedKeys(short iGameState)
{
	for(int iPlayer = 0; iPlayer < 4; iPlayer++)
	{
		CInputControl * inputControl = &inputControls[iPlayer]->inputGameControls[iGameState];
		COutputControl * outputControl = &outputControls[iPlayer];

		for(int iKey = 0; iKey < NUM_KEYS; iKey++)
		{
			outputControl->keys[iKey].fPressed = false;
		}
	}

	iPressedKey = 0;
}

//Clear all button pushed and down states
//Call this when switching from menu to game
void CPlayerInput::ResetKeys()
{
	for(int iPlayer = 0; iPlayer < 4; iPlayer++)
	{
		for(int iKey = 0; iKey < NUM_KEYS; iKey++)
		{
			outputControls[iPlayer].keys[iKey].fPressed = false;
			outputControls[iPlayer].keys[iKey].fDown = false;
		}
	}

	iPressedKey = 0;
}

//Called during game loop to read input events and see if 
//configured keys were pressed.  If they were, then turn on 
//key flags to be used by game logic
//iGameState == 0 for in game and 1 for menu

void setState(bool state, COutputControl *outputControl, int iKey) {
	if (state) {
		if(!outputControl->keys[iKey].fDown)
			outputControl->keys[iKey].fPressed = true;
			
		outputControl->keys[iKey].fDown = true;
	}else outputControl->keys[iKey].fDown = false;
}

void CPlayerInput::Update(SDL_Event event, short iGameState)
{
	for(short iPlayer = 0; iPlayer < 4; iPlayer++)
	{
		CInputControl * inputControl;
		COutputControl * outputControl;
		short iPlayerID = iPlayer;
		short iDeviceID = DEVICE_KEYBOARD;
#ifdef __vita__
		SceCtrlData pad;
		SceCtrlPortInfo info;
		sceCtrlGetControllerPortInfo(&info);
		if (info.port[iPlayer > 0 ? (iPlayer + 1) : iPlayer] == SCE_CTRL_TYPE_UNPAIRED) continue;
		sceCtrlPeekBufferPositive(iPlayer > 0 ? (iPlayer + 1) : iPlayer, &pad, 1);
		
		outputControl = &outputControls[iPlayer];
		iDeviceID = inputControls[iPlayer]->iDevice;
		
		//Ignore input for cpu controlled players
		if(iGameState == 0 && game_values.playercontrol[iPlayer] != 1)
			continue;
		
		if (iGameState == 0) { // Game
			setState((pad.buttons & SCE_CTRL_LEFT) || (pad.lx < 80), outputControl, 0);
			setState((pad.buttons & SCE_CTRL_RIGHT) || (pad.lx > 180), outputControl, 1);
			setState((pad.buttons & SCE_CTRL_UP) || (pad.ly < 80) || (pad.buttons & SCE_CTRL_CROSS), outputControl, 2);
			setState((pad.buttons & SCE_CTRL_DOWN) || (pad.ly > 180), outputControl, 3);
			setState(pad.buttons & SCE_CTRL_SQUARE, outputControl, 4);
			setState(pad.buttons & SCE_CTRL_SELECT, outputControl, 6);
			setState(pad.buttons & SCE_CTRL_START, outputControl, 7);
			setState(pad.buttons & SCE_CTRL_LTRIGGER, outputControl, 8);
			setState(pad.buttons & SCE_CTRL_RTRIGGER, outputControl, 9);
			setState(pad.buttons & SCE_CTRL_TRIANGLE, outputControl, 10);
			setState(pad.buttons & SCE_CTRL_CIRCLE, outputControl, 5);
		} else {
			setState((pad.buttons & SCE_CTRL_UP) || (pad.ly < 80), outputControl, 0);
			setState((pad.buttons & SCE_CTRL_DOWN) || (pad.ly > 180), outputControl, 1);
			setState((pad.buttons & SCE_CTRL_LEFT) || (pad.lx < 80), outputControl, 2);
			setState((pad.buttons & SCE_CTRL_RIGHT) || (pad.lx > 180), outputControl, 3);
			setState(pad.buttons & SCE_CTRL_CROSS, outputControl, 4);
			setState(pad.buttons & SCE_CTRL_CIRCLE, outputControl, 5);
			setState(pad.buttons & SCE_CTRL_SQUARE, outputControl, 6);
			setState(pad.buttons & SCE_CTRL_TRIANGLE, outputControl, 7);
			setState(pad.buttons & SCE_CTRL_LTRIGGER, outputControl, 8);
			setState(pad.buttons & SCE_CTRL_RTRIGGER, outputControl, 9);
			setState(pad.buttons & SCE_CTRL_START, outputControl, 10);
			setState(pad.buttons & SCE_CTRL_SELECT, outputControl, 11);
		}
#elif defined(__SWITCH__)
		outputControl = &outputControls[iPlayer];
		iDeviceID = inputControls[iPlayer]->iDevice;
		
		//Ignore input for cpu controlled players
		if(iGameState == 0 && game_values.playercontrol[iPlayer] != 1)
			continue;

		if (iPlayer == 0) iPlayer = CONTROLLER_P1_AUTO;
		uint64_t keysHeld = hidKeysHeld(iPlayer);
		JoystickPosition l_pos;
		hidJoystickRead(&l_pos, iPlayer, JOYSTICK_LEFT);
		int lx = l_pos.dx / 256;
		int ly = l_pos.dy / 256;

		if (iGameState == 0) { // Game
			setState((keysHeld & KEY_DLEFT) || (lx < 80), outputControl, 0);
			setState((keysHeld & KEY_DRIGHT) || (lx > 180), outputControl, 1);
			setState((keysHeld & KEY_DUP) || (ly < 80) || (keysHeld & KEY_B), outputControl, 2);
			setState((keysHeld & KEY_DDOWN) || (ly > 180), outputControl, 3);
			setState(keysHeld & KEY_Y, outputControl, 4);
			setState(keysHeld & KEY_MINUS, outputControl, 6);
			setState(keysHeld & KEY_PLUS, outputControl, 7);
			setState(keysHeld & KEY_L, outputControl, 8);
			setState(keysHeld & KEY_R, outputControl, 9);
			setState(keysHeld & KEY_X, outputControl, 10);
			setState(keysHeld & KEY_A, outputControl, 5);
		} else {
			setState((keysHeld & KEY_DUP) || (ly < 80), outputControl, 0);
			setState((keysHeld & KEY_DDOWN) || (ly > 180), outputControl, 1);
			setState((keysHeld & KEY_DLEFT) || (lx < 80), outputControl, 2);
			setState((keysHeld & KEY_DRIGHT) || (lx > 180), outputControl, 3);
			setState(keysHeld & KEY_B, outputControl, 4);
			setState(keysHeld & KEY_A, outputControl, 5);
			setState(keysHeld & KEY_Y, outputControl, 6);
			setState(keysHeld & KEY_X, outputControl, 7);
			setState(keysHeld & KEY_L, outputControl, 8);
			setState(keysHeld & KEY_R, outputControl, 9);
			setState(keysHeld & KEY_PLUS, outputControl, 10);
			setState(keysHeld & KEY_MINUS, outputControl, 11);
		}
		
		// Use L+R to switch between single and dual joycon mode
		if (iPlayer == 0) {
			lTriggerHeld = keysHeld & KEY_L;
			rTriggerHeld = keysHeld & KEY_R;

			if (!rTriggerHeld || !lTriggerHeld) {
				canChangeJoyconMode = 1;
			}
			if (rTriggerHeld && lTriggerHeld && canChangeJoyconMode) {
				singleJoyconMode = !singleJoyconMode;
				canChangeJoyconMode = 0;
			}
		}
#endif
		//This line might be causing input from some players not to be read
		//if(fFound)
			//break;
	}
}
