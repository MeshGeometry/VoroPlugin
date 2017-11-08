#pragma once
#include "voro++.hh"
#include <Urho3D/Core/Variant.h>

// conversion from voro++ voronoi cells to trimesh output

Urho3D::Variant GetMeshFromVoronoiCell(voro::voronoicell c);

Urho3D::VariantVector GetMeshesFromContainer(voro::container con);

template<class c_loop>
Urho3D::VariantVector draw_cells_iogram(voro::c_loop &vl, voro::container con);
