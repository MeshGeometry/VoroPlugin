#include "VoroToIogram.h"
#include "TriMesh.h"
#include "NMesh.h"
#include "Polyline.h"
#include <iostream>

using Urho3D::Variant;
using Urho3D::VariantMap;
using Urho3D::VariantType;
using Urho3D::VariantVector;
using Urho3D::Vector3;
using Urho3D::Vector;

Variant GetMeshFromVoronoiCell(voro::voronoicell &c, double x, double y, double z)
{
	int i, j, k, l, m, n;
	double *ptsp = c.pts;

	// set up the lists
	VariantVector vertices;
	VariantVector faces;

	//verts
	for (i = 0; i < c.p; i++, ptsp += 3) {
		//Vector3 v(*ptsp, ptsp[1], ptsp[2]);
		Vector3 v(x+*ptsp*0.5, y+ptsp[1]*0.5, z+ptsp[2]*0.5);
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

Variant GetNMeshFromVoronoiCell(voro::voronoicell &c, double x, double y, double z)
{
    int i, j, k, l, m, n;
    double *ptsp = c.pts;
    
    // set up the lists
    VariantVector vertices;
    VariantVector faces;
    
    //verts
    for (i = 0; i < c.p; i++, ptsp += 3) {
        //Vector3 v(*ptsp, ptsp[1], ptsp[2]);
        Vector3 v(x+*ptsp*0.5, y+ptsp[1]*0.5, z+ptsp[2]*0.5);
        vertices.Push(Variant(v));
    }
    
    //faces
    for (i = 1; i<c.p; i++) for (j = 0; j<c.nu[i]; j++) {
        k = c.ed[i][j];
        if (k >= 0) {
            std::vector<int> face_verts;
            c.ed[i][j] = -1 - k;
            l = c.cycle_up(c.ed[i][c.nu[i] + j], k);
            m = c.ed[k][l]; c.ed[k][l] = -1 - m;
            face_verts.push_back(i);
            face_verts.push_back(k);
            while (m != i) {
                n = c.cycle_up(c.ed[k][c.nu[k] + l], m);
                //fprintf(fp, ",<%d,%d,%d>\n", i, k, m);
                
                face_verts.push_back(m);
                k = m; l = n;
                m = c.ed[k][l]; c.ed[k][l] = -1 - m;
            }
            // now convert to entries on faces.
            int sz = face_verts.size();
            faces.Push(Variant(sz));
            for (int j = 0; j< sz; ++j)
                faces.Push(face_verts[j]);
        }
    }
    
    return NMesh_Make(vertices, faces);
    

}


VariantVector draw_cells_iogram(voro::c_loop_all &vl, voro::container &con) {
	VariantVector ret;
	voro::voronoicell c;
	int counter = 0;
	int pid, ps = con.ps; double x, y, z, r;
	if (vl.start()) {
		do {
 			if (con.compute_cell(c, vl)) {

				vl.pos(pid, x, y, z, r);

				Variant M = GetMeshFromVoronoiCell(c, x, y, z);
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

VariantVector draw_n_cells_iogram(voro::c_loop_all &vl, voro::container &con) {
    VariantVector ret;
    voro::voronoicell c;
    int counter = 0;
    int pid, ps = con.ps; double x, y, z, r;
    if (vl.start()) {
        do {
            if (con.compute_cell(c, vl)) {
                
                vl.pos(pid, x, y, z, r);
                
                Variant M = GetNMeshFromVoronoiCell(c, x, y, z);
                ret.Push(M);
                std::cout << "Pushing mesh " << counter << std::endl;
                ++counter;
                
            }
        } while (vl.inc());
    }
    return ret;
}

Urho3D::VariantVector GetNMeshesFromContainer(voro::container &con)
{
    voro::c_loop_all vl(con);
    VariantVector ret = draw_n_cells_iogram(vl, con);
    return ret;
}
