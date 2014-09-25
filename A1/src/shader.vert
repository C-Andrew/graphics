#version 330 

in vec3 vert;
in vec4 color;
uniform mat4 mvpMatrix;
out vec4 varyingColor;

void main()
{	
	varyingColor = color;
    gl_Position = mvpMatrix * vec4(vert, 1.0);
    // gl_Position = ftransform();
}
