/** Copyright (c) 2015 <zaklaus@github.io>. All rights reserved.
 * 
 * This file is part of parent project and is released under ZaKlaus Public Code License Version 1.2 (the 'Licence'). You may not use this file
 * except in compliance with the License. The rights granted to you under the
 * License may not be used to create, or enable the creation or redistribution
 * of, unlawful or unlicensed copies of a parent project, or to
 * circumvent, violate, or enable the circumvention or violation of, any terms
 * of an parent's project software license agreement.
 * 
 * You may obtain a copy of the License at
 * https://gist.github.com/zaklaus/ffc1accfc93af8ad4e41 and read it before using this file.
 *  
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 * KIND, either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 * Contributors:
 * - <zaklaus@github.io>
 **/
#include "c2p_xml.hpp"
#include <iostream>
#include <fstream>

CXML::XMLDocument * g_cDocument = NULL;

int main()
{
	g_cDocument = new CXML::XMLDocument("demo.xml");

	std::cout << "Original XML:" << std::endl;
	CXML::XMLNode::DumpNode(*g_cDocument, NULL);
	
	CXML::XMLNode goodNode = CXML::XMLNode("player", "Goob Newald");
	CXML::XMLProperty goodProperty = CXML::XMLProperty("mood", "good!");
	goodNode.m_cProps.push_back(goodProperty);

	g_cDocument->m_cNodes.at(0).m_cNodes.push_back(goodNode);
	std::cout << "Modified XML:" << std::endl;
	CXML::XMLNode::DumpNode(*g_cDocument, NULL);

	std::cout << "NOW CHECK YOUR EXEC FOLDER FOR FILE 'NEW.XML'" << std::endl;
	std::string out;
	CXML::XMLNode::ExportXML(*g_cDocument, out, NULL);

	std::ofstream file;
	file.open("NEW.XML");
	file << out;
	file.close();


	getchar();
	delete g_cDocument;
	return 0;
}