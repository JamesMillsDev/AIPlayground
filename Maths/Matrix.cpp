#include "pch.h"
#include "Matrix.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace Maths
{
	namespace
	{
		/**
		 * @brief Tolerance below which a determinant is treated as zero.
		 *
		 * Matrices with a determinant smaller than this are considered singular
		 * and invert to the zero matrix rather than producing huge garbage values.
		 */
		constexpr float SINGULAR_EPSILON = 1e-8f;

		/**
		 * @brief Reads an element of a Matrix4 using row/column (mathematical) order.
		 *
		 * Storage is column-major, so this is just a readability wrapper around
		 * the transposed-looking `m[col][row]` indexing.
		 * @param mat The matrix to read from.
		 * @param row The row index (0 to 3).
		 * @param col The column index (0 to 3).
		 * @return The element at (@p row, @p col).
		 */
		float At(const Matrix4& mat, const int row, const int col)
		{
			return mat.m[col][row];
		}

		/**
		 * @brief Computes the 3x3 minor of a Matrix4.
		 * @param mat The matrix.
		 * @param skipRow The row to delete.
		 * @param skipCol The column to delete.
		 * @return The determinant of the remaining 3x3 submatrix.
		 */
		float Minor3(const Matrix4& mat, const int skipRow, const int skipCol)
		{
			float sub[3][3];

			for (int row = 0, subRow = 0; row < 4; ++row)
			{
				if (row == skipRow)
				{
					continue;
				}

				for (int col = 0, subCol = 0; col < 4; ++col)
				{
					if (col == skipCol)
					{
						continue;
					}

					sub[subRow][subCol] = At(mat, row, col);
					++subCol;
				}

				++subRow;
			}

			return sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1])
				- sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0])
				+ sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);
		}

		/**
		 * @brief Computes a cofactor of a Matrix4.
		 * @param mat The matrix.
		 * @param row The row of the element the cofactor belongs to.
		 * @param col The column of the element the cofactor belongs to.
		 * @return The signed minor for (@p row, @p col).
		 */
		float Cofactor(const Matrix4& mat, const int row, const int col)
		{
			const float minor = Minor3(mat, row, col);
			return ((row + col) % 2 == 0) ? minor : -minor;
		}
	}

	// The unions rely on the column structs and the float arrays occupying the
	// same bytes, which only holds if the vectors are plain packed floats.
	static_assert(sizeof(Vector3) == 3 * sizeof(float),
		"Vector3 must be tightly packed for Matrix3's union to alias correctly.");
	static_assert(sizeof(Vector4) == 4 * sizeof(float),
		"Vector4 must be tightly packed for Matrix4's union to alias correctly.");
	static_assert(sizeof(Matrix3) == 9 * sizeof(float),
		"Matrix3 must be exactly 9 floats to match glm::mat3's layout.");
	static_assert(sizeof(Matrix4) == 16 * sizeof(float),
		"Matrix4 must be exactly 16 floats to match glm::mat4's layout.");

#pragma region Matrix3

	Matrix3 Matrix3::Identity()
	{
		return Matrix3(1.0f);
	}

	Matrix3 Matrix3::Zero()
	{
		return Matrix3(0.0f);
	}

	float Matrix3::Determinant(const Matrix3& m)
	{
		// a<row><col> naming, remembering that storage is m[col][row].
		const float a00 = m.m[0][0], a01 = m.m[1][0], a02 = m.m[2][0];
		const float a10 = m.m[0][1], a11 = m.m[1][1], a12 = m.m[2][1];
		const float a20 = m.m[0][2], a21 = m.m[1][2], a22 = m.m[2][2];

		return a00 * (a11 * a22 - a12 * a21)
			- a01 * (a10 * a22 - a12 * a20)
			+ a02 * (a10 * a21 - a11 * a20);
	}

	Matrix3 Matrix3::Transpose(const Matrix3& m)
	{
		Matrix3 result;

		for (int col = 0; col < 3; ++col)
		{
			for (int row = 0; row < 3; ++row)
			{
				result.m[col][row] = m.m[row][col];
			}
		}

		return result;
	}

	Matrix3 Matrix3::Inverse(const Matrix3& m)
	{
		const float det = Determinant(m);

		if (std::fabs(det) < SINGULAR_EPSILON)
		{
			return Zero();
		}

		const float a00 = m.m[0][0], a01 = m.m[1][0], a02 = m.m[2][0];
		const float a10 = m.m[0][1], a11 = m.m[1][1], a12 = m.m[2][1];
		const float a20 = m.m[0][2], a21 = m.m[1][2], a22 = m.m[2][2];

		// Cofactor matrix; c<row><col> is the signed minor of element (row, col).
		const float c00 = a11 * a22 - a12 * a21;
		const float c01 = -(a10 * a22 - a12 * a20);
		const float c02 = a10 * a21 - a11 * a20;
		const float c10 = -(a01 * a22 - a02 * a21);
		const float c11 = a00 * a22 - a02 * a20;
		const float c12 = -(a00 * a21 - a01 * a20);
		const float c20 = a01 * a12 - a02 * a11;
		const float c21 = -(a00 * a12 - a02 * a10);
		const float c22 = a00 * a11 - a01 * a10;

		const float invDet = 1.0f / det;

		// The inverse is the adjugate (transposed cofactor matrix) over the
		// determinant, so the cofactors are fed in transposed here.
		return
		{
			c00 * invDet, c10 * invDet, c20 * invDet,
			c01 * invDet, c11 * invDet, c21 * invDet,
			c02 * invDet, c12 * invDet, c22 * invDet
		};
	}

	Matrix3 Matrix3::Multiply(const Matrix3& lhs, const Matrix3& rhs)
	{
		Matrix3 result = Zero();

		for (int col = 0; col < 3; ++col)
		{
			for (int row = 0; row < 3; ++row)
			{
				float sum = 0.0f;

				for (int k = 0; k < 3; ++k)
				{
					sum += lhs.m[k][row] * rhs.m[col][k];
				}

				result.m[col][row] = sum;
			}
		}

		return result;
	}

	Matrix3::Matrix3() :
		values{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
	}
	{}

	Matrix3::Matrix3(float scalar) :
		values{
			scalar, 0.0f, 0.0f,
			0.0f, scalar, 0.0f,
			0.0f, 0.0f, scalar
	}
	{}

	Matrix3::Matrix3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22) :
		values{
				m00, m10, m20,
				m01, m11, m21,
				m02, m12, m22
		}
	{}

	Matrix3::Matrix3(const Vector3& col0, const Vector3& col1, const Vector3& col2) :
		values{
			col0.x, col0.y, col0.z,
			col1.x, col1.y, col1.z,
			col2.x, col2.y, col2.z
		}
	{}

	Matrix3::Matrix3(glm::mat3 mat) :
		values{
			mat[0][0], mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]
		}
	{}

	Matrix3::Matrix3(glm::mat4 mat) :
		values{
			mat[0][0], mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]
		}
	{}

	Matrix3::Matrix3(const Matrix4& mat) :
		values{
			mat.m[0][0], mat.m[0][1], mat.m[0][2],
			mat.m[1][0], mat.m[1][1], mat.m[1][2],
			mat.m[2][0], mat.m[2][1], mat.m[2][2]
		}
	{}

	Matrix3::Matrix3(const Matrix3& rhs)
	{
		std::ranges::copy(rhs.values, std::begin(values));
	}

	Matrix3::Matrix3(Matrix3&& rhs) noexcept
	{
		// Floats own nothing, so moving is just copying.
		std::ranges::copy(rhs.values, std::begin(values));
	}

	float Matrix3::Determinant() const
	{
		return Determinant(*this);
	}

	void Matrix3::Transpose()
	{
		std::swap(m[0][1], m[1][0]);
		std::swap(m[0][2], m[2][0]);
		std::swap(m[1][2], m[2][1]);
	}

	Matrix3 Matrix3::Transposed() const
	{
		return Transpose(*this);
	}

	void Matrix3::Invert()
	{
		*this = Inverse(*this);
	}

	Matrix3 Matrix3::Inverted() const
	{
		return Inverse(*this);
	}

	bool Matrix3::IsApproximatelyEqual(const Matrix3& rhs, const float e) const
	{
		for (int i = 0; i < 9; ++i)
		{
			if (std::fabs(values[i] - rhs.values[i]) > e)
			{
				return false;
			}
		}

		return true;
	}

	std::ostream& operator<<(std::ostream& stream, const Matrix3& mat)
	{
		// Printed row by row so the output reads like the maths, not the storage.
		for (int row = 0; row < 3; ++row)
		{
			stream << "[ ";

			for (int col = 0; col < 3; ++col)
			{
				stream << mat.m[col][row];

				if (col < 2)
				{
					stream << ", ";
				}
			}

			stream << " ]";

			if (row < 2)
			{
				stream << '\n';
			}
		}

		return stream;
	}

	Matrix3::operator glm::mat3() const
	{
		glm::mat3 result;

		for (int col = 0; col < 3; ++col)
		{
			for (int row = 0; row < 3; ++row)
			{
				result[col][row] = m[col][row];
			}
		}

		return result;
	}

	Matrix3& Matrix3::operator=(const Matrix3& rhs)
	{
		if (this != &rhs)
		{
			std::ranges::copy(rhs.values, std::begin(values));
		}

		return *this;
	}

	Matrix3 Matrix3::operator-() const
	{
		Matrix3 result;

		for (int i = 0; i < 9; ++i)
		{
			result.values[i] = -values[i];
		}

		return result;
	}

	bool Matrix3::operator==(const Matrix3& rhs) const
	{
		return IsApproximatelyEqual(rhs);
	}

	bool Matrix3::operator!=(const Matrix3& rhs) const
	{
		return !(*this == rhs);
	}

	bool Matrix3::operator<(const Matrix3& rhs) const
	{
		float lhsSum = 0.0f;
		float rhsSum = 0.0f;

		for (int i = 0; i < 9; ++i)
		{
			lhsSum += values[i] * values[i];
			rhsSum += rhs.values[i] * rhs.values[i];
		}

		return lhsSum < rhsSum;
	}

	Matrix3 Matrix3::operator+(const Matrix3& rhs) const
	{
		Matrix3 result;

		for (int i = 0; i < 9; ++i)
		{
			result.values[i] = values[i] + rhs.values[i];
		}

		return result;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			values[i] += rhs.values[i];
		}

		return *this;
	}

	Matrix3 Matrix3::operator-(const Matrix3& rhs) const
	{
		Matrix3 result;

		for (int i = 0; i < 9; ++i)
		{
			result.values[i] = values[i] - rhs.values[i];
		}

		return result;
	}

	Matrix3& Matrix3::operator-=(const Matrix3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			values[i] -= rhs.values[i];
		}

		return *this;
	}

	Matrix3 Matrix3::operator*(const Matrix3& rhs) const
	{
		return Multiply(*this, rhs);
	}

	Matrix3& Matrix3::operator*=(const Matrix3& rhs)
	{
		*this = Multiply(*this, rhs);
		return *this;
	}

	Vector3 Matrix3::operator*(const Vector3& rhs) const
	{
		return
		{
			m[0][0] * rhs.x + m[1][0] * rhs.y + m[2][0] * rhs.z,
			m[0][1] * rhs.x + m[1][1] * rhs.y + m[2][1] * rhs.z,
			m[0][2] * rhs.x + m[1][2] * rhs.y + m[2][2] * rhs.z
		};
	}

	Matrix3 Matrix3::operator*(const float rhs) const
	{
		Matrix3 result;

		for (int i = 0; i < 9; ++i)
		{
			result.values[i] = values[i] * rhs;
		}

		return result;
	}

	Matrix3& Matrix3::operator*=(const float rhs)
	{
		for (float& value : values)
		{
			value *= rhs;
		}

		return *this;
	}

	Matrix3 Matrix3::operator/(const float rhs) const
	{
		// Reciprocate once rather than dividing nine times.
		const float inv = 1.0f / rhs;
		return *this * inv;
	}

	Matrix3& Matrix3::operator/=(const float rhs)
	{
		const float inv = 1.0f / rhs;
		return *this *= inv;
	}

	Vector3& Matrix3::operator[](const int index)
	{
		// The union guarantees col1/col2/col3 sit contiguously over values[].
		return (&col1)[index];
	}

	const Vector3& Matrix3::operator[](const int index) const
	{
		return (&col1)[index];
	}

#pragma endregion

#pragma region Matrix4

	Matrix4 Matrix4::Identity()
	{
		return Matrix4(1.0f);
	}

	Matrix4 Matrix4::Zero()
	{
		return Matrix4(0.0f);
	}

	float Matrix4::Determinant(const Matrix4& m)
	{
		// Cofactor expansion along the first row.
		float det = 0.0f;

		for (int col = 0; col < 4; ++col)
		{
			det += At(m, 0, col) * Cofactor(m, 0, col);
		}

		return det;
	}

	Matrix4 Matrix4::Transpose(const Matrix4& m)
	{
		Matrix4 result;

		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				result.m[col][row] = m.m[row][col];
			}
		}

		return result;
	}

	Matrix4 Matrix4::Inverse(const Matrix4& m)
	{
		const float det = Determinant(m);

		if (std::fabs(det) < SINGULAR_EPSILON)
		{
			return Zero();
		}

		const float invDet = 1.0f / det;

		Matrix4 result;

		// inverse(row, col) = cofactor(col, row) / det, i.e. the adjugate is the
		// transposed cofactor matrix. Storage is m[col][row], so the indices below
		// end up looking un-swapped even though the transpose is happening.
		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				result.m[col][row] = Cofactor(m, col, row) * invDet;
			}
		}

		return result;
	}

	Matrix4 Matrix4::Multiply(const Matrix4& lhs, const Matrix4& rhs)
	{
		Matrix4 result = Zero();

		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				float sum = 0.0f;

				for (int k = 0; k < 4; ++k)
				{
					sum += lhs.m[k][row] * rhs.m[col][k];
				}

				result.m[col][row] = sum;
			}
		}

		return result;
	}

	Matrix4::Matrix4() :
		values{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	}
	{}

	Matrix4::Matrix4(float scalar) :
		values{
			scalar, 0.0f, 0.0f, 0.0f,
			0.0f, scalar, 0.0f, 0.0f,
			0.0f, 0.0f, scalar, 0.0f,
			0.0f, 0.0f, 0.0f, scalar
	}
	{}

	Matrix4::Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
		// Row-major arguments, column-major storage.
		:
		values{
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33
		}
	{}

	Matrix4::Matrix4(glm::mat4 mat) :
		values{
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]
	}
	{}

	Matrix4::Matrix4(const Matrix3& mat)
		: values{
			mat.m[0][0], mat.m[0][1], mat.m[0][2], 0.0f,
			mat.m[1][0], mat.m[1][1], mat.m[1][2], 0.0f,
			mat.m[2][0], mat.m[2][1], mat.m[2][2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		}
	{}

	Matrix4::Matrix4(const Matrix4& rhs)
	{
		std::ranges::copy(rhs.values, std::begin(values));
	}

	Matrix4::Matrix4(Matrix4&& rhs) noexcept
	{
		std::ranges::copy(rhs.values, std::begin(values));
	}

	float Matrix4::Determinant() const
	{
		return Determinant(*this);
	}

	void Matrix4::Transpose()
	{
		for (int col = 0; col < 4; ++col)
		{
			for (int row = col + 1; row < 4; ++row)
			{
				std::swap(m[col][row], m[row][col]);
			}
		}
	}

	Matrix4 Matrix4::Transposed() const
	{
		return Transpose(*this);
	}

	void Matrix4::Invert()
	{
		*this = Inverse(*this);
	}

	Matrix4 Matrix4::Inverted() const
	{
		return Inverse(*this);
	}

	bool Matrix4::IsApproximatelyEqual(const Matrix4& rhs, const float e) const
	{
		for (int i = 0; i < 16; ++i)
		{
			if (std::fabs(values[i] - rhs.values[i]) > e)
			{
				return false;
			}
		}

		return true;
	}

	std::ostream& operator<<(std::ostream& stream, const Matrix4& mat)
	{
		for (int row = 0; row < 4; ++row)
		{
			stream << "[ ";

			for (int col = 0; col < 4; ++col)
			{
				stream << mat.m[col][row];

				if (col < 3)
				{
					stream << ", ";
				}
			}

			stream << " ]";

			if (row < 3)
			{
				stream << '\n';
			}
		}

		return stream;
	}

	Matrix4::operator glm::mat4() const
	{
		glm::mat4 result;

		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				result[col][row] = m[col][row];
			}
		}

		return result;
	}

	Matrix4& Matrix4::operator=(const Matrix4& rhs)
	{
		if (this != &rhs)
		{
			std::ranges::copy(rhs.values, std::begin(values));
		}

		return *this;
	}

	Matrix4 Matrix4::operator-() const
	{
		Matrix4 result;

		for (int i = 0; i < 16; ++i)
		{
			result.values[i] = -values[i];
		}

		return result;
	}

	bool Matrix4::operator==(const Matrix4& rhs) const
	{
		return IsApproximatelyEqual(rhs);
	}

	bool Matrix4::operator!=(const Matrix4& rhs) const
	{
		return !(*this == rhs);
	}

	bool Matrix4::operator<(const Matrix4& rhs) const
	{
		float lhsSum = 0.0f;
		float rhsSum = 0.0f;

		for (int i = 0; i < 16; ++i)
		{
			lhsSum += values[i] * values[i];
			rhsSum += rhs.values[i] * rhs.values[i];
		}

		return lhsSum < rhsSum;
	}

	Matrix4 Matrix4::operator+(const Matrix4& rhs) const
	{
		Matrix4 result;

		for (int i = 0; i < 16; ++i)
		{
			result.values[i] = values[i] + rhs.values[i];
		}

		return result;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& rhs)
	{
		for (int i = 0; i < 16; ++i)
		{
			values[i] += rhs.values[i];
		}

		return *this;
	}

	Matrix4 Matrix4::operator-(const Matrix4& rhs) const
	{
		Matrix4 result;

		for (int i = 0; i < 16; ++i)
		{
			result.values[i] = values[i] - rhs.values[i];
		}

		return result;
	}

	Matrix4& Matrix4::operator-=(const Matrix4& rhs)
	{
		for (int i = 0; i < 16; ++i)
		{
			values[i] -= rhs.values[i];
		}

		return *this;
	}

	Matrix4 Matrix4::operator*(const Matrix4& rhs) const
	{
		return Multiply(*this, rhs);
	}

	Matrix4& Matrix4::operator*=(const Matrix4& rhs)
	{
		*this = Multiply(*this, rhs);
		return *this;
	}

	Vector3 Matrix4::operator*(const Vector3& rhs) const
	{
		// Implicit w = 1, and the resulting w is dropped rather than divided by,
		// so this is only correct for affine transforms.
		return 
		{
			m[0][0] * rhs.x + m[1][0] * rhs.y + m[2][0] * rhs.z + m[3][0],
			m[0][1] * rhs.x + m[1][1] * rhs.y + m[2][1] * rhs.z + m[3][1],
			m[0][2] * rhs.x + m[1][2] * rhs.y + m[2][2] * rhs.z + m[3][2]
		};
	}

	Matrix4 Matrix4::operator*(const float rhs) const
	{
		Matrix4 result;

		for (int i = 0; i < 16; ++i)
		{
			result.values[i] = values[i] * rhs;
		}

		return result;
	}

	Matrix4& Matrix4::operator*=(const float rhs)
	{
		for (float& value : values)
		{
			value *= rhs;
		}

		return *this;
	}

	Matrix4 Matrix4::operator/(const float rhs) const
	{
		const float inv = 1.0f / rhs;
		return *this * inv;
	}

	Matrix4& Matrix4::operator/=(const float rhs)
	{
		const float inv = 1.0f / rhs;
		return *this *= inv;
	}

	Vector4& Matrix4::operator[](const int index)
	{
		return (&col1)[index];
	}

	const Vector4& Matrix4::operator[](const int index) const
	{
		return (&col1)[index];
	}

#pragma endregion
}
