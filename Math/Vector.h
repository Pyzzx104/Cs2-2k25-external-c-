#pragma once
#include "../Render/includes.h"
#include "../utils/globals.h"



const float PI = acos(-1.0);

extern int screenWidth = GetSystemMetrics(SM_CXSCREEN);
extern int screenHeight = GetSystemMetrics(SM_CYSCREEN);



struct Vector2
{
	float x, y;
};

struct Vector4
{
	float w, x, y, z;
};


struct view_matrix_t
{
	float* operator[ ](int index)  
	{
		return matrix[index];
	}

	float matrix[4][4];

};


struct Vector3
{
    // Construtor
    Vector3(
        const float x = 0.f,
        const float y = 0.f,
        const float z = 0.f) noexcept :
        x(x), y(y), z(z) {}

    // Operador de subtra��o
    Vector3 operator-(const Vector3& other) const noexcept
    {
        return Vector3{ x - other.x, y - other.y, z - other.z };
    }

    // Operador de adi��o
    Vector3 operator+(const Vector3& other) const noexcept
    {
        return Vector3{ x + other.x, y + other.y, z + other.z };
    }

    // Operador de divis�o
    Vector3 operator/(const float factor) const noexcept
    {
        return Vector3{ x / factor, y / factor, z / factor };
    }

    // Operador de multiplica��o
    Vector3 operator*(const float factor) const noexcept
    {
        return Vector3{ x * factor, y * factor, z * factor };
    }

    // M�todo para calcular o comprimento do vetor
    float Length() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 WTS(view_matrix_t matrix) const
    {
        float _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
        float _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];

        float w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];

        if (w < 0.01f)
            return Vector3{ 0, 0, 0 };

        float inv_w = 1.f / w;
        _x *= inv_w;
        _y *= inv_w;

        float x = screenWidth / 2;
        float y = screenHeight / 2;

        x += 0.5f * _x * screenWidth + 0.5f;
        y -= 0.5f * _y * screenHeight + 0.5f;

        return Vector3{ x, y, w };
    }

    const Vector3 ToAngle() const noexcept
    {
        return Vector3{
            std::atan2(-z, std::hypot(x, y)) * (180.0f / PI),
            std::atan2(y, x) * (180.0f / PI),
            0.0f };
    }

    const bool IsZero() const noexcept
    {
        return x == 0.f && y == 0.f && z == 0.f;
    }

    // Dados do vetor
    float x, y, z;
};

