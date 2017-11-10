#include "VoroToIogram.h"
#include "TriMesh.h"
#include "Polyline.h"
#include <iostream>

using Urho3D::Variant;
using Urho3D::VariantMap;
using Urho3D::VariantType;
using Urho3D::VariantVector;
using Urho3D::Vector3;
using Urho3D::Vector;

Variant GetMeshFromVoronoiCell(voro::voronoicell &c) 
{
	int i, j, k, l, m, n;
	double *ptsp = c.pts;

	// set up the lists
	VariantVector vertices;
	VariantVector faces;

	//verts
	for (i = 0; i < c.p; i++, ptsp += 3) {
		Vector3 v(*ptsp, ptsp[1], ptsp[2]);
		vertices.Push(Variant(v));
	}

	//faces
	for (i = 1; i<c.p; i++) for (j = 0; j<c.nu[i]; j++) {
		k = c.ed[i][j];
		if (k >= 0) {
			c.ed[i][j] = -1 - k;
			l = c.cycle_up(c.ed[i][c.nu[i] + j], k);
			m = c.ed[k][l]; c.ed[k][l] = -1 - m;
			while (m != i) {
				n = c.cycle_up(c.ed[k][c.nu[k] + l], m);
				//fprintf(fp, ",<%d,%d,%d>\n", i, k, m);
				faces.Push(Variant(i));
				faces.Push(Variant(k));
				faces.Push(Variant(m));
				k = m; l = n;
				m = c.ed[k][l]; c.ed[k][l] = -1 - m;
			}
		}
	}
	return TriMesh_Make(vertices, faces);
}


VariantVector draw_cells_iogram(voro::c_loop_all &vl, voro::container &con) {
	VariantVector ret;
	voro::voronoicell c; double *pp;
	int counter = 0;
	if (vl.start()) {
		do {
 			if (con.compute_cell(c, vl)) {

				Variant M = GetMeshFromVoronoiCell(c);
				ret.Push(M);
				std::cout << "Pushing mesh " << counter << std::endl;
				++counter;
				
			}
		} while (vl.inc());
	}
	return ret;
}

Urho3D::VariantVector GetMeshesFromContainer(voro::container &con)
{
	voro::c_loop_all vl(con);
	VariantVector ret = draw_cells_iogram(vl, con);
	return ret;
}