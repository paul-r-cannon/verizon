// Paul R. Cannon
// paul_r_cannon@yahoo.com
// 240-344-6081

#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: stereo_rectify_images <camera_data_file> <left_image> <right_image>" << endl;
        return 1;
    }

    // read in camera and distortion matrices from the camera data file
    Mat camera_matrix(3, 3, CV_64F), distortion_coefficients(3, 3, CV_64F);
    FileStorage fs(argv[1], FileStorage::READ);
    if (!fs.isOpened())
    {
        cout << "error: could not open the camera data file: \"" << argv[1] << "\"; exiting..." << endl;
        return 1;
    }
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> distortion_coefficients;
    fs.release();

    cout << "\ncamera_matrix:\n" << camera_matrix << endl;
    cout << "\ndistortion_coefficients:\n" << distortion_coefficients << endl;

    // note: when taking images of my scene, I first took my left image and then slide the camera to the right by 10.0mm.  I assume that R and t in stereoRectify are the rotation matrix and translation vector, respectively, that convert a coordinate in the first (left) position camera coordinates to a coordinate in the second (right) position camera coordinates.  In other words, I assume that R and t represent this relationship: x2 = Rx1 + t.

    // estimated rotation matrix
    // note: this should be the Identity matrix since I made an effort not to rotate the second camera position w.r.t the first; that is, each of the coordinate axes of the second camera position are (roughly) parallel to those of the first camera position.
    double R_data[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    Mat R(3, 3, CV_64F, R_data);

    cout << "\nR:\n" << R << endl;

    // estimated tranlation vector
    // note: assuming that x2 = Rx1 + t is the correct relationship for R and t, the origin of the first camera position w.r.t. the second should be along the second camera's x-axis in the negative direction.
    double T_data[] = {-10.0, 0.0, 0.0};
    Mat T(3, 1, CV_64F, T_data);

    cout << "\nT:\n" << T << endl;

    // load in left and right images
    Mat left_image_orig = imread(argv[2], IMREAD_COLOR);
    Mat right_image_orig = imread(argv[3], IMREAD_COLOR);
    if (left_image_orig.empty())
    {
        cout <<  "error: no image data for left image \"" << argv[2] << "\"; exiting..." << endl;
        return 1;
    }
    if (right_image_orig.empty())
    {
        cout <<  "error: no image data for right image \"" << argv[3] << "\"; exiting..." << endl;
        return 1;
    }
    if (left_image_orig.size() != right_image_orig.size())
    {
        cout <<  "error: left image must have the same size as right image; exiting..." << endl;
        return 1;
    }
    Size image_size = left_image_orig.size();

    cout << "\nsize of images: " << image_size << endl;

    imshow("Original Left Image", left_image_orig);
    imshow("Original Right Image", right_image_orig);
    waitKey(0);

    // determine stereo rectification transforms from camera data and R and T estimates
    Mat R1(3, 3, CV_64F);
    Mat R2(3, 3, CV_64F);
    Mat P1(3, 4, CV_64F);
    Mat P2(3, 4, CV_64F);
    Mat Q(4, 4, CV_64F);
    Rect leftValidROI, rightValidROI;
    stereoRectify(camera_matrix, distortion_coefficients, camera_matrix, distortion_coefficients, image_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 0, image_size, &leftValidROI, &rightValidROI);

    // determine rectification maps from rectification transforms
    Mat left_x_map, left_y_map;
    Mat right_x_map, right_y_map;
    initUndistortRectifyMap(camera_matrix, distortion_coefficients, R1, P1, image_size, CV_32FC1, left_x_map, left_y_map);
    initUndistortRectifyMap(camera_matrix, distortion_coefficients, R2, P2, image_size, CV_32FC1, right_x_map, right_y_map);

    // create rectified images
    Mat left_image_rectified, right_image_rectified;
    remap(left_image_orig, left_image_rectified, left_x_map, left_y_map, INTER_LINEAR);
    remap(right_image_orig, right_image_rectified, right_x_map, right_y_map, INTER_LINEAR);

    imshow("Rectified Left Image", left_image_rectified);
    imshow("Rectified Right Image", right_image_rectified);
    waitKey(0);

    imwrite("left_image_rectified.png", left_image_rectified);
    imwrite("right_image_rectified.png", right_image_rectified);

    return 0;
}


