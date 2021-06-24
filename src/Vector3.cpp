#include "Vector3.h"

//public:
Vector3::Vector3() { xyz[0] = 0;
					xyz[1] = 0;
					xyz[2] = 0;
};
Vector3::Vector3(float x, float y, float z) {
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;};
void Vector3::setXYZ(float x, float y, float z) {
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
};
const float* Vector3::getXYZ() const {
	return xyz;
};

Vector3& Vector3::operator=(const Vector3& vec3) {
	xyz[0] = vec3.getXYZ()[0];
	xyz[1] = vec3.getXYZ()[1];
	xyz[2] = vec3.getXYZ()[2];
	return *this;

};
//left value
float& Vector3::operator[](const int i) { return xyz[i]; };
//right value
float Vector3::operator[](const int i) const { return xyz[i]; };

//private:
//	float xyz[3];
bool operator==(const Vector3& v1, const Vector3& v2) {
	return(v1[0] == v2[0])&& (v1[1] == v2[1])&& (v1[2] == v2[2]);
};
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Vector3(v1[0] + v2[0], v1[1] + v2[1], v1[2]+ v2[2]); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3(v1[0] - v2[0], v1[1] - v2[1],v1[2]-v2[2]); }
Vector3 operator-(const Vector3& v) { return Vector3(-v[0], -v[1], -v[2]);  }
Vector3 operator*(const float s, const Vector3& vec3) { return Vector3(vec3[0] * s, vec3[1] * s,vec3[2] * s); }
float dotProduct(const Vector3& v1, const Vector3& v2) {
	return (v1[0] * v2[0] + v1[1] * v2[1]
		+ v1[2] * v2[2]);
}