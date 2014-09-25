#version 330

uniform vec4 frag_color;
uniform int mode;
in  vec4 varyingColor;
out vec4 finalColor;

void main()
{	
	if(mode == 1){
		finalColor = frag_color;
	} else if(mode == 2){
		finalColor = frag_color;
	} else {
		finalColor = varyingColor;
	}
    
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
