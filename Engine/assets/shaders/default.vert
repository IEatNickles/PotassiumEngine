#version 460

layout(location=0) in vec3 a_position;

layout(binding=0) uniform Matrices {
  mat4 u_viewproj;
};
uniform mat4 u_world;

void main() {
  gl_Position = u_viewproj * u_world * vec4(a_position, 1.0);
}
