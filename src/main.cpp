// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#include "c2p_xml.cpp"

#include <iostream>
#include <fstream>

CXML::XMLDocument * Document = NULL;

int main()
{
	Document = new CXML::XMLDocument("demo.xml");

	std::cout << "Original XML:" << std::endl;
	CXML::XMLNode::DumpNode(*Document, NULL);
	
	CXML::XMLNode goodNode = CXML::XMLNode("player", "Goob Newald");
	CXML::XMLProperty goodProperty = CXML::XMLProperty("mood", "good!");
	goodNode.mProps.push_back(goodProperty);

	Document->mNodes.at(0).mNodes.push_back(goodNode);
	std::cout << "Modified XML:" << std::endl;
	CXML::XMLNode::DumpNode(*Document, NULL);

	std::cout << "NOW CHECK YOUR EXEC FOLDER FOR FILE 'NEW.XML'" << std::endl;
	std::string out;
	CXML::XMLNode::ExportXML(*Document, out, NULL); // 3rd parameter = 0 ; Export only target node's childs.

	std::ofstream file;
	file.open("NEW.XML");
	file << out;
	file.close();

	std::cout << "NOW CHECK YOUR EXEC FOLDER FOR FILE 'NODE.XML'" << std::endl;
	out = "";
	CXML::XMLNode::ExportXML((*Document).mNodes.at(1), out, 1); // 3rd parameter = 1 ; Include target node in export

	file.open("NODE.XML");
	file << out;
	file.close();

	getchar();
	delete Document;
	return 0;
}