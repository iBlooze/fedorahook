#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include "../SDK/VMatrix.hpp"
#include "../SDK/IVEngineClient.hpp"
#include "../Utilities/Platform.hpp"
#include <cmath>
#include <math.h>
#include <tgmath.h>
#define PI 3.14159265358979323846f
#define RADPI 57.295779513082f
#define PI_F	((float)(PI)) 

namespace math
{	
	//--------------------------------------------------------------------------------
	static void sincos(float a, float* s, float* c)
	{
		*s = sin(a);
		*c = cos(a);
	}

	static void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		auto dot_product = [](const Vector& a, const Vector& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		};

		out[0] = dot_product(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = dot_product(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = dot_product(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	}
	static void normalize_angles(QAngle& angles)
	{
		for (auto i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}

	static float normalize_angle(float angle) {
		if (!std::isfinite(angle))
		{
			return 0.0f;
		}

		return std::remainder(angle, 360.0f);
	}
	static float normalize_float(float angle) {
		auto revolutions = angle / 360.f;
		if (angle > 180.f || angle < -180.f) {
			revolutions = round(abs(revolutions));
			if (angle < 0.f)
				angle = (angle + 360.f * revolutions);
			else
				angle = (angle - 360.f * revolutions);
			return angle;
		}
		return angle;
	}

	static void NormalizeVector(Vector& vec)
	{
		for (auto i = 0; i < 3; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] > 180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}

	//--------------------------------------------------------------------------------
	static void clamp_angles(QAngle& angles)
	{
		angles.x = max(-89.0f, min(89.0f, normalize_angle(angles.x)));
		angles.y = normalize_angle(angles.y);
		angles.z = 0.0f;
	}
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	static void angle_vectors(const QAngle& angles, Vector& forward)
	{
		float	sp, sy, cp, cy;

		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	//--------------------------------------------------------------------------------
	static void angle_vectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;

		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);
		sincos(DEG2RAD(angles[2]), &sr, &cr);

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = (cr * cp);
	}
	//--------------------------------------------------------------------------------
	static void vector_angles(const Vector& forward, QAngle& angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles[1] = 0.0f;
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;
	}

	static Vector cross_prod(const Vector& a, const Vector& b)
	{
		return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	static void vector_angles(const Vector& forward, Vector& up, QAngle& angles)
	{
		Vector left = cross_prod(up, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / PI_F;
			angles.y = atan2f(forward.y, forward.x) * 180 / PI_F;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / PI_F;
		}
		else
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / PI_F;
			angles.y = atan2f(-left.x, left.y) * 180 / PI_F;
			angles.z = 0;
		}
	}
	//--------------------------------------------------------------------------------
	static bool screen_transform(const Vector& origin, Vector& screen)
	{
		static std::uintptr_t pViewMatrix;
		if (!pViewMatrix)
		{
			pViewMatrix = static_cast<std::uintptr_t>(platform::find_signature("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			pViewMatrix += 3;
			pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
			pViewMatrix += 176;
		}

		const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
		screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
		screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];
		screen.z = 0.0f;

		float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

		if (w < 0.001f)
		{
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	}
	//--------------------------------------------------------------------------------
	static bool wts(const Vector& origin, Vector& screen)
	{
		const auto screenTransform = [&origin, &screen]() -> bool
		{
			static std::uintptr_t pViewMatrix;
			if (!pViewMatrix)
			{
				pViewMatrix = static_cast<std::uintptr_t>(platform::find_signature("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
				pViewMatrix += 3;
				pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
				pViewMatrix += 176;
			}

			const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
			screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
			screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];
			screen.z = 0.0f;

			float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

			if (w < 0.001f)
			{
				screen.x *= 100000;
				screen.y *= 100000;
				return true;
			}

			float invw = 1.f / w;
			screen.x *= invw;
			screen.y *= invw;

			return false;
		};

		if (!screenTransform())
		{
			int iScreenWidth, iScreenHeight;
			//g_engine->GetScreenSize(iScreenWidth, iScreenHeight);

			screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
			screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

			return true;
		}
		return false;
	}

	static float get_fov(const QAngle& viewAngle, const QAngle& aimAngle)
	{
		Vector ang, aim;

		angle_vectors(viewAngle, aim);
		angle_vectors(aimAngle, ang);

		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}
	static inline float clamp(float yaw, float min, float max)
	{
		if (yaw > max)
			return max;
		else if (yaw < min)
			return min;

		return yaw;
	}
	static float get_dist(Vector src, Vector dst)
	{
		return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2) + pow(src.z - dst.z, 2));
	}

	static QAngle calc_angle(Vector src, Vector dst)
	{
		QAngle angles;
		Vector delta = src - dst;

		vector_angles(delta, angles);

		angles.Length();

		return angles;
	}

	static void smooth_angle(QAngle src, QAngle& dst, float factor)
	{
		QAngle delta = dst - src;

		normalize_angles(delta);

		dst = src + delta / factor;
	}


	static void angle_matrix(const QAngle& angles, matrix3x4_t& matrix)
	{
		float sr, sp, sy, cr, cp, cy;

		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);
		sincos(DEG2RAD(angles[2]), &sr, &cr);

		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;
		matrix[0][1] = sp * srcy - crsy;
		matrix[1][1] = sp * srsy + crcy;
		matrix[2][1] = sr * cp;

		matrix[0][2] = (sp * crcy + srsy);
		matrix[1][2] = (sp * crsy - srcy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	}

	static void set_matrix_column(const Vector& in, int column, matrix3x4_t& out)
	{
		out[0][column] = in.x;
		out[1][column] = in.y;
		out[2][column] = in.z;
	}

	static void angle_matrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix)
	{
		angle_matrix(angles, matrix);
		set_matrix_column(position, 3, matrix);
	}

	static float __fastcall angle_diff(float a1, float a2)
	{
		float val = fmodf(a1 - a2, 360.0);

		while (val < -180.0f) val += 360.0f;
		while (val > 180.0f) val -= 360.0f;

		return val;
	}
};