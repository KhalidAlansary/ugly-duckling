#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <string>
using namespace std;

class Xml {
private:
    string xml;

    // Applies formatting based on tag type
    string applyFormatting(const string& tag, const string& type, int& tabLevel);

public:
    // Constructor that initializes the XML string
    Xml(string s);

    // Parses and formats the XML string, returns formatted XML
    string parseAndFormatTags(const string& s);
};

#endif // FORMATTER_HPP
