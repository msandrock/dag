#include <fstream>
#include <set>
#include <string>
#include <vector>
#include "svg.hpp"
#include "dag.hpp"

const int OFFSET = 25;  // Offset to canvas corner
const int WIDTH = 240;
const int HEIGHT = 40;
const int XOFFSET = WIDTH + 100;
const int YOFFSET = HEIGHT + 50;
const int LABEL_MAX_LENGTH = 23;

/*
 * Emit markup for a single dependency node
 */
bool _write_node(std::fstream& stream,
    std::set<std::string>& renderedNodes,
    const dag::node_ptr& node) {

    if (renderedNodes.find(node->name) != renderedNodes.end()) {
        // Node already present in set
        return false;
    }

    // Source: https://stackoverflow.com/questions/5546346/how-to-place-and-center-text-in-an-svg-rectangle/44857272#44857272
    /*
    stream << "<svg width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\">" << std::endl;
    stream << "<rect x=\"0\" y=\"0\" width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\" />" << std::endl;
    stream << "<text x=\"50%\" y=\"50%\" dominant-baseline=\"middle\" text-anchor=\"middle\">" << node->name << "</text>" << std::endl;
    stream << "</svg>" << std::endl;
    */

    std::string nodeLabel = node->name;
    if (nodeLabel.length() > LABEL_MAX_LENGTH) {
        nodeLabel = nodeLabel.substr(0, LABEL_MAX_LENGTH) + "...";
    }

    stream << "<rect x=\"" << node->x * XOFFSET + OFFSET << "\" y=\"" << node->y * YOFFSET + OFFSET << "\" width=\"" << WIDTH << "\" height=\"" << HEIGHT << "\" />" << std::endl;
    stream << "<text x=\"" << (node->x * XOFFSET + WIDTH / 2 - 90 + OFFSET) << "\" y=\"" << (node->y * YOFFSET + 5 + HEIGHT / 2 + OFFSET) << "\">" << nodeLabel << "</text>" << std::endl;

    renderedNodes.insert(node->name);
    return true;
}

/**
 * Emit markup to draw a connecting line between two nodes
 */
void _write_edge(
    std::fstream& stream,
    const dag::node_ptr& nodeStart,
    const dag::node_ptr& nodeEnd) {
    
    stream << "<line x1=\"" << (nodeStart->x * XOFFSET + WIDTH + OFFSET) << "\" y1=\"" << (nodeStart->y * YOFFSET + 5 + HEIGHT / 2 + OFFSET) << "\" x2=\""<< (nodeEnd->x * XOFFSET + OFFSET) << "\" y2=\"" << (nodeEnd->y * YOFFSET + 5 + HEIGHT / 2 + OFFSET) << "\" marker-end=\"url(#arrow)\" />" << std::endl;
}

/**
 * Recursively emit markup for a list of nodes
 */
void _write_node_array(
    std::fstream& stream,
    std::set<std::string>& renderedNodes,
    const dag::node_vec& nodes,
    const dag::node_ptr& parentNode = nullptr) {
    
    for (auto node: nodes) {
        // Draw edge from parent node to connected node
        if (parentNode != nullptr) {
            _write_edge(stream, parentNode, node);
        }

        if (_write_node(stream, renderedNodes, node)) {
            _write_node_array(stream, renderedNodes, node->children, node);
        }
    }
}

/**
 * Calculate the maximum branch length
 */
int _get_branch_length(const dag::node_vec& nodes, int length) {
    if (nodes.size()) {
        length++;
    }

    int maxLength = length;
    for (auto node: nodes) {
        auto branchLength = _get_branch_length(node->children, length);

        if (branchLength > maxLength) {
            maxLength = branchLength;
        }
    }

    return maxLength;
}

/**
 * Calculate the maximum path length in the dag
 */
int _get_dag_length(const dag::node_vec& startNodes) {
    int length = startNodes.size() ? 1 : 0;
    int maxLength = length;
    for (auto startNode: startNodes) {
        auto branchLength = _get_branch_length(startNode->children, length);

        if(branchLength > maxLength) {
            maxLength = branchLength;
        }
    }

    return maxLength;
}

/**
 * Creates an svg for the given dags
 */
void write_svg(const dag::node_vec& startNodes, const std::string& filename) {
    std::fstream stream(filename, std::ios::out);
    auto length = _get_dag_length(startNodes);

    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
    stream << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
    stream << "version=\"1.1\" baseProfile=\"full\" ";
    stream << "viewBox=\"0 0 " << length * 350 << " " << length * 200 << "\" ";
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

    std::set<std::string> renderedNodes;
    _write_node_array(stream, renderedNodes, startNodes);

    stream << "</svg>";

    stream.close();
}
