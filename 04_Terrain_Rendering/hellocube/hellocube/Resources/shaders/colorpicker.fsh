
uniform vec3 objId;

//const vec3 objId = vec3(1.0, 0.0, 0.0);

void main(){

    gl_FragColor = vec4(objId, 1.0);
}