#include "PlaneDetection.h"

void addTagPos(cv::Point3f position, double tagSize, std::vector<std::vector<cv::Point3f>>& objPoints) {
    std::vector<cv::Point3f> tagCorners;
    tagCorners.emplace_back(position);
    tagCorners.emplace_back(cv::Point3f(position.x + tagSize, position.y, position.z));
    tagCorners.emplace_back(cv::Point3f(position.x + tagSize, position.y - tagSize, position.z));
    tagCorners.emplace_back(cv::Point3f(position.x, position.y - tagSize, position.z));
    objPoints.emplace_back(tagCorners);
}

void flattenVector(std::vector<std::vector<cv::Point3f>> &objPoints, std::vector<cv::Point3f> &newObjPoints) {
    for (std::vector<cv::Point3f> objPoint : objPoints) {
        newObjPoints.insert(end(newObjPoints), begin(objPoint), end(objPoint));
    }
}

void flattenVector(std::vector<std::vector<cv::Point2f>> &objPoints, std::vector<cv::Point2f> &newObjPoints) {
    for (std::vector<cv::Point2f> objPoint : objPoints) {
        newObjPoints.insert(end(newObjPoints), begin(objPoint), end(objPoint));
    }
}