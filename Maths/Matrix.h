#pragma once

#include <ostream>

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "Vector.h"

namespace Maths
{
	struct Matrix4;

	/**
	 * @brief A 3x3 matrix, commonly used for linear transforms (rotation/scale) of Vector3.
	 *
	 * Storage is column-major, matching glm's memory layout: `values[col * 3 + row]`.
	 * `m[col][row]` and `operator[](col)` both index by column first, exactly like
	 * `glm::mat3`, which makes conversion to/from glm a direct element-for-element copy.
	 */
	struct Matrix3
	{
	public:
		/**
		 * @brief Returns the 3x3 identity matrix.
		 * @return The identity matrix.
		 */
		static Matrix3 Identity();

		/**
		 * @brief Returns the 3x3 zero matrix.
		 * @return A matrix with every element set to 0.
		 */
		static Matrix3 Zero();

		/**
		 * @brief Computes the determinant of a matrix.
		 * @param m The matrix.
		 * @return The determinant of @p m.
		 */
		static float Determinant(const Matrix3& m);

		/**
		 * @brief Computes the transpose of a matrix.
		 * @param m The matrix to transpose.
		 * @return A copy of @p m with rows and columns swapped.
		 */
		static Matrix3 Transpose(const Matrix3& m);

		/**
		 * @brief Computes the inverse of a matrix.
		 * @param m The matrix to invert.
		 * @return The inverse of @p m, or the zero matrix if @p m is singular.
		 */
		static Matrix3 Inverse(const Matrix3& m);

		/**
		 * @brief Multiplies two matrices.
		 * @param lhs The left-hand matrix.
		 * @param rhs The right-hand matrix.
		 * @return The matrix product `lhs * rhs`.
		 */
		static Matrix3 Multiply(const Matrix3& lhs, const Matrix3& rhs);

	public:
		/**
		 * @brief The matrix's elements.
		 *
		 * All members alias the same underlying nine floats: the three column
		 * vectors, the `values` array, and the 2D `m` array are three different
		 * views onto the same data. Storage is column-major, so `values[col * 3 + row]`
		 * and `m[col][row]` both name the element at (row, col).
		 */
		union
		{
			struct
			{
				Vector3 col1; ///< The first column, matching `m[0]` and `(*this)[0]`.
				Vector3 col2; ///< The second column, matching `m[1]` and `(*this)[1]`.
				Vector3 col3; ///< The third column, matching `m[2]` and `(*this)[2]`.
			};

			float values[9]; ///< Flat array-indexed view of the elements, in column-major order.
			float m[3][3];   ///< Column-then-row indexed view of the elements: `m[col][row]`.
		};

	public:
		/**
		 * @brief Constructs the identity matrix.
		 */
		Matrix3();

		/**
		 * @brief Constructs a diagonal matrix with @p scalar on the diagonal.
		 * @param scalar The value placed on the diagonal; all other elements are 0.
		 */
		explicit Matrix3(float scalar);

		/**
		 * @brief Constructs a matrix from individual elements, given in row-major
		 *        reading order (element names are `m<row><col>`).
		 * @param m00 Row 0, column 0.
		 * @param m01 Row 0, column 1.
		 * @param m02 Row 0, column 2.
		 * @param m10 Row 1, column 0.
		 * @param m11 Row 1, column 1.
		 * @param m12 Row 1, column 2.
		 * @param m20 Row 2, column 0.
		 * @param m21 Row 2, column 1.
		 * @param m22 Row 2, column 2.
		 */
		Matrix3(float m00, float m01, float m02,
		        float m10, float m11, float m12,
		        float m20, float m21, float m22);

		/**
		 * @brief Constructs a matrix from three column vectors.
		 * @param col0 The first column.
		 * @param col1 The second column.
		 * @param col2 The third column.
		 */
		Matrix3(const Vector3& col0, const Vector3& col1, const Vector3& col2);

		/**
		 * @brief Constructs a matrix from a glm 3x3 matrix.
		 * @param mat The glm matrix to copy from.
		 */
		Matrix3(glm::mat3 mat);

		/**
		 * @brief Constructs a matrix from the upper-left 3x3 of a glm 4x4 matrix.
		 * @param mat The glm matrix to extract from.
		 */
		explicit Matrix3(glm::mat4 mat);

		/**
		 * @brief Constructs a matrix from the upper-left 3x3 of a Matrix4.
		 * @param mat The matrix to extract from.
		 */
		explicit Matrix3(const Matrix4& mat);

		/**
		 * @brief Copy constructor.
		 * @param rhs The matrix to copy.
		 */
		Matrix3(const Matrix3& rhs);

		/**
		 * @brief Move constructor.
		 * @param rhs The matrix to move from.
		 */
		Matrix3(Matrix3&& rhs) noexcept;

		/**
		 * @brief Default destructor.
		 */
		~Matrix3() = default;

	public:
		/**
		 * @brief Computes the determinant of this matrix.
		 * @return The determinant.
		 */
		float Determinant() const;

		/**
		 * @brief Transposes this matrix in place.
		 */
		void Transpose();

		/**
		 * @brief Returns a transposed copy of this matrix.
		 * @return A copy of this matrix with rows and columns swapped.
		 */
		Matrix3 Transposed() const;

		/**
		 * @brief Inverts this matrix in place.
		 *
		 * If this matrix is singular, it is set to the zero matrix.
		 */
		void Invert();

		/**
		 * @brief Returns an inverted copy of this matrix.
		 * @return The inverse of this matrix, or the zero matrix if it is singular.
		 */
		Matrix3 Inverted() const;

		/**
		 * @brief Checks whether this matrix is approximately equal to another.
		 * @param rhs The matrix to compare against.
		 * @param e The maximum allowed per-element difference.
		 * @return True if every element differs by no more than @p e.
		 */
		bool IsApproximatelyEqual(const Matrix3& rhs, float e = 1e-4f) const;

	public:
		/**
		 * @brief Writes a matrix to an output stream, row by row.
		 * @param stream The output stream to write to.
		 * @param mat The matrix to write.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Matrix3& mat);

		/**
		 * @brief Converts this matrix to a glm 3x3 matrix.
		 */
		operator glm::mat3() const;

		/**
		 * @brief Copy assignment operator.
		 * @param rhs The matrix to copy.
		 * @return A reference to this matrix.
		 */
		Matrix3& operator=(const Matrix3& rhs);

		/**
		 * @brief Returns the negation of this matrix.
		 * @return A matrix with every element negated.
		 */
		Matrix3 operator-() const;

		/**
		 * @brief Checks this matrix for approximate equality with another.
		 * @param rhs The matrix to compare against.
		 * @return True if the matrices are approximately equal.
		 */
		bool operator==(const Matrix3& rhs) const;

		/**
		 * @brief Checks this matrix for inequality with another.
		 * @param rhs The matrix to compare against.
		 * @return True if the matrices are not approximately equal.
		 */
		bool operator!=(const Matrix3& rhs) const;

		/**
		 * @brief Compares this matrix to another by sum of squared elements.
		 *
		 * There's no canonical ordering for matrices; this exists mainly to
		 * support sorted containers, analogous to Vector's magnitude-based
		 * `operator<`.
		 * @param rhs The matrix to compare against.
		 * @return True if this matrix's sum of squared elements is less than @p rhs's.
		 */
		bool operator<(const Matrix3& rhs) const;

		/**
		 * @brief Adds two matrices element-wise.
		 * @param rhs The matrix to add.
		 * @return The resulting sum matrix.
		 */
		Matrix3 operator+(const Matrix3& rhs) const;

		/**
		 * @brief Adds another matrix to this matrix in place, element-wise.
		 * @param rhs The matrix to add.
		 * @return A reference to this matrix.
		 */
		Matrix3& operator+=(const Matrix3& rhs);

		/**
		 * @brief Subtracts two matrices element-wise.
		 * @param rhs The matrix to subtract.
		 * @return The resulting difference matrix.
		 */
		Matrix3 operator-(const Matrix3& rhs) const;

		/**
		 * @brief Subtracts another matrix from this matrix in place, element-wise.
		 * @param rhs The matrix to subtract.
		 * @return A reference to this matrix.
		 */
		Matrix3& operator-=(const Matrix3& rhs);

		/**
		 * @brief Multiplies this matrix by another (matrix product, not element-wise).
		 * @param rhs The matrix to multiply by.
		 * @return The resulting matrix product.
		 */
		Matrix3 operator*(const Matrix3& rhs) const;

		/**
		 * @brief Multiplies this matrix by another in place (matrix product).
		 * @param rhs The matrix to multiply by.
		 * @return A reference to this matrix.
		 */
		Matrix3& operator*=(const Matrix3& rhs);

		/**
		 * @brief Transforms a column vector by this matrix.
		 * @param rhs The vector to transform.
		 * @return The transformed vector `(*this) * rhs`.
		 */
		Vector3 operator*(const Vector3& rhs) const;

		/**
		 * @brief Scales every element of this matrix by a scalar.
		 * @param rhs The scalar to multiply by.
		 * @return The resulting scaled matrix.
		 */
		Matrix3 operator*(float rhs) const;

		/**
		 * @brief Scales every element of this matrix by a scalar in place.
		 * @param rhs The scalar to multiply by.
		 * @return A reference to this matrix.
		 */
		Matrix3& operator*=(float rhs);

		/**
		 * @brief Divides every element of this matrix by a scalar.
		 * @param rhs The scalar to divide by.
		 * @return The resulting scaled matrix.
		 */
		Matrix3 operator/(float rhs) const;

		/**
		 * @brief Divides every element of this matrix by a scalar in place.
		 * @param rhs The scalar to divide by.
		 * @return A reference to this matrix.
		 */
		Matrix3& operator/=(float rhs);

		/**
		 * @brief Accesses a column by index.
		 * @param index The column index (0, 1, or 2).
		 * @return A reference to the requested column, aliased as a Vector3.
		 */
		Vector3& operator[](int index);

		/**
		 * @brief Accesses a column by index (const overload).
		 * @param index The column index (0, 1, or 2).
		 * @return A const reference to the requested column, aliased as a Vector3.
		 */
		const Vector3& operator[](int index) const;
	};

	/**
	 * @brief A 4x4 matrix, commonly used for affine/projective transforms.
	 *
	 * Storage is column-major, matching glm's memory layout: `values[col * 4 + row]`.
	 * `m[col][row]` and `operator[](col)` both index by column first, exactly like
	 * `glm::mat4`, which makes conversion to/from glm a direct element-for-element copy.
	 */
	struct Matrix4
	{
	public:
		/**
		 * @brief Returns the 4x4 identity matrix.
		 * @return The identity matrix.
		 */
		static Matrix4 Identity();

		/**
		 * @brief Returns the 4x4 zero matrix.
		 * @return A matrix with every element set to 0.
		 */
		static Matrix4 Zero();

		/**
		 * @brief Computes the determinant of a matrix.
		 * @param m The matrix.
		 * @return The determinant of @p m.
		 */
		static float Determinant(const Matrix4& m);

		/**
		 * @brief Computes the transpose of a matrix.
		 * @param m The matrix to transpose.
		 * @return A copy of @p m with rows and columns swapped.
		 */
		static Matrix4 Transpose(const Matrix4& m);

		/**
		 * @brief Computes the inverse of a matrix.
		 * @param m The matrix to invert.
		 * @return The inverse of @p m, or the zero matrix if @p m is singular.
		 */
		static Matrix4 Inverse(const Matrix4& m);

		/**
		 * @brief Multiplies two matrices.
		 * @param lhs The left-hand matrix.
		 * @param rhs The right-hand matrix.
		 * @return The matrix product `lhs * rhs`.
		 */
		static Matrix4 Multiply(const Matrix4& lhs, const Matrix4& rhs);

	public:
		/**
		 * @brief The matrix's elements.
		 *
		 * All members alias the same underlying sixteen floats: the four column
		 * vectors, the `values` array, and the 2D `m` array are three different
		 * views onto the same data. Storage is column-major, so `values[col * 4 + row]`
		 * and `m[col][row]` both name the element at (row, col).
		 */
		union
		{
			struct
			{
				Vector4 col1; ///< The first column, matching `m[0]` and `(*this)[0]`.
				Vector4 col2; ///< The second column, matching `m[1]` and `(*this)[1]`.
				Vector4 col3; ///< The third column, matching `m[2]` and `(*this)[2]`.
				Vector4 col4; ///< The fourth column, matching `m[3]` and `(*this)[3]`.
			};

			float values[16]; ///< Flat array-indexed view of the elements, in column-major order.
			float m[4][4];    ///< Column-then-row indexed view of the elements: `m[col][row]`.
		};

	public:
		/**
		 * @brief Constructs the identity matrix.
		 */
		Matrix4();

		/**
		 * @brief Constructs a diagonal matrix with @p scalar on the diagonal.
		 * @param scalar The value placed on the diagonal; all other elements are 0.
		 */
		explicit Matrix4(float scalar);

		/**
		 * @brief Constructs a matrix from individual elements, given in row-major
		 *        reading order (element names are `m<row><col>`).
		 * @param m00 Row 0, column 0.
		 * @param m01 Row 0, column 1.
		 * @param m02 Row 0, column 2.
		 * @param m03 Row 0, column 3.
		 * @param m10 Row 1, column 0.
		 * @param m11 Row 1, column 1.
		 * @param m12 Row 1, column 2.
		 * @param m13 Row 1, column 3.
		 * @param m20 Row 2, column 0.
		 * @param m21 Row 2, column 1.
		 * @param m22 Row 2, column 2.
		 * @param m23 Row 2, column 3.
		 * @param m30 Row 3, column 0.
		 * @param m31 Row 3, column 1.
		 * @param m32 Row 3, column 2.
		 * @param m33 Row 3, column 3.
		 */
		Matrix4(float m00, float m01, float m02, float m03,
		        float m10, float m11, float m12, float m13,
		        float m20, float m21, float m22, float m23,
		        float m30, float m31, float m32, float m33);

		/**
		 * @brief Constructs a matrix from a glm 4x4 matrix.
		 * @param mat The glm matrix to copy from.
		 */
		Matrix4(glm::mat4 mat);

		/**
		 * @brief Constructs a matrix by embedding a Matrix3 into the upper-left
		 *        3x3 of an identity matrix.
		 * @param mat The 3x3 matrix to embed.
		 */
		explicit Matrix4(const Matrix3& mat);

		/**
		 * @brief Copy constructor.
		 * @param rhs The matrix to copy.
		 */
		Matrix4(const Matrix4& rhs);

		/**
		 * @brief Move constructor.
		 * @param rhs The matrix to move from.
		 */
		Matrix4(Matrix4&& rhs) noexcept;

		/**
		 * @brief Default destructor.
		 */
		~Matrix4() = default;

	public:
		/**
		 * @brief Computes the determinant of this matrix.
		 * @return The determinant.
		 */
		float Determinant() const;

		/**
		 * @brief Transposes this matrix in place.
		 */
		void Transpose();

		/**
		 * @brief Returns a transposed copy of this matrix.
		 * @return A copy of this matrix with rows and columns swapped.
		 */
		Matrix4 Transposed() const;

		/**
		 * @brief Inverts this matrix in place.
		 *
		 * If this matrix is singular, it is set to the zero matrix.
		 */
		void Invert();

		/**
		 * @brief Returns an inverted copy of this matrix.
		 * @return The inverse of this matrix, or the zero matrix if it is singular.
		 */
		Matrix4 Inverted() const;

		/**
		 * @brief Checks whether this matrix is approximately equal to another.
		 * @param rhs The matrix to compare against.
		 * @param e The maximum allowed per-element difference.
		 * @return True if every element differs by no more than @p e.
		 */
		bool IsApproximatelyEqual(const Matrix4& rhs, float e = 1e-4f) const;

	public:
		/**
		 * @brief Writes a matrix to an output stream, row by row.
		 * @param stream The output stream to write to.
		 * @param mat The matrix to write.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Matrix4& mat);

		/**
		 * @brief Converts this matrix to a glm 4x4 matrix.
		 */
		operator glm::mat4() const;

		/**
		 * @brief Copy assignment operator.
		 * @param rhs The matrix to copy.
		 * @return A reference to this matrix.
		 */
		Matrix4& operator=(const Matrix4& rhs);

		/**
		 * @brief Returns the negation of this matrix.
		 * @return A matrix with every element negated.
		 */
		Matrix4 operator-() const;

		/**
		 * @brief Checks this matrix for approximate equality with another.
		 * @param rhs The matrix to compare against.
		 * @return True if the matrices are approximately equal.
		 */
		bool operator==(const Matrix4& rhs) const;

		/**
		 * @brief Checks this matrix for inequality with another.
		 * @param rhs The matrix to compare against.
		 * @return True if the matrices are not approximately equal.
		 */
		bool operator!=(const Matrix4& rhs) const;

		/**
		 * @brief Compares this matrix to another by sum of squared elements.
		 *
		 * There's no canonical ordering for matrices; this exists mainly to
		 * support sorted containers, analogous to Vector's magnitude-based
		 * `operator<`.
		 * @param rhs The matrix to compare against.
		 * @return True if this matrix's sum of squared elements is less than @p rhs's.
		 */
		bool operator<(const Matrix4& rhs) const;

		/**
		 * @brief Adds two matrices element-wise.
		 * @param rhs The matrix to add.
		 * @return The resulting sum matrix.
		 */
		Matrix4 operator+(const Matrix4& rhs) const;

		/**
		 * @brief Adds another matrix to this matrix in place, element-wise.
		 * @param rhs The matrix to add.
		 * @return A reference to this matrix.
		 */
		Matrix4& operator+=(const Matrix4& rhs);

		/**
		 * @brief Subtracts two matrices element-wise.
		 * @param rhs The matrix to subtract.
		 * @return The resulting difference matrix.
		 */
		Matrix4 operator-(const Matrix4& rhs) const;

		/**
		 * @brief Subtracts another matrix from this matrix in place, element-wise.
		 * @param rhs The matrix to subtract.
		 * @return A reference to this matrix.
		 */
		Matrix4& operator-=(const Matrix4& rhs);

		/**
		 * @brief Multiplies this matrix by another (matrix product, not element-wise).
		 * @param rhs The matrix to multiply by.
		 * @return The resulting matrix product.
		 */
		Matrix4 operator*(const Matrix4& rhs) const;

		/**
		 * @brief Multiplies this matrix by another in place (matrix product).
		 * @param rhs The matrix to multiply by.
		 * @return A reference to this matrix.
		 */
		Matrix4& operator*=(const Matrix4& rhs);

		/**
		 * @brief Transforms a point by this matrix, assuming an affine transform.
		 *
		 * @p rhs is treated as a point with an implicit w = 1; the result drops
		 * back to a Vector3 without a perspective divide. Not suitable for
		 * projective (e.g. perspective projection) matrices.
		 * @param rhs The point to transform.
		 * @return The transformed point.
		 */
		Vector3 operator*(const Vector3& rhs) const;

		/**
		 * @brief Scales every element of this matrix by a scalar.
		 * @param rhs The scalar to multiply by.
		 * @return The resulting scaled matrix.
		 */
		Matrix4 operator*(float rhs) const;

		/**
		 * @brief Scales every element of this matrix by a scalar in place.
		 * @param rhs The scalar to multiply by.
		 * @return A reference to this matrix.
		 */
		Matrix4& operator*=(float rhs);

		/**
		 * @brief Divides every element of this matrix by a scalar.
		 * @param rhs The scalar to divide by.
		 * @return The resulting scaled matrix.
		 */
		Matrix4 operator/(float rhs) const;

		/**
		 * @brief Divides every element of this matrix by a scalar in place.
		 * @param rhs The scalar to divide by.
		 * @return A reference to this matrix.
		 */
		Matrix4& operator/=(float rhs);

		/**
		 * @brief Accesses a column by index.
		 * @param index The column index (0 to 3).
		 * @return A pointer to the first of the column's 4 contiguous floats.
		 */
		Vector4& operator[](int index);

		/**
		 * @brief Accesses a column by index (const overload).
		 * @param index The column index (0 to 3).
		 * @return A const pointer to the first of the column's 4 contiguous floats.
		 */
		const Vector4& operator[](int index) const;
	};
}
