#include <iostream>
#include "igl/readOFF.h"
#include <Eigen/Dense>
#include "tetgen-master/tetgen.h"
#include <set>
#include "igl/writeOBJ.h"

using namespace Eigen;
using namespace std;

// this function reads the tetgen output
void readTetGen(tetgenio output, MatrixXi& F, MatrixXi& T, MatrixXd& V);

// this function merges the mesh
void mergeMesh(MatrixXi& T, MatrixXi& F, MatrixXd& V,
               MatrixXi& Ts, MatrixXi& Fs, MatrixXd& Vs,
               MatrixXi& Tp, MatrixXi& Fp, MatrixXd& Vp);

int main() {

    // read output to matrices
    MatrixXi Tp , Fp , Ts, Fs;
    MatrixXd Vp , Vs;

    // TetGen switches
    char TETGEN_SWITCHES[] = "pq1.414a0.002";

    // Behavior
    tetgenbehavior* behaviour;
    behaviour = new tetgenbehavior;

    // loads the input for hockey puck
    tetgenio* input_hp;
    input_hp = new tetgenio;

    if (input_hp->load_off("C:/Users/afg545/Documents/Coding/MergeMesh/resources/hockeypuck.off"))
    {
        tetgenio* output_hp;
        output_hp = new tetgenio;
        tetrahedralize(behaviour, input_hp, output_hp);
        readTetGen(*output_hp,Fs,Ts,Vs);
    }


    // loads the input for the straw
    tetgenio* input_straw;
    input_straw = new tetgenio;

    if(input_straw->load_off("C:/Users/afg545/Documents/Coding/MergeMesh/resources/straw.off"))
    {
        tetgenio* output_straw;
        output_straw = new tetgenio;
        tetrahedralize(behaviour, input_straw, output_straw);
        readTetGen(*output_straw,Fs,Ts,Vs);
    }

    //igl::writeOBJ("C:/Users/afg545/Documents/Coding/MergeMesh/resources/straw.obj",Vs,Fs);
    //igl::writeOBJ("C:/Users/afg545/Documents/Coding/MergeMesh/resources/puck.obj",Vp,Fp);

    // Plot the mesh
    //igl::opengl::glfw::Viewer viewer;
    //viewer.data().set_mesh(Vs, Fs);
    //viewer.launch();


    return 0;
}

void readTetGen(tetgenio output, MatrixXi& F, MatrixXi& T, MatrixXd& V)
{

    V.resize(output.numberofpoints,3);

    // create a vertex in the object for each point of the result
    for (int p = 0, pi = 0; p < output.numberofpoints; ++p, pi += 3)
    {
        Vector3d point;
        point(0) = (output.pointlist[pi + 0]);
        point(1) = (output.pointlist[pi + 1]);
        point(2) = (output.pointlist[pi + 2]);
        //a_chai3dMesh->newVertex(point);

        V.row(p) = point;

    }

    F.resize(output.numberoftrifaces,3);

    // create a triangle for each face on the surface
    for (int t = 0, ti = 0; t < output.numberoftrifaces; ++t, ti += 3)
    {
        Vector3i face;
        face(0) = (output.trifacelist[t+0]);
        face(0) = (output.trifacelist[t+0]);
        face(0) = (output.trifacelist[t+0]);

        F.row(t) = face;
    }



    // find out exactly which vertices are on the inside and outside
    set<int> inside, outside;
    for (int t = 0; t < output.numberoftrifaces * 3; ++t)
    {
        outside.insert(output.trifacelist[t]);
    }
    for (int p = 0; p < output.numberofpoints; ++p)
    {
        if (outside.find(p) == outside.end())
            inside.insert(p);
    }


    T.resize(output.numberoftetrahedra,4);

    for (int t = 0, ti = 0; t < output.numberoftetrahedra; ++t, ti += 4)
    {
        int v0 = output.tetrahedronlist[ti + 0];
        int v1 = output.tetrahedronlist[ti + 1];
        int v2 = output.tetrahedronlist[ti + 2];
        int v3 = output.tetrahedronlist[ti + 3];

        Eigen::RowVector4i tetrahedron;
        tetrahedron[0] = v0;
        tetrahedron[1] = v1;
        tetrahedron[2] = v2;
        tetrahedron[3] = v3;

        T.row(t) = (tetrahedron);
    }
    std::cout << V << std::endl;
}

void mergeMesh(MatrixXi& T, MatrixXi& F, MatrixXd& V,
               MatrixXi& Ts, MatrixXi& Fs, MatrixXd& Vs,
               MatrixXi& Tp, MatrixXi& Fp, MatrixXd& Vp)
{

}
