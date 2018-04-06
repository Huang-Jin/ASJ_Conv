#ifndef __MATCHER__
#define __MATCHER__
#include "aacj.h"

class Matcher
{
public:
	Matcher(const AACJDetection &aacj1, const AACJDetection &aacj2);
	~Matcher();

	void getEndPoints(const Junction &Junct, cv::Point2f *X)
	{
		X[0] = Junct.location;
		float theta1 = Junct.branch[0].branch;
		float scale1 = Junct.branch[0].branchScale;
		float theta2 = Junct.branch[1].branch;
		float scale2 = Junct.branch[0].branchScale;
		X[1] = X[0] + cv::Point2f(cos(theta1), sin(theta1))*scale1;
		X[2] = X[0] + cv::Point2f(cos(theta2), sin(theta2))*scale2;
	}

	cv::Mat distance;
	cv::Mat im1, im2;
	std::vector<Junction> Jlist1, Jlist2;
	std::vector<std::vector<cv::Mat>> H;
private:

};

Matcher::Matcher(const AACJDetection &aacj1, const AACJDetection &aacj2)
{
	Jlist1 = aacj1.Junctions;
	Jlist2 = aacj2.Junctions;
	im1 = aacj1.image.clone();
	im2 = aacj2.image.clone();
	size_t n1 = Jlist1.size(), n2 = Jlist2.size();
	distance = cv::Mat(n1, n2, CV_32FC1);
	cv::Point2f X[3], Y[3];
	for (int i = 0; i < n1; ++i)
	{
		std::vector<cv::Mat> temp;
		for (int j = 0; j < n2; ++j)
		{
			//std::cout << i << " " << j << std::endl;
			getEndPoints(Jlist1[i], X);
			getEndPoints(Jlist2[j], Y);
			cv::Mat homo = getAffineTransform(X, Y);
			temp.push_back(homo);
		}
		H.push_back(temp);
	}
}

Matcher::~Matcher()
{
}
#endif