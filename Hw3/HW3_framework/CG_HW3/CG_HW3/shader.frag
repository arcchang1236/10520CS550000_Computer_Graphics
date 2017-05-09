varying vec4 vv4color;
varying vec4 P;
varying vec4 N;

struct LightSourceParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 halfVector;
	vec4 spotDirection;
	float spotExponent;
	float spotCutoff; // (range: [0.0,90.0], 180.0)
	float spotCosCutoff; // (range: [1.0,0.0],-1.0)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct MaterialParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform MaterialParameters Material;
uniform LightSourceParameters LightSource[4];

uniform int modeONE;
uniform int modeTWO;
uniform int modeTHREE;

uniform int modeA;
uniform int modeD;
uniform int modeS;
uniform int modeC;

uniform vec3 eye;

vec4 vv4ambient = vec4(0.0);
vec4 vv4diffuse_D = vec4(0.0);
vec4 vv4specular_D = vec4(0.0);

vec4 vv4diffuse_P = vec4(0.0);
vec4 vv4specular_P = vec4(0.0);

vec4 vv4diffuse_S = vec4(0.0);
vec4 vv4specular_S = vec4(0.0);

void main() {
float dL = length(LightSource[1].position.xyz-P.xyz);
	float tmp = 1/(LightSource[1].constantAttenuation + LightSource[1].linearAttenuation*dL + LightSource[1].quadraticAttenuation*dL*dL);
	float fatt = min(1, tmp);

	float dL_a = length(LightSource[2].position.xyz-P.xyz);
	float tmp_a = 1/(LightSource[2].constantAttenuation + LightSource[2].linearAttenuation*dL_a + LightSource[2].quadraticAttenuation*dL_a*dL_a);
	float fatt_a = min(1, tmp_a);
	float se;
	if( dot(normalize(P.xyz-LightSource[2].position.xyz), normalize(LightSource[2].spotDirection.xyz)) > LightSource[2].spotCosCutoff){
		se = pow(max(dot(normalize(P.xyz-LightSource[2].position.xyz), normalize(LightSource[2].spotDirection.xyz)), 0), LightSource[2].spotExponent);
	}
	else{
		se = 0;
	}

	if(modeA == 1){
		vv4ambient = Material.ambient * LightSource[0].ambient;
	}
	if(modeD == 1){
		if(modeONE == 1){
			vv4diffuse_D = Material.diffuse * LightSource[0].diffuse * max(dot(normalize(N), normalize(LightSource[0].position-P)), 0);
		}
		if(modeTWO == 1){
			vv4diffuse_P = Material.diffuse * LightSource[1].diffuse * max(dot(normalize(N), normalize(LightSource[1].position-P)), 0);
			vv4diffuse_P *= fatt;
		}
		if(modeTHREE == 1){
			if(modeC == 0){
				vv4diffuse_S = Material.diffuse * LightSource[2].diffuse * max(dot(normalize(N), normalize(LightSource[2].position-P)), 0);
				vv4diffuse_S *= fatt_a * se;
			} else if(modeC == 1){
				vv4diffuse_S = Material.diffuse * LightSource[0].diffuse * max(dot(normalize(N), normalize(LightSource[0].position-P)), 0);
			} else if(modeC == 2){
				vv4diffuse_S = Material.diffuse * LightSource[1].diffuse * max(dot(normalize(N), normalize(LightSource[1].position-P)), 0);
				vv4diffuse_S *= fatt;
			}
		}
	}
	if(modeS == 1){
		if(modeONE == 1){
			vec4 H_D = normalize(normalize(LightSource[0].position-P) + normalize(vec4(eye, 1.0)-P));
			vv4specular_D = Material.specular * LightSource[0].specular * pow(max(dot(normalize(N), H_D), 0), 200);
		}
		if(modeTWO == 1){
			vec4 H_P = normalize(normalize(LightSource[1].position-P) + normalize(vec4(eye, 1.0)-P));
			vv4specular_P = Material.specular * LightSource[1].specular * pow(max(dot(normalize(N), H_P), 0), 200);
			vv4specular_P *= fatt;
		}
		if(modeTHREE == 1){
			if(modeC == 0){
				vec4 H_S = normalize(normalize(LightSource[2].position-P) + normalize(vec4(eye, 1.0)-P));
				vv4specular_S = Material.specular * LightSource[2].specular * pow(max(dot(normalize(N), H_S), 0), 200);
				vv4specular_S *= fatt_a * se;
			} else if(modeC == 1){
				vec4 H_D = normalize(normalize(LightSource[0].position-P) + normalize(vec4(eye, 1.0)-P));
				vv4specular_S = Material.specular * LightSource[0].specular * pow(max(dot(normalize(N), H_D), 0), 200);
			} else if(modeC == 2){
				vec4 H_P = normalize(normalize(LightSource[1].position-P) + normalize(vec4(eye, 1.0)-P));
				vv4specular_S = Material.specular * LightSource[1].specular * pow(max(dot(normalize(N), H_P), 0), 200);
				vv4specular_S *= fatt;
			}
		}
	}
	
	gl_FragColor = vv4ambient + (vv4diffuse_D + vv4diffuse_P + vv4diffuse_S) + (vv4specular_D + vv4specular_P + vv4specular_S);
}
