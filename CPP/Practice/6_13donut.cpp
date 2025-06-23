#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>
#include <csignal>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
#else
  #include <sys/ioctl.h>
  #include <unistd.h>
  #include <termios.h>
  #include <fcntl.h>
#endif

// 全局变量，用于信号处理
volatile bool running = true;

// 信号处理函数
void signalHandler(int signum) {
    running = false;
}

// 获取终端尺寸
void getTerminalSize(int& width, int& height) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hCon, &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top;
    } else {
        width = 80; 
        height = 40;
    }
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        width = ws.ws_col;
        height = ws.ws_row - 1;  // 留一行给提示符
    } else {
        width = 80; 
        height = 40;
    }
#endif
    
    // 确保最小尺寸
    if (width < 20) width = 20;
    if (height < 10) height = 10;
}

// 设置终端为非阻塞模式（仅在非Windows系统上使用）
void setNonBlockingInput() {
#ifndef _WIN32
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
#endif
}

// 恢复终端设置
void restoreTerminal() {
#ifndef _WIN32
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    fcntl(STDIN_FILENO, F_SETFL, 0);
#endif
}

int main() {
    // 注册信号处理函数
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    // 设置非阻塞输入
    setNonBlockingInput();
    
    // 获取终端尺寸
    int W, H;
    getTerminalSize(W, H);
    
    // 甜甜圈参数
    const float R1 = 1.0f;      // 内圆半径
    const float R2 = 2.0f;      // 外圆半径
    const float K2 = 5.0f;      // 视距
    
    // 计算合适的缩放比例，确保甜甜圈完整显示
    // 考虑字符宽高比补偿后的实际显示区域
    float scale = std::min(W / 16.0f, H / 8.0f);  // 调整比例因子，考虑X轴2倍拉伸
    float K1 = scale * K2 / 2.0f;  // 投影参数
    
    const float THETA_STEP = 0.05f;  // θ步长 - 减小步长提高质量
    const float PHI_STEP = 0.01f;    // φ步长 - 减小步长提高质量
    
    // 亮度字符，从暗到亮
    const char LUM[] = ".,-~:;=!*#$@";
    const int LUM_SIZE = sizeof(LUM) - 1;
    const float PI = 3.14159265f;

    // 缓冲区
    std::vector<char> buffer(W * H, ' ');
    std::vector<float> zbuf(W * H, 0.0f);

    float A = 0.0f, B = 0.0f;  // 旋转角度
    
    // 禁用stdio同步以提高性能
    std::ios::sync_with_stdio(false);
    
    // 隐藏光标并清屏
    std::cout << "\x1b[?25l\x1b[2J\x1b[H";
    std::cout << "旋转甜甜圈动画 - 按 Ctrl+C 退出\n\n";
    std::cout.flush();
    
    // 等待一下让用户看到提示
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while (running) {
        // 清空缓冲区
        std::fill(buffer.begin(), buffer.end(), ' ');
        std::fill(zbuf.begin(), zbuf.end(), 0.0f);

        // 生成甜甜圈的每一个点
        for (float theta = 0; theta < 2 * PI; theta += THETA_STEP) {
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);
            
            for (float phi = 0; phi < 2 * PI; phi += PHI_STEP) {
                float cosPhi = std::cos(phi);
                float sinPhi = std::sin(phi);

                // 计算圆环上的点
                float circleX = R2 + R1 * cosTheta;
                float circleY = R1 * sinTheta;

                // 应用旋转变换
                float x = circleX * (cosPhi * std::cos(B) + sinPhi * std::sin(A) * std::sin(B))
                        - circleY * std::cos(A) * std::sin(B);
                float y = circleX * (cosPhi * std::sin(B) - sinPhi * std::sin(A) * std::cos(B))
                        + circleY * std::cos(A) * std::cos(B);
                float z = circleX * sinPhi * std::cos(A) + circleY * std::sin(A);

                // 透视投影 - 补偿字符宽高比
                float oneOverZ = 1.0f / (z + K2);
                // 字符高度约是宽度的2倍，所以X轴需要拉伸2倍
                int screenX = static_cast<int>(W * 0.5f + K1 * oneOverZ * x * 2.0f);
                int screenY = static_cast<int>(H * 0.5f - K1 * oneOverZ * y);

                // 计算光照（改进的漫反射）
                float luminance = cosPhi * cosTheta * std::sin(B) 
                                - cosTheta * sinPhi * std::cos(B) 
                                - sinTheta * std::cos(A) 
                                + cosPhi * sinTheta * std::sin(A);
                
                // 增加环境光，确保背面也有一定亮度
                luminance = (luminance + 1.0f) * 0.5f;  // 将范围从[-1,1]映射到[0,1]

                // 检查点是否在屏幕范围内
                if (screenX >= 0 && screenX < W && screenY >= 0 && screenY < H && luminance > 0.1f) {
                    int index = screenX + screenY * W;
                    
                    // Z-buffer测试
                    if (oneOverZ > zbuf[index]) {
                        zbuf[index] = oneOverZ;
                        
                        // 根据亮度选择字符
                        int lumIndex = std::min(static_cast<int>(luminance * (LUM_SIZE - 1)), LUM_SIZE - 1);
                        lumIndex = std::max(lumIndex, 0);
                        buffer[index] = LUM[lumIndex];
                    }
                }
            }
        }

        // 输出到屏幕
        std::cout << "\x1b[H";  // 回到左上角
        for (int row = 0; row < H; ++row) {
            for (int col = 0; col < W; ++col) {
                std::cout << buffer[row * W + col];
            }
            if (row < H - 1) std::cout << '\n';
        }
        std::cout.flush();

        // 更新旋转角度
        A += 0.03f;  // 稍微调慢一点
        B += 0.015f; // 稍微调慢一点
        
        // 控制帧率
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 调整帧率
        
        // 检查终端尺寸是否改变
        int newW, newH;
        getTerminalSize(newW, newH);
        if (newW != W || newH != H) {
            W = newW;
            H = newH;
            buffer.resize(W * H);
            zbuf.resize(W * H);
            // 重新计算合适的缩放比例
            scale = std::min(W / 8.0f, H / 6.0f);
            K1 = scale * K2 / 2.0f;
        }
    }

    // 恢复终端设置
    std::cout << "\x1b[?25h\x1b[2J\x1b[H";  // 显示光标并清屏
    std::cout << "感谢观看！\n";
    restoreTerminal();
    
    return 0;
}