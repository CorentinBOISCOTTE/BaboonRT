#pragma once
#include "Color.h"
#include "glm/vec3.hpp"

class HitRecord;
class Ray;

class Material
{
public:
	Material() = default;
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
	public:
	explicit Lambertian(const glm::vec3 albedo) : m_albedo(albedo) {}
	bool Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	glm::vec3 m_albedo;
};

class Metal : public Material
{
	public:
	Metal(const glm::vec3 albedo, const float fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1.f ? fuzz : 1.f) {}
	bool Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
private:
	glm::vec3 m_albedo;
	float m_fuzz;
};
