//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.h
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
#pragma once
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <vector>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxerr.lib")


//////////////////////////////////////////////////////////////////////////
// To DEBUG D3D correctly:
//	Go to the Start Menu, Find Microsoft DirectX SDK (June 2010).
//  Go to the DirectX Control Panel.
//  In Direct3D 9 settings, Increase the Debug Output Level to the Max
//  Turn on debugging flags for Maximum validation, Break on Memory Leaks, etc.
//////////////////////////////////////////////////////////////////////////

// If in DEBUG mode, turn on D3D_DEBUG_INFO flag enable DXTrace, and d3dx9d.lib (debug version)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
		#define D3D_DEBUG_INFO
	#endif

	#pragma comment(lib, "d3dx9d.lib")
#else
	#pragma comment(lib, "d3dx9.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// DirectInput headers and libraries
//////////////////////////////////////////////////////////////////////////
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

// HRESULT macro that prints an error window
#ifndef HR
#define HR(x)												\
		{                                                   \
		HRESULT hr = x;									    \
		if(FAILED(hr))									    \
			{                                               \
			DXTraceA(__FILE__, __LINE__, hr, #x, TRUE);	    \
			}                                               \
		}                                                   
#endif

//////////////////////////////////////////////////////////////////////////
// For skinned animations
//////////////////////////////////////////////////////////////////////////
struct FrameEx : public D3DXFRAME
{
	D3DXMATRIX	toRoot;
};

class AllocMeshHierarchy : public ID3DXAllocateHierarchy
{
public:
	HRESULT STDMETHODCALLTYPE CreateFrame(
		THIS_ PCSTR Name,
		D3DXFRAME** ppNewFrame);
	HRESULT STDMETHODCALLTYPE CreateMeshContainer(
		PCSTR Name,
		const D3DXMESHDATA* pMeshData,
		const D3DXMATERIAL* pMaterials,
		const D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		const DWORD* pAdjacency,
		ID3DXSkinInfo* pSkinInfo,
		D3DXMESHCONTAINER** ppNewMeshContainer);
	HRESULT STDMETHODCALLTYPE DestroyFrame(
		THIS_ D3DXFRAME* pFrameToFree);
	HRESULT STDMETHODCALLTYPE DestroyMeshContainer(
		THIS_ D3DXMESHCONTAINER* pMeshContainerBase);
};

class CDirectXFramework
{
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing
	int					m_width;		// Window width
	int					m_height;		// Window height
	float				m_fRotation;	// Rotation variable for Cube 2
	RECT				screenRect;		// RECT for the entire size of window

	float				m_currTime;		// Time to render current frame
	float				m_prevTime;		// Time to render previous frame
	int					m_FPS;			// Frames per second
	float				m_elapsedTime;	// ms per frame
	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*			  m_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*	  m_pD3DDevice;	// Direct3D 9 Device
	D3DPRESENT_PARAMETERS m_D3Dpp;		// Device Presentation Parameters
	D3DCAPS9			  m_D3DCaps;	// Device Capabilities
	D3DXCOLOR			  m_clearColor; // Back-buffer Clear Color

	//////////////////////////////////////////////////////////////////////////
	// View and Projection Matrices
	//////////////////////////////////////////////////////////////////////////
	// Set these 3 component to initialize the view matrix (camera)
	D3DXVECTOR3 m_eyePos;
	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_upVec;

	// View matrix
	D3DXMATRIX m_viewMat;

	// Projection matrix
	D3DXMATRIX m_projMat;

	//////////////////////////////////////////////////////////////////////////
	// Vertex Declaration and Vertex and Index Buffers
	//////////////////////////////////////////////////////////////////////////
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

	// Cube data (Vertex and Index array)
	Vertex m_cubeVerts[24];	// Could use 8, but cannot texture or define valid normals
	WORD m_cubeIndices[36];

	// Pyramid data
	Vertex m_pyramidVerts[16];	
	WORD m_pyramidIndices[18];

	IDirect3DVertexDeclaration9*	m_pD3DVertexDecl;

	// Vertex Buffer
	IDirect3DVertexBuffer9*			m_pD3DVertexBuffer;
	// Index Buffer
	IDirect3DIndexBuffer9*			m_pD3DIndexBuffer;

	//////////////////////////////////////////////////////////////////////////
	// Pyramid
	//////////////////////////////////////////////////////////////////////////
	// Vertex Buffer
	IDirect3DVertexBuffer9*			m_pD3DVertexBufferPyramid;
	// Index Buffer
	IDirect3DIndexBuffer9*			m_pD3DIndexBufferPyramid;
	D3DXVECTOR3						m_pyramidTranslation;
	D3DXVECTOR3						m_pyramidRotation;
	float							m_pyramidScaling;

	//////////////////////////////////////////////////////////////////////////
	// Tri Grid
	//////////////////////////////////////////////////////////////////////////
	IDirect3DVertexBuffer9*			m_pD3DVertexBufferTriGrid;
	IDirect3DIndexBuffer9*			m_pD3DIndexBufferTriGrid;
	std::vector<Vertex>				m_triGridVerts;
	std::vector<DWORD>				m_triGridIndices;
	int 							m_numVerts;
	int 							m_numTris;

	//////////////////////////////////////////////////////////////////////////
	// Lighting, Material, Textures
	//////////////////////////////////////////////////////////////////////////
	D3DLIGHT9			m_Light;
	D3DMATERIAL9		m_Material[3];
	IDirect3DTexture9*	m_pTexture[4];

	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*			m_pD3DFont;		// Font Object

	//////////////////////////////////////////////////////////////////////////
	// DirectInput
	//////////////////////////////////////////////////////////////////////////
	IDirectInput8*		 m_pDIObject;	// DirectInput Object
	IDirectInputDevice8* m_pDIKeyboard;	// Keyboard Device
	IDirectInputDevice8* m_pDIMouse;	// Mouse Device 

	//////////////////////////////////////////////////////////////////////////
	// Effects
	//////////////////////////////////////////////////////////////////////////
	ID3DXEffect*		m_pEffect;
	ID3DXBuffer*		m_pEffectError;
	D3DXHANDLE			m_hTech;

	// Multianimation
	ID3DXEffect*		m_pEffect2;
	ID3DXBuffer*		m_pEffectError2;
	D3DXHANDLE			m_hTech2;

	//////////////////////////////////////////////////////////////////////////
	// D3DXMesh - Used for D3DXCreate functions and .x files
	//////////////////////////////////////////////////////////////////////////
	// D3DXCreate_() Meshes
	ID3DXMesh*			m_pBoxMesh;
	ID3DXMesh*			m_pSphereMesh;
	ID3DXMesh*			m_pCylinderMesh;
	ID3DXMesh*			m_pTorusMesh;
	ID3DXMesh*			m_pTeapotMesh;
	ID3DXMesh*			m_pPolyMesh;

	// Adjacency Buffer
	ID3DXBuffer*		m_pAdjBuffer;	

	// .x File Meshes
	ID3DXMesh*			m_pStaticXMesh;
	ID3DXMesh*			m_pAnimatedXMesh;
	ID3DXMesh*			m_pSkinnedXMesh;

	ID3DXBuffer*		m_pXMaterials;
	ID3DXBuffer*		m_pXMaterials2;
	ID3DXBuffer*		m_pXMaterials3;

	ID3DXBuffer*		m_pXEffects;
	ID3DXBuffer*		m_pXEffects2;
	ID3DXBuffer*		m_pXEffects3;

	DWORD				m_numMaterials;
	DWORD				m_numMaterials2;
	DWORD				m_numMaterials3;

	D3DMATERIAL9*		meshMaterials;	
	LPDIRECT3DTEXTURE9*	meshTextures;	

	D3DMATERIAL9*		meshMaterials2;	
	LPDIRECT3DTEXTURE9*	meshTextures2;	

	// skinned animation (tiny.x)
	D3DXMATERIAL*		meshMaterials3;	
	LPDIRECT3DTEXTURE9*	meshTextures3;

	//////////////////////////////////////////////////////////////////////////
	// Mesh Hierarchy / Animation Variables
	//////////////////////////////////////////////////////////////////////////
	struct BoneFrame
	{
		D3DXVECTOR3 pos;	// Relative to parent frame
		D3DXVECTOR3	angle;	// Relative to parent frame

		D3DXMATRIX	toParentXForm;
		D3DXMATRIX	toWorldXForm;
	};

	// Array of bones in this simple arm animation
	static const int NUM_BONES = 5;
	BoneFrame m_Bones[NUM_BONES];
	int m_BoneSelected;

	//////////////////////////////////////////////////////////////////////////
	// Skinned Animation
	//////////////////////////////////////////////////////////////////////////


	D3DXFRAME*					m_root;
	DWORD						m_maxVertInfluences;
	DWORD						m_numBones;
	ID3DXSkinInfo*				m_skinInfo;
	ID3DXAnimationController*	m_AnimController;

	std::vector<D3DXMATRIX>		m_finalXForms;
	std::vector<D3DXMATRIX*>	m_toRootXFormPtrs;

	static const int			MAX_NUM_BONES_SUPPORTED = 35;

public:
	//////////////////////////////////////////////////////////////////////////
	// Init and Shutdown are preferred to constructors and destructor,
	// due to having more control when to explicitly call them when global.
	//////////////////////////////////////////////////////////////////////////
	CDirectXFramework(void);
	~CDirectXFramework(void);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Init
	// Parameters:	HWND hWnd - Handle to the window for the application
	//				HINSTANCE hInst - Handle to the application instance
	//				bool bWindowed - Boolean to control windowed or full-screen
	// Return:		void
	// Description:	Ran once at the start.  Initialize DirectX components and 
	//				variables to control the application.  
	//////////////////////////////////////////////////////////////////////////
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Update
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					update call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Used 
	//				for updating variables and processing input commands prior
	//				to calling render.
	//////////////////////////////////////////////////////////////////////////
	void Update(float dt);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Render
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					render call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Render
	//				calls all draw call to render objects to the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render(float dt);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Shutdown
	// Parameters:	void
	// Return:		void
	// Description:	Runs once at the end of an application.  Destroy COM 
	//				objects and deallocate dynamic memory.
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// Name:		isDeviceLost
	// Parameters:	void
	// Return:		bool
	// Description:	If the device is lost due to various reasons (Alt+Tab in
	//				full-screen mode, etc.) this function will return true,
	//				so that it can be restored.
	//////////////////////////////////////////////////////////////////////////
	bool isDeviceLost();

	//////////////////////////////////////////////////////////////////////////
	// Name:		onLostDevice
	// Parameters:	void
	// Return:		bool
	// Description:	If a device has been lost, clean up COM objects that must
	//				be reallocated during the onResetDevice()
	//////////////////////////////////////////////////////////////////////////
	void onLostDevice();

	//////////////////////////////////////////////////////////////////////////
	// Name:		onResetDevice
	// Parameters:	void
	// Return:		bool
	// Description:	If a device has been restored, reinitialize COM objects
	//				that were lost.
	//////////////////////////////////////////////////////////////////////////
	void onResetDevice();

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	int getResolutionWidth() {return m_width;}
	int getResolutionHeight() {return m_height;}

	//////////////////////////////////////////////////////////////////////////
	// Modifiers
	//////////////////////////////////////////////////////////////////////////
	void setResolution(int w, int h) 
	{
		m_width = w; 
		m_height = h;
		if(m_pD3DDevice)
		{
			m_D3Dpp.BackBufferWidth = m_width;
			onLostDevice(); 
			HR(m_pD3DDevice->Reset(&m_D3Dpp));
			onResetDevice();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Skinned Animation Functions
	//////////////////////////////////////////////////////////////////////////
	void BuildToRootXForms(FrameEx* frame, D3DXMATRIX& parentsToRoot);
	const D3DXMATRIX* GetFinalXFormArray();
	void UpdateSkinned(float dt);
	void DrawSkinned();
	D3DXFRAME* FindNodeWithMesh(D3DXFRAME* frame);
	bool HasNormals(ID3DXMesh* mesh);
	void BuildSkinnedMesh(ID3DXMesh* mesh);
	void BuildToRootXFormPtrArray();
};
