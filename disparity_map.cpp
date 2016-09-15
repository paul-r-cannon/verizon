// Paul R. Cannon
// paul_r_cannon@yahoo.com
// 240-344-6081

#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: disparity_map <left_image> <right_image>" << endl;
        return 1;
    }

    // load in the images
    Mat imgLeft = imread(argv[1], IMREAD_GRAYSCALE);
    Mat imgRight = imread(argv[2], IMREAD_GRAYSCALE);
    if (imgLeft.empty())
    {
        cout <<  "error: no image data for image \"" << argv[1] << "\"; exiting..." << endl;
        return 1;
    }
    if (imgRight.empty())
    {
        cout <<  "error: no image data for image \"" << argv[2] << "\"; exiting..." << endl;
        return 1;
    }

    // allocate image to save disparities
    Mat imgDisparity16S = Mat(imgLeft.rows, imgLeft.cols, CV_16S);

    // allocate image to save output disparity image
    Mat imgDisparity8U = Mat(imgLeft.rows, imgLeft.cols, CV_8UC1);

    // determine the disparity image
    // note: the number of disparities must be positive and divisible by 16
    int ndisparities = 16*8;  // = 128
    int SADWindowSize = 21;   // default size of 21 yields best trade-off for this method
    Ptr<StereoBM> sbm = StereoBM::create(ndisparities, SADWindowSize);
    sbm->compute( imgLeft, imgRight, imgDisparity16S);

    // determine image extreme values
    double minVal, maxVal;
    minMaxLoc(imgDisparity16S, &minVal, &maxVal);
    cout << "minVal = " << minVal << "; maxVal = " << maxVal << endl;

    // create the output disparity image
    imgDisparity16S.convertTo(imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));

    // display the output disparity image
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", imgDisparity8U);
    waitKey(0);

    // save the output disparity image
    imwrite("disparity_image.png", imgDisparity8U);
    cout << "disparity_image.png created..." << endl;
    waitKey(0);

    return 0;
}
