
uniform vec3 objId;

varying vec2 uv;

uniform vec3 highlightColor;
uniform sampler2D sceneTex;
uniform sampler2D pickTex;
uniform vec4 selectedID;
uniform bool isSelectedWidget;

/*
  Returns true if the picktex contains different values at uv and uv + d
*/
bool texDiff(vec2 d) 
{
    return (texture2D(pickTex, uv + d) != texture2D(pickTex, uv)) && texture2D(pickTex, uv) != vec4(1, 0, 1, 0) && texture2D(pickTex, uv + d) != vec4(1, 0, 1, 0);
}

bool isBorder() 
{
    float delta = 0.008f;
    return texDiff(vec2(-delta, 0)) || texDiff(vec2(delta, 0)) || texDiff(vec2(0, -delta)) || texDiff(vec2(0, delta));
}

bool isOnWindowBorder() 
{
    float d = 0.01f;
    return uv.x < d || uv.x > 1.0-d || uv.y < d || uv.y > 1.0-d;
}

void main(void)
{
    if(isSelectedWidget && isOnWindowBorder()) 
	{
        gl_FragColor = vec4(highlightColor, 1.f);
    }
    else
	{
        vec4 color = texture2D(pickTex, uv);
        if(color.a > 0.0f && color == selectedID && isBorder()) 
		{
          gl_FragColor = vec4(highlightColor, 1.f);
        } 
		else 
		{
            gl_FragColor = texture2D(sceneTex, uv);
        }
    }
}
