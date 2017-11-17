#include "PluginRegistration.h"
#include <Urho3D/Core/Context.h>
#include "ComponentRegistration.h"
#include "Voro_Voronoi.h"
#include "Voro_PullToCentroid.h"
#include "Voro_VoronoiFromPoints.h"
#include "Voro_FrameNmesh.h"

using namespace Urho3D;

extern "C"
{

	DLLEXPORT void IogramPluginMain(Urho3D::Context* context)
	{
		RegisterIogramType<Voro_Voronoi>(context);
		RegisterIogramType<Voro_PullToCentroid>(context);
		RegisterIogramType<Voro_VoronoiFromPoints>(context);
        RegisterIogramType<Voro_FrameNmesh>(context);
	}

}
