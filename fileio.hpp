#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using std::vector;
using std::string;

namespace File{

class FileIO{

    private:
    vector<string> split(string& input, char delimiter);

    public:
    vector<vector<string>> readText(string filename);
    bool writeCsv(string filename,vector<vector<string>> writtingdata);
};  

}