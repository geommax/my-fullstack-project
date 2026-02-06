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
    string logFile;
    bool enableLogging;
};
