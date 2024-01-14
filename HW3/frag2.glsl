#version 120

vec3 eyePos = vec3(0, 0, 0);

vec3 I = vec3(1.0, 1.0, 0.0);     // Hot color for point light intensity
vec3 Iamb = vec3(0.9, 0.8, 0.2);   // Ambient light intensity
vec3 kd = vec3(0.8, 0.8, 0.2);      // Diffuse reflectance coefficient
vec3 ka = vec3(0.5, 0.5, 0.5);      // Ambient reflectance coefficient
vec3 ks = vec3(0.7, 0.7, 0.7);      // Specular reflectance coefficient

varying vec4 fragPos;
varying vec3 N;

void main(void)
{
    vec3 lightPos = vec3(fragPos) + vec3(0, 1, 1);
    vec3 L = normalize(lightPos - vec3(fragPos));
    vec3 V = normalize(eyePos - vec3(fragPos));
    vec3 H = normalize(L + V);
    float NdotL = dot(N, L);
    float NdotH = dot(N, H);

    vec3 diffuseColor = I * kd * max(0, NdotL);
    vec3 ambientColor = Iamb * ka;
    vec3 specularColor = I * ks * pow(max(0, NdotH), 20);

    gl_FragColor = vec4(ambientColor + diffuseColor, 1);
}