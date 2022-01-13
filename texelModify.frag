precision mediump float;

varying vec4 fColor;
varying vec2 fTexCoord;
varying vec4 fPosition;

uniform sampler2D sampler2d;

uniform float position[2];

uniform float colour[3];

void main() {
	
	if(fTexCoord.x == position[0] && fTexCoord.y == position[1]) {
		gl_FragColor = vec4(colour[0], colour[1], colour[2], 1.0);
		return;
	}
	
	gl_FragColor = texture2D(sampler2d, fTexCoord);

}
