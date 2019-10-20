#include <fstream>
#include <string>
#include <vector>
#include "svg.hpp"
#include "dag.hpp"

/*
 * Emit markup for a single dependency node
 */
void _write_node(std::fstream& stream, const dag::DagNode& node, int x, int y) {
    const int WIDTH = 200, HEIGHT = 80;
    stream << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\" />" << std::endl;
    stream << "<text x=\"" << (x + WIDTH / 2) << "\" y=\"" << (y + 5 + HEIGHT / 2) << "\">" << node.name << "</text>" << std::endl;
}

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
    stream << "viewBox=\"0 0 800 600\" ";
    stream << ">" << std::endl;

    stream << "<title>Titel der Datei</title>" << std::endl;
    stream << "<desc>Beschreibung/Textalternative zum Inhalt.</desc>" << std::endl;
    
    stream << "<style type=\"text/css\">" << std::endl;
    stream << "rect { fill: #ccc; }" << std::endl;
    stream << "text { fill: #f00; }" << std::endl;
    stream << "</style>" << std::endl;

    _write_node(stream, (*startNodes[0]), 25, 25);

    stream << "</svg>";

    stream.close();
}