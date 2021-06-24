#pragma once

#include "SolidShape3D.h"

class SolidSphere : public SolidShape3D
{
public:
	SolidSphere();
	SolidSphere(float r, int sl, int st);
	SolidSphere(const SolidSphere& sph);
	Vector3 getProperties() const;
	Vector3 getLocsIndex() const;
	void setLocsIndex(Vector3 vec3);
	bool collisionDetection(const SolidSphere& sph);
	void collisionHandling(SolidSphere& sph);
	void draw() const;

	bool getnotchecked() { return notchecked; }
	void setnotchecked(bool x) { notchecked = x; };
	void setnotchecked0(SolidSphere &sph) {
		sph.setnotchecked(false);
	}
	void setnotchecked1(SolidSphere &sph) {
		sph.setnotchecked(true);
	}

	int getdropcheck() { return dropcheck; };
	void setdropcheck(int x) { dropcheck = x; };
	void setdrop0(SolidSphere &sph) {
		sph.setdropcheck(0);
	}
	void setdrop1(SolidSphere &sph) {
		sph.setdropcheck(1);
	}
private:
	bool notchecked;
	int dropcheck;
	Vector3 properties;
	Vector3 LocsIndex;

};

