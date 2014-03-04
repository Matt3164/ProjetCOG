#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>


using namespace cv;

void InMatrix(int ind, Mat* inMat, Mat outMat){

	int indiceligne = 0;

	for(int i = 0; i < outMat.rows; i++ ){

		for(int j = 0; j < outMat.cols; j++){

			inMat->at<int>(indiceligne,ind) = outMat.at<int>(i,j);

			indiceligne++;
		}
	}

	
}

int main(int argc, char** argv){

	  Mat A = imread(argv[1], 0);
	 
	  Mat D(A, Rect(5,5,5,5));

	  std::cout << D << std::endl;
	  std::cout << "Taille de la matrice : " << D.rows << " " << D.cols << std::endl;
	  std::cout << "Premier terme : " << (int) D.at<uchar>(0,0) << std::endl;
	  std::cout << "Dernier terme : " << (int) D.at<uchar>(5,5) << std::endl;
	  std::cout << A.rows << " " << A.cols << std::endl;

	  int colbound = int(A.cols * A.rows / 5.);
	  Mat M(25,colbound, CV_8UC3, Scalar(0));

	  std::cout << "Je reprends la main " << std::endl;
	  	  
	  std::cout << A.rows << " " << A.cols << std::endl;

	  int indice = 0;

	  for(int i = 0; i < A.cols-6; i+=6){

		  for(int j = 0; j < A.rows-6; j+=6){

			  Mat E(A, Rect(i,j,5,5));

			  InMatrix(indice, &M, E);
			  indice++;
			  }
	  }

	  std::cout << indice << std::endl;


	  Mat feat(A, Rect(13,16,5,5));
	  Mat Vectfeat(25,1, CV_32F, Scalar(0));

	  InMatrix(0, &Vectfeat, feat);

	  cv::flann::KDTreeIndexParams indexParams(5);

	  //Create the index
	  
	  Mat features(M.rows, M.cols, CV_32F, Scalar(0));
	  
	  //std::cout << M(Range(1,5), Range(1,600)) << std::endl;
	  std::cout << M.rows << " " << M.cols << std::endl;
	  cv::flann::Index kdtree(features, indexParams);
	  
	  std::cout << "Performing a single search to find closest data point to mean : " << std::endl;
	  

	  

	  

	  
	  
	  return EXIT_SUCCESS;
}




