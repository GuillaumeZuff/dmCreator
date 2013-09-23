#include <string>
#include <vector>

struct dm_data {
    int width;
    int height;
    int channels;
    std::vector<unsigned char> pixels;
};

class DataMatrix {
public:
    DataMatrix();
    ~DataMatrix();

    void setData(const std::string &data);

    bool generate(dm_data &result);
    bool decode(const std::string &filePath, std::string &decodedText);

    std::string pathToFile() const;

private:
    std::string getPath(const std::string &filename) const;

private:
    std::string m_path; // output path
    std::string m_filename;
    std::string m_data;
};
