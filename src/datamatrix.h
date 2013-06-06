#include <string>

class DataMatrix {
	public:
		DataMatrix();
		~DataMatrix();

		void setPath(const std::string &path);
		void setData(const std::string &data);

		bool generate();
                bool decode(const std::string &filePath, std::string &decodedText);

		std::string pathToFile() const;

    private:
        std::string getPath(const std::string &filename) const;

	private:
		std::string m_path; // output path
		std::string m_filename;
		std::string m_data;
};
