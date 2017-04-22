varying vec3 vv3color;
uniform int RGB = 0;
void main() {
	if(RGB == 0)	gl_FragColor = vec4(vv3color, 1.0);
	if(RGB == 1)	gl_FragColor = vec4(vv3color[0], 0, 0, 1);
	if(RGB == 2)	gl_FragColor = vec4(0, vv3color[1], 0, 1);
	if(RGB == 3)	gl_FragColor = vec4(0, 0, vv3color[2], 1);
}
