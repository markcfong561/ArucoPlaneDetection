#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <stdio.h>

#include <pylon/InstantCamera.h>
#include <pylon/PylonImage.h>
#include <pylon/ImageFormatConverter.h>
#include <pylon/PylonIncludes.h>

#include "PlaneDetection.h"

int main(int argc, char *argv[])
{
    Pylon::PylonInitialize();
    Pylon::CTlFactory &tlFactory = Pylon::CTlFactory::GetInstance();
    Pylon::CInstantCamera camera(tlFactory.CreateFirstDevice());
    // cv::VideoCapture cap;
    cv::Mat image;

    // const std::string camIp = "https://172.16.142.74:8080/video"; // Using my phone camera

    // if (!cap.open(camIp))
    // {
    //     std::cout << "Error opening video stream or file" << std::endl;
    //     return -1;
    // }

    double camMat[3][3] = {2568.3693, 0, 1315.4255, 0, 2570.9603, 1078.3560, 0, 0, 1};
    double distCoeffs[4] = {0, 0, 0, 0};

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

    cv::Ptr<cv::aruco::Dictionary> boardDict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_1000);

    std::vector<int> boardIds({0, 1, 2, 3});

    // cv::Ptr<cv::aruco::Board> board = cv::aruco::Board::create(objPoints, boardDict, boardIds);
    cv::Ptr<cv::aruco::Board> board = cv::aruco::GridBoard::create(5, 7, .030, .006, boardDict);

    Pylon::CGrabResultPtr grab_result;
    Pylon::CPylonImage pylon_image;
    Pylon::CImageFormatConverter converter;
    converter.OutputPixelFormat = Pylon::PixelType_BGR8packed;

    camera.StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);

    while (true)
    {
        camera.RetrieveResult(1000, grab_result);
        converter.Convert(pylon_image, grab_result);
        image = cv::Mat(grab_result->GetHeight(), grab_result->GetWidth(), CV_8UC3, (uint8_t *)pylon_image.GetBuffer());

        // cap.read(image);
        // // check if we succeeded
        // if (image.empty())
        // {
        //     std::cerr << "ERROR! blank frame grabbed\n";
        //     break;
        // }
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        // std::cout << "HERE" << std::endl;
        cv::aruco::detectMarkers(image, board->dictionary, markerCorners, ids);
        if (ids.size() > 0)
        {
            cv::aruco::drawDetectedMarkers(image, markerCorners, ids);

            cv::Vec3d rvec, tvec;
            int valid = cv::aruco::estimatePoseBoard(markerCorners, ids, board, cvCamMat, cvDistCoeffs, rvec, tvec);
            if (valid > 0)
            {
                cv::aruco::drawAxis(image, cvCamMat, cvDistCoeffs, rvec, tvec, 0.1);
                if (!isnanf(tvec[0]))
                    std::cout << tvec << std::endl;
            }
        }
        cv::imshow("Output Window", image);

        if (cv::waitKey(1) == 27)
            break;
    }
}
