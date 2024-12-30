#include <graphviz/gvc.h>
#include <string>
#include <vector>

#include "util.hpp"

void draw(const std::vector<User>& users, const std::string& filename) {
  const std::string image_extension =
      filename.substr(filename.find_last_of(".") + 1);
  GVC_t* gvc = gvContext();
  Agraph_t* g = agopen(const_cast<char*>("G"), Agdirected, 0);
  for (const User& user : users) {
    Agnode_t* node = agnode(g, const_cast<char*>(user.id.c_str()), 1);
    for (const std::string& following : user.followers) {
      Agnode_t* following_node =
          agnode(g, const_cast<char*>(following.c_str()), 1);
      agedge(g, following_node, node, 0, 1);
    }
  }
  FILE* fp = fopen(filename.c_str(), "wb");
  gvLayout(gvc, g, "dot");
  gvRender(gvc, g, image_extension.c_str(), fp);
  fclose(fp);
}
