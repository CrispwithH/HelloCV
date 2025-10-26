https://www.yuque.com/ctiso/mmvw1o/vonsz12l3rzubkm9
https://www.yuque.com/ctiso/mmvw1o/ig7lxnsd6e87b541
https://www.yuque.com/ctiso/mmvw1o/veor8tfs6buo35g1

第三周：https://www.yuque.com/ctiso/mmvw1o/nhnmfriual5bevet?#《OpenCV的使用》
一、设计思路
 
通过OpenCV读取视频文件，对每一帧图像进行颜色空间转换、阈值分割等操作，识别交通信号灯的颜色（红、绿），然后在图像中标注信号灯位置并在左上角输出颜色字符串，最后将处理后的帧写入新视频文件。
 
二、实现步骤
 
步骤1：环境准备
 
确保已安装C++版本的OpenCV开发环境，可通过以下命令安装（以Ubuntu为例）：
 
bash
  
sudo apt install libopencv-dev build-essential
 
 
步骤2：创建项目文件
 
-  traffic_light_detection.cpp ：
 
cpp
  
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
using namespace std;

int main() {
    // 1. 读取视频
    VideoCapture cap("TrafficLight.mp4"); // 替换为视频实际路径
    if (!cap.isOpened()) {
        cerr << "无法打开视频文件" << endl;
        return -1;
    }

    // 2. 获取视频属性，用于创建输出视频
    int fps = cap.get(CAP_PROP_FPS);
    Size size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));
    VideoWriter writer("result.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size);

    Mat frame, hsv, mask_red, mask_green;
    while (cap.read(frame)) {
        if (frame.empty()) break;

        // 3. 转换颜色空间到HSV（便于颜色识别）
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 4. 分别创建红、绿色的掩码（根据HSV范围调整阈值）
        // 红色HSV范围（注意红色在HSV中是不连续的，需分两段）
        inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask_red);
        inRange(hsv, Scalar(170, 120, 70), Scalar(180, 255, 255), mask_red);
        bitwise_or(mask_red, mask_red, mask_red); // 合并两段红色掩码
        // 绿色HSV范围
        inRange(hsv, Scalar(40, 120, 70), Scalar(70, 255, 255), mask_green);

        // 5. 寻找掩码中的轮廓，识别信号灯位置
        vector<vector<Point>> contours_red, contours_green;
        findContours(mask_red, contours_red, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        findContours(mask_green, contours_green, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        string light_color = "Unknown";
        // 6. 判断信号灯颜色并标注
        if (!contours_red.empty()) {
            light_color = "Red";
            for (auto& cnt : contours_red) {
                Rect rect = boundingRect(cnt);
                rectangle(frame, rect, Scalar(0, 0, 255), 2); // 红色矩形标注
            }
        } else if (!contours_green.empty()) {
            light_color = "Green";
            for (auto& cnt : contours_green) {
                Rect rect = boundingRect(cnt);
                rectangle(frame, rect, Scalar(0, 255, 0), 2); // 绿色矩形标注
            }
        }

        // 7. 在图像左上角输出颜色字符串
        putText(frame, "Light Color: " + light_color, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2);

        // 8. 写入处理后的帧到输出视频
        writer.write(frame);

        // 可选：实时显示处理过程
        imshow("Traffic Light Detection", frame);
        if (waitKey(30) == 27) break; // ESC键退出
    }

    // 9. 释放资源
    cap.release();
    writer.release();
    destroyAllWindows();
    return 0;
}
 
 
-  CMakeLists.txt ：
 
cmake
  
cmake_minimum_required(VERSION 3.10)
project(TrafficLightDetection)
find_package(OpenCV REQUIRED)
add_executable(traffic_light_detection traffic_light_detection.cpp)
target_link_libraries(traffic_light_detection ${OpenCV_LIBS})
 
 
步骤3：构建并运行项目
 
bash
  
mkdir build && cd build
cmake ..
make
./traffic_light_detection
 
 
三、遇到的问题与解决方法
 
问题 解决方法 
视频无法打开 检查 TrafficLight.mp4 的路径是否正确，确保文件存在且格式支持 
信号灯颜色识别不准确 调整HSV阈值范围（可通过OpenCV的HSV调色板工具可视化调整） 
输出视频无内容 检查 VideoWriter 的编码格式（如 fourcc ）是否与本地环境兼容，可尝试更换为 CV_FOURCC('X','V','I','D') 等 
 
四、关键代码分析
 
- 颜色识别部分：通过 inRange 函数根据HSV范围创建掩码，利用红色在HSV中不连续的特点，分两段检测后合并，确保红色信号灯的准确识别。
- 轮廓检测与标注： findContours 函数提取掩码中的轮廓， boundingRect 获取轮廓的外接矩形，再通过 rectangle 在原图中标注，直观展示信号灯位置。
- 视频读写： VideoCapture 读取输入视频， VideoWriter 根据输入视频的属性（帧率、尺寸）创建输出视频，保证处理后视频的格式一致性。
