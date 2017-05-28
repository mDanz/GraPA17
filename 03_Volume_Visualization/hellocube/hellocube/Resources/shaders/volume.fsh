#version 400

const float BASE_STEP = 200.f, OPACITY_TERMINATION = 0.95f;

struct VolumeProps {
    int width;
    int height;
    int depth;
    float aspectX;
    float aspectY;
    float aspectZ;
    float minValue; // maximum normalized intensity value
    float maxValue; // (normalized: [0,1])
};

// lightning parameters
const vec4 lightPos = vec4(0.5f, 0.f, 2.f, 1.f);

uniform int displayMode = 0;

uniform sampler3D volumeSampler;
uniform sampler1D transferFunction;

uniform sampler2D entryPoints;
uniform sampler2D exitPoints;

uniform float step = 0.01f;
uniform mat4 mvMat;

uniform VolumeProps properties;

uniform vec4 idColor;


in vec2 fragPos;
//out vec4 outColor;

vec4 transFunc(float intensity) {
    // fit the value to the relevant transfer function interval
    intensity = (intensity / (properties.maxValue - properties.minValue) + properties.minValue);
    // perform classification with the look up texture
    return texture1D(transferFunction, intensity);
}

vec3 gradient(vec3 samplePos) {
    float h = 3.f/(properties.width + properties.height + properties.depth);
    float x = texture3D(volumeSampler, samplePos + vec3(h, 0, 0)).r
            - texture3D(volumeSampler, samplePos - vec3(h, 0, 0)).r;
    float y = texture3D(volumeSampler, samplePos + vec3(0, h, 0)).r
            - texture3D(volumeSampler, samplePos - vec3(0, h, 0)).r;
    float z = texture3D(volumeSampler, samplePos + vec3(0, 0, h)).r
            - texture3D(volumeSampler, samplePos - vec3(0, 0, h)).r;
    return normalize(vec3(x,y,z));
}


vec4 lighting(vec3 samplePos, vec3 color, float opacity) {

    // lightning parameters
    vec4 diffuseCol = vec4(color, 1.f);
    vec4 specularCol = vec4(0.2f);
    const float intensity = 1, shininess = 10;

    // calculate the surface normal with the gradient
    vec3 normal = gradient(samplePos);
    normal = normalize(normal);
    normal = -(transpose(inverse(mat3(mvMat))) * normal);

    // the eye vector (since we're in view space it's the position)
    vec3 worldPos = (mvMat * vec4(samplePos, 1.f)).xyz;
    vec3 posNorm = normalize(worldPos);

    // the normalized light vector
    vec4 lightPosView = lightPos;
    lightPosView = lightPosView / lightPosView.w;
    vec3 toLightNorm = normalize(lightPosView.xyz - worldPos);

    // the reflection vector
    vec3 reflect = reflect(toLightNorm, normal);

    // calculate the resulting (diffuse and specular) optical properties
    vec4 diffuseSum = intensity * diffuseCol * clamp(abs(dot(toLightNorm, normal)), 0.0, 1.0);
    vec4 specularSum = (shininess +2)/6.28318f * pow(clamp(abs(dot(posNorm, reflect)), 0.0, 1.0), shininess) * specularCol;

    return diffuseSum + specularSum;
}


vec4 directRendering(vec3 start, vec3 end) {

    if(start == end)
        return vec4(0.f);

    vec3 dir = (end - start);
    float t_end = length(dir);
    dir = normalize(dir);
    float diff = abs(step/t_end);

    float intensity;
    vec4 curCol;

    float alpha = 0, curOpacity;
    vec3 color = vec3(0.f);

    // iterative direct volume rendering sum:
    vec3 samplePos;
    bool cont = true;

    for(float t = 0; t <= t_end; t += diff) {   // diff vormals step

        // iterate along the ray
        samplePos = start + t*dir;

        intensity = texture3D(volumeSampler, start + t*dir).r;
        curCol = vec4(intensity, intensity, intensity, intensity);//transFunc(intensity);

        if(curCol.a > 0.0f) {
            // alpha correction for different step sizes
            curCol.a = 1.f - pow(1.f - curCol.a, step * BASE_STEP);
            curOpacity = (1.f - alpha) * curCol.a;
            // apply lighting at current position for the resulting color
            color += lighting(start + t*dir, curCol.rgb, curOpacity).rgb * curOpacity;
            // add the current alpha value to the opacity
            alpha += curOpacity;
        }

        // early ray termination
        if(alpha >= OPACITY_TERMINATION) {
            alpha = 1.f;
            break;
        }

    }
    return vec4(color, alpha);
}


vec4 maximumIntensity(vec3 start, vec3 end) {
    if(start == end)
        return vec4(0.f);

    vec3 dir = (end - start);
    float t_end = length(dir);
    dir = normalize(dir);
    float diff = abs(step/t_end);

    float intensity;
    float maxInt = -1.f;
    vec4 color = vec4(0.f);

    for(float t = 0; t <= t_end; t += diff) {
        intensity = texture3D(volumeSampler, start + t*dir).r;
        if(intensity > maxInt) {
            color = transFunc(intensity);
            maxInt = intensity;
        }
    }

    return color;
}

void main() {
    // obtain entry and exit points from the prerendered textures
    vec4 col = texture2D(entryPoints, fragPos);
    vec3 entryPoint = col.rgb;
    col = texture2D(exitPoints, fragPos);
    vec3 exitPoint = col.rgb;

    // Direct rendering
    vec4 outColor;

    if(displayMode == 0)
        outColor = directRendering(entryPoint, exitPoint);
    // Maximum Intensity Projection
    else if(displayMode == 1)
        outColor = maximumIntensity(entryPoint, exitPoint);

    // Debug modes
    else if(displayMode == 2) // entry points
        outColor = vec4(entryPoint, 1.f);
    else if(displayMode == 3) // exit points
        outColor = vec4(exitPoint, 1.f);
    else if(displayMode == 4) // debug box
        outColor = clamp(vec4(exitPoint, 0.f)*0.2f + directRendering(entryPoint, exitPoint), vec4(0), vec4(1));

    gl_FragData[0] = outColor;


    if(entryPoint == exitPoint) {
        gl_FragDepth = 0.999f;
        gl_FragData[1] = vec4(0.f);
    }
    else {
        gl_FragData[1] = idColor;
        gl_FragDepth = 0.1f;
    }

}