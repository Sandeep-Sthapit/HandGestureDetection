#ifndef DETECTOR
#define DETECTOR

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\background_segm.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"

static int init_count;

static int fist_count;
static int rpalm_count;
static int right_count;
static int no_posture_count;
static unsigned char flag_no_posture = 0;

static bool flag_go = 0;
static bool flag_back = 0;
static bool flag_right = 0;
static bool flag_left = 0;
static bool flag_stop = 0;
static bool flag_rpalm = 0;
static bool flag_fist = 0;
static bool flag_pright = 0;

static char posture = 0;
static bool change_posture = 0;


void reset_all_postures()
{
	flag_rpalm = 0;
	flag_fist = 0;
	flag_pright = 0;
}

void reset_all_posture_count()
{
	fist_count = 0;
	rpalm_count = 0;
	right_count = 0;
}

void getSkin(cv::Mat &src, cv::Mat &output)
{
	int time_count1 = 0;
	int time_count2 = 0;

	output = src;
	//detector for right palm
	cv::CascadeClassifier rpalm_cascade;
	if (!rpalm_cascade.load("rpalm.xml"))
		printf("--(!)Error loading fist\n");
	std::vector<cv::Rect> rpalm;
	rpalm_cascade.detectMultiScale(src, rpalm, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	cv::Rect big_rpalm;
	if (rpalm.size() != NULL)
	{	
		no_posture_count = 0;
		big_rpalm = rpalm[0];
		flag_rpalm = 1;
		flag_no_posture = flag_no_posture | 0x01;
	}
	else
	{
		flag_no_posture = flag_no_posture & 0xFE;
	}
	for (int i = 0; i < rpalm.size(); i++)
	{
		if ((rpalm[i].width*rpalm[i].height) >(big_rpalm.width*big_rpalm.height))// detecting biggest rect
		{
			big_rpalm = rpalm[i];
		}
		cv::Point center(rpalm[i].x + rpalm[i].width*0.5, rpalm[i].y + rpalm[i].height*0.5);
		cv::rectangle(output, rpalm[i], cv::Scalar(0, 255, 0), 2);
	}

	//detector for fist
	cv::CascadeClassifier fist_cascade;
	if (!fist_cascade.load("fist.xml"))
		printf("--(!)Error loading fist\n");
	std::vector<cv::Rect> fists;
	fist_cascade.detectMultiScale(src, fists, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	cv::Rect big_fist;
	if (fists.size() != NULL)
	{
		no_posture_count = 0;
		big_fist = fists[0];
		flag_fist = 1;
		flag_no_posture = flag_no_posture | 0x02;
	}
	else
	{
		flag_no_posture = flag_no_posture & 0xFD;
	}
	for (int i = 0; i < fists.size(); i++)
	{
		if ((fists[i].width*fists[i].height) >(big_fist.width*big_fist.height))//detecting biggest rect
		{
			big_fist = fists[i];
		}
		cv::Point center(fists[i].x + fists[i].width*0.5, fists[i].y + fists[i].height*0.5);
		cv::rectangle(output, fists[i], cv::Scalar(255, 0, 0), 2);
	}
	//detecting largest contour for gesture detection
	cv::CascadeClassifier right_cascade;
	if (!right_cascade.load("right.xml"))
		printf("--(!)Error loading fist\n");
	std::vector<cv::Rect> rect_right;
	right_cascade.detectMultiScale(src, rect_right, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	cv::Rect big_right;
	if (rect_right.size() != NULL)
	{
		no_posture_count = 0;
		big_right = rect_right[0];
		flag_pright = 1;
		flag_no_posture = flag_no_posture | 0x08;
	}
	else
	{
		flag_no_posture = flag_no_posture & 0xF7;
	}
	for (int i = 0; i < rect_right.size(); i++)
	{
		cv::Point center(rect_right[i].x + rect_right[i].width*0.5, rect_right[i].y + rect_right[i].height*0.5);
		cv::rectangle(output, rect_right[i], cv::Scalar(0, 0, 255), 2);
	}
	init_count++;
	if (init_count > 300)
		init_count = 0;
	//if no posture increase no posture count
	if (flag_no_posture == 0)
	{
		no_posture_count++;
	}
	if (no_posture_count > 1)
	{
		reset_all_postures();
	}

	// count the time after posture is detected
	if (flag_rpalm == 1)
	{
		rpalm_count++;
	}
	else{
		rpalm_count = 0;
	}
	if (flag_fist == 1)
	{
		fist_count++;
	}
	else{
		fist_count = 0;
	}
	if (flag_pright == 1)
	{
		right_count++;
	}
	else{
		right_count = 0;
	}

	
	//checking gesture
	if (flag_fist == 1 && flag_rpalm == 1)
	{

		if ((fist_count > rpalm_count))
		{
			flag_stop = 1;
			std::cout << "STOP \n";
			rpalm_count = 0;
			fist_count = 0;
		}
		else if ((fist_count < rpalm_count))
		{
			flag_go = 1;
			std::cout << "GO \n";
			rpalm_count = 0;
			fist_count = 0;
		}

	}
	if (flag_rpalm == 1 && flag_pright == 1)
	{
		if ((rpalm_count > right_count))
		{
			flag_right = 1;
			std::cout << "TURN RIGHT \n";
			right_count = 0;
			rpalm_count = 0;
		}
		else if ((rpalm_count < right_count))
		{
			flag_left = 1;
			std::cout << "TURN LEFT \n";
			right_count = 0;
			rpalm_count = 0;
		}

	}
	if (flag_fist == 1 && flag_pright == 1)
	{
		if ((fist_count < right_count))
		{
			flag_back = 1;
			std::cout << "GO BACK \n";
			right_count = 0;
			fist_count = 0;
		}
		else if ((fist_count > right_count))
		{
			right_count = 0;
			fist_count = 0;
		}

	}
	
}


#endif