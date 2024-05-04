// Vertex shader
#version 420

uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;			
uniform mat4 matModel_IT;		// Inverse transpose of the model matrix
uniform mat4 BoneMatrices[150];

in vec4 vCol;		
in vec4 vPos;		
in vec4 vNormal;	
in vec2 vTextureCoords;
in vec4 vBoneIds;
in vec4 vBoneWeights;

out vec4 colour;
out vec4 vertexWorldPos;	
out vec4 vertexWorldNormal;
out vec2 textureCoords;
out vec4 boneId;
out vec4 boneWeight;

uniform bool bUseHeightMap;
uniform bool bDiscardColoredAreasInHeightMap;
uniform bool bUseBones;

uniform sampler2D heightMapSampler;
uniform float heightScale;
uniform vec2 UVOffset;

void main()
{
	vec4 vertexModelPos = vec4(vPos.xyz, 1.0);
	
	vec2 UVFinal = vTextureCoords.st;
	
	if(bUseHeightMap)
	{
		UVFinal += UVOffset.yx;
		
		float height = texture( heightMapSampler, UVFinal.st ).r;
		
		//To scale blacked areas
		if(bDiscardColoredAreasInHeightMap)
		{
			height = (height < 0.005f) ? 1.0 : 0.0;
		}
		
		vertexModelPos.y += ( height * heightScale );
	}

	if (bUseBones)
	{
		mat4 boneTransform = BoneMatrices[int(vBoneIds[0])] * vBoneWeights[0];
		boneTransform += BoneMatrices[int(vBoneIds[1])] * vBoneWeights[1];
		boneTransform += BoneMatrices[int(vBoneIds[2])] * vBoneWeights[2];
		boneTransform += BoneMatrices[int(vBoneIds[3])] * vBoneWeights[3];
		vertexModelPos = boneTransform * vertexModelPos;
	}

	mat4 matMVP = matProjection * matView * matModel;
	gl_Position = matMVP * vec4(vertexModelPos.xyz, 1.0);
	
	// Rotate the normal by the inverse transpose of the model matrix
	vertexWorldNormal = matModel_IT * vec4(vNormal.xyz, 1.0f);
	vertexWorldNormal.xyz = normalize(vertexWorldNormal.xyz);
	vertexWorldNormal.w = 1.0f;
	
	//vertexWorldPos = matModel * vec4( vertexModelPos.xyz, 1.0f);
	vertexWorldPos = matModel * vec4( vPos.xyz, 1.0f);

	colour = vCol;
	textureCoords = UVFinal;
	boneId = vBoneIds;
	boneWeight = vBoneWeights;
}

