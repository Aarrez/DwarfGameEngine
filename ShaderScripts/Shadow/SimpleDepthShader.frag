#version 330 core


void main() {

    //  1gl_FragDepth = gl_FragCoord.z;
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}