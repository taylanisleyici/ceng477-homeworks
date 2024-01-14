#version 120

varying vec4 worldPos;

void main(void)
{
    vec2 pos = worldPos.xz;
    vec2 fractPos = fract(pos / vec2(5, 5));

    float stripSize = 0.5;  // Adjust this value to change the strip size

    if ((fractPos.x < stripSize) != (fractPos.y < stripSize))
        gl_FragColor = vec4(0.3, 0.3, 0.6, 1.0);  // Black
    else
        gl_FragColor = vec4(0.1, 0.1, 0.3, 1.0);  // White
}