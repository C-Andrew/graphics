#version 330

in vec3 colour;
uniform vec3 frag_color;
uniform float useColour;
out vec4 finalColor;

void main()
{	
	finalColor = vec4(frag_color, 1.0);
	if(useColour == 1){
		finalColor = vec4(colour, 1.0);
	}
    
}
