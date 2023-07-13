#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/calib3d.hpp>

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

    double camMat[3][3] = {3573.681082, 0, 733.98215, 0, 3572.916732, 576.44937, 0, 0, 1};
    double distCoeffs[5] = {-0.05969, -0.981368, -0.000368, -0.000843, 19.9579};

    cv::Mat cvCamMat(3, 3, 6, camMat);
    cv::Mat cvDistCoeffs(1, 5, 6, distCoeffs);

    std::vector<std::vector<cv::Point3f>> objPoints;
    double tagSize = 0.02;

    cv::Point3f tag0Pos(0, 0, 0);
    cv::Point3f tag1Pos(0.06, 0, 0);
    cv::Point3f tag2Pos(0, -0.06, 0);
    cv::Point3f tag3Pos(0.06, -0.06, 0);

    addTagPos(tag0Pos, tagSize, objPoints);
    addTagPos(tag1Pos, tagSize, objPoints);
    addTagPos(tag2Pos, tagSize, objPoints);
    addTagPos(tag3Pos, tagSize, objPoints);

    cv::Ptr<cv::aruco::Dictionary> boardDict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);

    std::vector<int> boardIds({0, 2, 1, 3});

    // cv::Ptr<cv::aruco::Board> board = cv::aruco::Board::create(objPoints, boardDict, boardIds);
    // cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 7, .030, .006, boardDict);

    // for (int i = 0; i < 4; i++) {
    //     std::cout << boardIds.at(i) << '\n';
    //     for (auto point : board->objPoints.at(i)) {
    //         std::cout << point << '\n';
    //     }
    //     std::cout << "\n\n";
    // }

    Pylon::CGrabResultPtr grab_result;
    Pylon::CPylonImage pylon_image;
    Pylon::CImageFormatConverter converter;
    converter.OutputPixelFormat = Pylon::PixelType_BGR8packed;

    camera.StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);

    std::vector<cv::Point3f> flattenedObjPoints;

    flattenVector(objPoints, flattenedObjPoints);

    // std::cout << flattenedObjPoints;

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
        std::vector<cv::Point2f> flattenedMarkerCorners;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        // std::cout << "HERE" << std::endl;
        cv::aruco::detectMarkers(image, boardDict, markerCorners, ids);
        flattenVector(markerCorners, flattenedMarkerCorners);
        if (ids.size() > 0)
        {
            cv::aruco::drawDetectedMarkers(image, markerCorners, ids);

            cv::Vec3d rvec, tvec;
            if (flattenedMarkerCorners.size() == flattenedObjPoints.size())
            {
                std::vector<cv::Point3f> fixedObjPoints;
                for (auto id : ids)
                {
                    if (id < objPoints.size())
                        fixedObjPoints.insert(end(fixedObjPoints), begin(objPoints.at(id)), end(objPoints.at(id)));
                }
                if (fixedObjPoints.size() == flattenedMarkerCorners.size())
                {
                    cv::solvePnP(fixedObjPoints, flattenedMarkerCorners, cvCamMat, cvDistCoeffs, rvec, tvec);
                    cv::aruco::drawAxis(image, cvCamMat, cvDistCoeffs, rvec, tvec, 0.1);
                }
            }
            // int valid = cv::aruco::estimatePoseBoard(markerCorners, ids, board, cvCamMat, cvDistCoeffs, rvec, tvec);
            // if (valid != 0)
            // {
            // if (!isnanf(tvec[0]))
            // std::cout << tvec << std::endl;
            // }
        }
        cv::imshow("Output Window", image);

        if (cv::waitKey(10) == 27)
            break;
    }
    // Pylon::PylonTerminate();
}
