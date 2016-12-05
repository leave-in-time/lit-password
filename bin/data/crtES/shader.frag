precision highp float;

uniform sampler2D tex0;
uniform float currentTime;
uniform vec2 resolution;

varying vec2 texCoordVarying;

void main() {
	vec2 uv = texCoordVarying.xy;
	vec3 frag;
	frag.r = texture2D(tex0, vec2(uv.x + .0007 * sin(currentTime / 4.), uv.y)).r;
	// frag.r = texture2D(tex0, uv).r;
	frag.g = texture2D(tex0, vec2(uv.x + .0007 * sin(currentTime / 4.), uv.y)).g;
	frag.b = texture2D(tex0, vec2(uv.x + .0007 * sin(currentTime / 4.), uv.y)).b;
	// frag.b = texture2D(tex0, uv).b;
	vec2 screenSpace = uv * resolution.xy;
	frag.rgb -= sin((screenSpace.y + (currentTime * 29.0))) * 0.05;
	gl_FragColor = vec4(frag.rgb, 1.);
}
