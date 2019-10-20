#ifndef SVG_HPP
#define SVG_HPP
#include <string>
#include <vector>
#include "dag.hpp"

void write_svg(const std::vector<std::shared_ptr<dag::DagNode>>& startNodes, const std::string& filename);

#endif