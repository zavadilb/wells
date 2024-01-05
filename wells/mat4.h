#ifndef MAT4_H
#define MAT4_H
#include "vec4.h"
#include <stdexcept>
//---------------------------
struct mat4 { // row-major matrix 4x4
//---------------------------
	vec4 rows[4];
public:
	mat4() {
		// unit matrix by default
		rows[0][0] = 1.0f;
		rows[1][1] = 1.0f;
		rows[2][2] = 1.0f;
		rows[3][3] = 1.0f;
	}
	mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		rows[0][0] = m00; rows[0][1] = m01; rows[0][2] = m02; rows[0][3] = m03;
		rows[1][0] = m10; rows[1][1] = m11; rows[1][2] = m12; rows[1][3] = m13;
		rows[2][0] = m20; rows[2][1] = m21; rows[2][2] = m22; rows[2][3] = m23;
		rows[3][0] = m30; rows[3][1] = m31; rows[3][2] = m32; rows[3][3] = m33;
	}
	mat4(vec4 it, vec4 jt, vec4 kt, vec4 ot) {
		rows[0] = it; rows[1] = jt; rows[2] = kt; rows[3] = ot;
	}

	vec4& operator[](int i) { return rows[i]; }
	vec4 operator[](int i) const { return rows[i]; }
	operator float* () const { return (float*)this; }

	void print() {
		rows[0].print();
		rows[1].print();
		rows[2].print();
		rows[3].print();
	}
};

inline vec4 operator*(const vec4& v, const mat4& mat) {
	return v[0] * mat[0] + v[1] * mat[1] + v[2] * mat[2] + v[3] * mat[3];
}

inline mat4 operator*(const mat4& left, const mat4& right) {
	mat4 result;
	for (int i = 0; i < 4; i++) result.rows[i] = left.rows[i] * right;
	return result;
}

inline mat4 TranslateMatrix(vec3 t) {
	return mat4(
		vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(t.x, t.y, t.z, 1));
}

inline mat4 ScaleMatrix(vec3 s) {
	return mat4(
		vec4(s.x, 0, 0, 0),
		vec4(0, s.y, 0, 0),
		vec4(0, 0, s.z, 0),
		vec4(0, 0, 0, 1));
}

inline mat4 RotationMatrix(float angle, vec3 w) {
	float c = cosf(angle), s = sinf(angle);
	w = normalize(w);
	return mat4(vec4(c * (1 - w.x * w.x) + w.x * w.x, w.x * w.y * (1 - c) + w.z * s, w.x * w.z * (1 - c) - w.y * s, 0),
		vec4(w.x * w.y * (1 - c) - w.z * s, c * (1 - w.y * w.y) + w.y * w.y, w.y * w.z * (1 - c) + w.x * s, 0),
		vec4(w.x * w.z * (1 - c) + w.y * s, w.y * w.z * (1 - c) - w.x * s, c * (1 - w.z * w.z) + w.z * w.z, 0),
		vec4(0, 0, 0, 1));
}

inline mat4 RotationMatrix(vec3 xyz) {
	return 
		RotationMatrix(xyz.x, vec3(1, 0, 0)) * 
		RotationMatrix(xyz.y, vec3(0, 1, 0)) * 
		RotationMatrix(xyz.z, vec3(0, 0, 1));
}

inline mat4 quickInverse(mat4& m) // Rotation and Translation matrix only
{
	mat4 inv;
	inv[0][0] = m[0][0]; inv[0][1] = m[1][0]; inv[0][2] = m[2][0]; inv[0][3] = 0.0f;
	inv[1][0] = m[0][1]; inv[1][1] = m[1][1]; inv[1][2] = m[2][1]; inv[1][3] = 0.0f;
	inv[2][0] = m[0][2]; inv[2][1] = m[1][2]; inv[2][2] = m[2][2]; inv[2][3] = 0.0f;
	inv[3][0] = -(m[3][0] * inv[0][0] + m[3][1] * inv[1][0] + m[3][2] * inv[2][0]);
	inv[3][1] = -(m[3][0] * inv[0][1] + m[3][1] * inv[1][1] + m[3][2] * inv[2][1]);
	inv[3][2] = -(m[3][0] * inv[0][2] + m[3][1] * inv[1][2] + m[3][2] * inv[2][2]);
	inv[3][3] = 1.0f;
	return inv;
}

inline vec3 applyTransform(vec3 t, mat4 tsf, float w = 1) { // w = 0 for vector, w = 1 for point
	vec4 t4 = vec4(t.x, t.y, t.z, w);
	t4 = t4 * tsf;
	if (t4.w != 0) {
		t4.x /= t4.w;
		t4.y /= t4.w;
		t4.z /= t4.w;
	}
	return vec3(t4.x, t4.y, t4.z);
}
inline mat4 PointAtMatrix(vec3& pos, vec3& target, vec3& up) // camera matrix is the inverse of this matrix
{
	const vec3 newForward = normalize(target - pos);
	const vec3 projected = newForward * dot(up, newForward);
	const vec3 newUp = normalize(up - projected);
	const vec3 newRight = cross(newUp, newForward);
	mat4 matrix;
	matrix[0][0] = newRight.x;		matrix[0][1] = newRight.y;		matrix[0][2] = newRight.z;		matrix[0][3] = 0.0f; // i
	matrix[1][0] = newUp.x;			matrix[1][1] = newUp.y;			matrix[1][2] = newUp.z;			matrix[1][3] = 0.0f; // j
	matrix[2][0] = newForward.x;	matrix[2][1] = newForward.y;	matrix[2][2] = newForward.z;	matrix[2][3] = 0.0f; // k
	matrix[3][0] = pos.x;			matrix[3][1] = pos.y;			matrix[3][2] = pos.z;			matrix[3][3] = 1.0f; // (0,0,0)
	return matrix;
}
inline mat4 ProjectionMatrix(float fov, float fp, float bp, float windowWidth, float windowHeight) {
	const float aspectRatio = (float)windowHeight / (float)windowWidth; // choose aspect ratio dynamically
	return mat4(
		aspectRatio * fov, 0, 0, 0,
		0, fov, 0, 0,
		0, 0, bp / (bp - fp), 1.0f,
		0, 0, (-bp * fp) / (bp - fp), 0
	);
}
#endif
