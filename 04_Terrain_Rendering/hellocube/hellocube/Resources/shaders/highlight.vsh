attribute vec3 in_position;

varying vec2 uv;

void main(void)
{
	uv = vec2((in_position.x + 1.0) / 2.f, (in_position.y + 1.0) / 2.f);
	gl_Position = vec4(in_position, 1.0);
}