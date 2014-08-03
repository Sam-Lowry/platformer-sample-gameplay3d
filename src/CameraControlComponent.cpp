#include "CameraControlComponent.h"

#include "GameObject.h"
#include "GameObjectController.h"
#include "Common.h"

namespace platformer
{
    CameraControlComponent::CameraControlComponent()
        : _camera(nullptr)
    {
    }

    CameraControlComponent::~CameraControlComponent()
    {
    }

    void CameraControlComponent::onStart()
    {
        _camera = GameObjectController::getInstance().getScene()->getActiveCamera();
        _camera->addRef();
    }

    void CameraControlComponent::finalize()
    {
        SAFE_RELEASE(_camera);
    }

    void CameraControlComponent::setTargetPosition(gameplay::Vector2 const & target)
    {
        _camera->getNode()->setTranslation(std::move(gameplay::Vector3(target.x, target.y, 0.0f)));
    }
}
