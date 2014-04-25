attribute vec3 a_position;
attribute vec4 a_color;
attribute vec3 a_velocity;
attribute float a_size;
attribute float a_lifetime;

varying vec4 v_color;

uniform mat4 u_mvp;

void main()
{
	v_color = a_color;
	
	vec3 newpos = a_position;
	
	gl_Position = u_mvp * vec4(newpos, 1.0);
}