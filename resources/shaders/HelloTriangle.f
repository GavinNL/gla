#version 330 core

in  vec4 fragmentColor;

out vec4 color;

uniform BlobSettings {
  vec4 InnerColor;
  vec4 OuterColor;
  float RadiusInner;
  float RadiusOuter;
};

void main()
{

    // Output color = red
    color = fragmentColor - 0*(InnerColor*RadiusOuter+OuterColor*RadiusInner);
}
