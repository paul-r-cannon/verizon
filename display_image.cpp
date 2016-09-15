// Paul R. Cannon
// paul_r_cannon@yahoo.com
// 240-344-6081

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << " Usage: display_image <image_to_display>" << endl;
        return 1;
    }

    // load in the image
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    // ensure that we have image data
    if (image.empty())
    {
        cout <<  "error: no image data for image \"" << argv[1] << "\"; exiting..." << endl;
        return 1;
    }

    // determine image extreme values
    double minVal, maxVal;
    minMaxLoc(image, &minVal, &maxVal);
    cout << "minVal = " << minVal << "; maxVal = " << maxVal << endl;

    // display the image
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image);
    waitKey(0);

    return 0;
}
