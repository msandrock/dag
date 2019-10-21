#include <fstream>
#include <string>
#include <vector>
#include "svg.hpp"
#include "dag.hpp"

/*
 * Emit markup for a single dependency node
 */
void _write_node(std::fstream& stream, std::shared_ptr<dag::DagNode> node) {
    const int OFFSET = 25;  // Offset to canvas corner
    const int WIDTH = 240, HEIGHT = 40;
    const int XOFFSET = WIDTH + 100;
    const int YOFFSET = HEIGHT + 50;

    // Source: https://stackoverflow.com/questions/5546346/how-to-place-and-center-text-in-an-svg-rectangle/44857272#44857272
    /*
    stream << "<svg width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\">" << std::endl;
    stream << "<rect x=\"0\" y=\"0\" width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\" />" << std::endl;
    stream << "<text x=\"50%\" y=\"50%\" dominant-baseline=\"middle\" text-anchor=\"middle\">" << node->name << "</text>" << std::endl;
    stream << "</svg>" << std::endl;
    */

    stream << "<rect x=\"" << node->x * XOFFSET + OFFSET << "\" y=\"" << node->y * YOFFSET + OFFSET << "\" width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\" />" << std::endl;
    //stream << "<ellipse cx=\"" << node->x * XOFFSET + OFFSET + WIDTH / 2 << "\" cy=\"" << node->y * YOFFSET + OFFSET + HEIGHT / 2 << "\" rx=\"" << WIDTH / 2 << "\" ry=\"" << HEIGHT / 2 << "\" />" << std::endl;
    stream << "<text x=\"" << (node->x * XOFFSET + WIDTH / 2 - 90 + OFFSET) << "\" y=\"" << (node->y * YOFFSET + 5 + HEIGHT / 2 + OFFSET) << "\">" << node->name << "</text>" << std::endl;
}

/**
 * Emit markup to draw a connecting line between two nodes
 */
void _write_edge(std::fstream& stream, std::shared_ptr<dag::DagNode> nodeStart, std::shared_ptr<dag::DagNode> nodeEnd) {
    const int OFFSET = 25;  // Offset to canvas corner
    const int WIDTH = 240, HEIGHT = 40;
    const int XOFFSET = WIDTH + 100;
    const int YOFFSET = HEIGHT + 50;
    stream << "<line x1=\"" << (nodeStart->x * XOFFSET + WIDTH + OFFSET) << "\" y1=\"" << (nodeStart->y * YOFFSET + 5 + HEIGHT / 2 + OFFSET) << "\" x2=\""<< (nodeEnd->x * XOFFSET + OFFSET) << "\" y2=\"" << (nodeEnd->y * YOFFSET + 5 + HEIGHT / 2 + OFFSET) << "\" marker-end=\"url(#arrow)\" />" << std::endl;
}

/**
 * Recursively emit markup for a list of nodes
 */
void _write_node_array(std::fstream& stream, const std::vector<std::shared_ptr<dag::DagNode>>& nodes, std::shared_ptr<dag::DagNode> parentNode = nullptr) {
    for (auto node: nodes) {
        // Draw edge from parent node to connected node
        if (parentNode != nullptr) {
            _write_edge(stream, parentNode, node);
        }

        _write_node(stream, node);
        _write_node_array(stream, node->children, node);
    }
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
    stream << "viewBox=\"0 0 1800 1600\" ";
    stream << ">" << std::endl;

    stream << "<title>DAG</title>" << std::endl;
    stream << "<desc>Generated using dag cli</desc>" << std::endl;

    // Source: https://developer.mozilla.org/de/docs/Web/SVG/Element/marker
    stream << "<defs>" << std::endl;
    stream << " <marker id=\"arrow\" viewBox=\"0 0 10 10\" refX=\"5\" refY=\"5\" ";
    stream << " markerWidth=\"12\" markerHeight=\"12\" ";
    stream << " orient=\"auto-start-reverse\">" << std::endl;
    stream << "     <path d=\"M 0 0 L 10 5 L 0 10 z\" fill=\"#f00\" />" << std::endl;
    stream << " </marker>" << std::endl;
    stream << "</defs>" << std::endl;

    stream << "<style type=\"text/css\">" << std::endl;
    stream << "rect, ellipse { stroke: #888; fill: #ccc; }" << std::endl;
    stream << "text { fill: #000; font-family: Arial, Sans-serif; }" << std::endl;
    stream << "line { stroke: #f00; }" << std::endl;
    stream << "</style>" << std::endl;

    _write_node_array(stream, startNodes);

    stream << "</svg>";

    stream.close();
}