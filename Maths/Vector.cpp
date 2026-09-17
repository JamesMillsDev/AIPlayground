#include "pch.h"
#include "Vector.h"
#include <cmath>
#include <limits>

namespace Maths
{
	// ============================= Vector2 =============================

	float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	float Vector2::Distance(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs - rhs).Magnitude();
	}

	Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, const float t)
	{
		return a + (b - a) * t;
	}

	Vector2 Vector2::Normalized(const Vector2& a)
	{
		return a.Normalised();
	}

	Vector2::Vector2()
		: x{ 0.0f }, y{ 0.0f }
	{}

	Vector2::Vector2(const float scalar)
		: x{ scalar }, y{ scalar }
	{}

	Vector2::Vector2(const float x, const float y)
		: x{ x }, y{ y }
	{}

	Vector2::Vector2(const ::Vector2 vec)
		: x{ vec.x }, y{ vec.y }
	{}

	Vector2::Vector2(const ::Vector3 vec)
		: x{ vec.x }, y{ vec.y }
	{}

	Vector2::Vector2(const glm::vec2 vec)
		: x{ vec.x }, y{ vec.y }
	{}

	Vector2::Vector2(const glm::vec3 vec)
		: x{ vec.x }, y{ vec.y }
	{}

	Vector2::Vector2(const struct Vector3& vec)
		: x{ vec.x }, y{ vec.y }
	{}

	Vector2::Vector2(const Vector4& vec)
		: x{ vec.x }, y{ vec.y }
	{}

	Vector2::Vector2(const Vector2& rhs)
		: x{ rhs.x }, y{ rhs.y }
	{}

	Vector2::Vector2(Vector2&& rhs) noexcept
		: x{ rhs.x }, y{ rhs.y }
	{}

	float Vector2::Dot(const Vector2& rhs) const
	{
		return Dot(*this, rhs);
	}

	float Vector2::Magnitude() const
	{
		return std::sqrt(MagnitudeSqr());
	}

	float Vector2::MagnitudeSqr() const
	{
		return Dot(*this, *this);
	}

	void Vector2::Normalise()
	{
		*this = Normalised();
	}

	Vector2 Vector2::Normalised() const
	{
		float mag = Magnitude();
		if (mag <= 0.0f)
		{
			return { 0.0f, 0.0f };
		}
		return { x / mag, y / mag };
	}

	bool Vector2::IsApproximatelyEqual(const Vector2& rhs, const float e) const
	{
		return std::fabs(x - rhs.x) <= e && std::fabs(y - rhs.y) <= e;
	}

	bool Vector2::IsApproximatelyEqual(const ::Vector2& rhs, float e) const
	{
		return std::fabs(x - rhs.x) <= e && std::fabs(y - rhs.y) <= e;
	}

	float Vector2::AngleBetween(const Vector2& rhs) const
	{
		const float denom = Magnitude() * rhs.Magnitude();
		if (denom <= 0.0f)
		{
			return 0.0f;
		}
		float cosAngle = Dot(rhs) / denom;
		cosAngle = std::fmax(-1.0f, std::fmin(1.0f, cosAngle));
		return std::acos(cosAngle);
	}

	float Vector2::Distance(const Vector2& rhs) const
	{
		return Distance(*this, rhs);
	}

	float Vector2::Angle2D() const
	{
		return std::atan2(y, x);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector2& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ")";
		return stream;
	}

	Vector2::operator ::Vector2() const
	{
		return { x, y };
	}

	Vector2::operator glm::vec<2, float>() const
	{
		return { x, y };
	}

	Vector2& Vector2::operator=(const Vector2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	Vector2 Vector2::operator-() const
	{
		return { -x, -y };
	}

	bool Vector2::operator==(const Vector2& rhs) const
	{
		return IsApproximatelyEqual(rhs, std::numeric_limits<float>::epsilon());
	}

	bool Vector2::operator==(const ::Vector2& rhs) const
	{
		return IsApproximatelyEqual(rhs, std::numeric_limits<float>::epsilon());
	}

	bool Vector2::operator!=(const Vector2& rhs) const
	{
		return !(*this == rhs);
	}

	bool Vector2::operator!=(const ::Vector2& rhs) const
	{
		return !(*this == rhs);
	}

	bool Vector2::operator<(const Vector2& rhs) const
	{
		return MagnitudeSqr() < rhs.MagnitudeSqr();
	}

	Vector2 Vector2::operator+(const Vector2& rhs) const
	{
		return { x + rhs.x, y + rhs.y };
	}

	Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return { x - rhs.x, y - rhs.y };
	}

	Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2 Vector2::operator*(const Vector2& rhs) const
	{
		return { x * rhs.x, y * rhs.y };
	}

	Vector2& Vector2::operator*=(const Vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	Vector2 Vector2::operator*(const float rhs) const
	{
		return { x * rhs, y * rhs };
	}

	Vector2& Vector2::operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	Vector2 Vector2::operator/(const float rhs) const
	{
		return { x / rhs, y / rhs };
	}

	Vector2& Vector2::operator/=(const float rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	float& Vector2::operator[](const int index)
	{
		return (index == 0) ? x : y;
	}

	const float& Vector2::operator[](const int index) const
	{
		return (index == 0) ? x : y;
	}

	// ============================= Vector3 =============================

	float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	float Vector3::Distance(const Vector3& lhs, const Vector3& rhs)
	{
		return (lhs - rhs).Magnitude();
	}

	Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
	{
		return
		{
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		};
	}

	Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, const float t)
	{
		return a + (b - a) * t;
	}

	Vector3 Vector3::Normalized(const Vector3& a)
	{
		return a.Normalised();
	}

	Vector3::Vector3()
		: x{ 0.0f }, y{ 0.0f }, z{ 0.0f }
	{}

	Vector3::Vector3(float scalar)
		: x{ scalar }, y{ scalar }, z{ scalar }
	{}

	Vector3::Vector3(float x, float y, float z)
		: x{ x }, y{ y }, z{ z }
	{}

	Vector3::Vector3(::Vector2 vec)
		: x{ vec.x }, y{ vec.y }, z{ 0.0f }
	{}

	Vector3::Vector3(::Vector3 vec)
		: x{ vec.x }, y{ vec.y }, z{ vec.z }
	{}

	Vector3::Vector3(glm::vec2 vec)
		: x{ vec.x }, y{ vec.y }, z{ 0.0f }
	{}

	Vector3::Vector3(glm::vec3 vec)
		: x{ vec.x }, y{ vec.y }, z{ vec.z }
	{}

	Vector3::Vector3(const Vector2& vec)
		: x{ vec.x }, y{ vec.y }, z{ 0.0f }
	{}

	Vector3::Vector3(const Vector4& vec)
		: x{ vec.x }, y{ vec.y }, z{ vec.z }
	{}

	Vector3::Vector3(const Vector3& rhs)
		: x{ rhs.x }, y{ rhs.y }, z{ rhs.z }
	{}

	Vector3::Vector3(Vector3&& rhs) noexcept
		: x{ rhs.x }, y{ rhs.y }, z{ rhs.z }
	{}

	float Vector3::Dot(const Vector3& rhs) const
	{
		return Dot(*this, rhs);
	}

	Vector3 Vector3::Cross(const Vector3& rhs) const
	{
		return Cross(*this, rhs);
	}

	float Vector3::Magnitude() const
	{
		return std::sqrt(MagnitudeSqr());
	}

	float Vector3::MagnitudeSqr() const
	{
		return Dot(*this, *this);
	}

	void Vector3::Normalise()
	{
		*this = Normalised();
	}

	Vector3 Vector3::Normalised() const
	{
		float mag = Magnitude();
		if (mag <= 0.0f)
		{
			return { 0.0f, 0.0f, 0.0f };
		}
		return { x / mag, y / mag, z / mag };
	}

	bool Vector3::IsApproximatelyEqual(const Vector3& rhs, const float e) const
	{
		return std::fabs(x - rhs.x) <= e && std::fabs(y - rhs.y) <= e && std::fabs(z - rhs.z) <= e;
	}

	float Vector3::AngleBetween(const Vector3& rhs) const
	{
		const float denom = Magnitude() * rhs.Magnitude();
		if (denom <= 0.0f)
		{
			return 0.0f;
		}
		float cosAngle = Dot(rhs) / denom;
		cosAngle = std::fmax(-1.0f, std::fmin(1.0f, cosAngle));
		return std::acos(cosAngle);
	}

	float Vector3::Distance(const Vector3& rhs) const
	{
		return Distance(*this, rhs);
	}

	float Vector3::Angle2D() const
	{
		return std::atan2(y, x);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector3& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return stream;
	}

	Vector3::operator ::Vector3() const
	{
		return { x, y, z };
	}

	Vector3::operator glm::vec<3, float>() const
	{
		return { x, y, z };
	}

	Vector3& Vector3::operator=(const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	Vector3 Vector3::operator-() const
	{
		return { -x, -y, -z };
	}

	bool Vector3::operator==(const Vector3& rhs) const
	{
		return IsApproximatelyEqual(rhs, std::numeric_limits<float>::epsilon());
	}

	bool Vector3::operator!=(const Vector3& rhs) const
	{
		return !(*this == rhs);
	}

	bool Vector3::operator<(const Vector3& rhs) const
	{
		return MagnitudeSqr() < rhs.MagnitudeSqr();
	}

	Vector3 Vector3::operator+(const Vector3& rhs) const
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}

	Vector3& Vector3::operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3 Vector3::operator-(const Vector3& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}

	Vector3& Vector3::operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3 Vector3::operator*(const Vector3& rhs) const
	{
		return { x * rhs.x, y * rhs.y, z * rhs.z };
	}

	Vector3& Vector3::operator*=(const Vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	Vector3 Vector3::operator*(const float rhs) const
	{
		return { x * rhs, y * rhs, z * rhs };
	}

	Vector3& Vector3::operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Vector3 Vector3::operator/(const float rhs) const
	{
		return { x / rhs, y / rhs, z / rhs };
	}

	Vector3& Vector3::operator/=(const float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	float& Vector3::operator[](const int index)
	{
		return (index == 0) ? x : (index == 1) ? y : z;
	}

	const float& Vector3::operator[](const int index) const
	{
		return (index == 0) ? x : (index == 1) ? y : z;
	}

	// ============================= Vector4 =============================

	float Vector4::Dot(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	float Vector4::Distance(const Vector4& lhs, const Vector4& rhs)
	{
		return (lhs - rhs).Magnitude();
	}

	Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, const float t)
	{
		return a + (b - a) * t;
	}

	Vector4 Vector4::Normalized(const Vector4& a)
	{
		return a.Normalised();
	}

	Vector4::Vector4()
		: x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f }
	{}

	Vector4::Vector4(float scalar)
		: x{ scalar }, y{ scalar }, z{ scalar }, w{ scalar }
	{}

	Vector4::Vector4(float x, float y, float z, float w)
		: x{ x }, y{ y }, z{ z }, w{ w }
	{}

	Vector4::Vector4(glm::vec4 vec)
		: x{ vec.x }, y{ vec.y }, z{ vec.z }, w{ vec.w }
	{}

	Vector4::Vector4(const Vector2& vec)
		: x{ vec.x }, y{ vec.y }, z{ 0.0f }, w{ 0.0f }
	{}

	Vector4::Vector4(const Vector3& vec)
		: x{ vec.x }, y{ vec.y }, z{ vec.z }, w{ 0.0f }
	{}

	Vector4::Vector4(const Vector3& vec, float w)
		: x{ vec.x }, y{ vec.y }, z{ vec.z }, w{ w }
	{}

	Vector4::Vector4(const Vector4& rhs)
		: x{ rhs.x }, y{ rhs.y }, z{ rhs.z }, w{ rhs.w }
	{}

	Vector4::Vector4(Vector4&& rhs) noexcept
		: x{ rhs.x }, y{ rhs.y }, z{ rhs.z }, w{ rhs.w }
	{}

	float Vector4::Dot(const Vector4& rhs) const
	{
		return Dot(*this, rhs);
	}

	float Vector4::Magnitude() const
	{
		return std::sqrt(MagnitudeSqr());
	}

	float Vector4::MagnitudeSqr() const
	{
		return Dot(*this, *this);
	}

	void Vector4::Normalise()
	{
		*this = Normalised();
	}

	Vector4 Vector4::Normalised() const
	{
		float mag = Magnitude();
		if (mag <= 0.0f)
		{
			return { 0.0f, 0.0f, 0.0f, 0.0f };
		}
		return { x / mag, y / mag, z / mag, w / mag };
	}

	bool Vector4::IsApproximatelyEqual(const Vector4& rhs, const float e) const
	{
		return std::fabs(x - rhs.x) <= e && std::fabs(y - rhs.y) <= e
			&& std::fabs(z - rhs.z) <= e && std::fabs(w - rhs.w) <= e;
	}

	float Vector4::AngleBetween(const Vector4& rhs) const
	{
		const float denom = Magnitude() * rhs.Magnitude();
		if (denom <= 0.0f)
		{
			return 0.0f;
		}
		float cosAngle = Dot(rhs) / denom;
		cosAngle = std::fmax(-1.0f, std::fmin(1.0f, cosAngle));
		return std::acos(cosAngle);
	}

	float Vector4::Distance(const Vector4& rhs) const
	{
		return Distance(*this, rhs);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector4& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return stream;
	}

	Vector4::operator glm::vec4() const
	{
		return { x, y, z, w };
	}

	Vector4& Vector4::operator=(const Vector4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	Vector4 Vector4::operator-() const
	{
		return { -x, -y, -z, -w };
	}

	bool Vector4::operator==(const Vector4& rhs) const
	{
		return IsApproximatelyEqual(rhs);
	}

	bool Vector4::operator!=(const Vector4& rhs) const
	{
		return !(*this == rhs);
	}

	bool Vector4::operator<(const Vector4& rhs) const
	{
		return MagnitudeSqr() < rhs.MagnitudeSqr();
	}

	Vector4 Vector4::operator+(const Vector4& rhs) const
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
	}

	Vector4& Vector4::operator+=(const Vector4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vector4 Vector4::operator-(const Vector4& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
	}

	Vector4& Vector4::operator-=(const Vector4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vector4 Vector4::operator*(const Vector4& rhs) const
	{
		return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
	}

	Vector4& Vector4::operator*=(const Vector4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	Vector4 Vector4::operator*(const float rhs) const
	{
		return { x * rhs, y * rhs, z * rhs, w * rhs };
	}

	Vector4& Vector4::operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	Vector4 Vector4::operator/(const float rhs) const
	{
		return { x / rhs, y / rhs, z / rhs, w / rhs };
	}

	Vector4& Vector4::operator/=(const float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	float& Vector4::operator[](const int index)
	{
		switch (index)
		{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return w;
		}
	}

	const float& Vector4::operator[](const int index) const
	{
		switch (index)
		{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return w;
		}
	}
}