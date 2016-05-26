#include <string>
#include <vector>

struct dm_data {
    int width;
    int height;
    int channels;
    std::vector<unsigned char> pixels;
};

struct dm_image {
    int cols;
    int rows;
    int channels;
    unsigned char *data;
};


class DataMatrix {
public:
    DataMatrix();
    ~DataMatrix();

    void setData(const std::string &data);

    bool generate(const std::string &text, dm_data &result);
    bool decode(const dm_image &image, std::string &decodedText);

    std::string pathToFile() const;

private:
    std::string m_path; // output path
    std::string m_filename;
};
