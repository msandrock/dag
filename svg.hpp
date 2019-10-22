#ifndef SVG_HPP
#define SVG_HPP
#include <string>
#include <vector>
#include "dag.hpp"

void write_svg(const dag::node_vec& startNodes, const std::string& filename);

#endif