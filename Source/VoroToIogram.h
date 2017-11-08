#pragma once
#include "voro++.hh"
#include <Urho3D/Core/Variant.h>

// conversion from voro++ voronoi cells to trimesh output

Urho3D::Variant GetMeshFromVoronoiCell(voro::voronoicell);
