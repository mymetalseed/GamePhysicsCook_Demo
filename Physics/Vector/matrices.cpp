#include "Compare.h"
#include "matrices.h"
#include <cmath>
#include <cfloat>
#include <iostream>

bool operator==(const mat2& l, const mat2& r) {
	for (int i = 0; i < /* 2 * 2 = */4; ++i) {
		if (!CMP(l.asArray[i], r.asArray[i])) {
			return false;
		}
	}
	return true;
}

bool operator==(const mat3& l, const mat3& r) {
	for (int i = 0; i < /* 3 * 3 = */ 9; ++i) {
		if (!CMP(l.asArray[i], r.asArray[i])) {
			return false;
		}
	}
	return true;
}
bool operator==(const mat4& l, const mat4& r) {
	for (int i = 0; i < /* 4 * 4 = */ 16; ++i) {
		if (!CMP(l.asArray[i], r.asArray[i])) {
			return false;
		}
	}
	return true;
}

bool operator!=(const mat2& l, const mat2& r) {
	return !(l == r);
}

bool operator!=(const mat3& l, const mat3& r) {
	return !(l == r);
}

bool operator!=(const mat4& l, const mat4& r) {
	return !(l == r);
}

std::ostream& operator<<(std::ostream& os, const mat2& m) {
	os << m._11 << ", " << m._12 << "\n";
	os << m._21 << ", " << m._22;
	return os;
}

std::ostream& operator<<(std::ostream& os, const mat3& m) {
	os << m._11 << ", " << m._12 << ", " << m._13 << "\n";
	os << m._21 << ", " << m._22 << ", " << m._23 << "\n";
	os << m._31 << ", " << m._32 << ", " << m._33;
	return os;
}

std::ostream& operator<<(std::ostream& os, const mat4& m) {
	os << m._11 << ", " << m._12 << ", " << m._13 << ", " << m._14 << "\n";
	os << m._21 << ", " << m._22 << ", " << m._23 << ", " << m._24 << "\n";
	os << m._31 << ", " << m._32 << ", " << m._33 << ", " << m._34 << "\n";
	os << m._41 << ", " << m._42 << ", " << m._43 << ", " << m._44;
	return os;
}

mat3 FastInverse(const mat3& mat) {
	return Transpose(mat);
}

//?
mat4 FastInverse(const mat4& mat) {
	mat4 inverse = Transpose(mat);
	inverse._41 = inverse._14 = 0.0f;
	inverse._42 = inverse._24 = 0.0f;
	inverse._43 = inverse._34 = 0.0f;

	vec3 right = vec3(mat._11, mat._12, mat._13);
	vec3 up = vec3(mat._21, mat._22, mat._23);
	vec3 forward = vec3(mat._31, mat._32, mat._33);
	vec3 position = vec3(mat._41, mat._42, mat._43);

	inverse._41 = -Dot(right, position);
	inverse._42 = -Dot(up, position);
	inverse._43 = -Dot(forward, position);

	return inverse;
}

void Transpose(const float* srcMat, float* dstMat, int srcRows, int srcCols) {
	for (int i = 0; i < srcRows * srcCols; ++i) {
		int row = i / srcRows;
		int col = i % srcRows;
		dstMat[i] = srcMat[srcCols * col + row];
	}
}

mat2 Transpose(const mat2& matrix) {
	mat2 result;
	Transpose(matrix.asArray, result.asArray, 2, 2);
	return result;
}

mat3 Transpose(const mat3& matrix) {
	mat3 result;
	Transpose(matrix.asArray, result.asArray, 3, 3);
	return result;
}

mat4 Transpose(const mat4& matrix) {
	mat4 result;
	Transpose(matrix.asArray, result.asArray, 4, 4);
	return result;
}

mat2 operator*(const mat2& matrix, float scalar) {
	mat2 result;
	for (int i = 0; i < 4; ++i) {
		result.asArray[i] = matrix.asArray[i] * scalar;
	}

	return result;
}

mat3 operator*(const mat3& matrix, float scalar) {
	mat3 result;
	for (int i = 0; i < 9; ++i) {
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

mat4 operator*(const mat4& matrix, float scalar) {
	mat4 result;
	for (int i = 0; i < 16; ++i) {
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

bool Multiply(float* out, const float* matA, int aRows, int aCols, const float* matB, int bRows, int bCols) {
	if (aCols != bRows) {
		return false;
	}

	for (int i = 0; i < aRows; ++i) {
		for (int j = 0; j < bCols; ++j) {
			out[bCols * i + j] = 0.0f;
			for (int k = 0; k < bRows; ++k) {
				out[bCols * i + j] += matA[aCols * i + k] * matB[bCols * k + j];
			}
		}
	}

	return true;
}

mat2 operator*(const mat2& matrixA, const mat2& matrixB) {
	mat2 result;
	Multiply(result.asArray, matrixA.asArray, 2, 2, matrixB.asArray, 2, 2);
	return result;
}

mat3 operator*(const mat3& matrixA, const mat3& matrixB) {
	mat3 result;
	Multiply(result.asArray, matrixA.asArray, 3, 3, matrixB.asArray, 3, 3);
	return result;
}

mat4 operator*(const mat4& matrixA, const mat4& matrixB) {
	mat4 result;
	Multiply(result.asArray, matrixA.asArray, 4, 4, matrixB.asArray, 4, 4);
	return result;
}

float Determinant(const mat2& matrix) {
	return matrix._11 * matrix._22 - matrix._12 * matrix._21;
}

float Determinant(const mat3& mat) {
	float result = 0.0f;

	mat3 cofactor = Cofactor(mat);
	for (int j = 0; j < 3; ++j) {
		result += mat.asArray[3 * 0 + j] * cofactor[0][j];
	}

	return result;
}

mat2 Cut(const mat3& mat, int row, int col) {
	mat2 result;
	int index = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == row || j == col) {
				continue;
			}
			result.asArray[index++] = mat.asArray[3 * i + j];
		}
	}

	return result;
}

mat3 Cut(const mat4& mat, int row, int col) {
	mat3 result;
	int index = 0;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == row || j == col) {
				continue;
			}
			result.asArray[index++] = mat.asArray[4 * i + j];
		}
	}

	return result;
}

//余子式
mat3 Minor(const mat3& mat) {
	mat3 result;
	//去掉第i行第j列后剩下的行列式
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result[i][j] = Determinant(Cut(mat, i, j));
		}
	}

	return result;
}

mat2 Minor(const mat2& mat) {
	return mat2(
		mat._22, mat._21,
		mat._12, mat._11
	);
}

//代数余子式
void Cofactor(float* out, const float* minor, int rows, int cols)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			out[cols * j + i] = minor[cols * j + i] * powf(-1.0f, i + j);
		}
	}
}

mat2 Cofactor(const mat2& mat) {
	mat2 result;
	Cofactor(result.asArray, Minor(mat).asArray, 2, 2);
	return result;
}

mat3 Cofactor(const mat3& mat) {
	mat3 result;
	Cofactor(result.asArray, Minor(mat).asArray, 3, 3);
	return result;
}

//伴随矩阵
mat2 Adjugate(const mat2& mat) {
	return Transpose(Cofactor(mat));
}

mat3 Adjugate(const mat3& mat) {
	return Transpose(Cofactor(mat));
}

mat4 Adjugate(const mat4& mat) {
	return Transpose(Cofactor(mat));
}



