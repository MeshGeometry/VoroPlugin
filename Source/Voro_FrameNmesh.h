#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Variant.h>
#include "IoComponentBase.h"

using namespace Urho3D;

class URHO3D_API Voro_FrameNmesh : public IoComponentBase {
    URHO3D_OBJECT(Voro_FrameNmesh, IoComponentBase)
public:
    Voro_FrameNmesh(Urho3D::Context* context);
    
    void SolveInstance(
                       const Urho3D::Vector<Urho3D::Variant>& inSolveInstance,
                       Urho3D::Vector<Urho3D::Variant>& outSolveInstance
                       );
    
    
    static String iconTexture;
    
    
    
};
