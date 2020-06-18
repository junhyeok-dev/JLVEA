#include <iostream>
#include <random>
#include <chrono>
#include "opencv4/opencv2/opencv.hpp"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "usage: JLVEA-cpp [encrypt|decrypt] FILENAME SEED" << std::endl;
        return 3;
    }

    //Read parameters
    int mode = 0;
    if (std::strcmp(argv[1], "encrypt") != 0) {
        mode = 0;
    } else if (std::strcmp(argv[1], "decrypt") != 0) {
        mode = 1;
    } else {
        std::cerr << "error: not proper encryption mode: " << argv[1] << std::endl;
        return 4;
    }
    std::string filename = argv[2];
    std::ifstream read;
    read.open(filename);
    if (!read) {
        std::cerr << "error: no such file: " << argv[2] << std::endl;
        return 5;
    }

    int seed = 0;
    try {
        seed = std::stoi(argv[3]);
    } catch (std::invalid_argument& e) {
        std::cerr << "error: please input proper seed number." << std::endl;
        return 6;
    }

    //Read video frame information
    cv::VideoCapture vid(filename);
    int width = (int)vid.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int)vid.get(cv::CAP_PROP_FRAME_HEIGHT);
    int pcount = width;

    cv::VideoWriter out;
    out.open("enc.mp4", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30.0, cv::Size(width, height));

    std::random_device random_device;
    std::mt19937_64 rnd(seed);
    std::uniform_int_distribution<int> wrange(0, width - 1);
    std::uniform_int_distribution<int> hrange(0, height - 1);

    std::srand(seed);

    int plist[3][pcount][4];

    //Generate Permutation List
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < pcount; j++) {
            for (int k = 0; k < 4; k++) {
                if (k < 2) {
                    plist[i][j][k] = wrange(rnd);
                } else {
                    plist[i][j][k] = hrange(rnd);
                }
            }
        }
    }

    double total_time = 0;
    int frame_no = 0;

    if (mode == 0) {
        //Permutate Image
        while(true) {
            std::chrono::system_clock::time_point start_enc = std::chrono::system_clock::now();

            cv::Mat frame = cv::Mat();
            vid >> frame;

            if (frame.empty()) {
                break;
            }

            std::cout << "Encrypting frame no." << std::to_string(frame_no) << std::endl;
            frame_no += 1;

            //Channel Split
            cv::Mat split[3];
            cv::split(frame, split);

            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < pcount; k++) {
                    int c1 = plist[j][k][0];
                    int c2 = plist[j][k][1];
                    int r1 = plist[j][k][2];
                    int r2 = plist[j][k][3];

                    cv::Mat col1 = split[j].col(c1);
                    cv::Mat col2 = split[j].col(c2);
                    cv::Mat row1 = split[j].row(r1);
                    cv::Mat row2 = split[j].row(r2);

                    col1.copyTo(split[j].col(c2));
                    col2.copyTo(split[j].col(c1));
                    row1.copyTo(split[j].row(r2));
                    row2.copyTo(split[j].row(r1));

                    for (int l = 0; l < 4; l++) {
                        if (l < 2) {
                            plist[j][k][l] = wrange(rnd);
                        } else {
                            plist[j][k][l] = hrange(rnd);
                        }
                    }
                }
            }

            cv::Mat enc_image = cv::Mat();
            cv::merge(split, 3, enc_image);
            out.write(enc_image);

            std::chrono::duration<double> enc_time = std::chrono::system_clock::now() - start_enc;
            total_time += enc_time.count();

            /*
            cv::namedWindow("Test", cv::WINDOW_AUTOSIZE);
            cv::imshow("Test", enc_image);
            cv::waitKey(1);
             */
        }
    }

    std::cout << "Total encryption time: " << std::to_string(total_time) << std::endl;
    std::cout << "Average encryption time: " << std::to_string(total_time / 100) << std::endl;

    return 0;
}
