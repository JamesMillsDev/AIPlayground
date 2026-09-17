#pragma once

#include <ostream>

#include <raylib/raymath.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Maths
{
	struct Vector3;
	struct Vector4;

	/**
	 * @brief A 2D vector with x/y (and aliased u/v) components.
	 */
	struct Vector2
	{
	public:
		/**
		 * @brief Computes the dot product of two vectors.
		 * @param lhs The first vector.
		 * @param rhs The second vector.
		 * @return The scalar dot product `lhs . rhs`.
		 */
		static float Dot(const Vector2& lhs, const Vector2& rhs);

		/**
		 * @brief Computes the distance between two points.
		 * @param lhs The first point.
		 * @param rhs The second point.
		 * @return The Euclidean distance between @p lhs and @p rhs.
		 */
		static float Distance(const Vector2& lhs, const Vector2& rhs);

		/**
		 * @brief Linearly interpolates between two vectors.
		 * @param a The vector at t = 0.
		 * @param b The vector at t = 1.
		 * @param t The interpolation factor, typically in the range [0, 1].
		 * @return The interpolated vector `a + (b - a) * t`.
		 */
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

		/**
		 * @brief Returns a normalised copy of a vector.
		 * @param a The vector to normalise.
		 * @return A unit-length vector in the same direction as @p a, or a
		 *         zero vector if @p a has zero magnitude.
		 */
		static Vector2 Normalized(const Vector2& a);

	public:
		/**
		 * @brief The vector's components.
		 *
		 * All members alias the same underlying two floats: `x`/`y`, `u`/`v`,
		 * and the `values` array are three different views onto the same data.
		 */
		union
		{
			struct
			{
				float x, y; ///< Positional components.
			};

			struct
			{
				float u, v; ///< Texture-coordinate aliases for x/y.
			};

			float values[2]; ///< Array-indexed view of the components.
		};

	public:
		/**
		 * @brief Constructs a zero vector (0, 0).
		 */
		Vector2();

		/**
		 * @brief Constructs a vector with both components set to the same value.
		 * @param scalar The value assigned to both x and y.
		 */
		explicit Vector2(float scalar);

		/**
		 * @brief Constructs a vector from individual components.
		 * @param x The x component.
		 * @param y The y component.
		 */
		Vector2(float x, float y);

		/**
		 * @brief Constructs a vector from an SFML 2D vector.
		 * @param vec The SFML vector to copy from.
		 */
		Vector2(::Vector2 vec);

		/**
		 * @brief Constructs a vector from an SFML 3D vector, dropping the z component.
		 * @param vec The SFML vector to copy from.
		 */
		Vector2(::Vector3 vec);

		/**
		 * @brief Constructs a vector from a glm 2D vector.
		 * @param vec The glm vector to copy from.
		 */
		Vector2(glm::vec2 vec);

		/**
		 * @brief Constructs a vector from a glm 3D vector, dropping the z component.
		 * @param vec The glm vector to copy from.
		 */
		Vector2(glm::vec3 vec);

		/**
		 * @brief Constructs a vector from a Vector3, dropping the z component.
		 * @param vec The Vector3 to copy from.
		 */
		Vector2(const Vector3& vec);

		/**
		 * @brief Constructs a vector from a Vector4, dropping the z and w components.
		 * @param vec The Vector4 to copy from.
		 */
		Vector2(const Vector4& vec);

		/**
		 * @brief Copy constructor.
		 * @param rhs The vector to copy.
		 */
		Vector2(const Vector2& rhs);

		/**
		 * @brief Move constructor.
		 * @param rhs The vector to move from.
		 */
		Vector2(Vector2&& rhs) noexcept;

		/**
		 * @brief Default destructor.
		 */
		~Vector2() = default;

	public:
		/**
		 * @brief Computes the dot product of this vector with another.
		 * @param rhs The other vector.
		 * @return The scalar dot product.
		 */
		float Dot(const Vector2& rhs) const;

		/**
		 * @brief Computes the length of this vector.
		 * @return The Euclidean magnitude of the vector.
		 */
		float Magnitude() const;

		/**
		 * @brief Computes the squared length of this vector.
		 *
		 * Cheaper than Magnitude() as it avoids a square root; prefer this
		 * when only comparing lengths.
		 * @return The squared Euclidean magnitude of the vector.
		 */
		float MagnitudeSqr() const;

		/**
		 * @brief Normalises this vector in place to unit length.
		 *
		 * If the vector has zero magnitude, it is set to the zero vector.
		 */
		void Normalise();

		/**
		 * @brief Returns a normalised copy of this vector.
		 * @return A unit-length vector in the same direction as this vector,
		 *         or a zero vector if this vector has zero magnitude.
		 */
		Vector2 Normalised() const;

		/**
		 * @brief Checks whether this vector is approximately equal to another.
		 * @param rhs The vector to compare against.
		 * @param e The maximum allowed per-component difference.
		 * @return True if each component differs by no more than @p e.
		 */
		bool IsApproximatelyEqual(const Vector2& rhs, float e = 1e-4f) const;

		/**
		 * @brief Checks whether this vector is approximately equal to another.
		 * @param rhs The vector to compare against.
		 * @param e The maximum allowed per-component difference.
		 * @return True if each component differs by no more than @p e.
		 */
		bool IsApproximatelyEqual(const ::Vector2& rhs, float e = 1e-4f) const;

		/**
		 * @brief Computes the angle between this vector and another.
		 * @param rhs The other vector.
		 * @return The angle between the two vectors, in radians.
		 */
		float AngleBetween(const Vector2& rhs) const;

		/**
		 * @brief Computes the distance between this vector and another, treating both as points.
		 * @param rhs The other point.
		 * @return The Euclidean distance between the two points.
		 */
		float Distance(const Vector2& rhs) const;

		/**
		 * @brief Computes the angle of this vector relative to the positive x-axis.
		 * @return The angle in radians, in the range (-pi, pi].
		 */
		float Angle2D() const;

	public:
		/**
		 * @brief Writes a vector to an output stream as "(x, y)".
		 * @param stream The output stream to write to.
		 * @param vec The vector to write.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Vector2& vec);

		/**
		 * @brief Converts this vector to an SFML 2D vector.
		 */
		operator ::Vector2() const;

		/**
		 * @brief Converts this vector to a glm 2D vector, with z set to 0.
		 */
		operator glm::vec2() const;

		/**
		 * @brief Copy assignment operator.
		 * @param rhs The vector to copy.
		 * @return A reference to this vector.
		 */
		Vector2& operator=(const Vector2& rhs);

		/**
		 * @brief Returns the negation of this vector.
		 * @return A vector with each component negated.
		 */
		Vector2 operator-() const;

		/**
		 * @brief Checks this vector for approximate equality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are approximately equal.
		 */
		bool operator==(const Vector2& rhs) const;

		/**
		 * @brief Checks this vector for approximate equality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are approximately equal.
		 */
		bool operator==(const ::Vector2& rhs) const;

		/**
		 * @brief Checks this vector for inequality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are not approximately equal.
		 */
		bool operator!=(const Vector2& rhs) const;

		/**
		 * @brief Checks this vector for inequality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are not approximately equal.
		 */
		bool operator!=(const ::Vector2& rhs) const;

		/**
		 * @brief Compares this vector to another by squared magnitude.
		 * @param rhs The vector to compare against.
		 * @return True if this vector's squared magnitude is less than @p rhs's.
		 */
		bool operator<(const Vector2& rhs) const;

		/**
		 * @brief Adds two vectors component-wise.
		 * @param rhs The vector to add.
		 * @return The resulting sum vector.
		 */
		Vector2 operator+(const Vector2& rhs) const;

		/**
		 * @brief Adds another vector to this vector in place, component-wise.
		 * @param rhs The vector to add.
		 * @return A reference to this vector.
		 */
		Vector2& operator+=(const Vector2& rhs);

		/**
		 * @brief Subtracts two vectors component-wise.
		 * @param rhs The vector to subtract.
		 * @return The resulting difference vector.
		 */
		Vector2 operator-(const Vector2& rhs) const;

		/**
		 * @brief Subtracts another vector from this vector in place, component-wise.
		 * @param rhs The vector to subtract.
		 * @return A reference to this vector.
		 */
		Vector2& operator-=(const Vector2& rhs);

		/**
		 * @brief Multiplies two vectors component-wise.
		 * @param rhs The vector to multiply by.
		 * @return The resulting component-wise product vector.
		 */
		Vector2 operator*(const Vector2& rhs) const;

		/**
		 * @brief Multiplies this vector by another in place, component-wise.
		 * @param rhs The vector to multiply by.
		 * @return A reference to this vector.
		 */
		Vector2& operator*=(const Vector2& rhs);

		/**
		 * @brief Scales this vector by a scalar.
		 * @param rhs The scalar to multiply by.
		 * @return The resulting scaled vector.
		 */
		Vector2 operator*(float rhs) const;

		/**
		 * @brief Scales this vector by a scalar in place.
		 * @param rhs The scalar to multiply by.
		 * @return A reference to this vector.
		 */
		Vector2& operator*=(float rhs);

		/**
		 * @brief Divides this vector by a scalar.
		 * @param rhs The scalar to divide by.
		 * @return The resulting scaled vector.
		 */
		Vector2 operator/(float rhs) const;

		/**
		 * @brief Divides this vector by a scalar in place.
		 * @param rhs The scalar to divide by.
		 * @return A reference to this vector.
		 */
		Vector2& operator/=(float rhs);

		/**
		 * @brief Accesses a component by index.
		 * @param index The component index (0 = x, 1 = y).
		 * @return A reference to the requested component.
		 */
		float& operator[](int index);

		/**
		 * @brief Accesses a component by index (const overload).
		 * @param index The component index (0 = x, 1 = y).
		 * @return A const reference to the requested component.
		 */
		const float& operator[](int index) const;

	};

	/**
	 * @brief A 3D vector with x/y/z (and aliased w/r/g/b) components.
	 */
	struct Vector3
	{
	public:
		/**
		 * @brief Computes the dot product of two vectors.
		 * @param lhs The first vector.
		 * @param rhs The second vector.
		 * @return The scalar dot product `lhs . rhs`.
		 */
		static float Dot(const Vector3& lhs, const Vector3& rhs);

		/**
		 * @brief Computes the distance between two points.
		 * @param lhs The first point.
		 * @param rhs The second point.
		 * @return The Euclidean distance between @p lhs and @p rhs.
		 */
		static float Distance(const Vector3& lhs, const Vector3& rhs);

		/**
		 * @brief Computes the cross product of two vectors.
		 * @param lhs The first vector.
		 * @param rhs The second vector.
		 * @return The vector `lhs x rhs`, perpendicular to both inputs.
		 */
		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

		/**
		 * @brief Linearly interpolates between two vectors.
		 * @param a The vector at t = 0.
		 * @param b The vector at t = 1.
		 * @param t The interpolation factor, typically in the range [0, 1].
		 * @return The interpolated vector `a + (b - a) * t`.
		 */
		static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

		/**
		 * @brief Returns a normalised copy of a vector.
		 * @param a The vector to normalise.
		 * @return A unit-length vector in the same direction as @p a, or a
		 *         zero vector if @p a has zero magnitude.
		 */
		static Vector3 Normalized(const Vector3& a);

	public:
		/**
		 * @brief The vector's components.
		 *
		 * All members alias the same underlying three floats: `x`/`y`/`z`,
		 * the `w` alias for `z`, `r`/`g`/`b`, and the `values` array are all
		 * different views onto the same data.
		 */
		union
		{
			struct
			{
				float x, y; ///< First two positional components.

				union
				{
					float z; ///< Third positional component.
					float w; ///< Alias for z, useful when treating the vector as a homogeneous coordinate.
				};
			};

			struct
			{
				float r, g, b; ///< Colour-channel aliases for x/y/z.
			};

			float values[3]; ///< Array-indexed view of the components.
		};

	public:
		/**
		 * @brief Constructs a zero vector (0, 0, 0).
		 */
		Vector3();

		/**
		 * @brief Constructs a vector with all components set to the same value.
		 * @param scalar The value assigned to x, y, and z.
		 */
		explicit Vector3(float scalar);

		/**
		 * @brief Constructs a vector from individual components.
		 * @param x The x component.
		 * @param y The y component.
		 * @param z The z component.
		 */
		Vector3(float x, float y, float z);

		/**
		 * @brief Constructs a vector from an SFML 2D vector, with z set to 0.
		 * @param vec The SFML vector to copy from.
		 */
		Vector3(::Vector2 vec);

		/**
		 * @brief Constructs a vector from an SFML 3D vector.
		 * @param vec The SFML vector to copy from.
		 */
		Vector3(::Vector3 vec);

		/**
		 * @brief Constructs a vector from a glm 2D vector, with z set to 0.
		 * @param vec The glm vector to copy from.
		 */
		Vector3(glm::vec2 vec);

		/**
		 * @brief Constructs a vector from a glm 3D vector.
		 * @param vec The glm vector to copy from.
		 */
		Vector3(glm::vec3 vec);

		/**
		 * @brief Constructs a vector from a Vector2, with z set to 0.
		 * @param vec The Vector2 to copy from.
		 */
		Vector3(const Vector2& vec);

		/**
		 * @brief Constructs a vector from a Vector4, dropping the w component.
		 * @param vec The Vector4 to copy from.
		 */
		Vector3(const Vector4& vec);

		/**
		 * @brief Copy constructor.
		 * @param rhs The vector to copy.
		 */
		Vector3(const Vector3& rhs);

		/**
		 * @brief Move constructor.
		 * @param rhs The vector to move from.
		 */
		Vector3(Vector3&& rhs) noexcept;

		/**
		 * @brief Default destructor.
		 */
		~Vector3() = default;

	public:
		/**
		 * @brief Computes the dot product of this vector with another.
		 * @param rhs The other vector.
		 * @return The scalar dot product.
		 */
		float Dot(const Vector3& rhs) const;

		/**
		 * @brief Computes the cross product of this vector with another.
		 * @param rhs The other vector.
		 * @return The vector `*this x rhs`, perpendicular to both vectors.
		 */
		Vector3 Cross(const Vector3& rhs) const;

		/**
		 * @brief Computes the length of this vector.
		 * @return The Euclidean magnitude of the vector.
		 */
		float Magnitude() const;

		/**
		 * @brief Computes the squared length of this vector.
		 *
		 * Cheaper than Magnitude() as it avoids a square root; prefer this
		 * when only comparing lengths.
		 * @return The squared Euclidean magnitude of the vector.
		 */
		float MagnitudeSqr() const;

		/**
		 * @brief Normalises this vector in place to unit length.
		 *
		 * If the vector has zero magnitude, it is set to the zero vector.
		 */
		void Normalise();

		/**
		 * @brief Returns a normalised copy of this vector.
		 * @return A unit-length vector in the same direction as this vector,
		 *         or a zero vector if this vector has zero magnitude.
		 */
		Vector3 Normalised() const;

		/**
		 * @brief Checks whether this vector is approximately equal to another.
		 * @param rhs The vector to compare against.
		 * @param e The maximum allowed per-component difference.
		 * @return True if each component differs by no more than @p e.
		 */
		bool IsApproximatelyEqual(const Vector3& rhs, float e = 1e-4f) const;

		/**
		 * @brief Computes the angle between this vector and another.
		 * @param rhs The other vector.
		 * @return The angle between the two vectors, in radians.
		 */
		float AngleBetween(const Vector3& rhs) const;

		/**
		 * @brief Computes the distance between this vector and another, treating both as points.
		 * @param rhs The other point.
		 * @return The Euclidean distance between the two points.
		 */
		float Distance(const Vector3& rhs) const;

		/**
		 * @brief Computes the angle of this vector's x/y components relative to the positive x-axis.
		 * @return The angle in radians, in the range (-pi, pi].
		 */
		float Angle2D() const;

	public:
		/**
		 * @brief Writes a vector to an output stream as "(x, y, z)".
		 * @param stream The output stream to write to.
		 * @param vec The vector to write.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Vector3& vec);

		/**
		 * @brief Converts this vector to an SFML 3D vector.
		 */
		operator ::Vector3() const;

		/**
		 * @brief Converts this vector to a glm 3D vector.
		 */
		operator glm::vec3() const;

		/**
		 * @brief Copy assignment operator.
		 * @param rhs The vector to copy.
		 * @return A reference to this vector.
		 */
		Vector3& operator=(const Vector3& rhs);

		/**
		 * @brief Returns the negation of this vector.
		 * @return A vector with each component negated.
		 */
		Vector3 operator-() const;

		/**
		 * @brief Checks this vector for approximate equality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are approximately equal.
		 */
		bool operator==(const Vector3& rhs) const;

		/**
		 * @brief Checks this vector for inequality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are not approximately equal.
		 */
		bool operator!=(const Vector3& rhs) const;

		/**
		 * @brief Compares this vector to another by squared magnitude.
		 * @param rhs The vector to compare against.
		 * @return True if this vector's squared magnitude is less than @p rhs's.
		 */
		bool operator<(const Vector3& rhs) const;

		/**
		 * @brief Adds two vectors component-wise.
		 * @param rhs The vector to add.
		 * @return The resulting sum vector.
		 */
		Vector3 operator+(const Vector3& rhs) const;

		/**
		 * @brief Adds another vector to this vector in place, component-wise.
		 * @param rhs The vector to add.
		 * @return A reference to this vector.
		 */
		Vector3& operator+=(const Vector3& rhs);

		/**
		 * @brief Subtracts two vectors component-wise.
		 * @param rhs The vector to subtract.
		 * @return The resulting difference vector.
		 */
		Vector3 operator-(const Vector3& rhs) const;

		/**
		 * @brief Subtracts another vector from this vector in place, component-wise.
		 * @param rhs The vector to subtract.
		 * @return A reference to this vector.
		 */
		Vector3& operator-=(const Vector3& rhs);

		/**
		 * @brief Multiplies two vectors component-wise.
		 * @param rhs The vector to multiply by.
		 * @return The resulting component-wise product vector.
		 */
		Vector3 operator*(const Vector3& rhs) const;

		/**
		 * @brief Multiplies this vector by another in place, component-wise.
		 * @param rhs The vector to multiply by.
		 * @return A reference to this vector.
		 */
		Vector3& operator*=(const Vector3& rhs);

		/**
		 * @brief Scales this vector by a scalar.
		 * @param rhs The scalar to multiply by.
		 * @return The resulting scaled vector.
		 */
		Vector3 operator*(float rhs) const;

		/**
		 * @brief Scales this vector by a scalar in place.
		 * @param rhs The scalar to multiply by.
		 * @return A reference to this vector.
		 */
		Vector3& operator*=(float rhs);

		/**
		 * @brief Divides this vector by a scalar.
		 * @param rhs The scalar to divide by.
		 * @return The resulting scaled vector.
		 */
		Vector3 operator/(float rhs) const;

		/**
		 * @brief Divides this vector by a scalar in place.
		 * @param rhs The scalar to divide by.
		 * @return A reference to this vector.
		 */
		Vector3& operator/=(float rhs);

		/**
		 * @brief Accesses a component by index.
		 * @param index The component index (0 = x, 1 = y, 2 = z).
		 * @return A reference to the requested component.
		 */
		float& operator[](int index);

		/**
		 * @brief Accesses a component by index (const overload).
		 * @param index The component index (0 = x, 1 = y, 2 = z).
		 * @return A const reference to the requested component.
		 */
		const float& operator[](int index) const;

	};

	/**
	 * @brief A 4D vector with x/y/z/w (and aliased r/g/b/a) components.
	 *
	 * SFML has no 4-component vector type, so unlike Vector2/Vector3 there is
	 * no `sf::Vector4` constructor or conversion operator here.
	 */
	struct Vector4
	{
	public:
		/**
		 * @brief Computes the dot product of two vectors.
		 * @param lhs The first vector.
		 * @param rhs The second vector.
		 * @return The scalar dot product `lhs . rhs`.
		 */
		static float Dot(const Vector4& lhs, const Vector4& rhs);

		/**
		 * @brief Computes the distance between two points.
		 * @param lhs The first point.
		 * @param rhs The second point.
		 * @return The Euclidean distance between @p lhs and @p rhs.
		 */
		static float Distance(const Vector4& lhs, const Vector4& rhs);

		/**
		 * @brief Linearly interpolates between two vectors.
		 * @param a The vector at t = 0.
		 * @param b The vector at t = 1.
		 * @param t The interpolation factor, typically in the range [0, 1].
		 * @return The interpolated vector `a + (b - a) * t`.
		 */
		static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);

		/**
		 * @brief Returns a normalised copy of a vector.
		 * @param a The vector to normalise.
		 * @return A unit-length vector in the same direction as @p a, or a
		 *         zero vector if @p a has zero magnitude.
		 */
		static Vector4 Normalized(const Vector4& a);

	public:
		/**
		 * @brief The vector's components.
		 *
		 * All members alias the same underlying four floats: `x`/`y`/`z`/`w`,
		 * `r`/`g`/`b`/`a`, and the `values` array are three different views
		 * onto the same data.
		 */
		union
		{
			struct
			{
				float x, y, z, w; ///< Positional/homogeneous components.
			};

			struct
			{
				float r, g, b, a; ///< Colour-channel aliases for x/y/z/w.
			};

			float values[4]; ///< Array-indexed view of the components.
		};

	public:
		/**
		 * @brief Constructs a zero vector (0, 0, 0, 0).
		 */
		Vector4();

		/**
		 * @brief Constructs a vector with all components set to the same value.
		 * @param scalar The value assigned to x, y, z, and w.
		 */
		explicit Vector4(float scalar);

		/**
		 * @brief Constructs a vector from individual components.
		 * @param x The x component.
		 * @param y The y component.
		 * @param z The z component.
		 * @param w The w component.
		 */
		Vector4(float x, float y, float z, float w);

		/**
		 * @brief Constructs a vector from a glm 4D vector.
		 * @param vec The glm vector to copy from.
		 */
		Vector4(glm::vec4 vec);

		/**
		 * @brief Constructs a vector from a Vector2, with z and w set to 0.
		 * @param vec The Vector2 to copy from.
		 */
		Vector4(const Vector2& vec);

		/**
		 * @brief Constructs a vector from a Vector3, with w set to 0.
		 * @param vec The Vector3 to copy from.
		 */
		Vector4(const Vector3& vec);

		/**
		 * @brief Constructs a vector from a Vector3 and an explicit w, useful
		 *        for treating the result as a homogeneous point (w = 1) or
		 *        direction (w = 0).
		 * @param vec The x/y/z components to copy from.
		 * @param w The w component.
		 */
		Vector4(const Vector3& vec, float w);

		/**
		 * @brief Copy constructor.
		 * @param rhs The vector to copy.
		 */
		Vector4(const Vector4& rhs);

		/**
		 * @brief Move constructor.
		 * @param rhs The vector to move from.
		 */
		Vector4(Vector4&& rhs) noexcept;

		/**
		 * @brief Default destructor.
		 */
		~Vector4() = default;

	public:
		/**
		 * @brief Computes the dot product of this vector with another.
		 * @param rhs The other vector.
		 * @return The scalar dot product.
		 */
		float Dot(const Vector4& rhs) const;

		/**
		 * @brief Computes the length of this vector.
		 * @return The Euclidean magnitude of the vector.
		 */
		float Magnitude() const;

		/**
		 * @brief Computes the squared length of this vector.
		 *
		 * Cheaper than Magnitude() as it avoids a square root; prefer this
		 * when only comparing lengths.
		 * @return The squared Euclidean magnitude of the vector.
		 */
		float MagnitudeSqr() const;

		/**
		 * @brief Normalises this vector in place to unit length.
		 *
		 * If the vector has zero magnitude, it is set to the zero vector.
		 */
		void Normalise();

		/**
		 * @brief Returns a normalised copy of this vector.
		 * @return A unit-length vector in the same direction as this vector,
		 *         or a zero vector if this vector has zero magnitude.
		 */
		Vector4 Normalised() const;

		/**
		 * @brief Checks whether this vector is approximately equal to another.
		 * @param rhs The vector to compare against.
		 * @param e The maximum allowed per-component difference.
		 * @return True if each component differs by no more than @p e.
		 */
		bool IsApproximatelyEqual(const Vector4& rhs, float e = 1e-4f) const;

		/**
		 * @brief Computes the angle between this vector and another.
		 * @param rhs The other vector.
		 * @return The angle between the two vectors, in radians.
		 */
		float AngleBetween(const Vector4& rhs) const;

		/**
		 * @brief Computes the distance between this vector and another, treating both as points.
		 * @param rhs The other point.
		 * @return The Euclidean distance between the two points.
		 */
		float Distance(const Vector4& rhs) const;

	public:
		/**
		 * @brief Writes a vector to an output stream as "(x, y, z, w)".
		 * @param stream The output stream to write to.
		 * @param vec The vector to write.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Vector4& vec);

		/**
		 * @brief Converts this vector to a glm 4D vector.
		 */
		operator glm::vec4() const;

		/**
		 * @brief Copy assignment operator.
		 * @param rhs The vector to copy.
		 * @return A reference to this vector.
		 */
		Vector4& operator=(const Vector4& rhs);

		/**
		 * @brief Returns the negation of this vector.
		 * @return A vector with each component negated.
		 */
		Vector4 operator-() const;

		/**
		 * @brief Checks this vector for approximate equality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are approximately equal.
		 */
		bool operator==(const Vector4& rhs) const;

		/**
		 * @brief Checks this vector for inequality with another.
		 * @param rhs The vector to compare against.
		 * @return True if the vectors are not approximately equal.
		 */
		bool operator!=(const Vector4& rhs) const;

		/**
		 * @brief Compares this vector to another by squared magnitude.
		 * @param rhs The vector to compare against.
		 * @return True if this vector's squared magnitude is less than @p rhs's.
		 */
		bool operator<(const Vector4& rhs) const;

		/**
		 * @brief Adds two vectors component-wise.
		 * @param rhs The vector to add.
		 * @return The resulting sum vector.
		 */
		Vector4 operator+(const Vector4& rhs) const;

		/**
		 * @brief Adds another vector to this vector in place, component-wise.
		 * @param rhs The vector to add.
		 * @return A reference to this vector.
		 */
		Vector4& operator+=(const Vector4& rhs);

		/**
		 * @brief Subtracts two vectors component-wise.
		 * @param rhs The vector to subtract.
		 * @return The resulting difference vector.
		 */
		Vector4 operator-(const Vector4& rhs) const;

		/**
		 * @brief Subtracts another vector from this vector in place, component-wise.
		 * @param rhs The vector to subtract.
		 * @return A reference to this vector.
		 */
		Vector4& operator-=(const Vector4& rhs);

		/**
		 * @brief Multiplies two vectors component-wise.
		 * @param rhs The vector to multiply by.
		 * @return The resulting component-wise product vector.
		 */
		Vector4 operator*(const Vector4& rhs) const;

		/**
		 * @brief Multiplies this vector by another in place, component-wise.
		 * @param rhs The vector to multiply by.
		 * @return A reference to this vector.
		 */
		Vector4& operator*=(const Vector4& rhs);

		/**
		 * @brief Scales this vector by a scalar.
		 * @param rhs The scalar to multiply by.
		 * @return The resulting scaled vector.
		 */
		Vector4 operator*(float rhs) const;

		/**
		 * @brief Scales this vector by a scalar in place.
		 * @param rhs The scalar to multiply by.
		 * @return A reference to this vector.
		 */
		Vector4& operator*=(float rhs);

		/**
		 * @brief Divides this vector by a scalar.
		 * @param rhs The scalar to divide by.
		 * @return The resulting scaled vector.
		 */
		Vector4 operator/(float rhs) const;

		/**
		 * @brief Divides this vector by a scalar in place.
		 * @param rhs The scalar to divide by.
		 * @return A reference to this vector.
		 */
		Vector4& operator/=(float rhs);

		/**
		 * @brief Accesses a component by index.
		 * @param index The component index (0 = x, 1 = y, 2 = z, 3 = w).
		 * @return A reference to the requested component.
		 */
		float& operator[](int index);

		/**
		 * @brief Accesses a component by index (const overload).
		 * @param index The component index (0 = x, 1 = y, 2 = z, 3 = w).
		 * @return A const reference to the requested component.
		 */
		const float& operator[](int index) const;

	};
}