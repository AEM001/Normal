#include <fstream> // File stream library
#include <iostream> // For standard output and error output
#include <string> // For using std::string

int main() {
    // 1. Create an ifstream object
    // ifstream is derived from istream, specialized for file input
    // Use constructor to directly open file "my_input_file.txt"
    // std::ios::in is the default open mode for ifstream
    std::ifstream inFile("output.txt", std::ios::in);

    // 2. Check if file opened successfully
    if (inFile.is_open()) {
        std::string word1, word2;
        int number;

        // 3. Read data from file
        // Use extraction operator >> to read from file stream
        // >> operator skips whitespace (spaces, tabs, newlines), then reads until next whitespace
        inFile >> word1; // Read first "word"
        inFile >> word2; // Read second "word"
        inFile >> number; // Read an integer

        // 4. Output the read data to standard output
        std::cout << "First word read: " << word1 << std::endl;
        std::cout << "Second word read: " << word2 << std::endl;
        std::cout << "Number read: " << number << std::endl;

        // 5. Close the file
        inFile.close();

        std::cout << "Data successfully read from file." << std::endl;

    } else {
        std::cerr << "Error: Failed to open file for reading! Please ensure my_input_file.txt exists and has content." << std::endl;
        return 1;
    }

    return 0;
}