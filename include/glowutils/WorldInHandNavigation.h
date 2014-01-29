#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>
#include <glowutils/AxisAlignedBoundingBox.h>

namespace glowutils
{

class AbstractCoordinateProvider;
class Camera;

class GLOWUTILS_API WorldInHandNavigation
{
public:
	enum InteractionMode
	{
		NoInteraction
	,	PanInteraction
	,	RotateInteraction
	};

public:
	WorldInHandNavigation();
	virtual ~WorldInHandNavigation();

    void setBoundaryHint(const AxisAlignedBoundingBox & aabb); ///< is currently ignored

    void setCamera(Camera * camera);
    void setCoordinateProvider(AbstractCoordinateProvider * provider);

    virtual void reset();

    InteractionMode mode() const;

public:
	void panBegin(const glm::ivec2 & mouse);
	void panProcess(const glm::ivec2 & mouse);
	void panEnd();

	void rotateBegin(const glm::ivec2 & mouse);
	void rotateProcess(const glm::ivec2 & mouse);
	void rotateEnd();

    void pan(glm::vec3 t);
    void rotate(float hAngle, float vAngle);

	void scaleAtCenter(float scale);
	void scaleAtMouse(const glm::ivec2 & mouse,	float scale);
	void resetScaleAtMouse(const glm::ivec2 & mouse);

//	void enforceWholeMapVisible(const float offset = 0.08);

	// constraints
protected:
	void enforceTranslationConstraints(glm::vec3 & p) const;
	void enforceRotationConstraints(
		float & hAngle
	,	float & vAngle) const;
	void enforceScaleConstraints(
		float & scale
	,	glm::vec3 & i) const;

	// math

	const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse) const;
    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse
    ,   const glm::vec3 & p0) const;
    const glm::vec3 mouseRayPlaneIntersection(
        bool & intersects
    ,   const glm::ivec2 & mouse
	,	const glm::vec3 & p0
	,	const glm::mat4 & viewProjectionInverted) const;

protected:
    Camera * m_camera;
    AxisAlignedBoundingBox m_aabb;

    AbstractCoordinateProvider * m_coordsProvider;

    bool m_rotationHappened;
	InteractionMode m_mode;

    glm::vec3 m_homeEye;
    glm::vec3 m_homeCenter;
    glm::vec3 m_homeUp;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::mat4 m_viewProjectionInverted;

    glm::vec3  m_i0;
    glm::vec3  m_i1;
    bool  m_i0Valid; // stores if initial interaction pick yielded valid depth
    glm::ivec2  m_m0;
};

} // namespace glowutils
