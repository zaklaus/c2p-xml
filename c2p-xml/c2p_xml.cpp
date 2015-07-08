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
#include <algorithm>
#include <fstream>

#define REPEAT(p,q) for(int __i = 0; __i < p; __i++){ std::cout << q; }
#define FOR(p,q) for(int __i = p; __i < q; __i++)
using namespace CXML;

XMLProperty::XMLProperty(std::string name, std::string text)
{
	this->m_szPropName = name;
	this->m_szPropText = text;
}

XMLNode::XMLNode(std::string name, std::string buffer)
{
	XMLNode::ParseXML(*this, buffer, name);
}

XMLNode::~XMLNode(){}

XMLDocument::XMLDocument(std::string DocumentName)
{
	std::string str, buffer;
	std::ifstream myfile (DocumentName.c_str());
	if (myfile.is_open())
	{
		while (std::getline(myfile, str))
		{
		  buffer += str;
		}  
		myfile.close();
	}
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\t'), buffer.end());
	XMLNode::ParseXML(*this, buffer, "document");
}

void XMLNode::DumpNode(XMLNode& node, int rec)
{
	REPEAT(rec, '-'); std::cout << node.m_szNodeName;
	if (node.m_cProps.size() == 0)
	{
		std::cout << " []:" << std::endl;
	}
	else 
	{
		std::cout << " [";
		for(int i = 0; i < node.m_cProps.size(); i++)
		{
			std::cout << node.m_cProps.at(i).m_szPropName << ": \"" <<
				node.m_cProps.at(i).m_szPropText << "\", ";
		}
		std::cout << "]:" << std::endl;
	}
	if (node.m_cNodes.size() == 0)
	{
		REPEAT(rec+2, '-'); std::cout << node.m_szNodeContent << "," << std::endl;
	}
	else 
	{
		for(int i = 0; i < node.m_cNodes.size(); i++)
		{
			XMLNode::DumpNode(node.m_cNodes.at(i), rec+1);
		}
	}
}

void XMLNode::ExportXML(XMLNode& node, std::string& output, int rec)
{
	if(rec != 0)
	{
		FOR(0,rec-1)
		{
			output += '\t';
		}
		output += "<" + node.m_szNodeName;
	}
	if (node.m_cProps.size() == 0)
	{
		if(rec != 0)
		{
			if(node.m_cNodes.size())
			{
				output += ">\n";
				
			}
			else
			{
				output += ">";
			}
		}
	}
	else 
	{
		output += " ";
		for(int i = 0; i < node.m_cProps.size(); i++)
		{
			output += node.m_cProps.at(i).m_szPropName + "=\"" +
				node.m_cProps.at(i).m_szPropText + "\"";

			if (i+1 != node.m_cProps.size())
			{
				output += " ";
			}
		}
		if(rec != 0)
		{
			if(node.m_cNodes.size())
			{
				output += ">\n";
			}
			else
			{
				output += ">";
			}
		}
	}
	if (node.m_cNodes.size() == 0)
	{
		output += node.m_szNodeContent;
	}
	else 
	{
		for(int i = 0; i < node.m_cNodes.size(); i++)
		{
			XMLNode::ExportXML(node.m_cNodes.at(i), output, rec+1);
		}
	}
	
	if(rec != 0)
	{
		if(node.m_cNodes.size())
		{
			FOR(0,rec-1)
			{
				output += '\t';
			}
		}
		output += "</" + node.m_szNodeName + ">\n";
	}
}

void XMLNode::ParseXML(XMLNode& node, std::string buffer, std::string name)
{
	node.m_szNodeName = name;

	char state = NODE_SEEK;
	std::vector<XMLProperty> props;
	std::string nodeName, nodeContent, propName, propText;
	int acu = 0;
	for(std::string::iterator it = buffer.begin(); it != buffer.end(); ++it)
	{
		switch(state)
		{
		case NODE_SEEK:
			if (*it == '<')
			{
				state = NODE_NAME;
			}
			else
			{
				nodeContent += *it;
			}
			break;
		case NODE_NAME:
			if (*it == '>')
			{
				state = NODE_READ;
			}
			else if (*it == ' ')
			{
				state = NODE_PROP;
			}
			else 
			{
				if(*it != '<')
				{
					nodeName += *it;
				}
			}
			break;
		case NODE_READ:
			if(*it == '<' && *(it+1) != '/')
			{
				nodeContent += *it;
				acu++;
			}
			else if(*it == '<' && *(it+1) == '/')
			{
				if (acu)
				{
					nodeContent += *it;
					acu--;
				} 
				else
				{
					it += nodeName.size() + 2;
					state = NODE_SEEK;
					XMLNode newnode = XMLNode(nodeName, nodeContent);
					newnode.m_cProps = props;
					node.m_cNodes.push_back(newnode);
					nodeName = "";
					nodeContent = "";
				}
			}
			else
			{
				nodeContent += *it;
			}
			break;
		case NODE_PROP:
			if (*it == '>')
			{
				state = NODE_READ;
			}
			else if (*it != ' ')
			{
				state = NODE_PROP_NAME;
				propName += *it;
			}
			break;
		case NODE_PROP_NAME:
			if (*it != '=')
			{
				propName += *it;
			}
			else
			{
				state = NODE_PROP_TEXT;
				it++;
			}
			break;
		case NODE_PROP_TEXT:
			if (*it != '\"')
			{
				propText += *it;
			}
			else 
			{
				state = NODE_PROP;
				XMLProperty prop = XMLProperty(propName, propText);
				props.push_back(prop);
				propName = "";
				propText = "";
			}
			break;
		}
	}
	node.m_szNodeContent = nodeContent;
}

XMLDocument::~XMLDocument(){}