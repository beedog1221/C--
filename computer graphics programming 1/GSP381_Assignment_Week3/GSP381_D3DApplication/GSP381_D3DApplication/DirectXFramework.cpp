//////////////////////////////////////////////////////////////////////////D3DPRESENT_PARAMETERS m_D3Dpp;
// Name:	DirectXFramework.cpp
// Date:	April 2nd, 2010
// Author:	Kyle Lauing [klauing@devry.edu] or [kylelauing@gmail.com]
// Purpose: This file is used to create a very simple framework for using
//			DirectX 9 for the GSP 381 course for DeVry University.
// Disclaimer:	
//			Copyright © 2010 by DeVry Educational Development Corporation.
//			All rights reserved.  No part of this work may be reproduced 
//			or used in any form or by any means – graphic, electronic, or 
//			mechanical, including photocopying, recording, Web distribution 
//			or information storage and retrieval systems – without the 
//			prior consent of DeVry Educational Development Corporation.
//////////////////////////////////////////////////////////////////////////
#include "DirectXFramework.h"

CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
	m_clearColor	= D3DXCOLOR(0.0f, 0.4f, 0.8f, 1.0f);
	// Zero out the memory of the presentation parameters structure
	ZeroMemory(&m_D3Dpp, sizeof(m_D3Dpp));  

	m_bVideoPlaying	= true;
	ZeroMemory(m_bKeyDown, sizeof(m_bKeyDown));

	// TODO: Set initial values for all values in your .h
}

CDirectXFramework::~CDirectXFramework(void)
{
	// If Shutdown is not explicitly called correctly, call it when 
	// this class is destroyed or falls out of scope as an error check.
	Shutdown();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd;

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device
	//////////////////////////////////////////////////////////////////////////

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find the width and height of window using hWnd and GetClientRect()
	GetClientRect(hWnd, &screenRect);
	m_width = screenRect.right - screenRect.left;
	m_height = screenRect.bottom - screenRect.top;

	// Set D3D Device presentation parameters before creating the device
	m_D3Dpp.hDeviceWindow				= hWnd;										// Handle to the focus window
	m_D3Dpp.Windowed					= bWindowed;								// Windowed or Full-screen boolean
	m_D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	m_D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	m_D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	m_D3Dpp.BackBufferFormat			= bWindowed?D3DFMT_UNKNOWN:D3DFMT_X8R8G8B8;	// Back-buffer format, 8 bits for Red, Green, and Blue for each pixel
	m_D3Dpp.BackBufferHeight			= m_height;									// Make sure resolution is supported, use adapter modes
	m_D3Dpp.BackBufferWidth				= m_width;									// (Same as above)
	m_D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	m_D3Dpp.PresentationInterval		= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	m_D3Dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	m_D3Dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;					// Full-screen refresh rate, use adapter modes or default
	m_D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	m_D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	HR(m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps));

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	HR(m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,			// which adapter to use, set to primary
		D3DDEVTYPE_HAL,				// device type to use, set to hardware rasterization
		hWnd,						// handle to the focus window
		deviceBehaviorFlags,		// behavior flags
		&m_D3Dpp,					// presentation parameters
		&m_pD3DDevice));			// returned device pointer

	//////////////////////////////////////////////////////////////////////////
	// TODO: Initialize run-once objects and variables here
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////

	// Load a font for private use for this process
	AddFontResourceEx(L"Delicious-Roman.otf", FR_PRIVATE, 0);

	// Load D3DXFont, each font style you want to support will need an ID3DXFont
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Delicious-Roman"), &m_pD3DFont);

	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	// Create a sprite object, note you will only need one for all 2D sprites
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Create a texture, each different 2D sprite to display to the screen
	// will need a new texture object.  If drawing the same sprite texture
	// multiple times, just call that sprite's Draw() with different 
	// transformation values.
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"test.tga", 0, 0, 0, 0, 
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DCOLOR_XRGB(255, 0, 255), &m_imageInfo, 0, &m_pTexture);

	// Load second sprite texture, drawn 6 times, load only once!
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"test.png", 0, 0, 0, 0, 
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DCOLOR_XRGB(255, 0, 255), &m_imageInfo2, 0, &m_pTexture2);

	// Seed rand() with time
	srand(timeGetTime());

	// Initialize each sprite with random values ONCE, hence why this is in the init
	for(int i = 0; i < 6; ++i)
	{
		spriteObj[i].position = D3DXVECTOR3(float(rand() % m_width), float(rand() % m_height), 0.0f); // Random position within screen width and height
		spriteObj[i].rotation = D3DXToRadian(rand() % 360); // Random rotation
		spriteObj[i].scale = ((3.0f - 0.5f) * ((float)rand() / RAND_MAX)) + 0.5f; // Generate a random float between 0.5 and 3.0
		spriteObj[i].color = D3DCOLOR_ARGB(rand() % 255, rand() % 255, rand() % 255, rand() % 255); // Random color and alpha
	}

	//////////////////////////////////////////////////////////////////////////
	// Initialize DirectInput
	//////////////////////////////////////////////////////////////////////////

	// Create the DI Object
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);

	// Initialize Keyboard
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);

	// Initialize Mouse
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	// Set up Keyboard
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// Set up Mouse (c_dfDIMouse2 = 8 button mouse)
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

	//////////////////////////////////////////////////////////////////////////
	// Initialize FMOD and load/stream sounds
	//////////////////////////////////////////////////////////////////////////
	// The following code is taken from the "Platform Specific Issues" section 
	// and the "Getting Started" section of the FMOD documentation.

	//Use the following code as a basis for your Windows start up sequence.

	/*
	Create a System object and initialize.
	*/

	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		//return 0;
	}

	result = system->getNumDrivers(&numdrivers);
	ERRCHECK(result);

	if (numdrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(result);
	}
	else
	{
		result = system->getDriverCaps(0, &caps, 0, &speakermode);
		ERRCHECK(result);

		result = system->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
		ERRCHECK(result);

		if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
		{                                                   /* You might want to warn the user about this. */
			result = system->setDSPBufferSize(1024, 10);
			ERRCHECK(result);
		}

		result = system->getDriverInfo(0, name, 256, 0);
		ERRCHECK(result);

		if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
		{
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			ERRCHECK(result);
		}
	}

	// This initialization uses 100 'virtual voices'
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
	{
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ERRCHECK(result);

		result = system->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
		ERRCHECK(result);
	}

	// Load a sound effect from file
	// FMOD_DEFAULT flag represents: FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
	// Space bar sound effect
	result = system->createSound("tada.wav", FMOD_DEFAULT, 0, &sound[0]);
	ERRCHECK(result);

	// Number 1 Key
	result = system->createSound("ding.wav", FMOD_DEFAULT, 0, &sound[1]);
	ERRCHECK(result);

	// Number 2 Key
	result = system->createSound("chord.wav", FMOD_DEFAULT, 0, &sound[2]);
	ERRCHECK(result);

	// Number 3 Key
	result = system->createSound("jaguar.wav", FMOD_DEFAULT, 0, &sound[3]);
	ERRCHECK(result);

	// Number 4 Key
	result = system->createSound("swish.wav", FMOD_DEFAULT, 0, &sound[4]);
	ERRCHECK(result);

	// Stream - Background music (looping)
	// Start background music, once intro video has finished playing or is interrupted
	result = system->createStream("wave.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &sound[5]);
	ERRCHECK(result);


	//////////////////////////////////////////////////////////////////////////
	// Initialize DirectShow
	//////////////////////////////////////////////////////////////////////////

	// Initialize COM
	CoInitialize(NULL);

	// Create out filter graph
	CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder);

	// Media Control
	m_pGraphBuilder->QueryInterface( 	IID_IMediaControl, 	(void**)&m_pMediaControl);

	// Media Event
	m_pGraphBuilder->QueryInterface( 	IID_IMediaEvent, (void**)&m_pMediaEvent);

	// Stream intro video file
	m_pGraphBuilder->RenderFile(L"intro.wmv", NULL);

	// Video Window
	m_pMediaControl->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow);

	// Setup the window
	m_pVideoWindow->put_Owner((OAHWND)m_hWnd);

	// Set the style
	m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);

	// Obtain the size of the window
	RECT WinRect;
	GetClientRect(m_hWnd, &WinRect);

	// Set the video size to the size of the window
	m_pVideoWindow->SetWindowPosition(WinRect.left, WinRect.top, WinRect.right, WinRect.bottom);


	// Run (Play) the graph at the end of initialization
	m_pMediaControl->Run();
}

void CDirectXFramework::Update(float dt)
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: Any non-graphics related code can go here to be updated per frame
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Keyboard Input
	//////////////////////////////////////////////////////////////////////////

	// buffer - Stores our keyboard device state
	char buffer[256];
	ZeroMemory(buffer, sizeof(buffer));

	// Get the input device state
	HRESULT hr;
	hr = m_pDIKeyboard->GetDeviceState( sizeof(buffer), (LPVOID)&buffer );

	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIKeyboard->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;

		// Read the device state again
		m_pDIKeyboard->GetDeviceState(sizeof(buffer), buffer);
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Mouse Input
	//////////////////////////////////////////////////////////////////////////
	// Stores our mouse state for an 8 button mouse.
	DIMOUSESTATE2 mouseState;
	ZeroMemory(&mouseState, sizeof(mouseState));

	// Get the input device state
	hr = m_pDIMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &mouseState );
	if(FAILED(hr))
	{
		hr = m_pDIMouse->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIMouse->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;
		// Read the device state again
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectShow Update (Check video completion and if 'Enter' key is pressed
	//////////////////////////////////////////////////////////////////////////

	if(m_bVideoPlaying)
	{
		//////////////////////////////////////////////////////////////////////////
		// Stop video and continue with application, if Enter pressed [DIK_RETURN]
		//////////////////////////////////////////////////////////////////////////
		if(buffer[DIK_RETURN] & 0x80)
		{
			// Act on exit key here. (Continue with next game state, etc)
			m_pMediaControl->Stop();
			m_bVideoPlaying = false;

			m_pVideoWindow->put_Visible(OAFALSE);
			m_pVideoWindow->put_Owner((OAHWND)m_hWnd);

			// Clean up DirectShow variables now that it is completed.
			SAFE_RELEASE(m_pMediaControl);
			SAFE_RELEASE(m_pVideoWindow);
			SAFE_RELEASE(m_pGraphBuilder);
		}

		//////////////////////////////////////////////////////////////////////////
		// If video has finished playing, continue with application.
		//////////////////////////////////////////////////////////////////////////
		long evCode, param1, param2;

		if(SUCCEEDED(m_pMediaEvent->GetEvent(&evCode, &param1, &param2, 0)))
		{
			m_pMediaEvent->FreeEventParams(evCode, param1, param2);

			// If video is complete (EC_COMPLETE)
			if(evCode == EC_COMPLETE)
			{
				// Act on complete call here. (Continue with next game state, etc)
				m_pMediaControl->Stop();
				m_bVideoPlaying = false;

				m_pVideoWindow->put_Visible(OAFALSE);
				m_pVideoWindow->put_Owner((OAHWND)m_hWnd);

				// Clean up DirectShow variables now that it is completed.
				SAFE_RELEASE(m_pMediaControl);
				SAFE_RELEASE(m_pVideoWindow);
				SAFE_RELEASE(m_pGraphBuilder);

			}
		}

	}
	else
	{

		//////////////////////////////////////////////////////////////////////////
		//	Keyboard Code Examples: [DIK (DirectInput Key) codes we translate]
		//	DIK_0 – DIK_9
		//	DIK_NUMPAD0 – DIK_NUMPAD9
		//	DIK_A – DIK_Z
		//	DIK_F1 – DIK_F12
		//	DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT	// Arrow Keys
		//	DIK_SPACE, DIK_TAB, DIK_CAPITAL, DIK_LCONTROL (Left Ctrl Key), 
		//  DIK_RCONTROL (Right Ctrl Key), DIK_RETURN, DIK_LMENU (Left Alt Key), 
		//  DIK_LWIN (Left Windows Key), DIK_LSHIFT (Left Shift Key), etc.
		//	Complete list under Keyboard Device in the documentation.
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//	Mouse variables:  [MouseState2 structure supports 8 button mice]
		//	lX				-	X-axis mouse relative coordinates
		//	lY				-	Y-axis mouse relative coordinates
		//	lZ				-	Mouse wheel relative coordinates
		//	rgbButtons[8]	-	Array of 8 mouse buttons
		//
		//	Usually mouse hardware maps the button layout in a standard way for 
		//	the first 4 buttons, after that it depends on the mouse hardware layout
		//	rgbButtons[0]	-	Left Mouse Button
		//	rgbButtons[1]	-	Right Mouse Button
		//	rgbButtons[2]	-	Middle Mouse Button (click scroll wheel)
		//	rgbButtons[3]	-	Side Mouse Button 1
		//	rgbButtons[4]	-	Side Mouse Button 2
		//	rgbButtons[5]	-	Side Mouse Button 3
		//	rgbButtons[6]	-	Side Mouse Button 4
		//	rgbButtons[7]	-	Side Mouse Button 5
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//	Act on Input
		//////////////////////////////////////////////////////////////////////////

		// Keyboard
		if(buffer[DIK_W] & 0x80)
		{
			//spriteObj[5].position.y -= 0.1f;
			spriteObj[5].position.y -= 100.0f * dt;

		}
		if(buffer[DIK_A] & 0x80)
		{
			spriteObj[5].position.x -= 100.0f * dt;
		}
		if(buffer[DIK_S] & 0x80)
		{
			spriteObj[5].position.y += 100.0f * dt;
		}
		if(buffer[DIK_D] & 0x80)
		{
			spriteObj[5].position.x += 100.0f * dt;
		}

		// Mouse
		spriteObj[5].position.x += mouseState.lX;
		spriteObj[5].position.y += mouseState.lY;

		if(mouseState.rgbButtons[0] & 0x80)
		{
			spriteObj[5].scale -= 0.01f;
		}
		if(mouseState.rgbButtons[1] & 0x80)
		{
			spriteObj[5].scale += 0.01f;
		}

		// Clamp sprite position to boundaries of the screen
		if(spriteObj[5].position.x <= 0)
			spriteObj[5].position.x = 0;
		if(spriteObj[5].position.x >= m_width)
			spriteObj[5].position.x = m_width;
		if(spriteObj[5].position.y <= 0)
			spriteObj[5].position.y = 0;
		if(spriteObj[5].position.y >= m_height)
			spriteObj[5].position.y = m_height;

		// Keboard - Sprite 0 Movement
		if(buffer[DIK_UP] & 0x80)
		{
			spriteObj[0].position.y -= 0.3f;
		}
		if(buffer[DIK_LEFT] & 0x80)
		{
			spriteObj[0].position.x -= 0.3f;
		}
		if(buffer[DIK_DOWN] & 0x80)
		{
			spriteObj[0].position.y += 0.3f;
		}
		if(buffer[DIK_RIGHT] & 0x80)
		{
			spriteObj[0].position.x += 0.3f;
		}

		// Mouse - Sprite 0 Rotation
		spriteObj[0].rotation += mouseState.lX * 0.01f;

		// Mouse - Sprite 0 Scaling
		if(mouseState.rgbButtons[0] & 0x80)
		{
			spriteObj[0].scale -= 0.01f;
			// Clamp scaling to 0.5f (half original size)
			if(spriteObj[0].scale <= 0.5f)
			{
				spriteObj[0].scale = 0.5f;
			}
		}
		if(mouseState.rgbButtons[1] & 0x80)
		{
			spriteObj[0].scale += 0.01f;

			// Clamp scaling to 0.5f (half original size)
			if(spriteObj[0].scale >= 3.0f)
			{
				spriteObj[0].scale = 3.0f;
			}
		}
		if(mouseState.rgbButtons[2] & 0x80)
		{
			spriteObj[0].scale = 1.0f;
		}

		// Sprite 0 clamping to screen space
		if(spriteObj[0].position.x <= 0)
			spriteObj[0].position.x = 0;
		if(spriteObj[0].position.x >= m_width)
			spriteObj[0].position.x = m_width;
		if(spriteObj[0].position.y <= 0)
			spriteObj[0].position.y = 0;
		if(spriteObj[0].position.y >= m_height)
			spriteObj[0].position.y = m_height;

		//////////////////////////////////////////////////////////////////////////
		// FMOD Update
		//////////////////////////////////////////////////////////////////////////
		system->update();

		// Space bar to play sound effect
		// If key is pressed, and the bool to detect if a key was pressed
		// in a previous frame is false, then enter statement
		if(buffer[DIK_SPACE] & 0x80)
		{
			if(!m_bKeyDown[DIK_SPACE])
			{
				m_bKeyDown[DIK_SPACE] = true;
				result = system->playSound(FMOD_CHANNEL_FREE, sound[0], false, &channel[0]);
				ERRCHECK(result);
			}
		}
		// If the key is NOT pressed, and DIK_SPACE is true, then a key-up event just occurred
		else
		{
			if(m_bKeyDown[DIK_SPACE])
			{
				m_bKeyDown[DIK_SPACE] = false;
			}
		}

		// Number key 1-4

		if(buffer[DIK_1] & 0x80)
		{
			if(!m_bKeyDown[DIK_1])
			{
				m_bKeyDown[DIK_1] = true;
				result = system->playSound(FMOD_CHANNEL_FREE, sound[1], false, &channel[1]);
				ERRCHECK(result);
			}
		}
		// If the key is NOT pressed, and DIK_1 is true, then a key-up event just occurred
		else
		{
			if(m_bKeyDown[DIK_1])
			{
				m_bKeyDown[DIK_1] = false;
			}
		}

		if(buffer[DIK_2] & 0x80)
		{
			if(!m_bKeyDown[DIK_2])
			{
				m_bKeyDown[DIK_2] = true;
				result = system->playSound(FMOD_CHANNEL_FREE, sound[2], false, &channel[2]);
				ERRCHECK(result);
			}
		}
		// If the key is NOT pressed, and DIK_2 is true, then a key-up event just occurred
		else
		{
			if(m_bKeyDown[DIK_2])
			{
				m_bKeyDown[DIK_2] = false;
			}
		}

		if(buffer[DIK_3] & 0x80)
		{
			if(!m_bKeyDown[DIK_3])
			{
				m_bKeyDown[DIK_3] = true;
				result = system->playSound(FMOD_CHANNEL_FREE, sound[3], false, &channel[3]);
				ERRCHECK(result);
			}
		}
		// If the key is NOT pressed, and DIK_3 is true, then a key-up event just occurred
		else
		{
			if(m_bKeyDown[DIK_3])
			{
				m_bKeyDown[DIK_3] = false;
			}
		}

		if(buffer[DIK_4] & 0x80)
		{
			if(!m_bKeyDown[DIK_4])
			{
				m_bKeyDown[DIK_4] = true;
				result = system->playSound(FMOD_CHANNEL_FREE, sound[4], false, &channel[4]);
				ERRCHECK(result);
			}
		}
		// If the key is NOT pressed, and DIK_4 is true, then a key-up event just occurred
		else
		{
			if(m_bKeyDown[DIK_4])
			{
				m_bKeyDown[DIK_4] = false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// FPS and milliseconds per frame
		//////////////////////////////////////////////////////////////////////////
		// Calculate Frames per Second
		m_currTime += dt;

		static int fpsCounter = 0;
		if(m_currTime >= 1.0f)
		{
			// fps update
			m_currTime = 0;
			m_FPS = fpsCounter;
			fpsCounter = 0;	

			// updated ms every ~1s fps counter is updated
			m_elapsedTime = dt * 1000;
		}
		else
		{
			++fpsCounter;
		}

	}
}

void CDirectXFramework::Render(float dt)
{
	if(!m_bVideoPlaying)
	{
		//////////////////////////////////////////////////////////////////////////
		// All draw calls between swap chain's functions, and pre-render and post- 
		// render functions (Clear and Present, BeginScene and EndScene)
		//////////////////////////////////////////////////////////////////////////

		// Clear the back buffer, call BeginScene()
		HR(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , m_clearColor, 1.0f, 0));
		HR(m_pD3DDevice->BeginScene())

			//////////////////////////////////////////////////////////////////////////
			// TODO: Draw 3D Objects
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			// Draw 2D sprites
			//////////////////////////////////////////////////////////////////////////
			// Note: You should only be calling the sprite object's begin and end once, 
			// with all draw calls of sprites between them

			// Call Sprite's Begin to start rendering 2D sprite objects
			HR(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK));

		//////////////////////////////////////////////////////////////////////////
		// Matrix Transformations to control sprite position, scale, and rotate
		// Set these matrices for each object you want to render to the screen
		//////////////////////////////////////////////////////////////////////////
		D3DXMATRIX transMat, rotMat, scaleMat, worldMat;
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&worldMat);

		D3DXMatrixScaling(&scaleMat, 2.0f, 2.0f, 0.0f);			// Scaling
		D3DXMatrixRotationZ(&rotMat, D3DXToRadian(90.0f));		// Rotation on Z axis, value in radians, converting from degrees
		D3DXMatrixTranslation(&transMat, 300.0f, 300.0f, 0.0f);	// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world

		// Set Transform
		m_pD3DSprite->SetTransform(&worldMat);

		// Draw the texture with the sprite object
		m_pD3DSprite->Draw(m_pTexture, 0, &D3DXVECTOR3(m_imageInfo.Width * 0.5f, m_imageInfo.Height * 0.5f, 0.0f),
			0, D3DCOLOR_ARGB(255, 255, 255, 255));

		//////////////////////////////////////////////////////////////////////////
		// Assignment 1
		//////////////////////////////////////////////////////////////////////////
		for(int i = 0; i < 5; ++i)
		{
			D3DXMatrixScaling(&scaleMat, spriteObj[i].scale, spriteObj[i].scale, 0.0f);			// Scaling
			D3DXMatrixRotationZ(&rotMat, spriteObj[i].rotation);	// Rotation on Z axis, value in radians, converting from degrees
			D3DXMatrixTranslation(&transMat, spriteObj[i].position.x, spriteObj[i].position.y, 0.0f);	// Translation
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world

			// Set Transform
			m_pD3DSprite->SetTransform(&worldMat);

			// Draw the texture with the sprite object
			m_pD3DSprite->Draw(m_pTexture2, 0, &D3DXVECTOR3(m_imageInfo2.Width * 0.5f, m_imageInfo2.Height * 0.5f, 0.0f),
				0, spriteObj[i].color);
		}

		// Special case sprite, rotate constantly with the help of timeGetTime()
		// Time is always increasing, so we can use that and scale the speed of it
		D3DXMatrixScaling(&scaleMat, spriteObj[5].scale, spriteObj[5].scale, 0.0f);			// Scaling
		D3DXMatrixRotationZ(&rotMat, timeGetTime() * 0.001f);	// Adjust speed of rotation by scaling the time value by an amount
		D3DXMatrixTranslation(&transMat, spriteObj[5].position.x, spriteObj[5].position.y, 0.0f);	// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world

		// Set Transform
		m_pD3DSprite->SetTransform(&worldMat);

		// Draw the texture with the sprite object
		m_pD3DSprite->Draw(m_pTexture2, 0, &D3DXVECTOR3(m_imageInfo2.Width * 0.5f, m_imageInfo2.Height * 0.5f, 0.0f),
			0, spriteObj[5].color);


		// End drawing 2D sprites
		m_pD3DSprite->End();

		//////////////////////////////////////////////////////////////////////////
		// Draw Text
		//////////////////////////////////////////////////////////////////////////

		// Draw Text, using DT_TOP, DT_RIGHT for placement in the top right of the
		// screen.  DT_NOCLIP can improve speed of text rendering, but allows text
		// to be drawn outside of the rect specified to draw text in.
		m_pD3DFont->DrawText(0, L"<Student Name Here>", -1, &screenRect, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

		// Draw FPS & ms
		wchar_t buffer[64];
		swprintf_s(buffer, 64, L"FPS: %d\nMs: %0.2f", m_FPS, m_elapsedTime);
		m_pD3DFont->DrawText(0, buffer, -1, &screenRect, DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

		// EndScene, and Present the back buffer to the display buffer
		HR(m_pD3DDevice->EndScene())
			HR(m_pD3DDevice->Present(0, 0, 0, 0));
	}
}

void CDirectXFramework::Shutdown()
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: Free dynamically allocated memory in this run-once function 
	//		 called when an application is closed down.
	// Note: Release COM objects in the opposite order they were created in
	//////////////////////////////////////////////////////////////////////////
	// Release COM objects in the opposite order they were created in

	SAFE_RELEASE(m_pDIObject);	
	SAFE_RELEASE(m_pDIKeyboard);
	SAFE_RELEASE(m_pDIMouse);

	SAFE_RELEASE(m_pTexture2);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pD3DSprite);

	SAFE_RELEASE(m_pD3DFont);
	RemoveFontResourceEx(L"Delicious-Roman.otf", FR_PRIVATE, 0);

	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);
}

void CDirectXFramework::onLostDevice()
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: COM objects that  need to be destroyed on lost device
	//////////////////////////////////////////////////////////////////////////
	m_pD3DFont->OnLostDevice();
	m_pD3DSprite->OnLostDevice();
}

void CDirectXFramework::onResetDevice()
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: COM objects to restore after a lost device is reacquired
	//////////////////////////////////////////////////////////////////////////
	m_pD3DFont->OnResetDevice();
	m_pD3DSprite->OnResetDevice();
}

bool CDirectXFramework::isDeviceLost()
{
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset at the moment,
	// then sleep until next message loop cycle.
	if(hr == D3DERR_DEVICELOST)
	{
		Sleep(20);
		return true;
	}
	else if(hr == D3DERR_DRIVERINTERNALERROR) // If lost due to driver error, quit
	{
		MessageBox(0, L"Internal Driver Error.  This application must exit.", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	else if(hr == D3DERR_DEVICENOTRESET) // The device is lost but has not been reset
	{
		onLostDevice();
		m_pD3DDevice->Reset(&m_D3Dpp);
		onResetDevice();

		// Device no longer lost
		return false;
	}
	else
	{
		// The device is not currently lost
		return false;
	}
}

