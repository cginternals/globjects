#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>
#include <glowutils/AxisAlignedBoundingBox.h>


class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace glow
{

class AbstractCoordinateProvider;
class Camera;


class GLOWUTILS_API WorldInHandNavigation
{
	enum InteractionMode
	{
		NoInteraction
	,	PanInteraction
	,	RotateInteraction
	};

public:
	WorldInHandNavigation(Camera & camera);
	virtual ~WorldInHandNavigation();

    void setBoundaryHint(const AxisAlignedBoundingBox & aabb); ///< is currently ignored
    void setCoordinateProvider(AbstractCoordinateProvider * provider);

	virtual void reset(bool update = true);

protected:
	void panningBegin(const glm::ivec2 & mouse);
	void panningProcess(const glm::ivec2 & mouse);
	void panningEnd();

	void rotatingBegin(const glm::ivec2 & mouse);
	void rotatingProcess(const glm::ivec2 & mouse);
	void rotatingEnd();

    void pan(glm::vec3 t);
    void rotate(float hAngle, float vAngle);

	void scaleAtCenter(float scale);
	void scaleAtMouse(const glm::ivec2 & mouse,	float scale);
	void resetScaleAtMouse(const glm::ivec2 & mouse);

//	void enforceWholeMapVisible(const float offset = 0.08);

	// constraints

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
    Camera & m_camera;
    AxisAlignedBoundingBox m_aabb;

    AbstractCoordinateProvider * m_coordsProvider;

    bool m_rotationHappened;
	InteractionMode m_mode;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::mat4 m_viewProjectionInverted;

    glm::vec3  m_i0;
    glm::vec3  m_i1;
    bool  m_i0Valid; // stores if initial interaction pick yielded valid depth
    glm::ivec2  m_m0;
};

} // namespace glow
