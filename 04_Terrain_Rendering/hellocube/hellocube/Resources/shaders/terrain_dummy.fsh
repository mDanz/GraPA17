#version 440

#pragma optionNV unroll all

struct Material 
{
    float minHeight;
    float maxHeight;
    float minSlope;
    float maxSlope;
    float specular;
    sampler2D facture;
};

in vec3 posInWorld;
in float height;

uniform Material materials[4];

layout(location=0) out vec4 colorBuffer;
layout(location=1) out vec4 idBuffer;

//const Material materials[4] = Material[](Material(0.f, 0.2f, 0.8f, 0.1f, 1.f), Material(0.25f, 0.45f, 0.6f, 0.7f, 10.f), Material(0.5f, 0.75f, -1.f, -1.f, 3.f), Material(0.8f, 1.f, 0.0f, 0.4f, 50.f));



vec4 material(vec2 uv, float height, float slope) {
    float totalAmount = 0.f;
    vec4 color = vec4(0.f);

    // slope intervals override the height based mixing
    //for(int i=0; i < 4; i++) 
    //{
    //    if(slope >= materials[i].minS && slope <= materials[i].maxS) 
    //    {
    //        float span = materials[i].maxS - materials[i].minS;
    //        float weight = 4.f * (1.f - 2.f*abs(slope - materials[i].minS - span/2.f)/span);
    //        color = weight * vec4(texture(fracture[i], uv).xyz, materials[i].specular);
    //        totalAmount += weight;
    //        break;
    //    }
    //}

    //   if no slope color is used: height based mixing
    // calculate the mixing weights
    float amount[4] = float[](0.f, 0.f, 0.f, 0.f);
    float top = 0.2f;
    float border = (1.f - 4.f * top)/4.f;
    for(int i = 0; i < 4; i++) 
    {
        if(height <= top*(i+1) || i == 3) 
        {
            amount[i] = 1.f;
            break;
        } 
        else if(height < top*(i+1) + border) 
        {
            amount[i+1] = (height - top*(i+1))/border;
            amount[i] = 1.f - amount[i+1];
            break;
        }
    }

    // mix the materials
    //vec3 testCol[4] = vec3[4](vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(1,1,0));
    for(int i = 0; i < 4; i++) 
    {
        color += amount[i] * vec4(texture2D(materials[i].facture, uv).xyz, materials[i].specular);
        totalAmount += amount[i];
    }

    // normalize the value
    color /= totalAmount;
    return color;
}



void main()
{
	vec4 color = vec4(material(posInWorld.xz/10, height, 0.f));//normal.y);


	colorBuffer = color;
	idBuffer = vec4(1.0f, .0f, .0f, 1.f);
}