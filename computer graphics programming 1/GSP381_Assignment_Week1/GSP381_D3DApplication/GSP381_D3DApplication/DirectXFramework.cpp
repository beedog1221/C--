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

	//////////////////////////////////////////////////////////////////////////
	// Assignment 1
	//////////////////////////////////////////////////////////////////////////
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

}

void CDirectXFramework::Update(float dt)
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: Any non-graphics related code can go here to be updated per frame
	//////////////////////////////////////////////////////////////////////////

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

void CDirectXFramework::Render(float dt)
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

void CDirectXFramework::Shutdown()
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: Free dynamically allocated memory in this run-once function 
	//		 called when an application is closed down.
	// Note: Release COM objects in the opposite order they were created in
	//////////////////////////////////////////////////////////////////////////
	// Release COM objects in the opposite order they were created in
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

