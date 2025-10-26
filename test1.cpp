#include <opencv4/opencv2/opencv.hpp> 
using namespace cv; 
using namespace std;

int main() {
VideoCapture cap("/home/zhangshu/test/sp.mp4"); 
int fps = cap.get(CAP_PROP_FPS);
if(fps<=0) fps = 30;
Size size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));
VideoWriter writer("result.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size);

Mat frame, hsv, mask_red, mask_green;
while (cap.read(frame)) {
    if (frame.empty()) break;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask_red);
    inRange(hsv, Scalar(170, 120, 70), Scalar(180, 255, 255), mask_red);
    bitwise_or(mask_red, mask_red, mask_red); 
    inRange(hsv, Scalar(40, 120, 70), Scalar(70, 255, 255), mask_green);
    vector<vector<Point>> contours_red, contours_green;
    findContours(mask_red, contours_red, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(mask_green, contours_green, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    string light_color = "Unknown";
    if (!contours_red.empty()) {
        light_color = "Red";
        for (auto& cnt : contours_red) {
            Rect rect = boundingRect(cnt);
            rectangle(frame, rect, Scalar(0, 0, 255), 2); 
        }
    } else if (!contours_green.empty()) {
        light_color = "Green";
        for (auto& cnt : contours_green) {
            Rect rect = boundingRect(cnt);
            rectangle(frame, rect, Scalar(0, 255, 0), 2); 
        }
    }


    putText(frame, "Light Color: " + light_color, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2);


    writer.write(frame);


    imshow("Traffic Light Detection", frame);
    if (waitKey(30) == 27) break;
}

cap.release();
writer.release();
destroyAllWindows();
return 0;

}  
