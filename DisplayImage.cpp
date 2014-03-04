#include <cv.h>
#include <highgui.h>

using namespace cv;

void InMatrix(int ind, Mat* inMat, Mat outMat){

	int indiceligne = 0;

	for(int i = 0; i <= outMat.rows; i++ ){

		for(int j = 0; j <= outMat.cols; j++){

			inMat->at<int>(indiceligne,ind) = outMat.at<int>(i,j);

			indiceligne++;
		}
	}
}

int main(int argc, char** argv){

	  Mat A = imread(argv[1], 0);
	  //Mat D=A(Range(1,5), Range(2,4));
	  Mat D(A, Rect(4,4,4,4));

	  int colbound = int(A.cols * A.rows / 5.);
	  Mat M(35,colbound, CV_8UC3, Scalar(0));

	  std::cout << "Je reprends la main " << std::endl;
	  std::cout << D <<std::endl;
	  std::cout << Rect(4,4,4,4).tl() << std::endl;
	  std::cout << Rect(4,4,4,4).br() << std::endl;
	  std::cout << (int) D.at<uchar>(4,3) << std::endl;
	  D.at<uchar>(4,3) = 5;
	  std::cout << (int) D.at<uchar>(4,3) << std::endl;
	  std::cout << A.rows << " " << A.cols << std::endl;

	  int indice = 0;

	  for(int i = 0; i < 120/*A.rows-6*/; i+=6){

		  for(int j = 0; j < 120 /*A.cols-6*/; j+=6){

			  Mat E(A, Rect(i,j,5,5));

			  InMatrix(indice, &M, E);
			  indice++;
			  std::cout << indice << std::endl;

			  }
	  }

	  std::cout << M.rows << " " << M.cols << std::endl;

	  return EXIT_SUCCESS;
}




