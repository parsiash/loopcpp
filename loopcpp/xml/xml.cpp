#include<xml.h>
#include<iostream>
#include<vector>
#include<string_utility.h>

using namespace std;

XML_Tag * extract_tags_recursive(char * data, unsigned int * tag_count)
{
	unsigned int char_idx = 0;
	unsigned int child_count = 0;
	unsigned int recursive_tag_count = 1;

	unsigned int open_tags_count = 0;
	vector<XML_Tag*> child_tags;
	char * tag_definition = nullptr;
	char * tag_content = nullptr;
	unsigned int tag_content_start = 0;
	unsigned int tag_content_end = 0;

	//parsing the string
	while (true)
	{
		char c = data[char_idx];
		if (c == '\0')
		{
			break;
		}

		if (c == '<')
		{
			if (data[char_idx + 1] != '/')
			{
				open_tags_count++;

				//here we recusively call extract to extract a child
				if (open_tags_count == 2)
				{
					unsigned int child_recursive_tag_count;
					XML_Tag * child = extract_tags_recursive((data + char_idx), &child_recursive_tag_count);
					child_tags.push_back(child);
					child_count++;

					recursive_tag_count += child_recursive_tag_count;
				}
			}
			else
			{
				open_tags_count--;
				
				//detect tag content end index
				if (open_tags_count == 0)
				{
					tag_content_end = char_idx - 1;
				}
			}
		}

		if (c == '>' && !tag_definition && open_tags_count == 1)
		{
			//determine tag definition length
			unsigned int tag_definition_len = char_idx - 1;
			if (data[char_idx - 1] == '/')
			{
				tag_definition_len--;
			}

			//determine tag definition
			tag_definition = (char *)malloc((tag_definition_len + 1) * sizeof(char));
			strncpy(tag_definition, (data + 1), tag_definition_len);
			tag_definition[tag_definition_len] = '\0';

			//detect tag content start
			if (data[char_idx - 1] != '/')
			{
				tag_content_start = char_idx + 1;
			}
		}

		//handle inline tags
		if (c == '>' && data[char_idx - 1] == '/')
		{
			open_tags_count--;
		}
		
		if (open_tags_count == 0)
		{
			break;
		}

		char_idx++;
	}

	XML_Tag * tag = (XML_Tag *)malloc(sizeof(XML_Tag));
	
	//set tag's children
	tag->child_count = child_count;
	tag->children = (XML_Tag **)malloc(child_count * sizeof(XML_Tag *));
	for (int i = 0; i < child_count; i++)
	{
		tag->children[i] = child_tags[i];
	}

	//set tag names and attributes
	vector<char *> definition_parts = split_string(tag_definition, ' ');
	tag->attribute_count = definition_parts.size() - 1;
	if (definition_parts.size() > 1)
	{
		tag->attributes = (XML_Tag_Attribute **)malloc((definition_parts.size() - 1) * sizeof(XML_Tag_Attribute *));
		for (int i = 1; i < definition_parts.size(); i++)
		{
			XML_Tag_Attribute * attribute = (XML_Tag_Attribute *)malloc(sizeof(XML_Tag_Attribute));
			auto attribute_parts = split_string(definition_parts[i], '=');
			if (attribute_parts.size() != 2)
			{
				cout << "malformed xml" << endl;
				return NULL;
			}

			attribute->name = attribute_parts[0];
			attribute->value = remove_character(attribute_parts[1], '\"');
			free(attribute_parts[1]);

			tag->attributes[i - 1] = attribute;
		}
	}
	else
	{
		tag->attributes = nullptr;
	}
	tag->tag_name = definition_parts[0];


	//set tag content
	if (tag_content_end > tag_content_start)
	{
		unsigned int tag_content_len = tag_content_end - tag_content_start + 1;
		tag_content = (char *)malloc((tag_content_len + 1) * sizeof(char));
		strncpy(tag_content, data + tag_content_start, tag_content_len);
		tag_content[tag_content_len] = '\0';
		
	}
	tag->tag_content = tag_content;

	//set tag count
	(*tag_count) = recursive_tag_count;

	return tag;
}

XML_Data * create_xml_data(const char * raw_data)
{	
	//finding the root index, (discarding <xml?...?> tag>)
	unsigned int root_index = 0;
	while (true)
	{
		char c = raw_data[root_index];

		if (c == '\0')
		{
			return NULL;
		}

		if (c == '<' && raw_data[root_index+1] != '?')
		{
			break;
		}

		root_index++;
	}

	//recursive extract from the root index
	unsigned int all_tags_count;
	XML_Tag * root_tag = extract_tags_recursive((char*)(raw_data + root_index), &all_tags_count);

	XML_Data * xml_data = (XML_Data *)malloc(sizeof(XML_Data));
	xml_data->root = root_tag;

	//bfs for iterating all the index and store them in xml_data->all_tags
	xml_data->all_tags_count = all_tags_count;
	xml_data->all_tags = (XML_Tag **)malloc(all_tags_count * sizeof(XML_Tag *));
	xml_data-> all_tags[0] = root_tag;

	unsigned int all_tags_index = 1;
	unsigned int processed_tag_index = 0;

	while (processed_tag_index < all_tags_count)
	{
		XML_Tag * current_tag = xml_data->all_tags[processed_tag_index];
		processed_tag_index++;

		for (int i = 0; i < current_tag->child_count; i++)
		{
			xml_data->all_tags[all_tags_index] = current_tag->children[i];
			all_tags_index++;
		}
	}
	return xml_data;
}

void free_xml_tag(XML_Tag * tag)
{
	free(tag->children);
	free(tag->tag_name);
	free(tag->tag_content);
	free(tag);
}

void free_xml_data(XML_Data * xml_data)
{
	for (int i = 0; i < xml_data->all_tags_count; i++)
	{
		free_xml_tag(xml_data->all_tags[i]);
	}

	free(xml_data->all_tags);
	free(xml_data);
}

bool XML_Tag::has_attribute(const char * attribute_name)
{
	for (int i = 0; i < this->attribute_count; i++)
	{
		if (!strcmp(this->attributes[i]->name, attribute_name))
		{
			return true;
		}
	}
	
	return false;
}

XML_Tag_Attribute * XML_Tag::get_attribute(const char * attribute_name) 
{
	for (int i = 0; i < this->attribute_count; i++)
	{
		if (!strcmp(this->attributes[i]->name, attribute_name))
		{
			return this->attributes[i];
		}
	}

	return nullptr;
}

vector<XML_Tag *> XML_Tag::get_tags_by_name(const char * name)
{
	vector<XML_Tag *> result_vector;
	vector<XML_Tag *> tag_queue;

	tag_queue.push_back(this);

	while (tag_queue.size() > 0)
	{
		XML_Tag * current_tag = tag_queue.back();
		tag_queue.pop_back();

		if (!strcmp(current_tag->tag_name, name))
		{
			result_vector.push_back(current_tag);
		}

		for (int i = 0; i < current_tag->child_count; i++)
		{
			tag_queue.push_back(current_tag->children[i]);
		}
	}

	return result_vector;
}

XML_Tag * XML_Tag::get_tag_by_name(const char * name)
{
	vector<XML_Tag *> result_vector;
	vector<XML_Tag *> tag_queue;

	tag_queue.push_back(this);

	while (tag_queue.size() > 0)
	{
		XML_Tag * current_tag = tag_queue.back();
		tag_queue.pop_back();

		if (!strcmp(current_tag->tag_name, name))
		{
			return current_tag;
		}

		for (int i = 0; i < current_tag->child_count; i++)
		{
			tag_queue.push_back(current_tag->children[i]);
		}
	}

	return nullptr;
}

XML_Tag * XML_Tag::get_by_attribute(const char * attribute_name, const char * attribute_value)
{
	vector<XML_Tag *> tag_queue;
	tag_queue.push_back(this);

	while (tag_queue.size() > 0)
	{
		auto current_tag = tag_queue.back();
		tag_queue.pop_back();

		if (current_tag->has_attribute(attribute_name))
		{
			auto value = current_tag->get_attribute(attribute_name)->value;

			if (!strcmp(value, attribute_value))
			{
				return current_tag;
			}
		}

		for (int i = 0; i < current_tag->child_count; i++)
		{
			tag_queue.push_back(current_tag->children[i]);
		}
	}

	return nullptr;
}

