// Paul R. Cannon
// paul_r_cannon@yahoo.com
// 240-344-6081

#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: generate_point_cloud <disparity_image> <texture_image>" << endl;
        return 1;
    }

    // load the disparity image
    Mat disparityImage = imread(argv[1], IMREAD_GRAYSCALE);
    if (disparityImage.empty())
    {
        cout <<  "error: no image data for image \"" << argv[1] << "\"; exiting..." << endl;
        return 1;
    }
    cout << "disparityImage.rows = " << disparityImage.rows << endl;
    cout << "disparityImage.cols = " << disparityImage.cols << endl;
    cout << "disparityImage.channels() = " << disparityImage.channels() << endl;
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", disparityImage);
    waitKey(0);

    // load the texture image
    Mat textureImage = imread(argv[2], IMREAD_COLOR);
    if (textureImage.empty())
    {
        cout <<  "error: no image data for image \"" << argv[2] << "\"; exiting..." << endl;
        return 1;
    }
    cout << "textureImage.rows = " << textureImage.rows << endl;
    cout << "textureImage.cols = " << textureImage.cols << endl;
    cout << "textureImage.channels() = " << textureImage.channels() << endl;
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", textureImage);
    waitKey(0);

    // ensure that the disparity image and the texture image are the same size
    if (disparityImage.size() != textureImage.size())
    {
        cout << "error: disparity and texture images must be the same size; exiting..." << endl;
    }

    // create a simple Q matrix
    Mat Q(4, 4, CV_64F);
    Q.at<double>(0,0) = 1.0;
    Q.at<double>(0,1) = 0.0;
    Q.at<double>(0,2) = 0.0;
    Q.at<double>(0,3) = -450.0; // -cx
    Q.at<double>(1,0) = 0.0;
    Q.at<double>(1,1) = 1.0;
    Q.at<double>(1,2) = 0.0;
    Q.at<double>(1,3) = -375.0; // -cy
    Q.at<double>(2,0) = 0.0;
    Q.at<double>(2,1) = 0.0;
    Q.at<double>(2,2) = 0.0;
    Q.at<double>(2,3) = 800.0; // focal length (in pixels)
    Q.at<double>(3,0) = 0.0;
    Q.at<double>(3,1) = 0.0;
    Q.at<double>(3,2) = -1.0/100.0; // -1.0/baseline (baseline in world units: m or mm)
    Q.at<double>(3,3) = 0.0; // (cx - cx')/baseline -- assume (cx - cx') is 0

    // note: Q is the disparity to depth conversion matrix
    // Z = f*B/d
    // where:
    // Z = distance along the camera Z axis
    // f = focal length (in pixels)
    // B = baseline (in metres)
    // d = disparity (in pixels)

    // determine 3D coordinates
    Mat XYZ(disparityImage.size(), CV_32FC3);
    reprojectImageTo3D(disparityImage, XYZ, Q, false, CV_32F);

    // output point cloud points to pts file format

    ofstream fout("point_cloud.pts");
    fout.setf(ios_base::fixed);
    fout.precision(6);
    fout.setf(ios_base::showpoint);

    // for convenience
    float *XYZ_p = reinterpret_cast<float *>(XYZ.data);
    uchar *textureImage_p = textureImage.data;
    int nrows = disparityImage.rows;
    int ncols = disparityImage.cols;
    cout << "nrows = " << nrows << endl;
    cout << "ncols = " << ncols << endl;

    // iterate over the rows and columns
    for (int row_i = 0; row_i < nrows; row_i++)
    {
        for (int col_i = 0; col_i < ncols; col_i++)
        {
            // output xyz coordinates
            if (isfinite(XYZ_p[row_i*ncols*3 + col_i*3 + 0]) && isfinite(XYZ_p[row_i*ncols*3 + col_i*3 + 1]) && isfinite(XYZ_p[row_i*ncols*3 + col_i*3 + 2]))
            {
                fout.width(11);
                fout << XYZ_p[row_i*ncols*3 + col_i*3 + 0] << ' ';
                fout.width(11);
                fout << XYZ_p[row_i*ncols*3 + col_i*3 + 1] << ' ';
                fout.width(11);
                fout << XYZ_p[row_i*ncols*3 + col_i*3 + 2] << ' ';

                // output color in RGB order; note: opencv uses BGR order by default
                fout.width(3);
                fout << static_cast<int>(textureImage_p[row_i*ncols*3 + col_i*3 + 2]) << ' '; // R
                fout.width(3);
                fout << static_cast<int>(textureImage_p[row_i*ncols*3 + col_i*3 + 1]) << ' '; // G
                fout.width(3);
                fout << static_cast<int>(textureImage_p[row_i*ncols*3 + col_i*3 + 0]) << ' '; // B

                // end the line
                fout << endl;
            }
        }
    }
    fout << endl;
    fout.close();

    return 0;
}

