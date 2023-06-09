#include "PlaneDetection.h"

void addTagPos(cv::Point3f position, double tagSize, std::vector<std::vector<cv::Point3f>>& objPoints) {
    std::vector<cv::Point3f> tagCorners;
    tagCorners.push_back(position);
    tagCorners.push_back(cv::Point3f(position.x, position.y + tagSize, position.z));
    tagCorners.push_back(cv::Point3f(position.x + tagSize, position.y + tagSize, position.z));
    tagCorners.push_back(cv::Point3f(position.x + tagSize, position.y, position.z));
    objPoints.push_back(tagCorners);
}