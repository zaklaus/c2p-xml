// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#ifndef C2P_XML_H
#define C2P_XML_H

#include <string>
#include <vector>

enum {
    NODE_SEEK,
    NODE_NAME,
    NODE_READ,
    NODE_PROP,
    NODE_PROP_NAME,
    NODE_PROP_TEXT
};
namespace CXML {
	
	class XMLProperty {
	public:
		std::string mPropName, mPropText;

		XMLProperty(std::string name, std::string text);
		~XMLProperty(){};
	};

	class XMLNode {
	public:
		std::string mNodeName, mNodeContent;
		std::vector<XMLNode> mNodes;
		std::vector<XMLProperty> mProps;

		XMLNode(std::string name, std::string buffer);
		XMLNode(){};
		~XMLNode();

		static void ParseXML(XMLNode& node, std::string buffer, std::string name);
		static void DumpNode(XMLNode& node, int rec);
		static void ExportXML(XMLNode& node, std::string& output, int rec);
	};

	class XMLDocument : public XMLNode {
	public:
		std::string mDocumentName;

		XMLDocument(std::string DocumentName);
		~XMLDocument();
	};
}

#endif
