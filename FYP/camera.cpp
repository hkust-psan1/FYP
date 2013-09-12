#include "camera.h"

Camera::Camera(const Vec3& e, const Vec3& d, int w, int h)
    : eye(e), dir(d), width(w), height(h) {
}

Ray Camera::getCameraRay(int x, int y) const
{
	return Ray(Vec3(0, 0, 6), 
<<<<<<< HEAD
		Vec3(2 * ((float)x / width - 0.5), 3 * ((float)y / height - 0.5), -3));
=======
		Vec3(2 * ((float)x / width - 0.2), 3 * ((float)y / height - 1), -3));
>>>>>>> dc45e7c46656daf7b2166739aeb7d3ce3bff442b
}
