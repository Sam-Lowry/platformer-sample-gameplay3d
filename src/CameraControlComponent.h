#ifndef PLATFORMER_CAMERA_COMPONENT_H
#define PLATFORMER_CAMERA_COMPONENT_H

#include "Component.h"

namespace platformer
{
    /**
     * Controls the active camera, any smoothing/zooming etc should be applied by and configured via this component
     *
     * @script{ignore}
    */
    class CameraControlComponent : public Component
    {
    public:
        explicit CameraControlComponent();
        ~CameraControlComponent();

        void setTargetPosition(gameplay::Vector2 const & target);
    protected:
        virtual void onStart() override;
        virtual void finalize() override;
    private:
        CameraControlComponent(CameraControlComponent const &);

        gameplay::Camera * _camera;
    };
}

#endif
