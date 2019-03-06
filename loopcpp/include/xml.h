#pragma once
#include<vector>

struct XML_Tag_Attribute
{
	char * name;
	char * value;
};

struct XML_Tag
{
	char * tag_name;
	char * tag_content;

	unsigned int attribute_count;
	XML_Tag_Attribute ** attributes;

	unsigned int child_count;
	XML_Tag ** children;

	std::vector<XML_Tag *> get_tags_by_name(const char * name);
	XML_Tag * get_tag_by_name(const char * name);
	bool has_attribute(const char * attribute_name);
	XML_Tag_Attribute * get_attribute(const char * attribute_name);
	XML_Tag * get_by_attribute(const char * attribute_name, const char * attribute_value);
};

struct XML_Data
{
	XML_Tag * root;
	unsigned int all_tags_count;
	XML_Tag ** all_tags;
};


XML_Data * create_xml_data(const char * raw_data);
void free_xml_tag(XML_Tag * tag);
void free_xml_data(XML_Data * xml_data);