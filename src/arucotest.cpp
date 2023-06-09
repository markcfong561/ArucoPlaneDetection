#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <stdio.h>

#include "PlaneDetection.h"

int main(int argc, char *argv[])
{
    cv::VideoCapture cap;
    cv::Mat image;

    const std::string camIp = "https://172.16.142.74:8080/video"; // Using my phone camera

    if (!cap.open(camIp))
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    double camMat[3][3] = {4.4, 0, 960, 0, 4.4, 540, 0, 0, 1};
    double distCoeffs[4] = {1, 1, 1, 1};

    cv::Mat cvCamMat(3, 3, 6, camMat);
    cv::Mat cvDistCoeffs(1, 4, 6, distCoeffs);

    std::vector<std::vector<cv::Point3f>> objPoints;
    double tagSize = .065;

    cv::Point3f tag0Pos(0, 0, 0);
    cv::Point3f tag1Pos(0.21, 0, 0);
    cv::Point3f tag2Pos(0.42, 0.2, 0);
    cv::Point3f tag3Pos(0.21, 0.11, 0);

    addTagPos(tag0Pos, tagSize, objPoints);
    addTagPos(tag1Pos, tagSize, objPoints);
    addTagPos(tag2Pos, tagSize, objPoints);
    addTagPos(tag3Pos, tagSize, objPoints);

    cv::Ptr<cv::aruco::Dictionary> boardDict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    std::vector<int> boardIds({0, 1, 2, 3});

    cv::Ptr<cv::aruco::Board> board = cv::aruco::Board::create(objPoints, boardDict, boardIds);

    while (true)
    {

        cap.read(image);
        // check if we succeeded
        if (image.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        cv::aruco::detectMarkers(image, board->dictionary, markerCorners, ids);
        if (ids.size() > 0)
        {
            cv::aruco::drawDetectedMarkers(image, markerCorners, ids);

            cv::Vec3d rvec, tvec;
            int valid = cv::aruco::estimatePoseBoard(markerCorners, ids, board, cvCamMat, cvDistCoeffs, rvec, tvec);
            if (valid > 0)
            {
                cv::aruco::drawAxis(image, cvCamMat, cvDistCoeffs, rvec, tvec, 0.1);
            }
        }
        cv::imshow("Output Window", image);

        if (cv::waitKey(1) == 27)
            break;
    }
}
