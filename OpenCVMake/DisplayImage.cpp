#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <fstream>
#include <iostream>


using namespace cv;

void InMatrix(int ind, Mat* inMat, Mat outMat){

	int indiceligne = 0;

	for(int i = 0; i < outMat.rows; i++ ){

		for(int j = 0; j < outMat.cols; j++){

			inMat->at<float>(indiceligne,ind) = (float) outMat.at<uchar>(i,j);

			indiceligne++;
		}
	}

	
}

void ToIndex(int indice, int col, int* tab){

	if( (indice % col) == 0 ){
		tab[1] = col;
	}else{

		tab[1] = indice % col;
	}

	tab[0] = int ((indice - tab[1]) / col) + 1;
}

int main(int argc, char** argv){

	  Mat A = imread(argv[1], 0);
	 
	  int colbound = int(A.cols * A.rows / 4.);
	  Mat M(25,colbound, CV_32F, Scalar(0));
	  	  	  
	  
	  int indice = 0;

	  for(int i = 0; i <= A.cols-5; i+=2){

		  for(int j = 0; j <= A.rows-5; j+=2){

			  Mat E(A, Rect(i,j,5,5));

			  InMatrix(indice, &M, E);
			  indice++;
			  }
	  }
	  

	  

	  
	  

	  

	  //cv::flann::KDTreeIndexParams indexParams(4);
	  

	  
	  
	  
	  
	  std::cout << "Performing a single search to find closest data point to mean : " << std::endl;


	  std::vector<int> index(1);
	  std::vector<float> dist(1);
	  RNG rng;
	  double result = 0;

	  std::string dir = "test.txt";

	  std::ofstream myfile;

	  myfile.open(dir);
	  myfile << "Test: \n";	  
	  for(int j = 100; j < 10000; j+=100){
		  result = 0;
		  cv::flann::KMeansIndexParams indexParams(2056,j,cvflann::FLANN_CENTERS_KMEANSPP);
		  cv::flann::Index kdtree(M.t(), indexParams);
		  for(int i = 0; i < 100; i++){
		  
			  Mat feat(A, Rect(rng.uniform(0,A.cols-6),rng.uniform(0,A.rows-6),5,5));
			  Mat Vectfeat(25,1, CV_32F, Scalar(0));
			  InMatrix(0, &Vectfeat, feat);
	  
			  kdtree.knnSearch(Vectfeat.t(), index, dist, 1);

	  		  std::cout << "(index, dist) : " << index[0] << " " << dist[0] << std::endl;
			  result+=dist[0];

		  }
		  myfile << j << " " <<result << "\n";
	  
	  }

	  myfile.close();
	  return EXIT_SUCCESS;
}




