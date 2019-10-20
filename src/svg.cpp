#include <string>
#include <fstream>
#include "svg.hpp"

/**
 * Creates an svg for the given dags
 */
void write_svg(const std::string& filename) {
    std::fstream stream(filename, std::ios::out);

    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\"";
    stream << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
    stream << "version=\"1.1\" baseProfile=\"full\" ";
    stream << "width=\"800mm\" height=\"600mm\" ";
    stream << "viewBox=\"-400 -300 800 600\">" << std::endl;

    stream << "<title>Titel der Datei</title>" << std::endl;
    stream << "<desc>Beschreibung/Textalternative zum Inhalt.</desc>" << std::endl;

    stream << "<rect x=\"25\" y=\"25\" width=\"200\" height=\"200\" />" << std::endl;

    stream << "</svg>";

    stream.close();
}