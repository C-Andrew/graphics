#version 330

// uniform vec3 frag_color;
in  vec4 varyingColor;
out vec4 finalColor;

void main()
{
    finalColor = varyingColor;
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
