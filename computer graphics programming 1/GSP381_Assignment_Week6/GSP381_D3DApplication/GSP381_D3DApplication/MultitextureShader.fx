// Global variables - Passed in from application

float4x4 worldViewProjMat;			// Transforms positions to view space
float4x4 worldInverseTransposeMat;	// Transforms normals to world space
float4x4 worldMat;					// World matrix
float3 eyePos;						// Eye position (camera position)

// Light
float3 lightPos;					// Light Position
float3 ambientLight;				// Ambient Light Color
float3 specularLight;				// Specular Light Color
float3 diffuseLight;				// Diffuse Light Color
float3 lightAttenuation;			// Light Attenuation (Stored as array of 3 floats for Atten0, Atten1, Atten2)
float  lightRange = 5.0f;

// Material
float4 ambientMaterial;				// Ambient Material Color
float4 diffuseMaterial;				// Diffuse Material Color (ambient material, scaled down version of diffuse)
float4 specularMaterial;			// Specular Material Color
float  specularPower;				// Specular Power

// Textures
float textureTileScale;				// Texture tile scaling
texture tex0;						// Image texture to be applied to object
sampler texSamp0 = sampler_state	// Texture sampler state
{
	Texture = <tex0>;
	// Trilinear Filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
	AddressV  = WRAP;
	
};

texture tex1;						// Image texture to be applied to object
sampler texSamp1 = sampler_state	// Texture sampler state
{
	Texture = <tex1>;
	// Trilinear Filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

texture tex2;						// Image texture to be applied to object
sampler texSamp2 = sampler_state	// Texture sampler state
{
	Texture = <tex2>;
	// Trilinear Filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

texture blendMap;						// Image texture to be applied to object
sampler texSampBlend = sampler_state	// Texture sampler state
{
	Texture = <blendMap>;
	// Trilinear Filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

struct VS_OUT
{
	float4 posH : POSITION0;		// Vertex position in homgenous clip space
	float3 normW : TEXCOORD0;		// Vertex normal in world space
	float3 posW : TEXCOORD1;		// Vertex position in world space
	float2 tiledTexC : TEXCOORD2;	// Vertex UV texture coords tiled
	float2 nonTiledTexC : TEXCOORD3; // Vertex UV texture coords non-tiled
};


// Vertex Shader
VS_OUT VS(float3 pos : POSITION0, float3 norm : NORMAL0, float2 texC : TEXCOORD0)
{
	VS_OUT vsOut = (VS_OUT) 0;

	// Transform vertex position to homogenous clip space (pos * WVPMat)
	vsOut.posH = mul(float4(pos, 1.0f), worldViewProjMat);
	
	// Transform vertex position to world space
	vsOut.posW = mul(float4(pos, 1.0f), worldMat).xyz;
	
	// Transform vertex normal to world space (norm * WITMat)
	vsOut.normW = mul(float4(norm, 1.0f), worldInverseTransposeMat).xyz;
	
	// Normalize the normals to unit length
	vsOut.normW = normalize(vsOut.normW);
	
	// Pass on texture coordinate to pixel shader
	vsOut.tiledTexC = texC * textureTileScale;
	vsOut.nonTiledTexC = texC;

	return vsOut;	
}

// Pixel Shader
float4 PS(VS_OUT vsOut) : COLOR
{	
	// Layer maps are tiled
	float3 color0 = tex2D(texSamp0, vsOut.tiledTexC).rgb;
	float3 color1 = tex2D(texSamp1, vsOut.tiledTexC).rgb;
	float3 color2 = tex2D(texSamp2, vsOut.tiledTexC).rgb;
	
	// Blend map is not tiled
	float3 blend = tex2D(texSampBlend, vsOut.nonTiledTexC).rgb;
	
	// Find inverse of all the blend weights so that we
	// can scale the total color to the range of [0, 1]
	float totalInverse = 1.0f / (blend.r + blend.g + blend.b);
	
	// Scale the colors by each layer by its corresponding
	// weight stored in the blend map.
	color0 *= blend.r * totalInverse;
	color1 *= blend.g * totalInverse;
	color2 *= blend.b * totalInverse;
	
	// Sum all of the color textures together
	float3 texColor = color0 + color1 + color2;
	
	// Interpolated normals can "unnormalize", so normalize
	vsOut.normW = normalize(vsOut.normW);
	
	// Compute vector from the vertex to the eye position (V)
	float3 toEye = normalize(eyePos - vsOut.posW);
	
	// Light Direction Vector (L)
	float3 lightDir = normalize(lightPos - vsOut.posW);
	
	// Determine how much specular (if any), makes it to the eye
	
	//////////////////////////////////////////////////////////////////////////
	// Phong specular calculation
	//////////////////////////////////////////////////////////////////////////
	// Compute the reflection vector
	//float3 R = reflect(-lightDir, vsOut.normW);
	//float T = pow(max(dot(R, toEye), 0.0f), specularPower);
	
	//////////////////////////////////////////////////////////////////////////
	// Blinn-Phong specular calculation
	//////////////////////////////////////////////////////////////////////////
	// Half vector used in Blinn-Phong shading (faster than Phong)
	float3 H = normalize(lightDir + normalize(toEye));
	float T = pow(max(dot(H, vsOut.normW), 0.0f), specularPower);
	
	// Determine the diffuse light intensit that strikes the vertex
	float S = max(dot(lightDir, vsOut.normW), 0.0f);
	
	// Compute Ambient, Diffuse, Specular terms seperately
	float3 spec = T * (specularMaterial * specularLight).rgb;
	float3 diffuse = S * (diffuseMaterial * texColor * diffuseLight).rgb;
	float3 ambient = (ambientMaterial * ambientLight * texColor);
	
	// Attenuation
	//float d = distance(lightPos, vsOut.posW);
	//float A = lightAttenuation.x + lightAttenuation.y * d + lightAttenuation.z * d * d;
	
	// Sum all of the terms together and copy diffuse alpha
	//return float4( ambient + (diffuse + spec) / A, diffuseMaterial.a);

	float3 attenuation = saturate(1 - dot(lightDir / lightRange, lightDir / lightRange));
	// Sum all of the terms together and copy diffuse alpha
	return float4( ambient + (diffuse + spec) * attenuation, diffuseMaterial.a);
	
}

// Technique and Passes
technique techMT
{
	pass p0
	{
		// Specify vertex and pixel shader associated with this pass
		vertexShader = compile vs_3_0 VS();	// Vertex Shader 3.0, compiles VS()
		pixelShader = compile ps_3_0 PS();	// Pixel Shader 3.0, compiles PS()
		
		// Render states associated with this pass

		// Uncomment these states below, to use diffuse material's 
		// alpha component to render translucent geometry.
		//FillMode = Wireframe;
		// AlphaBlendEnable = true;
		// SrcBlend = SrcAlpha;
		// DestBlend = InvSrcAlpha;
		// BlendOp = Add;
	}
}