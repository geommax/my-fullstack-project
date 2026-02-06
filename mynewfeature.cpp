#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// Configuration structure
struct Config {
    double base;
    int exponent;
};


Class MyNewFeature {
public:
    MyNewFeature(const Config& config) : config(config) {}
    double calculate() {
        return pow(config.base, config.exponent);
    }

};
