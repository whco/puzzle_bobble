#include "SolidSphere.h"
#include <math.h>

SolidSphere::SolidSphere() {
	properties.setXYZ(25, 30, 30);
	setdrop0(*this);
	setnotchecked(1);
}

SolidSphere::SolidSphere(float r, int sl, int st) : SolidShape3D() {
	properties.setXYZ(r, sl, st);
	setdrop0(*this);
	setnotchecked(1);
}
SolidSphere::SolidSphere(const SolidSphere& sph) : SolidShape3D(sph) {
	properties = sph.properties;
	this->ColorIndex = sph.ColorIndex;
	this->notchecked = sph.notchecked;
	this->dropcheck = sph.dropcheck;
	this->LocsIndex = sph.LocsIndex;
}

Vector3 SolidSphere::getProperties() const{
	return properties;
}

Vector3 SolidSphere::getLocsIndex() const { return LocsIndex;};

void SolidSphere::setLocsIndex(Vector3 vec3) {
	LocsIndex = vec3;
};


bool SolidSphere::collisionDetection(const SolidSphere& sph) {
	/* Implementation: collision detection */
	if (
		(sqrt(
		pow((getCenter() - sph.getCenter())[0], 2) + pow((getCenter() - sph.getCenter())[1], 2)
		+ pow((getCenter() - sph.getCenter())[2], 2)
	) <= (getProperties()[0] + sph.getProperties()[0])
			) 
		&&
		(dotProduct(getCenter() -sph.getCenter(), getCenter() - sph.getCenter())>
			dotProduct( (getCenter() +getVelocity())-(sph.getCenter() +sph.getVelocity()),
			(getCenter() + getVelocity()) - (sph.getCenter() + sph.getVelocity()))
		)
		)return 1;
	else return 0;


}

void SolidSphere::collisionHandling(SolidSphere& sph) {
	if (collisionDetection(sph)) {
	}
		//setVelocity(0, 0, 0);
	//sph.setVelocity(0,0,0);
	//sph.setVelocity(0, 0, 0);
		/* Implementation: collision handling */
	/*{
		Vector3 v1 = getVelocity();
		Vector3 v2 = sph.getVelocity();
		Vector3 c1 = getCenter();
		Vector3 c2 = sph.getCenter();
		setVelocity((v1 - (dotProduct(v1 - v2, c1 - c2) / dotProduct(c1 - c2, c1 - c2) )* (c1 - c2)));
		sph.setVelocity((v2 - (dotProduct(v2 - v1, c2 - c1) / dotProduct(c2 - c1, c2 - c1)) * (c2 - c1)));
	}*/
}

void SolidSphere::draw() const {
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glTranslatef(center[0], center[1], center[2]);

	glutSolidSphere(properties[0], properties[1], properties[2]);
	glPopMatrix();
}

//void SolidSphere::setnotchecked(bool x) { notchecked = x; }