precision mediump float;

varying vec4 fColor;
varying vec2 fTexCoord;
varying vec4 fPosition;

uniform sampler2D sampler2d;

void main() {
	
	gl_FragColor = texture2D(sampler2d, fTexCoord);

}
