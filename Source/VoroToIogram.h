#pragma once
#include "voro++.hh"
#include <Urho3D/Core/Variant.h>



// conversion from voro++ voronoi cells to trimesh output
#define EXPORT __attribute__((visibility("default")))

EXPORT
Urho3D::Variant GetMeshFromVoronoiCell(voro::voronoicell &c, double x, double y, double z);

Urho3D::VariantVector GetMeshesFromContainer(voro::container &con);

Urho3D::VariantVector draw_cells_iogram(voro::c_loop_all &vl, voro::container &con);



