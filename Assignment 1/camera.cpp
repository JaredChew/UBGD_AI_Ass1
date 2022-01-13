#include "camera.h"

Camera::Camera(Keyboard* const kbd, const float& cameraYaw, const float& cameraPitch, const float& cameraDistance, const float& cameraSide, const float& cameraScroll, const float& cameraDepth, const float& cameraSpeed) : kbd(kbd) {

	this->cameraYaw = cameraYaw;
	this->cameraPitch = cameraPitch;
	this->cameraDistance = cameraDistance;

	this->cameraSide = cameraSide;
	this->cameraScroll = cameraScroll;
	this->cameraDepth = cameraDepth;

	this->cameraSpeed = cameraSpeed;

}

Camera::~Camera() { }

Matrix4 Camera::getViewMatrixWithoutTranslate(void) {
	return Matrix4::rotate(cameraYaw, Vector3(1.0f, 0.0f, 0.0f)) * Matrix4::rotate(cameraPitch, Vector3(0.0f, 1.0f, 0.0f));
}

void Camera::updateCamera() {

	if (kbd->isPressed('D')) cameraSide += cameraSpeed * 0.02f;
	if (kbd->isPressed('A')) cameraSide -= cameraSpeed * 0.02f;

	if (kbd->isPressed('W')) cameraScroll += cameraSpeed * 0.02f;
	if (kbd->isPressed('S')) cameraScroll -= cameraSpeed * 0.02f;

	gViewMatrix = Matrix4::translate(Vector3(cameraSide, cameraScroll, -cameraDistance)) *
				  Matrix4::rotate(cameraYaw, Vector3(1.0f, 0.0f, 0.0f)) *
				  Matrix4::rotate(cameraPitch, Vector3(0.0f, 1.0f, 0.0f)) *
				  Matrix4::rotate(cameraDepth, Vector3(0.0f, 0.0f, 1.0f));

}