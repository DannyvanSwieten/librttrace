#pragma once

#include "quaternion.hpp"

struct InstanceTransform
{
	static constexpr size_t SIZE = 12;
	static constexpr size_t COLUMNS = 3;
	static constexpr size_t ROWS = 4;
	float transform[12];
};

InstanceTransform operator*(const InstanceTransform& lhs, const InstanceTransform& rhs);
InstanceTransform quat_to_transform(const Quaternion& q);

class Transform
{
public:
	Transform();
	Transform(const Float3& translation, const Float3& rotation, const Float3& scale);

	InstanceTransform get_instance_transform() const;

	void set_translation(const Float3& translation);
	void set_rotation(const Float3& rotation);
	void set_scale(const Float3& scale);

	Float3 translation() const;
	Float3 rotation() const;
	Float3 scale() const;

private:
	Float3 m_translation;
	Float3 m_rotation;
	Float3 m_scale;
};