// #version 330 core
// out vec4 FragColor;
// // uniform vec4 ourColor;
// in vec2 TexCoord;
// uniform sampler2D texture1;
// void main()
// {
// 	vec2 st = TexCoord.xy * 1;
// 	// vec2 st = gl_FragCoord.xy * 0.032;
// 	float angle = 0.;
// 	mat2 rot = mat2(cos(angle), -sin(angle),sin(angle), cos(angle));
// 	st = rot * st;

// 	vec3 color=vec3(abs(step(0.5,fract(st.x))-step(0.5,fract(st.y))),0.1,0.2);
// 	FragColor = vec4(color, 1.0);
// }

#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	// if (TexCoord.y > 40.) {
	// 	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	// } else {
	// 	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	// }
	FragColor = texture(texture1, TexCoord);
	// FragColor = vec4 (TexCoord.x, TexCoord.y, 0.0, .1);
}