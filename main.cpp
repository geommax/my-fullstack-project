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

// Get current timestamp string
string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    stringstream ss;
    ss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << "." << setfill('0') << setw(3) << ms.count();
    return ss.str();
}

// Read configuration from file
bool readConfigFile(const string& filename, Config& config) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open config file: " << filename << endl;
        return false;
    }
    
    string line;
    while (getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Parse key=value pairs
        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (key == "base") {
                config.base = stod(value);
            } else if (key == "exponent" || key == "expo") {
                config.exponent = stoi(value);
            } else if (key == "logfile") {
                config.logFile = value;
            } else if (key == "enable_logging") {
                config.enableLogging = (value == "true" || value == "1");
            }
        }
    }
    
    file.close();
    return true;
}

// Write log entry to file and console
void logOutput(ofstream& logFile, const string& message, bool enableLogging) {
    string timestamp = getCurrentTimestamp();
    string logEntry = "[" + timestamp + "] " + message;
    
    // Always output to console
    cout << logEntry << endl;
    
    // Write to file if logging is enabled
    if (enableLogging && logFile.is_open()) {
        logFile << logEntry << endl;
        logFile.flush();
    }
}

// Perform incremental multiplication with 1-second delays
void performLinearCalculation(double base, int exponent, ofstream& logFile, bool enableLogging) {
    cout << "\n=== LINEAR GROWTH (Incremental Multiplication: B * E) ===" << endl;
    logOutput(logFile, "Starting Linear Growth Calculation", enableLogging);
    logOutput(logFile, "Base = " + to_string(base) + ", Exponent = " + to_string(exponent), enableLogging);
    
    double result = base;
    
    // First step: just the base
    this_thread::sleep_for(chrono::seconds(1));
    logOutput(logFile, "Step 1: " + to_string(base) + " = " + to_string(result), enableLogging);
    
    // Incremental multiplication: base * 2, base * 3, ..., base * exponent
    for (int i = 2; i <= exponent; i++) {
        this_thread::sleep_for(chrono::seconds(1));
        result = base * i;
        
        stringstream ss;
        ss << "Step " << i << ": " << base << " × " << i << " = " << result;
        logOutput(logFile, ss.str(), enableLogging);
    }
    
    logOutput(logFile, "Final Linear Result: " + to_string(result), enableLogging);
    cout << endl;
}

// Perform incremental exponentiation with 1-second delays
void performExponentialCalculation(double base, int exponent, ofstream& logFile, bool enableLogging) {
    cout << "=== EXPONENTIAL GROWTH (Incremental Exponentiation: B^E) ===" << endl;
    logOutput(logFile, "Starting Exponential Growth Calculation", enableLogging);
    logOutput(logFile, "Base = " + to_string(base) + ", Exponent = " + to_string(exponent), enableLogging);
    
    double result = base;
    
    // First step: base^1
    this_thread::sleep_for(chrono::seconds(1));
    logOutput(logFile, "Step 1: " + to_string(base) + "^1 = " + to_string(result), enableLogging);
    
    // Incremental exponentiation: base^2, base^3, ..., base^exponent
    for (int i = 2; i <= exponent; i++) {
        this_thread::sleep_for(chrono::seconds(1));
        result = pow(base, i);
        
        stringstream ss;
        ss << "Step " << i << ": " << base << "^" << i << " = ";
        
        // Format large numbers in scientific notation
        if (abs(result) > 1e6 || (abs(result) < 0.000001 && result != 0)) {
            ss << scientific << setprecision(6) << result;
        } else {
            ss << fixed << setprecision(6) << result;
        }
        
        logOutput(logFile, ss.str(), enableLogging);
    }
    
    stringstream finalResult;
    finalResult << "Final Exponential Result: ";
    if (abs(result) > 1e6 || (abs(result) < 0.000001 && result != 0)) {
        finalResult << scientific << setprecision(6) << result;
    } else {
        finalResult << fixed << setprecision(6) << result;
    }
    logOutput(logFile, finalResult.str(), enableLogging);
    cout << endl;
}

// Display help message
void printHelp() {
    cout << "Growth Pattern Calculator - Standalone C++ Application\n" << endl;
    cout << "Usage:" << endl;
    cout << "  1. With command-line arguments:" << endl;
    cout << "     ./growth_calc <base> <exponent>" << endl;
    cout << "     Example: ./growth_calc 2 5\n" << endl;
    cout << "  2. With config file:" << endl;
    cout << "     ./growth_calc --config <config_file>" << endl;
    cout << "     Example: ./growth_calc --config config.txt\n" << endl;
    cout << "  3. Default config file:" << endl;
    cout << "     ./growth_calc" << endl;
    cout << "     (Uses config.txt in current directory)\n" << endl;
    cout << "Options:" << endl;
    cout << "  --help, -h     Show this help message" << endl;
    cout << "  --config FILE  Specify config file path\n" << endl;
    cout << "Output:" << endl;
    cout << "  - Results are displayed on terminal with 1-second delay between steps" << endl;
    cout << "  - Logs are written to file specified in config (if enabled)" << endl;
    cout << "  - Linear Growth: B × 1, B × 2, B × 3, ..., B × E" << endl;
    cout << "  - Exponential Growth: B^1, B^2, B^3, ..., B^E" << endl;
}

int main(int argc, char* argv[]) {
    Config config;
    config.base = 2.0;
    config.exponent = 5;
    config.logFile = "logs/growth_calc.log";
    config.enableLogging = true;
    
    bool useConfigFile = false;
    string configFilePath = "config.txt";
    
    // Parse command-line arguments
    if (argc > 1) {
        string arg1 = argv[1];
        
        if (arg1 == "--help" || arg1 == "-h") {
            printHelp();
            return 0;
        } else if (arg1 == "--config") {
            if (argc < 3) {
                cerr << "Error: --config requires a file path" << endl;
                printHelp();
                return 1;
            }
            configFilePath = argv[2];
            useConfigFile = true;
        } else if (argc >= 3) {
            // Direct command-line arguments: base and exponent
            try {
                config.base = stod(argv[1]);
                config.exponent = stoi(argv[2]);
                
                // Optional log file path
                if (argc >= 4) {
                    config.logFile = argv[3];
                }
                
                cout << "Using command-line parameters:" << endl;
                cout << "  Base = " << config.base << endl;
                cout << "  Exponent = " << config.exponent << endl;
                cout << "  Log file = " << config.logFile << endl;
            } catch (const exception& e) {
                cerr << "Error: Invalid arguments. Expected: <base> <exponent> [logfile]" << endl;
                printHelp();
                return 1;
            }
        } else {
            cerr << "Error: Invalid arguments" << endl;
            printHelp();
            return 1;
        }
    } else {
        // No arguments, try to read default config file
        useConfigFile = true;
    }
    
    // Read from config file if specified
    if (useConfigFile) {
        cout << "Reading configuration from: " << configFilePath << endl;
        if (!readConfigFile(configFilePath, config)) {
            cerr << "Failed to read config file. Using default values." << endl;
            cerr << "Default: base=2, exponent=5" << endl;
            cout << "\nTo create a config file, run: ./growth_calc --help" << endl;
        } else {
            cout << "Configuration loaded successfully:" << endl;
            cout << "  Base = " << config.base << endl;
            cout << "  Exponent = " << config.exponent << endl;
            cout << "  Log file = " << config.logFile << endl;
        }
    }
    
    // Validate input
    if (config.exponent < 1) {
        cerr << "Error: Exponent must be at least 1" << endl;
        return 1;
    }
    
    if (config.exponent > 100) {
        cerr << "Warning: Large exponent (" << config.exponent << ") may take a long time!" << endl;
        cout << "Estimated time: " << (config.exponent * 2) << " seconds" << endl;
    }
    
    // Open log file
    ofstream logFile;
    if (config.enableLogging) {
        logFile.open(config.logFile, ios::app);
        if (!logFile.is_open()) {
            cerr << "Warning: Could not open log file: " << config.logFile << endl;
            cerr << "Continuing without file logging..." << endl;
            config.enableLogging = false;
        } else {
            cout << "Logging to: " << config.logFile << "\n" << endl;
        }
    }
    
    // Log session start
    logOutput(logFile, "========================================", config.enableLogging);
    logOutput(logFile, "NEW CALCULATION SESSION STARTED", config.enableLogging);
    logOutput(logFile, "========================================", config.enableLogging);
    
    // Perform calculations
    performLinearCalculation(config.base, config.exponent, logFile, config.enableLogging);
    performExponentialCalculation(config.base, config.exponent, logFile, config.enableLogging);
    
    // Log session end
    logOutput(logFile, "========================================", config.enableLogging);
    logOutput(logFile, "CALCULATION SESSION COMPLETED", config.enableLogging);
    logOutput(logFile, "========================================", config.enableLogging);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    cout << "Calculation completed!" << endl;
    if (config.enableLogging) {
        cout << "Logs saved to: " << config.logFile << endl;
    }
    
    return 0;
}
