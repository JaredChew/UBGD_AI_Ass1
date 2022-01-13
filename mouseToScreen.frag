precision mediump float;

varying vec4 fColor;
varying vec2 fTexCoord;
varying vec4 fPosition;

uniform sampler2D sampler2d;

uniform float mousePosX;
uniform float mousePosY;

uniform int mouseIsPressed;

uniform mat4 nonNormalisedMatrix;

bool sameMouseToScreenPos() {

	vec4 nonNormalisedMousePos = (fPosition * nonNormalisedMatrix) * vec4(mousePosX, mousePosY, 0.0, 0.0);

	if(gl_FragCoord.x != nonNormalisedMousePos.x) {
		return false;
	}

	if(gl_FragCoord.y != nonNormalisedMousePos.y) {
		return false;
	}

	return true;

}

void main() {

	vec4 colour;

	if(mouseIsPressed == 1 && sameMouseToScreenPos()) {
		
		if(texture2D(sampler2d, fTexCoord).r == 1.0) {
			colour = vec4(0.0, 0.0, 0.0, 1.0);
		}
		else {
			colour = vec4(1.0, 1.0, 1.0, 1.0);
		}

	}
	else {

		colour = texture2D(sampler2d, fTexCoord);

	}

	gl_FragColor = colour;

}