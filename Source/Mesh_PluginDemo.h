#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Variant.h>
#include "IoComponentBase.h"

using namespace Urho3D;

class URHO3D_API Mesh_FieldRemesh : public IoComponentBase {
	URHO3D_OBJECT(Mesh_FieldRemesh, IoComponentBase)
public:
	Mesh_FieldRemesh(Urho3D::Context* context);

	void SolveInstance(
		const Urho3D::Vector<Urho3D::Variant>& inSolveInstance,
		Urho3D::Vector<Urho3D::Variant>& outSolveInstance
		);


	static String iconTexture;



};