#pragma once
#include "voro++.hh"
#include <Urho3D/Core/Variant.h>



// conversion from voro++ voronoi cells to trimesh output

Urho3D::Variant GetMeshFromVoronoiCell(voro::voronoicell &c, double x, double y, double z);

Urho3D::Variant GetNMeshFromVoronoiCell(voro::voronoicell &c, double x, double y, double z);

Urho3D::VariantVector GetMeshesFromContainer(voro::container &con);

Urho3D::VariantVector draw_cells_iogram(voro::c_loop_all &vl, voro::container &con);

Urho3D::VariantVector GetNMeshesFromContainer(voro::container &con);

Urho3D::VariantVector draw_n_cells_iogram(voro::c_loop_all &vl, voro::container &con);



