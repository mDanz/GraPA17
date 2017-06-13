#version 400

in vec3 posInWorld;
in vec3 posInView;
in float height;
in vec3 normal;
flat in int eLod;

uniform mat4 frag_viewMat;
uniform mat3 frag_normalMat;

uniform sampler2D fracture[4];
uniform sampler2D testSampler;

uniform vec3 idColor;

struct FracDef {
    float minH;
    float maxH;
    float minS;
    float maxS;
    float specular;
};

const FracDef materials[4] = FracDef[4](FracDef(0.f, 0.2f, 0.8f, 0.1f, 1.f), FracDef(0.25f, 0.45f, 0.6f, 0.7f, 10.f),
                                        FracDef(0.5f, 0.75f, -1.f, -1.f, 3.f), FracDef(0.8f, 1.f, 0.0f, 0.4f, 50.f));

layout(location=0) out vec4 colorBuffer;
layout(location=1) out vec4 idBuffer;


// returns color.rgb + the specular parameter as the alpha value
vec4 material(vec2 uv, float height, float slope) {
    float totalAmount = 0.f;
    vec4 color = vec4(0.f);

    // slope intervals override the height based mixing
    for(int i=0; i<4; i++) {
        if(slope >= materials[i].minS && slope <= materials[i].maxS) {
            float span = materials[i].maxS - materials[i].minS;
            float weight = 4.f * (1.f - 2.f*abs(slope - materials[i].minS - span/2.f)/span);
            color = weight * vec4(texture(fracture[i], uv).xyz, materials[i].specular);
            totalAmount += weight;
            break;
        }
    }

    //   if no slope color is used: height based mixing
    // calculate the mixing weights
    float amount[4] = float[4](0.f, 0.f, 0.f, 0.f);
    float top = 0.2f;
    float border = (1.f - 4.f * top)/4.f;
    for(int i=0; i<4; i++) {
        if(height <= top*(i+1) || i == 4) {
            amount[i] = 1.f;
            break;
        } else if(height < top*(i+1) + border) {
            amount[i+1] = (height - top*(i+1))/border;
            amount[i] = 1.f - amount[i+1];
            break;
        }
    }

    // mix the materials
    vec3 testCol[4] = vec3[4](vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(1,1,0));
    for(int i=0; i<4; i++) {
        color += amount[i] * vec4(texture(fracture[i], uv).xyz, materials[i].specular);
        totalAmount += amount[i];
    }

    // normalize the value
    color /= totalAmount;
    return color;
}

vec3 lighting(vec3 pos, vec3 normal, vec3 viewPos, vec3 color, float shininess) {

    normal = vec3(normal.x, normal.y, normal.z);
    //return normal;

    const int LIGHT_COUNT = 2;

    const vec3 lights[2] = vec3[2](vec3(0.f, 500.f, 60.f), vec3(40.f, 100.f, 10.f));
    vec3 diffuseCol = vec3(1.f, 0.9f, 1.f);
    vec3 specularCol = vec3(1.f);
    vec3 ambientCol = vec3(0.1f, 0.08f, 0.09f);
    float intensity = 1.f;

    vec3 retCol = ambientCol;


    for(int i = 0; i < LIGHT_COUNT; i++) {
        vec3 lightPos = lights[i];

        // the eye vector
        vec3 toEye = normalize(-viewPos);//normalize(-(frag_viewMat * vec4(0.f, 0, 0, 1)).xyz);

        // the light vector
        vec3 toLight = normalize(lightPos - pos);

        // the reflection vector
        vec3 reflect = reflect(toLight, normal);

        //return vec3(normalize(pos));//toEye - normal);

        // calculate the resulting (diffuse and specular) optical properties
        vec3 diffuseSum = intensity * diffuseCol * max(dot(normal, toLight), 0.f);
        vec3 specularSum = (shininess+2)/6.28318f * pow(max(dot(-toEye, reflect), 0.f), shininess) * specularCol;

        retCol += diffuseSum + specularSum;
    }
    //retCol = clamp(retCol, vec3(0.f), vec3(2.f));
    return retCol * color;
}

void main(void)
{    
    // get color from the frac materials
    //vec4 color = material(posInWorld.xz/10.f, height, normal.y);

// LOD Visualization ------------
//    color = vec4((eLod%3)/3.f, ((eLod+1)%4)/4.f, ((eLod+2)%3)/3.f, 1.f);
//    if(eLod==0)
//        color = vec4(1.f);
// ------------------------------


    // apply lighting
    colorBuffer = vec4(1.f);//vec4(lighting(posInWorld, normalize(normal), posInView, color.rgb, color.a), 1.f);

    idBuffer = vec4(idColor, 1.f);//vec4(0.f);
}
