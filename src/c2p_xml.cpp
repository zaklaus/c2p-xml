// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#include "c2p_xml.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>

#define REPEAT(p,q) for(int __i = 0; __i < p; __i++){ std::cout << q; }
#define FOR(p,q) for(int __i = p; __i < q; __i++)
using namespace CXML;

XMLProperty::XMLProperty(std::string name, std::string text)
{
	this->mPropName = name;
	this->mPropText = text;
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
	REPEAT(rec, '-'); std::cout << node.mNodeName;
	if (node.mProps.size() == 0)
	{
		std::cout << " []:" << std::endl;
	}
	else 
	{
		std::cout << " [";
		for(int i = 0; i < node.mProps.size(); i++)
		{
			std::cout << node.mProps.at(i).mPropName << ": \"" <<
				node.mProps.at(i).mPropText << "\", ";
		}
		std::cout << "]:" << std::endl;
	}
	if (node.mNodes.size() == 0)
	{
		REPEAT(rec+2, '-'); std::cout << node.mNodeContent << "," << std::endl;
	}
	else 
	{
		for(int i = 0; i < node.mNodes.size(); i++)
		{
			XMLNode::DumpNode(node.mNodes.at(i), rec+1);
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
		output += "<" + node.mNodeName;
	}
	if (node.mProps.size() == 0)
	{
		if(rec != 0)
		{
			if(node.mNodes.size())
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
		for(int i = 0; i < node.mProps.size(); i++)
		{
			output += node.mProps.at(i).mPropName + "=\"" +
				node.mProps.at(i).mPropText + "\"";

			if (i+1 != node.mProps.size())
			{
				output += " ";
			}
		}
		if(rec != 0)
		{
			if(node.mNodes.size())
			{
				output += ">\n";
			}
			else
			{
				output += ">";
			}
		}
	}
	if (node.mNodes.size() == 0)
	{
		output += node.mNodeContent;
	}
	else 
	{
		for(int i = 0; i < node.mNodes.size(); i++)
		{
			XMLNode::ExportXML(node.mNodes.at(i), output, rec+1);
		}
	}
	
	if(rec != 0)
	{
		if(node.mNodes.size())
		{
			FOR(0,rec-1)
			{
				output += '\t';
			}
		}
		output += "</" + node.mNodeName + ">\n";
	}
}

void XMLNode::ParseXML(XMLNode& node, std::string buffer, std::string name)
{
	node.mNodeName = name;

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
					newnode.mProps = props;
					node.mNodes.push_back(newnode);
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
	node.mNodeContent = nodeContent;
}

XMLDocument::~XMLDocument(){}