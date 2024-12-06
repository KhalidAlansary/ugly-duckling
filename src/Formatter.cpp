#include "Formatter.hpp"
#include <iostream>
#include <string>
using namespace std;

Xml::Xml(string s) : xml(s) {}

// Applies formatting based on tag type
string Xml::applyFormatting(const string& tag, const string& type, int& tabLevel) {
    string formattedString;

    if (type == "closing") {
        tabLevel--;
        formattedString += '\n' + string(tabLevel * 4, ' ') + tag;
    } else if (type == "opening") {
        formattedString += '\n' + string(tabLevel * 4, ' ') + tag;
        tabLevel++;
    } else if (type == "self-closing") {
        formattedString += '\n' + string(tabLevel * 4, ' ') + tag;
    } else { // content
        formattedString += tag;
    }

    return formattedString;
}

// Parses and formats the XML string
string Xml::parseAndFormatTags(const string& s) {
    int stringSize = s.size();
    string formattedString;
    int tabLevel = 0;

    string currentTag;
    bool insideTag = false;

    for (int i = 0; i < stringSize; i++) {
        if (s[i] == '<') {
            if (!currentTag.empty()) {
                formattedString += applyFormatting(currentTag, "content", tabLevel);
                currentTag.clear();
            }

            insideTag = true;
            currentTag += s[i];
        } else if (s[i] == '>') {
            currentTag += s[i];
            insideTag = false;

            // Determine tag type
            if (currentTag[1] == '/') {
                formattedString += applyFormatting(currentTag, "closing", tabLevel);
            } else if (currentTag[currentTag.size() - 2] == '/') {
                formattedString += applyFormatting(currentTag, "self-closing", tabLevel);
            } else {
                formattedString += applyFormatting(currentTag, "opening", tabLevel);
            }

            currentTag.clear();
        } else {
            if (insideTag) {
                currentTag += s[i];
            } else {
                currentTag += s[i]; // Collect content between tags
            }
        }
    }

    return formattedString;
}

int main() {
    string inputXML = R"(
<root>
    <user>
        <name>John</name>
        <posts>
            <post>Sample post</post>
            <post>Another post</post>
        </posts>
    </user>
</root>)";

    Xml xmlProcessor(inputXML);

    string formattedXML = xmlProcessor.parseAndFormatTags(inputXML);

    // Print the formatted XML
    cout << "Formatted XML:\n" << formattedXML << endl;

    return 0;
}