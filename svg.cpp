#include <fstream>
#include <string>
#include <vector>
#include "svg.hpp"
#include "dag.hpp"

/**
 * Creates an svg for the given dags
 */
void write_svg(const std::vector<std::shared_ptr<dag::DagNode>>& startNodes, const std::string& filename) {
    std::fstream stream(filename, std::ios::out);

    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
    stream << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
    stream << "version=\"1.1\" baseProfile=\"full\" ";
    stream << "width=\"800\" height=\"600\" ";
    stream << "viewBox=\"0 0 800 600\">" << std::endl;

    stream << "<title>Titel der Datei</title>" << std::endl;
    stream << "<desc>Beschreibung/Textalternative zum Inhalt.</desc>" << std::endl;
    
    stream << "<style type=\"text/css\">" << std::endl;
    stream << "rect { background-color: #ccc; }" << std::endl;
    stream << "</style>" << std::endl;

    stream << "<rect x=\"25\" y=\"25\" width=\"200\" height=\"200\" />" << std::endl;

    stream << "</svg>";

    stream.close();
}