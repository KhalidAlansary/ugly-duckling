#ifndef SUGGESTER_HPP
#define SUGGESTER_HPP

#include <string>

#include "parser.hpp"
#include "util.hpp"

/** Suggests users to follow based on the followers of the user with the given
 * ID.
 *
 * @param users The list of users.
 * @param user_id The ID of the user for which to suggest followers.
 * @return A string containing the suggested followers.
 */
std::string suggest(const std::vector<User>& users, const std::string& user_id);

#endif