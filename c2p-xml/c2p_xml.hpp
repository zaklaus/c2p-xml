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
#ifndef C2P_XML_H
#define C2P_XML_H

#include <string>
#include <vector>

#define NODE_SEEK 0
#define NODE_NAME 1
#define NODE_READ 2
#define NODE_PROP 3
#define NODE_PROP_NAME 4
#define NODE_PROP_TEXT 5

namespace CXML {
	
	class XMLProperty {
	public:
		std::string				m_szPropName, m_szPropText;

		XMLProperty(std::string, std::string);
		~XMLProperty(){};
	};

	class XMLNode {
	public:
		std::string					m_szNodeName, m_szNodeContent;
		std::vector<XMLNode>		m_cNodes;
		std::vector<XMLProperty>	m_cProps;

		XMLNode(std::string, std::string);
		XMLNode(){};
		~XMLNode();

		static void ParseXML(XMLNode&, std::string, std::string);
		static void DumpNode(XMLNode&, int);
	};

	class XMLDocument : public XMLNode {
	public:
		std::string				m_szDocumentName;

		XMLDocument(std::string);
		~XMLDocument();
	};
}

#endif
