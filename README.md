# Aruco-based Plane Detection

# Introduction
Getting a plane's location relative to a camera is vital in laser calibration of the LS. Aruco markers are used in many computer vision applications where a plane's position is required. Aruco markers are binary square fiducial markers that are used for camera pose estimation.

## Purpose
This project serves as a tool to detect a plane relative to a calibrated camera using aruco markers.

## Usage
This tool utilizes OpenCV's board detection is neatly packaged for ease of use.

<u> 

### Define Board 

</u>
Board detection requires 3 things: marker locations, dictionary used, and marker ids.

<br />

To define marker locations, first define the size of the tags you're using in meters. Then create a ```std::vector<std::vector<cv::Point3f>>``` and insert ```cv::Point3f``` into it for every tag's position

Example:
```
double tagSize = 0.05;
std::vector<std::vector<cv::Point3f>> objPoints;

cv::Point3f tag0Pos(   0,    0, 0 );
cv::Point3f tag1Pos( 0.2,    0, 0 );
cv::Point3f tag2Pos( 0.2,  0.2, 0 );
cv::Point3f tag3Pos(   0,  0.2, 0 );

addTagPos(tag0Pos, tagSize, objPoints);
addTagPos(tag1Pos, tagSize, objPoints);
addTagPos(tag2Pos, tagSize, objPoints);
addTagPos(tag3Pos, tagSize, objPoints);
```

To define the dictionary used, call ```cv::aruco::getPredefinedDictionary()```

Example (ID 0-49 4x4 Aruco markers):
```
cv::Ptr<cv::aruco::Dictionary> boardDict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
```

Finally, to define marker id's used on the board, create a ```std::vector<int>``` and add them **IN THE ORDER YOU ADDED TO THE LOCATIONS**

Example (ID's 0-3):
```
std::vector<int> boardIds({0, 1, 2, 3});
```

Define your parameters into a ```cv::aruco::Board```
```
cv::Ptr<cv::aruco::Board> board = cv::aruco::Board::create(objPoints, boardDict, boardIds);
```

<u>

### Define camera intrinsics
</u>

Board pose estimation requires the camera intrinsics so these need to be defined. This is found after camera calibration
Example: 
```
double camMat[3][3] = {fx, 0, cx, 0, fy, cy, 0, 0, 1};
double distCoeffs[4] = {1, 1, 1, 1};

cv::Mat cvCamMat(3, 3, 6, camMat);
cv::Mat cvDistCoeffs(1, 4, 6, distCoeffs);
```
<u>

### Detect Markers and Board
</u>

The next step is the actual detection. Once you've defined your board, grab frames and pass it through the marker detection
```
std::vector<int> ids;
std::vector<std::vector<cv::Point2f>> markerCorners;
cv::aruco::detectMarkers(image, board->dictionary, markerCorners, ids);
```

Once the markers are detection, find their pose.
```
if (ids.size() > 0) {
    cv::Vec3d rvec, tvec;
    int valid = cv::aruco::estimatePoseBoard(markerCorners, ids, board, cvCamMat, cvDistCoeffs, rvec, tvec);
}
```
The size of the ids vector determines if any markers were actually detected and ```cv::aruco::estimatePoseBoard()``` returns > 0 if a board is valid/detected. If valid, rvec holds a rotation vector of the plane relative to the camera and tvec holds the translation vector of the plane relative to the camera in m.

# How to use demo
Build the project:
```bash
mkdir build && cd build
cmake ..
make
```
Executable is saved in the root directory of this project

# Dependencies
OpenCV 3.2.0

# References
https://docs.opencv.org/3.2.0/db/da9/tutorial_aruco_board_detection.html