// Copyright 2015-2019 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_MATH_QUATERNION_HPP
#define OUZEL_MATH_QUATERNION_HPP

#include <cmath>
#include <cstddef>
#include <limits>
#include "math/Vector.hpp"

namespace ouzel
{
    template <typename T> class Quaternion final
    {
    public:
        T v[4]{0};

        constexpr Quaternion() noexcept {}

        constexpr Quaternion(const T x, const T y, const T z, const T w) noexcept:
            v{x, y, z, w}
        {
        }

        inline T& operator[](size_t index) noexcept { return v[index]; }
        constexpr T operator[](size_t index) const noexcept { return v[index]; }

        inline T& x() noexcept { return v[0]; }
        constexpr T x() const noexcept { return v[0]; }

        inline T& y() noexcept { return v[1]; }
        constexpr T y() const noexcept { return v[1]; }

        inline T& z() noexcept { return v[2]; }
        constexpr T z() const noexcept { return v[2]; }

        inline T& w() noexcept { return v[3]; }
        constexpr T w() const noexcept { return v[3]; }

        static constexpr Quaternion identity() noexcept
        {
            return Quaternion(0, 0, 0, 1);
        }

        constexpr const Quaternion operator*(const Quaternion& q) const noexcept
        {
            return Quaternion(v[0] * q.v[3] + v[1] * q.v[2] - v[2] * q.v[1] + v[3] * q.v[0],
                              -v[0] * q.v[2] + v[1] * q.v[3] + v[2] * q.v[0] + v[3] * q.v[1],
                              v[0] * q.v[1] - v[1] * q.v[0] + v[2] * q.v[3] + v[3] * q.v[2],
                              -v[0] * q.v[0] - v[1] * q.v[1] - v[2] * q.v[2] + v[3] * q.v[3]);
        }

        inline Quaternion& operator*=(const Quaternion& q) noexcept
        {
            constexpr T tempX = v[0] * q.v[3] + v[1] * q.v[2] - v[2] * q.v[1] + v[3] * q.v[0];
            constexpr T tempY = -v[0] * q.v[2] + v[1] * q.v[3] + v[2] * q.v[0] + v[3] * q.v[1];
            constexpr T tempZ = v[0] * q.v[1] - v[1] * q.v[0] + v[2] * q.v[3] + v[3] * q.v[2];
            constexpr T tempW = -v[0] * q.v[0] - v[1] * q.v[1] - v[2] * q.v[2] + v[3] * q.v[3];

            v[0] = tempX;
            v[1] = tempY;
            v[2] = tempZ;
            v[3] = tempW;

            return *this;
        }

        constexpr const Quaternion operator*(const T scalar) const noexcept
        {
            return Quaternion(v[0] * scalar,
                              v[1] * scalar,
                              v[2] * scalar,
                              v[3] * scalar);
        }

        inline Quaternion& operator*=(const T scalar) noexcept
        {
            v[0] *= scalar;
            v[1] *= scalar;
            v[2] *= scalar;
            v[3] *= scalar;

            return *this;
        }

        constexpr const Quaternion operator/(const T scalar) const noexcept
        {
            return Quaternion(v[0] / scalar,
                              v[1] / scalar,
                              v[2] / scalar,
                              v[3] / scalar);
        }

        inline Quaternion& operator/=(const T scalar) noexcept
        {
            v[0] /= scalar;
            v[1] /= scalar;
            v[2] /= scalar;
            v[3] /= scalar;

            return *this;
        }

        constexpr const Quaternion operator-() const noexcept
        {
            return Quaternion(-v[0], -v[1], -v[2], -v[3]);
        }

        constexpr const Quaternion operator+(const Quaternion& q) const noexcept
        {
            return Quaternion(v[0] + q.v[0],
                              v[1] + q.v[1],
                              v[2] + q.v[2],
                              v[3] + q.v[3]);
        }

        inline Quaternion& operator+=(const Quaternion& q) noexcept
        {
            v[0] += q.v[0];
            v[1] += q.v[1];
            v[2] += q.v[2];
            v[3] += q.v[3];

            return *this;
        }

        constexpr const Quaternion operator-(const Quaternion& q) const noexcept
        {
            return Quaternion(v[0] - q.v[0],
                              v[1] - q.v[1],
                              v[2] - q.v[2],
                              v[3] - q.v[3]);
        }

        inline Quaternion& operator-=(const Quaternion& q) noexcept
        {
            v[0] -= q.v[0];
            v[1] -= q.v[1];
            v[2] -= q.v[2];
            v[3] -= q.v[3];

            return *this;
        }

        constexpr bool operator==(const Quaternion& q) const noexcept
        {
            return v[0] == q.v[0] && v[1] == q.v[1] && v[2] == q.v[2] && v[3] == q.v[3];
        }

        constexpr bool operator!=(const Quaternion& q) const noexcept
        {
            return v[0] != q.v[0] || v[1] != q.v[1] || v[2] != q.v[2] || v[3] != q.v[3];
        }

        inline void negate() noexcept
        {
            v[0] = -v[0];
            v[1] = -v[1];
            v[2] = -v[2];
            v[3] = -v[3];
        }

        inline void conjugate() noexcept
        {
            v[0] = -v[0];
            v[1] = -v[1];
            v[2] = -v[2];
        }

        inline void invert() noexcept
        {
            constexpr T n2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]; // norm squared
            if (n2 <= std::numeric_limits<T>::min())
                return;

            // conjugate divided by norm squared
            v[0] = -v[0] / n2;
            v[1] = -v[1] / n2;
            v[2] = -v[2] / n2;
            v[3] = v[3] / n2;
        }

        inline auto getNorm() const noexcept
        {
            constexpr T n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
            if (n == T(1)) // already normalized
                return 1;

            return std::sqrt(n);
        }

        void normalize() noexcept
        {
            constexpr T s = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
            if (s == T(1)) // already normalized
                return;

            T n = std::sqrt(s);
            if (n <= std::numeric_limits<T>::min()) // too close to zero
                return;

            n = T(1) / n;
            v[0] *= n;
            v[1] *= n;
            v[2] *= n;
            v[3] *= n;
        }

        Quaternion normalized() const noexcept
        {
            constexpr T s = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
            if (s == T(1)) // already normalized
                return *this;

            T n = std::sqrt(s);
            if (n <= std::numeric_limits<T>::min()) // too close to zero
                return *this;

            n = T(1) / n;
            return *this * n;
        }

        void rotate(const T angle, const Vector<3, T>& axis) noexcept
        {
            const auto normalizedAxis = axis.normalized();

            const T cosAngle = std::cos(angle / T(2));
            const T sinAngle = std::sin(angle / T(2));

            v[0] = normalizedAxis.v[0] * sinAngle;
            v[1] = normalizedAxis.v[1] * sinAngle;
            v[2] = normalizedAxis.v[2] * sinAngle;
            v[3] = cosAngle;
        }

        void getRotation(T& angle, Vector<3, T>& axis) const noexcept
        {
            angle = T(2) * std::acos(v[3]);
            const T s = std::sqrt(T(1) - v[3] * v[3]);
            if (s <= std::numeric_limits<T>::min()) // too close to zero
            {
                axis.v[0] = v[0];
                axis.v[1] = v[1];
                axis.v[2] = v[2];
            }
            else
            {
                axis.v[0] = v[0] / s;
                axis.v[1] = v[1] / s;
                axis.v[2] = v[2] / s;
            }
        }

        Vector<3, T> getEulerAngles() const noexcept
        {
            Vector<3, T> result;
            result.v[0] = std::atan2(2 * (v[1] * v[2] + v[3] * v[0]), v[3] * v[3] - v[0] * v[0] - v[1] * v[1] + v[2] * v[2]);
            result.v[1] = std::asin(-2 * (v[0] * v[2] - v[3] * v[1]));
            result.v[2] = std::atan2(2 * (v[0] * v[1] + v[3] * v[2]), v[3] * v[3] + v[0] * v[0] - v[1] * v[1] - v[2] * v[2]);
            return result;
        }

        inline auto getEulerAngleX() const noexcept
        {
            return std::atan2(T(2) * (v[1] * v[2] + v[3] * v[0]), v[3] * v[3] - v[0] * v[0] - v[1] * v[1] + v[2] * v[2]);
        }

        inline auto getEulerAngleY() const noexcept
        {
            return std::asin(T(-2) * (v[0] * v[2] - v[3] * v[1]));
        }

        inline auto getEulerAngleZ() const noexcept
        {
            return std::atan2(T(2) * (v[0] * v[1] + v[3] * v[2]), v[3] * v[3] + v[0] * v[0] - v[1] * v[1] - v[2] * v[2]);
        }

        void setEulerAngles(const Vector<3, T>& angles) noexcept
        {
            constexpr T angleR = angles.v[0] / T(2);
            const T sr = std::sin(angleR);
            const T cr = std::cos(angleR);

            constexpr T angleP = angles.v[1] / T(2);
            const T sp = std::sin(angleP);
            const T cp = std::cos(angleP);

            constexpr T angleY = angles.v[2] / T(2);
            const T sy = std::sin(angleY);
            const T cy = std::cos(angleY);

            const T cpcy = cp * cy;
            const T spcy = sp * cy;
            const T cpsy = cp * sy;
            const T spsy = sp * sy;

            v[0] = sr * cpcy - cr * spsy;
            v[1] = cr * spcy + sr * cpsy;
            v[2] = cr * cpsy - sr * spcy;
            v[3] = cr * cpcy + sr * spsy;
        }

        inline const Vector<3, T> operator*(const Vector<3, T>& vector) const noexcept
        {
            return rotateVector(vector);
        }

        inline Vector<3, T> rotateVector(const Vector<3, T>& vector) const noexcept
        {
            constexpr Vector<3, T> q(v[0], v[1], v[2]);
            const Vector<3, T> t = T(2) * q.cross(vector);
            return vector + (v[3] * t) + q.cross(t);
        }

        inline Vector<3, T> getRightVector() const noexcept
        {
            return rotateVector(Vector<3, T>(1, 0, 0));
        }

        inline Vector<3, T> getUpVector() const noexcept
        {
            return rotateVector(Vector<3, T>(0, 1, 0));
        }

        inline Vector<3, T> getForwardVector() const noexcept
        {
            return rotateVector(Vector<3, T>(0, 0, 1));
        }

        inline Quaternion& lerp(const Quaternion& q1, const Quaternion& q2, T t) noexcept
        {
            *this = (q1 * (T(1) - t)) + (q2 * t);
            return *this;
        }
    };

    using QuaternionF = Quaternion<float>;
}

#endif // OUZEL_MATH_QUATERNION_HPP
