#version 330 core

uniform mat4 u_cameraToClip; //projection matrix
uniform mat4 u_objectToWorld; //model matrix
uniform mat4 u_worldToCamera; //view matrix

in vec3 inRestPosition; //object space
in vec2 inUV0;
in vec3 inRestTangent;
in vec3 inRestBitangent;
in vec3 inRestNormal;
in vec4 inColor;
in uint inBoneIndeces[4];
in vec3 inBoneWeights;

out vec3 passPosedPosition; //world space
out vec2 passUV0;
out vec3 passPosedTangent;
out vec3 passPosedBitangent;
out vec3 passPosedNormal;
out vec4 passColor;

const int MAX_BONES = 128;
uniform mat4 u_boneToObject[MAX_BONES];

void main( void ){
	//vec4 restPosition = vec4(inRestPosition, 1.0f);
	vec4 pos = vec4(inRestPosition, 1.0f);

	//float boneWeight3 = 1.0f - inBoneWeights.x - inBoneWeights.y - inBoneWeights.z;

	//vec4 pos = u_boneToObject[inBoneIndeces[0]] * restPosition * inBoneWeights.x;
	//pos += u_boneToObject[inBoneIndeces[1]] * restPosition * inBoneWeights.y;
	//pos += u_boneToObject[inBoneIndeces[2]] * restPosition * inBoneWeights.z;
	//pos += u_boneToObject[inBoneIndeces[3]] * restPosition * boneWeight3;

	pos = u_objectToWorld * pos;
	passPosedPosition = vec3(pos);

	pos = u_cameraToClip * u_worldToCamera * pos;

	passUV0 = inUV0;
	passColor = inColor;

	//vec4 restTangent = vec4(inRestTangent, 0.0f);
	//passPosedTangent = (u_boneToObject[inBoneIndeces[0]] * restTangent * inBoneWeights.x).xyz;
	//passPosedTangent += (u_boneToObject[inBoneIndeces[1]] * restTangent * inBoneWeights.y).xyz;
	//passPosedTangent += (u_boneToObject[inBoneIndeces[2]] * restTangent * inBoneWeights.z).xyz;
	//passPosedTangent += (u_boneToObject[inBoneIndeces[3]] * restTangent * boneWeight3).xyz;

	//vec4 restBitangent = vec4(inRestBitangent, 0.0f);
	//passPosedBitangent = (u_boneToObject[inBoneIndeces[0]] * restBitangent * inBoneWeights.x).xyz;
	//passPosedBitangent += (u_boneToObject[inBoneIndeces[1]] * restBitangent * inBoneWeights.y).xyz;
	//passPosedBitangent += (u_boneToObject[inBoneIndeces[2]] * restBitangent * inBoneWeights.z).xyz;
	//passPosedBitangent += (u_boneToObject[inBoneIndeces[3]] * restBitangent * boneWeight3).xyz;

	//vec4 restNormal = vec4(inRestNormal, 0.0f);
	//passPosedNormal = (u_boneToObject[inBoneIndeces[0]] * restNormal * inBoneWeights.x).xyz;
	//passPosedNormal += (u_boneToObject[inBoneIndeces[1]] * restNormal * inBoneWeights.y).xyz;
	//passPosedNormal += (u_boneToObject[inBoneIndeces[2]] * restNormal * inBoneWeights.z).xyz;
	//passPosedNormal += (u_boneToObject[inBoneIndeces[3]] * restNormal * boneWeight3).xyz;

	gl_Position = pos;
}