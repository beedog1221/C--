// Global variables - Passed in from application

float4x4 worldViewProjMat;			// Transforms positions to view space
float4x4 worldInverseTransposeMat;	// Transforms normals to world space
float4x4 worldMat;					// World matrix
float3 eyePos;						// Eye position (camera position)

// Animation Data
float4x4 finalXForms[35];			// Matrix Palette

// Light
float3 lightPos;					// Light Position
float3 ambientLight;				// Ambient Light Color
float3 specularLight;				// Specular Light Color
float3 diffuseLight;				// Diffuse Light Color
float3 lightAttenuation;			// Light Attenuation (Stored as array of 3 floats for Atten0, Atten1, Atten2)

// Material
float4 ambientMaterial;				// Ambient Material Color
float4 diffuseMaterial;				// Diffuse Material Color (ambient material, scaled down version of diffuse)
float4 specularMaterial;			// Specular Material Color
float  specularPower;				// Specular Power

// Texture
bool isTex = true;
texture tex;						// Image texture to be applied to object
sampler texSamp = sampler_state		// Texture sampler state
{
	Texture = <tex>;
	// Trilinear Filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_OUT
{
	float4 posH : POSITION0;		// Vertex position in homgenous clip space
	float3 normW : TEXCOORD0;		// Vertex normal in world space
	float3 posW : TEXCOORD1;		// Vertex position in world space
	float2 tex0 : TEXCOORD2;		// Vertex uv texture coordinate 0
};

// Vertex Shader
VS_OUT VS(float3 pos : POSITION0, float3 norm : NORMAL0, float2 tex0 : TEXCOORD0, 
	float weight0 : BLENDWEIGHT0, int4 boneIndex : BLENDINDICES0)
{
	VS_OUT vsOut = (VS_OUT) 0;
	
	// Animation vertex processing
	float weight1 = 1.0f - weight0;
	
	float4 p = weight0 * mul(float4(pos, 1.0f), finalXForms[boneIndex[0]]);
	p		+= weight1 * mul(float4(pos, 1.0f), finalXForms[boneIndex[1]]);
	p.w = 1.0f;
	
	// We do not normalize normals, done in pixel shader now.
	float4 n = weight0 * mul(float4(norm, 0.0f), finalXForms[boneIndex[0]]);
	n		+= weight1 * mul(float4(norm, 0.0f), finalXForms[boneIndex[1]]);
	n.w = 0.0f;
	
	// Transform vertex normal to world space (norm * WITMat)
	vsOut.normW = mul(n, worldInverseTransposeMat).xyz;

	// Transform vertex position to world space
	vsOut.posW = mul(p, worldMat).xyz;	
	
	// Transform vertex position to homogenous clip space (pos * WVPMat)
	vsOut.posH = mul(p, worldViewProjMat);
	
	// Normalize the normals to unit length
	//vsOut.normW = normalize(vsOut.normW);
	
	// Pass on texture coordinate to pixel shader
	vsOut.tex0 = tex0;

	return vsOut;	
}

// Pixel Shader
float4 PS(VS_OUT vsOut): COLOR
{
	// Get texel from texture map taht gets mapped to this pixel
	float4 texColor = tex2D(texSamp, vsOut.tex0);

	// Interpolated normals can "unnormalize", so normalize
	vsOut.normW = normalize(vsOut.normW);
	
	// Compute vector from the vertex to the eye position (V)
	float3 toEye = normalize(eyePos - vsOut.posW);
	
	// Light Direction Vector (L)
	float3 lightDir = normalize(lightPos - vsOut.posW);
	//float3 lightDir = normalize(vsOut.posW - lightPos);
	
	
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
	float3 H = normalize(-lightDir + toEye);
	float T = pow(max(dot(H, vsOut.normW), 0.0f), specularPower);
	
	// Determine the diffuse light intensity that strikes the vertex
	float S = max(dot(lightDir, vsOut.normW), 0.0f);
	
	// Compute Ambient, Diffuse, Specular terms seperately
	float3 spec = T * (specularMaterial * specularLight).rgb;
	float3 diffuse;
	//if(isTex)
	//	diffuse = S * ((diffuseMaterial * texColor) * diffuseLight).rgb;
	//else
		diffuse = S * (diffuseMaterial * diffuseLight).rgb;
	
	float3 ambient = (ambientMaterial * ambientLight);
	
	// Attenuation
	float d = distance(lightPos, vsOut.posW);
	float A = lightAttenuation.x + lightAttenuation.y * d + lightAttenuation.z * d * d;
		
	// Sum all of the terms together and copy diffuse alpha
	return float4(((ambient + diffuse / A) * texColor.rgb + spec / A) , diffuseMaterial.a * texColor.a);
}

// Technique and Passes
technique tech0
{
	pass p0
	{
		// Specify vertex and pixel shader associated with this pass
		vertexShader = compile vs_3_0 VS();	// Vertex Shader 3.0, compiles VS()
		pixelShader = compile ps_3_0 PS();	// Pixel Shader 3.0, compiles PS()
		
		// Render states associated with this pass
	}
}