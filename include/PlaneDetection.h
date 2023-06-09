#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>

#include <stdio.h>

/**  @brief Adds tag position to object points on an aruco board
 * @param position position of tag to be added
 * @param tagSize size of the tag being added
 * @param objPoints objPoints vector where tag is to be added to
 */
void addTagPos(cv::Point3f position, double tagSize, std::vector<std::vector<cv::Point3f>>& objPoints);

