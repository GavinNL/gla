#version 330 core

in vec4 vColour;

out vec4 oColour;

void main() {
    oColour = vColour;
}

// uniform sampler2D qt_Texture0;
// varying vec4 qt_TexCoord0;
//
// void main(void)
// {
//     gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
// }
