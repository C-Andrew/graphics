#version 330 

in vec3 vert;
uniform mat4 mvpMatrix;

uniform vec3 Kd;
uniform vec3 Ks; 
uniform vec3 Shiny;

uniform mat3 normal;
uniform vec4 lightPos;
uniform mat4 eyePos;

uniform float isPicking;

out vec3 colour;
void main()
{	
	if(isPicking == 0){
		float kaval = 0.6;
		float laval = 0.3;
		float ldval = 0.9;
		float lsval = 3.0;
		vec3 ka = vec3(kaval, kaval, kaval);

	    vec3 la = vec3(laval, laval, laval);
	    vec3 ld = vec3(ldval, ldval, ldval);
	    vec3 ls = vec3(lsval, lsval, lsval);

		vec3 tnorm = normalize( normal * vert );
		vec4 eyeCoords =  mvpMatrix * vec4(vert, 1.0);
		vec3 s = normalize( vec3( lightPos - eyeCoords ));
		vec3 v = normalize(-eyeCoords.xyz);

		vec3 r = reflect(-s, tnorm);
		vec3 ambient = la * ka;
		float sDotN = max( dot(s, tnorm), 0.0 );
		vec3 diffuse = ld * Kd * sDotN;
		vec3 spec = vec3(0.0);

		spec = ls * Ks * pow(max(dot(r,v), 0.0), Shiny.x);

		colour = ambient + diffuse + spec;
	}
	else{
		colour = Kd;
	}
    gl_Position = mvpMatrix * vec4(vert, 1.0);
	// colour = Kd;
    // gl_Position = ftransform();
}
