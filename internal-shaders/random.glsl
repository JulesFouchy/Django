const float a = 43758.5453;
const vec2 v = vec2(12.9898, 78.233);
const float delta = 0.01;

float rand(float id, float seed){
  return fract(sin(dot(vec2(id*delta, seed), v)) * a) * 2.0 - 1.0;
}