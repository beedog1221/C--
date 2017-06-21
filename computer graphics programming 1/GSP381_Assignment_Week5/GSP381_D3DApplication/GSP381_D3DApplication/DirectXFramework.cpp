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
	m_bVsync				= false;
	m_pD3DObject			= 0;
	m_pD3DDevice			= 0;
	m_pD3DFont				= 0;		
	m_pDIObject				= 0;	
	m_pDIKeyboard			= 0;	
	m_pDIMouse				= 0;	
	m_fRotation				= 0.0f;
	m_pyramidTranslation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pyramidRotation		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pyramidScaling		= 1.0f;

	m_clearColor			= D3DXCOLOR(0.0f, 0.4f, 0.8f, 1.0f);
	// Zero out the memory of the presentation parameters structure
	ZeroMemory(&m_D3Dpp, sizeof(m_D3Dpp));  
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
	// View and Projection Matrices
	//////////////////////////////////////////////////////////////////////////

	// Initialize View Matrix
	m_eyePos	= D3DXVECTOR3(0.0f, 2.0f, -10.0f);	// Camera position
	m_lookAt	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Pos. camera is viewing
	m_upVec		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// Rotational orientation 

	// Easily calculate the view matrix with 3 intuitive vectors
	D3DXMatrixLookAtLH(
		&m_viewMat,	// Returned viewMat
		&m_eyePos,	// Eye Position
		&m_lookAt,	// LookAt Position
		&m_upVec);	// Up Vector

	// Initialize perspective projection matrix, this creates view frustum
	D3DXMatrixPerspectiveFovLH(
		&m_projMat,						// Returned ProjMat
		D3DXToRadian(65.0f),			// Field of View
		(float)m_width / (float)m_height,	// Aspect Ratio
		1.0f,							// Near Plane
		2000.0f);						// Far Plane

	//////////////////////////////////////////////////////////////////////////
	// Vertex Declaration and Vertex and Index Buffers
	//////////////////////////////////////////////////////////////////////////
	// Vertex declaration
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	// Create vertex declaration
	m_pD3DDevice->CreateVertexDeclaration(decl, &m_pD3DVertexDecl);

	//////////////////////////////////////////////////////////////////////////
	// Load cube data (verts and indices) into respective arrays
	//////////////////////////////////////////////////////////////////////////
	// Load vertex info

	// Front
	m_cubeVerts[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_cubeVerts[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_cubeVerts[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_cubeVerts[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&m_cubeVerts[0].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVec3Normalize(&m_cubeVerts[1].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVec3Normalize(&m_cubeVerts[2].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVec3Normalize(&m_cubeVerts[3].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_cubeVerts[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[1].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[2].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Back
	m_cubeVerts[4].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_cubeVerts[5].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_cubeVerts[6].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_cubeVerts[7].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&m_cubeVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&m_cubeVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&m_cubeVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&m_cubeVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	m_cubeVerts[4].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_cubeVerts[5].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[6].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[7].uv = D3DXVECTOR2(1.0f, 0.0f);

	// Top
	m_cubeVerts[8].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_cubeVerts[9].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_cubeVerts[10].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_cubeVerts[11].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	D3DXVec3Normalize(&m_cubeVerts[8].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[9].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[10].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[11].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_cubeVerts[8].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[9].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[10].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[11].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Bottom
	m_cubeVerts[12].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_cubeVerts[13].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_cubeVerts[14].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_cubeVerts[15].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_cubeVerts[12].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[13].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[14].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[15].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_cubeVerts[12].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_cubeVerts[13].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[14].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[15].uv = D3DXVECTOR2(1.0f, 0.0f);

	// Left
	m_cubeVerts[16].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_cubeVerts[17].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_cubeVerts[18].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_cubeVerts[19].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&m_cubeVerts[16].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[17].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[18].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[19].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	m_cubeVerts[16].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[17].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[18].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[19].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Right
	m_cubeVerts[20].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_cubeVerts[21].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_cubeVerts[22].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_cubeVerts[23].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_cubeVerts[20].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[21].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[22].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[23].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	m_cubeVerts[20].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[21].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[22].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[23].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Load index info, refers into index into verts array to compose triangles
	// Note: A clockwise winding order of verts will show the front face.

	// Front
	m_cubeIndices[0] = 0;  m_cubeIndices[1] = 1;  m_cubeIndices[2] = 2;		// Triangle 0
	m_cubeIndices[3] = 0;  m_cubeIndices[4] = 2;  m_cubeIndices[5] = 3;		// Triangle 1
	// Back
	m_cubeIndices[6] = 4;  m_cubeIndices[7] = 5;  m_cubeIndices[8] = 6;		// Triangle 2
	m_cubeIndices[9] = 4;  m_cubeIndices[10] = 6; m_cubeIndices[11] = 7;	// Triangle 3
	// Top
	m_cubeIndices[12] = 8; m_cubeIndices[13] = 9; m_cubeIndices[14] = 10;	// Triangle 4
	m_cubeIndices[15] = 8; m_cubeIndices[16] = 10; m_cubeIndices[17] = 11;	// Triangle 5
	// Bottom
	m_cubeIndices[18] = 12; m_cubeIndices[19] = 13; m_cubeIndices[20] = 14;	// Triangle 6
	m_cubeIndices[21] = 12; m_cubeIndices[22] = 14; m_cubeIndices[23] = 15;	// Triangle 7
	// Left
	m_cubeIndices[24] = 16; m_cubeIndices[25] = 17; m_cubeIndices[26] = 18;	// Triangle 8
	m_cubeIndices[27] = 16; m_cubeIndices[28] = 18; m_cubeIndices[29] = 19;	// Triangle 9
	// Right
	m_cubeIndices[30] = 20; m_cubeIndices[31] = 21; m_cubeIndices[32] = 22;	// Triangle 10
	m_cubeIndices[33] = 20; m_cubeIndices[34] = 22; m_cubeIndices[35] = 23;	// Triangle 11

	//////////////////////////////////////////////////////////////////////////
	// Pyramid
	//////////////////////////////////////////////////////////////////////////
	// Bottom
	// NOTE: Normals are just a rough approximation, use your textbook to calculate them properly!
	m_pyramidVerts[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_pyramidVerts[1].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_pyramidVerts[2].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_pyramidVerts[3].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_pyramidVerts[0].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_pyramidVerts[1].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_pyramidVerts[2].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_pyramidVerts[3].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_pyramidVerts[0].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_pyramidVerts[1].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_pyramidVerts[2].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_pyramidVerts[3].uv = D3DXVECTOR2(1.0f, 0.0f);

	// Front
	m_pyramidVerts[4].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pyramidVerts[5].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_pyramidVerts[6].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&m_pyramidVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f ));
	m_pyramidVerts[4].uv = D3DXVECTOR2(0.5f, 0.0f);
	m_pyramidVerts[5].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_pyramidVerts[6].uv = D3DXVECTOR2(0.0f, 1.0f);

	// Back
	m_pyramidVerts[7].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pyramidVerts[8].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_pyramidVerts[9].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_pyramidVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[8].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[9].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f ));
	m_pyramidVerts[7].uv = D3DXVECTOR2(0.5f, 0.0f);
	m_pyramidVerts[8].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_pyramidVerts[9].uv = D3DXVECTOR2(0.0f, 1.0f);

	// Left
	m_pyramidVerts[10].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pyramidVerts[11].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_pyramidVerts[12].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_pyramidVerts[10].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[11].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[12].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f ));
	m_pyramidVerts[10].uv = D3DXVECTOR2(0.5f, 0.0f);
	m_pyramidVerts[11].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_pyramidVerts[12].uv = D3DXVECTOR2(0.0f, 1.0f);

	// Right
	m_pyramidVerts[13].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pyramidVerts[14].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_pyramidVerts[15].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&m_pyramidVerts[13].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[14].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f ));
	D3DXVec3Normalize(&m_pyramidVerts[15].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f ));
	m_pyramidVerts[13].uv = D3DXVECTOR2(0.5f, 0.0f);
	m_pyramidVerts[14].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_pyramidVerts[15].uv = D3DXVECTOR2(0.0f, 1.0f);

	// Bottom
	m_pyramidIndices[0] = 0; m_pyramidIndices[1] = 1; m_pyramidIndices[2] = 2;		// Triangle 0
	m_pyramidIndices[3] = 0; m_pyramidIndices[4] = 2; m_pyramidIndices[5] = 3;		// Triangle 1

	// Front
	m_pyramidIndices[6] = 4; m_pyramidIndices[7] = 5; m_pyramidIndices[8] = 6;		// Triangle 2
	// Back
	m_pyramidIndices[9] = 7; m_pyramidIndices[10] = 8; m_pyramidIndices[11] = 9;	// Triangle 3

	// Left
	m_pyramidIndices[12] = 10; m_pyramidIndices[13] = 11; m_pyramidIndices[14] = 12;// Triangle 4

	// Right
	m_pyramidIndices[15] = 13; m_pyramidIndices[16] = 14; m_pyramidIndices[17] = 15;// Triangle 5

	//////////////////////////////////////////////////////////////////////////
	// Generate Tri Grid
	//////////////////////////////////////////////////////////////////////////
	GenTriGrid(100, 100, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_triGridVerts, m_triGridIndices);
	m_numVerts = 100 * 100;
	m_numTris = 99 * 99 * 2;

	// Create Vertex Buffer
	m_pD3DDevice->CreateVertexBuffer(
		4 * 6 * sizeof(Vertex),		// Length in bytes to allocate buffer (num quads * num sides * sizeof(Vertex))
		D3DUSAGE_WRITEONLY,		// Usage
		0,						// Used only with FVF, we are not using
		D3DPOOL_MANAGED,		// Memory Pool
		&m_pD3DVertexBuffer,	// Vertex Buffer
		0);						// No longer used, set to 0

	// Create Index Buffer
	m_pD3DDevice->CreateIndexBuffer(
		3 * 12 * sizeof(WORD),	// Length in bytes to allocate buffer (3 verts * num triangles * sizeof(WORD))
		D3DUSAGE_WRITEONLY,		// Usage
		D3DFMT_INDEX16,			// Index Format
		D3DPOOL_MANAGED,		// Memory Pool
		&m_pD3DIndexBuffer,		// Index Buffer
		0);						// No longer used

	// Pyramid
	// Create Vertex Buffer
	m_pD3DDevice->CreateVertexBuffer(
		16 * sizeof(Vertex),		// Length in bytes to allocate buffer (num quads * num sides * sizeof(Vertex))
		D3DUSAGE_WRITEONLY,			// Usage
		0,							// Used only with FVF, we are not using
		D3DPOOL_MANAGED,			// Memory Pool
		&m_pD3DVertexBufferPyramid,	// Vertex Buffer
		0);							// No longer used, set to 0

	// Create Index Buffer
	m_pD3DDevice->CreateIndexBuffer(
		18 * sizeof(WORD),			// Length in bytes to allocate buffer (3 verts * num triangles * sizeof(WORD))
		D3DUSAGE_WRITEONLY,			// Usage
		D3DFMT_INDEX16,				// Index Format
		D3DPOOL_MANAGED,			// Memory Pool
		&m_pD3DIndexBufferPyramid,	// Index Buffer
		0);							// No longer used

	//////////////////////////////////////////////////////////////////////////
	// Tri Grid
	//////////////////////////////////////////////////////////////////////////
	// Create Vertex Buffer
	m_pD3DDevice->CreateVertexBuffer(
		m_numVerts * sizeof(Vertex),// Length in bytes to allocate buffer (num quads * num sides * sizeof(Vertex))
		D3DUSAGE_WRITEONLY,			// Usage
		0,							// Used only with FVF, we are not using
		D3DPOOL_MANAGED,			// Memory Pool
		&m_pD3DVertexBufferTriGrid,	// Vertex Buffer
		0);							// No longer used, set to 0

	// Create Index Buffer
	m_pD3DDevice->CreateIndexBuffer(
		m_numTris * 3 * sizeof(WORD),// Length in bytes to allocate buffer (3 verts * num triangles * sizeof(WORD))
		D3DUSAGE_WRITEONLY,			// Usage
		D3DFMT_INDEX16,				// Index Format
		D3DPOOL_MANAGED,			// Memory Pool
		&m_pD3DIndexBufferTriGrid,	// Index Buffer
		0);							// No longer used

	//////////////////////////////////////////////////////////////////////////
	// Accessing vertex buffer memory
	//////////////////////////////////////////////////////////////////////////
	void* pVerts;
	// Lock vertex buffer
	m_pD3DVertexBuffer->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		&pVerts,	// Double pointer to data
		0);			// Flags

	// Modify data
	memcpy(pVerts, m_cubeVerts, 4 * 6 * sizeof(Vertex));

	// Unlock vertex buffer
	m_pD3DVertexBuffer->Unlock();

	// Pyramid
	// Lock vertex buffer
	m_pD3DVertexBufferPyramid->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		&pVerts,	// Double pointer to data
		0);			// Flags

	// Modify data
	memcpy(pVerts, m_pyramidVerts, 16 * sizeof(Vertex));

	// Unlock vertex buffer
	m_pD3DVertexBufferPyramid->Unlock();

	//////////////////////////////////////////////////////////////////////////
	// Tri Grid
	//////////////////////////////////////////////////////////////////////////
	// Lock vertex buffer
	Vertex* v = 0;
	m_pD3DVertexBufferTriGrid->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		(void**)&v,	// Double pointer to data
		0);			// Flags

	// Modify data
	for(DWORD i = 0; i < (DWORD)m_numVerts; ++i)
	{
		v[i] = m_triGridVerts[i];
	}
	// Unlock vertex buffer
	m_pD3DVertexBufferTriGrid->Unlock();

	//////////////////////////////////////////////////////////////////////////
	// Accessing index buffer memory
	//////////////////////////////////////////////////////////////////////////
	void* pIndices;
	// Lock index buffer
	m_pD3DIndexBuffer->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		&pIndices,	// Double pointer to data
		0);			// Flags

	// Modify data
	memcpy(pIndices, m_cubeIndices, 3 * 12 * sizeof(WORD));

	// Unlock index buffer
	m_pD3DIndexBuffer->Unlock();

	// Pyramid
	// Lock index buffer
	m_pD3DIndexBufferPyramid->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		&pIndices,	// Double pointer to data
		0);			// Flags

	// Modify data
	memcpy(pIndices, m_pyramidIndices, 18 * sizeof(WORD));

	// Unlock index buffer
	m_pD3DIndexBufferPyramid->Unlock();

	//////////////////////////////////////////////////////////////////////////
	// Tri Grid
	//////////////////////////////////////////////////////////////////////////
	// Lock index buffer
	WORD* k = 0;
	m_pD3DIndexBufferTriGrid->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		(void**)&k,	// Double pointer to data
		0);			// Flags

	// Modify data
	for(DWORD i = 0; i < (DWORD)m_numTris * 3; ++i)
	{
		k[i] = (WORD)m_triGridIndices[i];
	}
	// Unlock index buffer
	m_pD3DIndexBufferTriGrid->Unlock();

	// Enable Z-Buffer (Depth Buffer), Make sure you have:
	// The two presentation parameters set
	// The flag in the Clear function
	//m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	//////////////////////////////////////////////////////////////////////////
	// Lighting, Material, and Texture
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Light
	//////////////////////////////////////////////////////////////////////////
	ZeroMemory(&m_Light, sizeof(m_Light));

	// Ambient light color emitted from this light
	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Diffuse light color emitted from this light
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Specular light color emitted from this light
	m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Light Type (Point) Requires: Position, Range, Attenuation
	m_Light.Type = D3DLIGHT_POINT;	// Point, alternatively D3DLIGHT_DIRECTIONAL or D3DLIGHT_SPOT
	// Light Position
	m_Light.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
	// Range of Light
	m_Light.Range = 10.0f;
	// Light Attenuation
	m_Light.Attenuation0 = 0.0f;	// Constant
	m_Light.Attenuation1 = 0.05f;	// Linear
	m_Light.Attenuation2 = 0.0f;	// Quadratic

	//////////////////////////////////////////////////////////////////////////
	// Material (How light is reflected off of an object)
	//////////////////////////////////////////////////////////////////////////
	ZeroMemory(&m_Material[0], sizeof(m_Material[0]));

	m_Material[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Ambient color reflected
	m_Material[0].Diffuse = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);		// Diffuse color reflected
	m_Material[0].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Emissive color reflected
	m_Material[0].Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);		// Specular
	m_Material[0].Power = 30.0f;									// Specular highlight intensity


	ZeroMemory(&m_Material[1], sizeof(m_Material[1]));

	m_Material[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Ambient color reflected
	m_Material[1].Diffuse = D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f);		// Diffuse color reflected
	m_Material[1].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Emissive color reflected
	m_Material[1].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Specular
	m_Material[1].Power = 30.0f;									// Specular highlight intensity



	ZeroMemory(&m_Material[2], sizeof(m_Material[2]));

	m_Material[2].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Ambient color reflected
	m_Material[2].Diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);		// Diffuse color reflected
	m_Material[2].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Emissive color reflected
	m_Material[2].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Specular
	m_Material[2].Power = 30.0f;									// Specular highlight intensity

	ZeroMemory(&m_Material[3], sizeof(m_Material[3]));

	m_Material[3].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Ambient color reflected
	m_Material[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Diffuse color reflected
	m_Material[3].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Emissive color reflected
	m_Material[3].Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);		// Specular
	m_Material[3].Power = 10.0f;									// Specular highlight intensity

	//////////////////////////////////////////////////////////////////////////
	// Texture
	//////////////////////////////////////////////////////////////////////////
	// Load Texture
	D3DXCreateTextureFromFile(m_pD3DDevice, L"ground2.bmp", &m_pTexture[0]);
	D3DXCreateTextureFromFile(m_pD3DDevice, L"seafloor.bmp", &m_pTexture[1]);
	D3DXCreateTextureFromFile(m_pD3DDevice, L"floor.bmp", &m_pTexture[2]);
	D3DXCreateTextureFromFile(m_pD3DDevice, L"grass0.dds", &m_pTexture[3]);
	D3DXCreateTextureFromFile(m_pD3DDevice, L"stone2.dds", &m_pTexture[4]);
	D3DXCreateTextureFromFile(m_pD3DDevice, L"ground0.dds", &m_pTexture[5]);
	D3DXCreateTextureFromFile(m_pD3DDevice, L"blendmap.jpg", &m_pTexture[6]);
	//////////////////////////////////////////////////////////////////////////
	// Effect
	//////////////////////////////////////////////////////////////////////////
	// Load effect from file (Lab5.fx)	
#ifdef _DEBUG	// If Debug flag is set in compiler, use shader debug

	m_pEffectError = 0;
	D3DXCreateEffectFromFile(m_pD3DDevice, L"Lab5.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 
		0, &m_pEffect, &m_pEffectError);
	if(m_pEffectError)
	{
		MessageBoxA(0, (char*)m_pEffectError->GetBufferPointer(), 0, 0);
	}

	m_pEffectError = 0;
	D3DXCreateEffectFromFile(m_pD3DDevice, L"MultitextureShader.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 
		0, &m_pTerrainEffect, &m_pEffectError);

	if(m_pEffectError)
	{
		MessageBoxA(0, (char*)m_pEffectError->GetBufferPointer(), "MultitextureShader.fx - HLSL Error", 0);
	}

#else			// else do not use debug flag

	D3DXCreateEffectFromFile(m_pD3DDevice, L"Lab5.fx", 0, 0, 0, 0, &m_pEffect,
		&m_pEffectError);
	D3DXCreateEffectFromFile(m_pD3DDevice, L"MultitextureShader.fx", 0, 0, 0, 0, &m_pTerrainEffect,
		&m_pEffectError);
#endif

	// Specify technique by name, alternatively you could find the first valid tech
	m_hTech = m_pEffect->GetTechniqueByName("tech0");


	//////////////////////////////////////////////////////////////////////////
	// Set shader parameters that are the same for all objects
	//////////////////////////////////////////////////////////////////////////
	m_pEffect->SetFloatArray("eyePos", (float*)m_eyePos, 3);
	m_pEffect->SetFloatArray("lightPos", (float*)&m_Light.Position, 3);
	m_pEffect->SetFloatArray("ambientLight", (float*)&m_Light.Ambient, 3);
	m_pEffect->SetFloatArray("specularLight", (float*)&m_Light.Specular, 3);
	m_pEffect->SetFloatArray("diffuseLight", (float*)&m_Light.Diffuse, 3);
	m_pEffect->SetFloatArray("lightAttenuation", (float*)&D3DXVECTOR3(m_Light.Attenuation0, m_Light.Attenuation1, m_Light.Attenuation2), 3);

	//////////////////////////////////////////////////////////////////////////
	// Assignment Effect (Multi-textured 'terrain')
	//////////////////////////////////////////////////////////////////////////
	m_hTerrainTech = m_pTerrainEffect->GetTechniqueByName("techMT");

	//////////////////////////////////////////////////////////////////////////
	// Set shader parameters that are the same for all objects
	//////////////////////////////////////////////////////////////////////////
	m_pTerrainEffect->SetFloatArray("eyePos", (float*)m_eyePos, 3);
	m_pTerrainEffect->SetFloatArray("ambientLight", (float*)&m_Light.Ambient, 3);
	m_pTerrainEffect->SetFloatArray("specularLight", (float*)&m_Light.Specular, 3);
	m_pTerrainEffect->SetFloatArray("diffuseLight", (float*)&m_Light.Diffuse, 3);
	m_pTerrainEffect->SetFloatArray("lightAttenuation", (float*)&D3DXVECTOR3(m_Light.Attenuation0, m_Light.Attenuation1, m_Light.Attenuation2), 3);

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////

	// Load a font for private use for this process
	AddFontResourceEx(L"Delicious-Roman.otf", FR_PRIVATE, 0);

	// Load D3DXFont, each font style you want to support will need an ID3DXFont
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Delicious-Roman"), &m_pD3DFont);

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

}

void CDirectXFramework::Update(float dt)
{
	//////////////////////////////////////////////////////////////////////////
	// Update light position (Animated light)
	//////////////////////////////////////////////////////////////////////////
	float t = timeGetTime() / 800.0f;
	float r = 35.0f;
	m_Light.Position.x = sinf(t) * r;
	m_Light.Position.z = cosf(t) * r;
	m_Light.Position.y = 30.0f;

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

	// If either CTRL key is held on the keyboard, activate transformation
	// input gathering for the pyramid.

	// Keyboard
	if(buffer[DIK_LCONTROL] & 0x80 || buffer[DIK_RCONTROL] &0x80)
	{
		// Mouse

		// Left Mouse Button - Translation
		if(mouseState.rgbButtons[0] & 0x80)
		{
			m_pyramidTranslation.x += mouseState.lX * 0.01f; // X Axis
			m_pyramidTranslation.y -= mouseState.lY * 0.01f; // Y Axis
			m_pyramidTranslation.z += mouseState.lZ * 0.01f; // Mouse Wheel

		}

		// Right Mouse Button - Rotation
		if(mouseState.rgbButtons[1] & 0x80)
		{
			m_pyramidRotation.y -= mouseState.lX * 0.01f; // X Axis
			m_pyramidRotation.x -= mouseState.lY * 0.01f; // Y Axis
			m_pyramidRotation.z += mouseState.lZ * 0.1f; // Mouse Wheel
		}

		// Middle Mouse Button - Scaling
		if(mouseState.rgbButtons[2] & 0x80)
		{
			m_pyramidScaling += mouseState.lX * 0.01f; // X Axis
			m_pyramidScaling += mouseState.lY * 0.01f; // Y Axis
			m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		}
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
	// Draw 3D Objects
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Matrix Transformations to control each objects position, scale, and 
	// rotation.  Set these matrices for each object you want to render.
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIX transMat, rotMat, scaleMat, worldMat, WIT;
	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	m_pD3DDevice->SetStreamSource(0, m_pD3DVertexBuffer, 0, sizeof(Vertex));

	m_pD3DDevice->SetIndices(m_pD3DIndexBuffer);

	m_pD3DDevice->SetVertexDeclaration(m_pD3DVertexDecl);
	//////////////////////////////////////////////////////////////////////////
	// Effect
	//////////////////////////////////////////////////////////////////////////
	// All drawn primitives are sharing the same shader, just pass in different
	// app variables to the shader and call CommitChanges() prior to Draw calls

	// Set the technique to use
	m_pEffect->SetTechnique(m_hTech);

	// Begin Effect
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		//////////////////////////////////////////////////////////////////////////
		// Cube 1
		//////////////////////////////////////////////////////////////////////////
		// Calculate Matrix Transform
		D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);			// Scaling
		D3DXMatrixRotationYawPitchRoll(&rotMat, timeGetTime() * 0.001f, 0.0f, 0.0f); // Rotation on Yaw, Pitch, and Roll
		D3DXMatrixTranslation(&transMat, -4.0f, 0.0f, 0.0f);		// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world
		
		// Set object specific effect parameters
		D3DXMATRIX WIT;
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		m_pEffect->SetFloatArray("lightPos", (float*)&m_Light.Position, 3);
		m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * m_viewMat * m_projMat));
		m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
		m_pEffect->SetMatrix("worldMat", &worldMat);
		m_pEffect->SetValue("ambientMaterial", &m_Material[0].Ambient, sizeof(D3DXCOLOR));
		m_pEffect->SetValue("diffuseMaterial", &m_Material[0].Diffuse, sizeof(D3DXCOLOR));
		m_pEffect->SetValue("specularMaterial", &m_Material[0].Specular, sizeof(D3DXCOLOR));
		m_pEffect->SetFloat("specularPower", m_Material[0].Power);
		m_pEffect->SetTexture("tex", m_pTexture[0]);


		// After setting object specific parameters, commit changes
		m_pEffect->CommitChanges();

		// Draw cube
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4 * 6, 0, 12);

		//////////////////////////////////////////////////////////////////////////
		// Cube 2
		//////////////////////////////////////////////////////////////////////////
		// Calculate Matrix Transform
		D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);			// Scaling
		D3DXMatrixRotationYawPitchRoll(&rotMat, timeGetTime() * -0.001f, 0.0f, 0.0f); // Rotation on Yaw, Pitch, and Roll
		D3DXMatrixTranslation(&transMat, 4.0f, 0.0f, 0.0f);		// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world
		
		// Set object specific effect parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		m_pEffect->SetFloatArray("lightPos", (float*)&m_Light.Position, 3);
		m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * m_viewMat * m_projMat));
		m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
		m_pEffect->SetMatrix("worldMat", &worldMat);
		m_pEffect->SetValue("ambientMaterial", &m_Material[1].Ambient, sizeof(D3DXCOLOR));
		m_pEffect->SetValue("diffuseMaterial", &m_Material[1].Diffuse, sizeof(D3DXCOLOR));
		m_pEffect->SetValue("specularMaterial", &m_Material[1].Specular, sizeof(D3DXCOLOR));
		m_pEffect->SetFloat("specularPower", m_Material[1].Power);
		m_pEffect->SetTexture("tex", m_pTexture[1]);

		// After setting object specific parameters, commit changes
		m_pEffect->CommitChanges();

		// Draw cube
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4 * 6, 0, 12);

		//////////////////////////////////////////////////////////////////////////
		// Pyramid
		//////////////////////////////////////////////////////////////////////////
		m_pD3DDevice->SetStreamSource(0, m_pD3DVertexBufferPyramid, 0, sizeof(Vertex));

		m_pD3DDevice->SetIndices(m_pD3DIndexBufferPyramid);

		m_pD3DDevice->SetVertexDeclaration(m_pD3DVertexDecl);

		// Calculate Matrix Transform
		D3DXMatrixScaling(&scaleMat, m_pyramidScaling, m_pyramidScaling, m_pyramidScaling);			// Scaling
		D3DXMatrixRotationYawPitchRoll(&rotMat, m_pyramidRotation.y, m_pyramidRotation.x, m_pyramidRotation.z); // Rotation on Yaw, Pitch, and Roll
		D3DXMatrixTranslation(&transMat, m_pyramidTranslation.x, m_pyramidTranslation.y, m_pyramidTranslation.z);		// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world

		// Set object specific parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		m_pEffect->SetFloatArray("lightPos", (float*)&m_Light.Position, 3);
		m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * m_viewMat * m_projMat));
		m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
		m_pEffect->SetMatrix("worldMat", &worldMat);
		m_pEffect->SetValue("ambientMaterial", &m_Material[2].Ambient, sizeof(D3DXCOLOR));
		m_pEffect->SetValue("diffuseMaterial", &m_Material[2].Diffuse, sizeof(D3DXCOLOR));
		m_pEffect->SetValue("specularMaterial", &m_Material[2].Specular, sizeof(D3DXCOLOR));
		m_pEffect->SetFloat("specularPower", m_Material[2].Power);
		m_pEffect->SetTexture("tex", m_pTexture[2]);

		// After setting object specific parameters, commit changes
		m_pEffect->CommitChanges();

		// Draw cube
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 16, 0, 6);

		m_pEffect->EndPass();
	}

	m_pEffect->End();

	//////////////////////////////////////////////////////////////////////////
	// Second Effect (Multi-texture)
	//////////////////////////////////////////////////////////////////////////
	// Set the technique to use
	m_pTerrainEffect->SetTechnique(m_hTerrainTech);

	// Begin Effect
	numPasses = 0;
	m_pTerrainEffect->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; ++i)
	{
		m_pTerrainEffect->BeginPass(i);

		//////////////////////////////////////////////////////////////////////////
		// Tri Grid
		//////////////////////////////////////////////////////////////////////////
		m_pD3DDevice->SetStreamSource(0, m_pD3DVertexBufferTriGrid, 0, sizeof(Vertex));

		m_pD3DDevice->SetIndices(m_pD3DIndexBufferTriGrid);

		m_pD3DDevice->SetVertexDeclaration(m_pD3DVertexDecl);

		// Calculate Matrix Transform
		D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);			// Scaling
		D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f); // Rotation on Yaw, Pitch, and Roll
		D3DXMatrixTranslation(&transMat, 0.0f, -1.0f, 0.0f);		// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world

		// Set object specific parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		m_pTerrainEffect->SetFloatArray("lightPos", (float*)&m_Light.Position, 3);
		m_pTerrainEffect->SetMatrix("worldViewProjMat", &(worldMat * m_viewMat * m_projMat));
		m_pTerrainEffect->SetMatrix("worldInverseTransposeMat", &WIT);
		m_pTerrainEffect->SetMatrix("worldMat", &worldMat);
		m_pTerrainEffect->SetValue("ambientMaterial", &m_Material[3].Ambient, sizeof(D3DXCOLOR));
		m_pTerrainEffect->SetValue("diffuseMaterial", &m_Material[3].Diffuse, sizeof(D3DXCOLOR));
		m_pTerrainEffect->SetValue("specularMaterial", &m_Material[3].Specular, sizeof(D3DXCOLOR));
		m_pTerrainEffect->SetFloat("specularPower", m_Material[3].Power);
		m_pTerrainEffect->SetTexture("tex0", m_pTexture[3]);
		m_pTerrainEffect->SetTexture("tex1", m_pTexture[4]);
		m_pTerrainEffect->SetTexture("tex2", m_pTexture[5]);
		m_pTerrainEffect->SetTexture("blendMap", m_pTexture[6]);
		m_pTerrainEffect->SetFloat("textureTileScale", 16.0f);



		// After setting object specific parameters, commit changes
		m_pTerrainEffect->CommitChanges();

		// Draw tri grid
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_numVerts, 0, m_numTris);



		m_pTerrainEffect->EndPass();
	}

	m_pTerrainEffect->End();

	//////////////////////////////////////////////////////////////////////////
	// Draw 2D sprites
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// TODO: Draw Text
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
	HR(m_pD3DDevice->EndScene());
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
	SAFE_RELEASE(m_pTerrainEffect);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pEffectError);

	for(int i = 0; i < 3; ++i)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}

	SAFE_RELEASE(m_pD3DIndexBufferPyramid);
	SAFE_RELEASE(m_pD3DVertexBufferPyramid);
	SAFE_RELEASE(m_pD3DIndexBuffer);
	SAFE_RELEASE(m_pD3DVertexBuffer);
	SAFE_RELEASE(m_pD3DVertexDecl);

	SAFE_RELEASE(m_pDIKeyboard);
	SAFE_RELEASE(m_pDIMouse);
	SAFE_RELEASE(m_pDIObject);

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
}

void CDirectXFramework::onResetDevice()
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: COM objects to restore after a lost device is reacquired
	//////////////////////////////////////////////////////////////////////////
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

void CDirectXFramework::GenTriGrid(int numVertRows, int numVertCols, float dx, float dz, const D3DXVECTOR3& center, 
								   std::vector<Vertex>& verts, std::vector<DWORD>& indices)
{
	int numVerts = numVertRows * numVertCols;
	int numCellRows = numVertRows - 1;
	int numCellCols = numVertCols - 1;

	int numTris = numCellRows * numCellCols * 2;

	float width = (float)numCellCols * dx;
	float depth = (float)numCellRows * dz;

	//////////////////////////////////////////////////////////////////////////
	// Build Verts
	//////////////////////////////////////////////////////////////////////////
	verts.resize(numVerts);

	// Offsets to translate grid from quadrant 4 to the center of coordinate system
	float xOffset = -width * 0.5f;
	float zOffset = depth * 0.5f;

	int k = 0;

	for(float i = 0; i < numVertRows; ++i)
	{
		for(float j = 0; j < numVertCols; ++j)
		{
			DWORD index = (DWORD)i * 100 + j;
			// Negate the depth coordinate to put in quadrant 4.  Offset to center about coord system.
			verts[k].position.x = j * dx + xOffset;
			verts[k].position.z = -i * dz + zOffset;
			verts[k].position.y = 0;

			verts[k].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			verts[k].uv = D3DXVECTOR2((verts[index].position.x + (0.5f * width)) / width, (verts[index].position.z + (0.5f * depth)) / -depth);

			// Translate so that the center of the grid is at the specified 'center' parameter.
			D3DXMATRIX T;
			D3DXMatrixTranslation(&T, center.x, center.y, center.z);
			D3DXVec3TransformCoord(&verts[k].position, &verts[k].position, &T);

			++k;	// Next Vertex
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Build Indices
	//////////////////////////////////////////////////////////////////////////
	indices.resize(numTris * 3);

	// Generate indices for each quad.
	k = 0;
	for(DWORD i = 0; i < (DWORD)numCellRows; ++i)
	{
		for(DWORD j = 0; j < (DWORD)numCellCols; ++j)
		{
			indices[k] = i * numVertCols + j;
			indices[k + 1] = i * numVertCols + j + 1;
			indices[k + 2] = (i + 1) * numVertCols + j;

			indices[k + 3] = (i + 1) * numVertCols + j;
			indices[k + 4] = i * numVertCols + j + 1;
			indices[k + 5] = (i + 1) * numVertCols + j + 1;

			// Next Quad
			k += 6;
		}
	}
}