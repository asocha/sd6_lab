#version 330 core

const int MAX_LIGHTS = 16;

uniform sampler2D u_textureDiffuse;
uniform sampler2D u_textureNormal; //tangent space
uniform sampler2D u_textureSpecGlossEmit;

uniform mat4 u_objectToWorld; //model matrix

uniform int u_lightCount;

uniform vec4 u_ambientLightColor[MAX_LIGHTS];
uniform vec3 u_lightWorldPosition[MAX_LIGHTS];
uniform vec3 u_lightWorldDirection[MAX_LIGHTS];
uniform vec4 u_lightColor[MAX_LIGHTS];
uniform float u_lightAngleMin[MAX_LIGHTS];
uniform float u_lightAngleMax[MAX_LIGHTS];
uniform float u_lightPower[MAX_LIGHTS];
uniform float u_lightSpotFactor[MAX_LIGHTS];
uniform float u_lightSpotPower[MAX_LIGHTS];
uniform float u_lightDirWeight[MAX_LIGHTS];
uniform float u_lightIntensityMin[MAX_LIGHTS];
uniform float u_lightIntensityMax[MAX_LIGHTS];
uniform float u_lightInnerRadius[MAX_LIGHTS];
uniform float u_lightOuterRadius[MAX_LIGHTS];
uniform float u_lightAmbientness[MAX_LIGHTS];
uniform float u_intensityOutsidePenumbra[MAX_LIGHTS];

uniform vec3 u_cameraWorldPosition;

in vec3 passPosedPosition; //world space
in vec2 passUV0;
in vec3 passPosedTangent;
in vec3 passPosedBitangent;
in vec3 passPosedNormal;
in vec4 passColor;

out vec4 outColor;
out vec3 outPosedPosition; //world space
out vec2 outUV;

//------------------------------------------------------------------------
vec3 ComputeLight(int lightIndex, vec3 pos, vec3 normal, vec3 camPosition){
   vec3 lightDir = u_lightWorldPosition[lightIndex] - pos; //vertex to light
   float dist = length(lightDir);
   lightDir /= dist;

   float spotAngle = dot(lightDir, -u_lightWorldDirection[lightIndex]);
   float spotCoefficient = 1.0f - smoothstep(u_lightAngleMin[lightIndex], u_lightAngleMax[lightIndex], spotAngle); //how much inside the cone of light?
   spotCoefficient = max(spotCoefficient, u_intensityOutsidePenumbra[lightIndex]); //add light outside the cone if the light has any
   float spotFactor = spotCoefficient * max(pow(spotAngle, u_lightSpotPower[lightIndex]), u_lightSpotFactor[lightIndex]);
   float lightIntensity = clamp(u_lightPower[lightIndex] * spotFactor, u_lightIntensityMin[lightIndex], u_lightIntensityMax[lightIndex]);
   lightIntensity *= smoothstep(u_lightOuterRadius[lightIndex], u_lightInnerRadius[lightIndex], dist); //cap at inner radius, no light outside outer radius

   lightDir *= u_lightDirWeight[lightIndex];
   lightDir -= ((1.0f - u_lightDirWeight[lightIndex]) * u_lightWorldDirection[lightIndex]);

   //ambient
   vec3 finalColor = u_ambientLightColor[lightIndex].rgb * u_ambientLightColor[lightIndex].a * lightIntensity * u_lightAmbientness[lightIndex];

   //diffuse
   float ndotl = dot(normal, lightDir);
   float diffuseIntensity = clamp(lightIntensity * ndotl * u_lightColor[lightIndex].a, 0.0f, 1.0f);

   finalColor += diffuseIntensity * u_lightColor[lightIndex].rgb * (1.0f - u_lightAmbientness[lightIndex]);

   //specular
   float specularity = texture(u_textureSpecGlossEmit, passUV0).r;
   float glossiness = texture(u_textureSpecGlossEmit, passUV0).g;

   vec3 reflectedLight = reflect(-lightDir, normal);
   float specularIntensity = lightIntensity * u_lightColor[lightIndex].a * specularity * (0.5 + 0.5 * glossiness) * pow(dot(normalize(camPosition - pos), reflectedLight), 1.0 + glossiness);
   specularIntensity = clamp(specularIntensity, 0.0f, 1.0f);

   finalColor += specularIntensity * u_lightColor[lightIndex].rgb * (1.0f - u_lightAmbientness[lightIndex]) * (1.0f - step(dot(reflectedLight, normal), 0.0f)); //prevent spec from a light behind the object

   return finalColor;
}


//------------------------------------------------------------------------
void main(void){
   outPosedPosition = passPosedPosition;
   outUV = passUV0;

   //vec4 diffuseTexel = texture(u_textureDiffuse, passUV0);

   //texture normal - need to convert color-space to a real-space (tangent-space)
   //vec3 normal = texture(u_textureNormal, passUV0).rgb;
   //normal = (normal * vec3(2.0f, 2.0f, 1.0f)) - vec3(1.0f, 1.0f, 0.0f);
   //normal = normalize(normal);

   //convert normal to object-space and then to world-space
   //mat3 tangentToObject = mat3(passPosedTangent, passPosedBitangent, passPosedNormal); //TBN matrix
   //normal = tangentToObject * normal;
   //normal = mat3(u_objectToWorld) * normal;

   //vec3 finalColor;

   //add color for each light
  // for (int i = 0; i < min(u_lightCount, MAX_LIGHTS); ++i){
   //   vec3 colorFromLight = ComputeLight(i, passPosedPosition, normal, u_cameraWorldPosition);
   //   finalColor += colorFromLight * diffuseTexel.rgb;
   //}

   //finalColor *= passColor.rgb;

   //float alpha = passColor.a * diffuseTexel.a;

   //emissive
   //float emissive = texture(u_textureSpecGlossEmit, passUV0).b;
   //finalColor += emissive;

   //finalColor = clamp(finalColor, 0.0f, 1.0f);

   //outColor = vec4(finalColor, alpha);
   outColor = passColor;
}