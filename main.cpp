/*
 * main.cpp
 *
 *  Created on: 6 mars 2014
 *      Author: matthieulegoff
 */


#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "nl.h"
#include <strstream>

using namespace cv;

void InMatrixI(int ind, Mat* inMat, Mat outMat){

	int indiceligne = 0;

	for(int i = 0; i < outMat.rows; i++ ){

		for(int j = 0; j < outMat.cols; j++){

			inMat->at<float>(indiceligne,ind) = (float) outMat.at<uchar>(i,j);

			indiceligne++;
		}
	}
}

void InMatrixF(int ind, Mat* inMat, Mat outMat){

	int indiceligne = 0;

	for(int i = 0; i < outMat.rows; i++ ){

		for(int j = 0; j < outMat.cols; j++){

			inMat->at<float>(indiceligne,ind) = outMat.at<float>(i,j);

			indiceligne++;
		}
	}
}



void ToIndex(int indice, int col, int* tab){

	tab[1] = indice % col;

	tab[0] = int ((indice - tab[1]) / col);
}


int ToIndice(int i, int j, int col){

	return i*col + j;

}

void ToMatrix(std::map<int,int> &map, Mat* matrice, int ligne, int col){


	for(int i = 0; i < ligne; i++){

		for(int j = 0; j < col; j++){

			matrice->at<float>(i,j) = map[ToIndice(i,j,col)];
		}

	}

}

int main(int argc, char** argv){

	//Initialisation

	int w = 4;
	int alpha = 2;
	int pmax = 10;
	int qmax = 10;

	int ligneX = pmax * alpha + w;
	int colX = qmax*alpha + w;

	int tailleX = ligneX*colX;

	int nbVoisinageX = (pmax)*(qmax);

	std::cout << "Taille de X : [" << ligneX << " * " << colX << "]" << std::endl;
	std::cout << "Nombre de voisinages : " << nbVoisinageX << std::endl;

	Mat image = imread(argv[1], 0);

	std::cout << "Initialisation : [OK]" << std::endl;

	//Initialisation de l'arbre de recherche

	int pmaxZ = int((image.cols - w)/alpha);
	int qmaxZ = int((image.rows - w)/alpha);
	int NbVoisinageZ = (pmaxZ)*(qmaxZ);

	Mat VoisConcat(w*w,NbVoisinageZ, CV_32F, Scalar(0));

	std::cout << "Matrice M : " << VoisConcat.rows << " " << VoisConcat.cols << std::endl;

	int indice = 0;

	for(int i = 0; i < image.cols - w; i+=alpha){

		for(int j = 0; j < image.rows-w; j+=alpha){

			Mat E(image, Rect(i,j,w,w));

			InMatrixI(indice, &VoisConcat, E);
			indice++;
		}
	}

	std::cout << "Construction de la matrice M : [OK]" << std::endl;


	//Paramtres ˆ dŽterminer
	cv::flann::KMeansIndexParams indexParams(4,10000,cvflann::FLANN_CENTERS_KMEANSPP);
	//cv::flann::KDTreeIndexParams indexParams(1000);
	cv::flann::Index kdtree(VoisConcat.t(), indexParams);

	std::cout << "Construction de l'index : [OK]" << std::endl;


	//Initialisation des voisinages

	std::map<int, Mat> Voisinage;
	RNG rng;

	for(int i = 0; i < nbVoisinageX; i++){

		int x = rng.uniform(1,image.cols-5);
		int y = rng.uniform(1,image.rows-5);

		Mat A(image, Rect(x,y,5,5));
		Voisinage[i] =  A;
	}

	std::cout << "Initialisation des voisinages : [OK]" << std::endl;

	//Initialisation des pixels de dŽpart

	std::map<int, int> X;

	for(int i = 0; i < tailleX; i++){

		X[i] = rng.uniform(0,255);
	}

	std::cout << "Initialisation des pixels de X : [OK]" << std::endl;

	Mat MatX(ligneX,colX, CV_32F, Scalar(0));
	ToMatrix(X, &MatX, ligneX, colX);


	imwrite( "imRes/InitX.jpg", MatX );

	std::cout << "Affichage de X : [OK]" << std::endl;



	//DŽbut de la boucle

	double epsilon = 0;

	for(int compt = 0; compt < 10; compt++){

		//Optimisation ˆ voisinage fixŽ
		epsilon = 0.;
		nlNewContext();

		nlSolverParameteri(NL_NB_VARIABLES, tailleX);
		nlSolverParameteri(NL_SOLVER, NL_BICGSTAB);
		nlSolverParameteri(NL_LEAST_SQUARES, NL_TRUE);
		nlSolverParameteri(NL_MAX_ITERATIONS, 100);

		nlBegin(NL_SYSTEM);

		std::cout << "Initialisation de OpenNL : [OK]" << std::endl;

		for(int i = 0; i <= tailleX; i++){

			nlSetVariable(i, X[i]);

		}

		std::cout << "OpenNL: Definition des variables : [OK]" << std::endl;

		nlBegin(NL_MATRIX);

		int * tab = new int[2];

		for(int i = 0; i < nbVoisinageX; i++){
			ToIndex(i, pmax+1, tab);

			for(int j=0; j<w;j++){
				for(int k = 0; k < w; k++){
					//Construction de ligne
					nlBegin(NL_ROW);

					int indice = ToIndice(tab[0]*alpha + j, tab[1]*alpha + k, colX);

					//std::cout << tailleX << " " << tab[0] << " " << tab[1]  << " " << k << " " << j << " " << indice<< std::endl;

					for(int l = 0; l < tailleX; l++){
						if(l==indice){
							nlCoefficient(l, 1.);
						}else{
							nlCoefficient(l, 0.);
						}
					}
					nlEnd(NL_ROW);

					//std::cout << "OpenNL mis a jour de la ligne "<< i*w*w + j*w + k <<" : [OK]" << std::endl;

					//Construction du deuxime membre
					nlRowParameterd(NL_RIGHT_HAND_SIDE, (float) Voisinage[i].at<uchar>(k,j));

					//std::cout << "OpenNL mis a jour du membre de droite "<< i*w*w + j*w + k <<" : [OK]" << std::endl;
				}
			}
		}
		nlEnd(NL_MATRIX);

		std::cout << "Initialisation de A : [OK]" << std::endl;

		nlEnd(NL_SYSTEM);

		nlSolve();

		std::cout << "Resolution du probleme : [OK]" << std::endl;

		//Mis ˆ jour de X

		for(int i = 0; i <= tailleX; i++){
			epsilon += (X[i] - nlGetVariable(i))*(X[i] - nlGetVariable(i));
			X[i] = nlGetVariable(i);
		}

		std::cout << "Mis a jour de X : [OK]" << std::endl;

		if(epsilon < 0.0001){
			break;
		}
		ToMatrix(X, &MatX, ligneX, colX);
		std::stringstream ss;

		ss << "imRes/Res" << compt << ".bmp";

		imwrite( ss.str(), MatX );

		std::cout << "Affichage de la nouvelle image : [OK]" << std::endl;

		//Mis ˆ jour des voisinages

		int* tab1 = new int[2];
		std::vector<int> index(1);
		std::vector<float> dist(1);

		std::cout << "FLANN : Recherche dans l'arbre Begin" << std::endl;

		for(int i = 0; i < nbVoisinageX; i++){
		//for(int i = 0; i < NbVoisinageZ; i++){

			//std::cout << "Nouvelle Recherche" << std::endl;
			ToIndex(i,pmax, tab1 );
			Mat feat(MatX, Rect(tab1[1]*alpha, tab1[0]*alpha,w,w));
			Mat Vectfeat(w*w,1, CV_32F, Scalar(0));
			//std::cout << "Entree : \n" << feat << std::endl;
			InMatrixF(0, &Vectfeat, feat);

			kdtree.knnSearch(Vectfeat.t(), index, dist, 1);

			ToIndex(index[0], pmaxZ, tab1);
			Mat A(image, Rect(tab1[1]*alpha, tab1[0]*alpha, w, w ));
			//std::cout << "Resultat : \n" <<  A << std::endl;
			Voisinage[i] = A;

			//std::cout << "Recherche finie" << std::endl;

		}

		delete [] tab1;

		std::cout << "Recherche des plus proches voisins : [OK]" << std::endl;
	}



	return 0;
}



