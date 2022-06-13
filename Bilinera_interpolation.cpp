#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std::chrono;
using namespace cv;
using namespace std;

// x_scale ,y_scale are  scaling factor
void Inter_Linear(Mat& original, Mat& interpolated, double x_scale, double y_scale)
{
    // new dimensions for interpolated image
    int interpolated_rows = round(x_scale * original.rows);
    int interpolated_cols = round(y_scale * original.cols);
    //Creating empty Mat of new scale size
    interpolated = Mat(interpolated_rows, interpolated_cols, original.type());

    for (int i = 0; i < interpolated.rows; i++)
    {
        //geometric center alignment
        double index_i = (i + 0.5) / x_scale + 0.5;

        //prevent cross-border
        if (index_i < 0)
            index_i = 0;
        if (index_i > original.rows - 1)
            index_i = original.rows - 1;

        //adjacent 4*4 pixel rows (coordinates)
        int i1 = floor(index_i);
        int i2 = ceil(index_i);

        //u is the decimal part of the floating-point coordinate line
        double u = index_i - i1;
        for (int j = 0; j < interpolated.cols; j++)
        {
            //Geometric center alignment
            double index_j = (j + 0.5) / y_scale + 0.5;

            //Prevent cross-border
            if (index_j < 0)
                index_j = 0;
            if (index_j > original.cols - 1)
                index_j = original.cols - 1;

            //Columns of adjacent 4*4 pixels (coordinates)
            int j1 = floor(index_j);
            int j2 = ceil(index_j);

            //v is the decimal part of the floating-point coordinate column
            double v = index_j - j1;

            if (original.channels() == 1)
            {
                //Grayscale image
                interpolated.at<uchar>(i, j) = (1 - u) * (1 - v) * original.at<uchar>(i1, j1) + (1 - u) * v * original.at<uchar>(i1, j2) + u * (1 - v) * original.at<uchar>(i2, j1) + u * v * original.at<uchar>(i2, j2);
            }
            else
            {
                //Color image
                interpolated.at<Vec3b>(i, j)[0] = (1 - u) * (1 - v) * original.at<Vec3b>(i1, j1)[0] + (1 - u) * v * original.at<Vec3b>(i1, j2)[0] + u * (1 - v) * original.at<Vec3b>(i2, j1)[0] + u * v * original.at<Vec3b>(i2, j2)[0];
                interpolated.at<Vec3b>(i, j)[1] = (1 - u) * (1 - v) * original.at<Vec3b>(i1, j1)[1] + (1 - u) * v * original.at<Vec3b>(i1, j2)[1] + u * (1 - v) * original.at<Vec3b>(i2, j1)[1] + u * v * original.at<Vec3b>(i2, j2)[1];
                interpolated.at<Vec3b>(i, j)[2] = (1 - u) * (1 - v) * original.at<Vec3b>(i1, j1)[2] + (1 - u) * v * original.at<Vec3b>(i1, j2)[2] + u * (1 - v) * original.at<Vec3b>(i2, j1)[2] + u * v * original.at<Vec3b>(i2, j2)[2];
            }
        }
    }
}



int main()
{
    string window_name = "";
    double scale_of_x = 1, scale_of_y = 1; // By default original image 
    string image_name = "";
    cout << " Enter name ( with extension ) of image for resizing : ";
    cin >> image_name;

    // Taking input for scaling of image
    cout << " Enter Scale for X-axis : ";
    cin >> scale_of_x;
    cout << " Enter Scale for Y-axis : ";
    cin >> scale_of_y;

    Mat original_image = imread(image_name);
    Mat interpolated_image;

    // Handling zero size mat case
    scale_of_x = scale_of_x == 0 ? 1 : scale_of_x;
    scale_of_y = scale_of_y == 0 ? 1 : scale_of_y;

    auto start = high_resolution_clock::now();

    //Calling Bilinear Interpolation funtion
    Inter_Linear(original_image, interpolated_image, scale_of_x, scale_of_y);

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to 
    // get durarion. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    //Changing original image to interpolated image 
    imwrite(image_name, interpolated_image);

    //Showing output of converted image 
    window_name += "Interpolated Image of scale " + to_string(round(scale_of_x)) + "*" + to_string(round(scale_of_y)) + "  Time Taken : " + to_string(duration.count()) + " microseconds ";
    imshow(window_name, interpolated_image);

    // Wait for any keystroke
    waitKey(0);

    return 0;
}