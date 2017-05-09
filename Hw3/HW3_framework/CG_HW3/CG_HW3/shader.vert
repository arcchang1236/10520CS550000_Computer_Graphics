attribute vec4 av4position;
attribute vec3 av3normal;

varying vec4 vv4color;
varying vec4 P;
varying vec4 N;

uniform mat4 mvp;
uniform mat4 MN;
uniform mat4 R;



void main() {	
	P = MN * av4position;
	N = R * vec4(av3normal, 0.0);

	gl_Position = mvp * av4position;
}
