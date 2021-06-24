#include "SolidShape3D.h"
#include <random>


SolidShape3D::SolidShape3D() {
	center.setXYZ(-200,-300,0);
	velocity.setXYZ(0, 0, 0);

	setMTL();
}

SolidShape3D::SolidShape3D(const SolidShape3D& sh3d) {
	setCenter(sh3d.center);
	setVelocity(sh3d.velocity);
	mtl.setMTL(sh3d.mtl);

}

void SolidShape3D::setCenter(float x, float y, float z) {
	center[0] = x;
	center[1] = y;
	center[2] = z;
}

void SolidShape3D::setCenter(const Vector3& c) {
	center = c;
}

Vector3 SolidShape3D::getCenter() const {
	return center;
}

void SolidShape3D::setVelocity(float x, float y, float z) {
	velocity[0] = x;
	velocity[1] = y;
	velocity[2] = z;
}

void SolidShape3D::setVelocity(const Vector3& v) {
	velocity = v;
}

Vector3 SolidShape3D::getVelocity() const {
	return velocity;
}

void SolidShape3D::move() {
	center = center + velocity;
}

void SolidShape3D::setMTL() {
	//srand(time(NULL));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0,3);

	float n1 = 0;
	float n2 = 0;
	float n3 = 0;

	//int color_case = (int)rand() % 4;
	int color_case = dis(gen);

	if (color_case == 0) {
		n1 = 1;
		n2 = 0;
		n3 = 0;
		setColorIndex(0);
	}
	if (color_case == 1) {
		n1 = 0;
		n2 = 1;
		n3 = 0;
		setColorIndex(1);
	}
	if (color_case == 2) {
		n1 = 0;
		n2 = 0;
		n3 = 1;
		setColorIndex(2);
	}
	if (color_case == 3) {
		n1 = 1;
		n2 = 1;
		n3 = 0;
		setColorIndex(3);
	}

	mtl.setEmission(0.1, 0.1, 0.1, 1);
	mtl.setAmbient(n1, n2, n3, 1);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl.setSpecular(1.0, 1.0, 1.0, 1);
	mtl.setShininess(10);
}

int SolidShape3D::getColorIndex() const { return ColorIndex; }
void SolidShape3D::setColorIndex(int n) { ColorIndex = n; };

Material SolidShape3D::getMTL() const { return mtl; };

