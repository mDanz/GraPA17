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
in vec3 posInView;
in float height;
in vec3 normal;
flat in int eLod;

uniform Material materials[4];

layout(location=0) out vec4 colorBuffer;
layout(location=1) out vec4 idBuffer;

//const Material materials[4] = Material[](Material(0.f, 0.2f, 0.8f, 0.1f, 1.f), Material(0.25f, 0.45f, 0.6f, 0.7f, 10.f), Material(0.5f, 0.75f, -1.f, -1.f, 3.f), Material(0.8f, 1.f, 0.0f, 0.4f, 50.f));



vec4 material(vec2 uv, float height, float slope) 
{
    float totalAmount = 0.f;
    vec4 color = vec4(0.f);

    // slope intervals override the height based mixing
    for(int i=0; i < 4; i++) 
    {
        if(slope >= materials[i].minSlope && slope <= materials[i].maxSlope) 
        {
            float span = materials[i].maxSlope - materials[i].minSlope;
            float weight = 4.f * (1.f - 2.f*abs(slope - materials[i].minSlope - span/2.f)/span);
            color = weight * vec4(texture(materials[i].facture, uv).xyz, materials[i].specular);
            totalAmount += weight;
            break;
        }
    }

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
    vec3 testCol[4] = vec3[4](vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(1,1,0));
    for(int i = 0; i < 4; i++) 
    {
        color += amount[i] * vec4(texture2D(materials[i].facture, uv).xyz, materials[i].specular);
        totalAmount += amount[i];
    }

    // normalize the value
    color /= totalAmount;
    return color;
}

vec3 lighting(vec3 pos, vec3 normal, vec3 viewPos, vec3 color, float shininess) 
{
    normal = vec3(normal.x, normal.y, normal.z);
    //return normal;

    const int LIGHT_COUNT = 2;

    const vec3 lights[2] = vec3[2](vec3(0.f, 500.f, 60.f), vec3(40.f, 100.f, 10.f));
    vec3 diffuseCol = vec3(1.f, 0.9f, 1.f);
    vec3 specularCol = vec3(1.f);
    vec3 ambientCol = vec3(0.1f, 0.08f, 0.09f);
    float intensity = 1.f;

    vec3 retCol = ambientCol;


    for(int i = 0; i < LIGHT_COUNT; i++) 
    {
        vec3 lightPos = lights[i];
        vec3 toEye = normalize(-viewPos);//normalize(-(frag_viewMat * vec4(0.f, 0, 0, 1)).xyz);//
        vec3 toLight = normalize(lightPos - pos);
        vec3 reflect = reflect(toLight, normal);

        //return vec3(normalize(pos));//toEye - normal);

        vec3 diffuseSum = intensity * diffuseCol * max(dot(normal, toLight), 0.f);
        vec3 specularSum = (shininess+2)/6.28318f * pow(max(dot(-toEye, reflect), 0.f), shininess) * specularCol;

        retCol += diffuseSum + specularSum;
    }
    retCol = clamp(retCol, vec3(0.f), vec3(2.f));
    return retCol * color;
}

void main()
{
	vec4 color = vec4(material((posInWorld.xz+512/2)/512, height, normal.y));

// LOD Visualization ------------
	vec4 lodColor = vec4((eLod%3)/3.f, ((eLod+1)%4)/4.f, ((eLod+2)%3)/3.f, 1.f);
	if(eLod==0) 
	{
	    lodColor = vec4(1.f);
	}
// ------------------------------

	colorBuffer = color;//vec4(lighting(posInWorld, normalize(normal), posInView, color.rgb, color.a), 1.f);
	idBuffer = lodColor;//lodColor;//texture2D(heightMap, (posInWorld.xz+4096/2)/4096);
}