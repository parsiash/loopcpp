#include<Resources.h>
#include<iostream>
#include<string>
#include<file_utility.h>
#include<xml.h>
#include<string_utility.h>
#include<Mesh.h>
#include<unordered_map>
#include<sstream>
#include<graphics.h>

using namespace std;

const char * all_model_names[] =
{
	"cube",
	"building",
	"sphere",
	"hello"
};


const char * all_shader_names[] = 
{
	"basic",
	"light"
};


vector<Mesh_Data *> load_model(const char * model_path)
{
	cout << "loading model from file : " << model_path << endl;

	const char * file_content = get_file_content(model_path);

	XML_Data * xml_data = create_xml_data(file_content);

	auto geometry_tags = xml_data->root->get_tags_by_name("geometry");

	struct Input_Source
	{
		char * source_id;
		unsigned int data_count;
		unsigned int stride;
		float * data;

		float * get_data(unsigned int index)
		{
			return data + index * stride;
		}
	};

	vector<Mesh_Data *> meshes;

	//read geometries
	for (auto geometry_tag : geometry_tags)
	{
		if (geometry_tag->has_attribute("id"))
		{
			char * geometry_name = geometry_tag->get_attribute("name")->value;
			cout << "loading geometry name = " << geometry_name << endl;

			//read input sources
			auto source_tags = geometry_tag->get_tags_by_name("source");
			vector<Input_Source> input_sources;

			for (auto source_tag: source_tags)
			{
				Input_Source source;
				source.source_id = source_tag->get_attribute("id")->value;

				auto data_tag = source_tag->get_tag_by_name("float_array");

				if(data_tag)
				{
					source.data_count = atoi(data_tag->get_attribute("count")->value);

					source.data = (float *)malloc(source.data_count * sizeof(float));
					vector<char *> data_tokens = split_string(data_tag->tag_content, ' ');

					for (int i = 0; i < source.data_count; i++)
					{
						source.data[i] = atof(data_tokens[i]);
					}

					//extract stride value from technique_common tag
					source.stride = atoi(source_tag->get_tag_by_name("technique_common")->get_tag_by_name("accessor")->get_attribute("stride")->value);
				}

				input_sources.push_back(source);
			}


			//extract tirangles' data
			auto triangles_tag = geometry_tag->get_tag_by_name("triangles");
			unsigned int triangle_count = atoi(triangles_tag->get_attribute("count")->value);

			//extract vertices data from input sources
			Input_Source * vertext_source = nullptr, * normal_source = nullptr, * uv_source = nullptr;
			char * vertex_source_id = nullptr, * normal_source_id = nullptr, * uv_source_id = nullptr;

			auto vertex_input_tag = geometry_tag->get_by_attribute("semantic", "POSITION");
			auto normal_input_tag = geometry_tag->get_by_attribute("semantic", "NORMAL");
			auto uv_input_tag = geometry_tag->get_by_attribute("semantic", "TEXCOORD");

			if (vertex_input_tag)
			{
				vertex_source_id = remove_character(vertex_input_tag->get_attribute("source")->value, '#');
			}

			if (normal_input_tag)
			{
				normal_source_id = remove_character(normal_input_tag->get_attribute("source")->value, '#');
			}

			if (uv_input_tag)
			{
				uv_source_id = remove_character(uv_input_tag->get_attribute("source")->value, '#');
			}

			if (!vertex_source_id)
			{
				cout << "no vertex data for geometry " << geometry_tag->get_attribute("name")->value;
				continue;
			}

			if (!normal_source_id)
			{
				cout << "no normal data for geometry " << geometry_tag->get_attribute("name")->value;
				continue;
			}

			for (int i = 0; i < input_sources.size(); i++)
			{
				auto input_source = &input_sources[i];
				if (!strcmp(input_source->source_id, vertex_source_id))
				{
					vertext_source = input_source;
				}

				if (!strcmp(input_source->source_id, normal_source_id))
				{
					normal_source = input_source;
				}

				if (uv_source_id)
				{
					if(!strcmp(input_source->source_id, uv_source_id))
					{
						uv_source = input_source;
					}
				}

			}

			//fill mesh data
			unsigned int index_stride = (uv_source ? 3 : 2);
			unsigned int vertex_count = triangle_count * 3;
			unsigned int index_count = index_stride * vertex_count;
			Vertex * vertices = (Vertex *) malloc(vertex_count * sizeof(Vertex));

			vector<char *> index_tokens = split_string(triangles_tag->get_tag_by_name("p")->tag_content, ' ');

			for (int i = 0; i < vertex_count; i++)
			{
				unsigned int position_index = atoi(index_tokens[i * index_stride]);
				unsigned int normal_index = atoi(index_tokens[i * index_stride + 1]);
				float * position_data = vertext_source->get_data(position_index);
				float * normal_data = normal_source->get_data(normal_index);

				vertices[i].position.x = position_data[0];
				vertices[i].position.y = position_data[1];
				vertices[i].position.z = position_data[2];


				vertices[i].normal.x = normal_data[0];
				vertices[i].normal.y = normal_data[1];
				vertices[i].normal.z = normal_data[2];

				if (uv_source)
				{
					unsigned int uv_index = atoi(index_tokens[i * index_stride + 2]);
					float * uv_data = uv_source->get_data(uv_index);

					vertices[i].uv.x = uv_data[0];
					vertices[i].uv.y = uv_data[1];
				}
				else
				{
					vertices[i].uv.x = 0.0f;
					vertices[i].uv.y = 0.0f;
				}
			}

			Mesh_Data * mesh_data = (Mesh_Data *)malloc(sizeof(Mesh_Data));
			strcpy(mesh_data->name, geometry_name);
			mesh_data->vertex_count = vertex_count;
			mesh_data->vertices = vertices;

			meshes.push_back(mesh_data);
		}
	}

	free_xml_data(xml_data);
	return meshes;
}

vector<Mesh_Data *> load_all_models()
{
	vector<Mesh_Data *> meshes;

	for (auto model_name : all_model_names)
	{
		stringstream model_path_sstream;
		model_path_sstream << "../resources/" << model_name << ".dae";
		auto model_meshes = load_model(model_path_sstream.str().c_str());
		meshes.insert(meshes.end(), model_meshes.begin(), model_meshes.end());
	}

	return meshes;
}

Shader * load_shader(const char * shader_name)
{
	stringstream vertex_shader_sstream, fragment_shader_sstream;

	vertex_shader_sstream << "../shaders/" << shader_name << ".vs";
	fragment_shader_sstream << "../shaders/" << shader_name << ".fs";

	Shader * shader = create_shader(shader_name, vertex_shader_sstream.str().c_str(), fragment_shader_sstream.str().c_str());

	return shader;
}

vector<Shader *> load_all_shaders()
{
	vector<Shader *> shaders;
	for (auto shader_name : all_shader_names)
	{
		Shader * shader = load_shader(shader_name);
		shaders.push_back(shader);
	}

	return shaders;
}