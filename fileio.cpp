#include "fileio.hpp"

class FileIO{

    vector<string> split(string& input, char delimiter){
        std::istringstream stream(input);
        string field;
        vector<string> result;
        while (getline(stream, field, delimiter)) {
            result.push_back(field);
        }
        return result;
    };

    vector<vector<string>> readText(string filename){
        std::ifstream ifs(filename);
        vector<vector<string>> datalist;
        string line;
        while (getline(ifs, line)) {
            vector<string> strvec = split(line, ',');
            datalist.push_back(strvec);
        }
        return datalist;
    }
    bool writeCsv(string filename,vector<vector<string>> writtingdata){
        std::ofstream ofs(filename);
        for(auto i=0;i<=writtingdata.size();i++){
            ofs << writtingdata.at(i).at(0);
            ofs << writtingdata.at(i).at(1);
        }
        return true;
    };
}; 