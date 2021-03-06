#ifndef CAMERA_H
#define CAMERA_H

// System Headers
#include <iostream>

// OpenGL Headers
#if defined(_WIN32)
	#include <GL/glew.h>
	#if defined(GLEW_EGL)
		#include <GL/eglew.h>
	#elif defined(GLEW_OSMESA)
		#define GLAPI extern
		#include <GL/osmesa.h>
	#elif defined(_WIN32)
		#include <GL/wglew.h>
	#elif !defined(__APPLE__) && !defined(__HAIKU__) || defined(GLEW_APPLE_GLX)
		#include <GL/glxew.h>
	#endif

	// OpenGL Headers
	#define GLFW_INCLUDE_GLCOREARB
	#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
	#define GLFW_INCLUDE_GLCOREARB
	#include <GLFW/glfw3.h>
	#include <OpenGL/gl3.h>
#endif

// GLM Core Headers
#include <glm/glm.hpp>

// GLM GTC Extensions (Stable)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

// GLM GTX Extensions (Experimental)
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

// --------------------------------------------------------------------------------
// Abstract base class
// --------------------------------------------------------------------------------
class Camera {
public:
	// Constructor
	Camera(GLFWwindow *window) : mWindow(window) {}
	virtual ~Camera() {}

	// Camera View Matrix
	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getOrientationMatrix() = 0;
	virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3 &position) =0;


	// GLFW Input
	virtual void onMouseButton(GLFWwindow *window, int button, int action, int mods) = 0;
	virtual void onCursorPosition(GLFWwindow *window, double x, double y) = 0;

	// Update Camera
	virtual void update(float dt) = 0;

	void setInvertPitch(bool invertPitch) {
		Camera::invertPitch = invertPitch;
	}

protected:
	// GLFW Window
	GLFWwindow *mWindow;
	bool invertPitch = false;

};

// --------------------------------------------------------------------------------
// Free Look Camera
// --------------------------------------------------------------------------------
class FreeLookCamera : public Camera {
public:
	// Constructor
	FreeLookCamera(GLFWwindow *window);
	virtual ~FreeLookCamera() {}

	// Camera View Matrix
	glm::mat4 getViewMatrix() override;
	glm::mat4 getOrientationMatrix() override;

    const glm::vec3 &getPosition() const override;

    void setPosition(const glm::vec3 &position) override;

    // GLFW Input
	void onMouseButton(GLFWwindow *window, int button, int action, int mods) override;
	void onCursorPosition(GLFWwindow *window, double x, double y) override;

	// Update Camera
	void update(float dt) override;

	// Movement
	virtual void moveForward(float x);
	virtual void moveBackward(float x);
	virtual void moveRight(float x);
	virtual void moveLeft(float x);

	// Rotations
	virtual void pitch(float angle);
	virtual void yaw(float angle);
	virtual void roll(float angle);

protected:
	// Data Members
	glm::vec3 mPosition;
	glm::quat mOrientation;
	float mSpeed;
	glm::vec2 mCursorPosition;
	bool mTrack;
};

// --------------------------------------------------------------------------------
// Gimbal Free Look Camera
// --------------------------------------------------------------------------------
class GimbalFreeLookCamera : public FreeLookCamera {
public:
	// Constructor
	GimbalFreeLookCamera(GLFWwindow *window);
	virtual ~GimbalFreeLookCamera() {}

	// Rotations
	void pitch(float angle) override;
	void yaw(float angle) override;
	void roll(float angle) override;


private:
	// Data Members
	float mViewAngleX, mViewAngleY;
};

#endif // CAMERA_H