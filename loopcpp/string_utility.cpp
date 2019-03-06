#include<string_utility.h>

using namespace std;

vector<char *> split_string(const char * string, char delimiter)
{
	vector<char *> parts;
	unsigned int char_idx = 0;
	unsigned int start_pos = 0;

	while (true)
	{
		char c = string[char_idx];
		
		if (c == '\0')
		{
			if (char_idx > start_pos)
			{
				char * last_part = (char *)malloc(char_idx - start_pos + 2 * sizeof(char));
				strcpy(last_part, string + start_pos);
				parts.push_back(last_part);
			}
			break;
		}

		if (c == delimiter)
		{
			if (char_idx > start_pos)
			{
				char * part = (char *)malloc((char_idx - start_pos + 1) * sizeof(char));
				strncpy(part, string + start_pos, char_idx - start_pos);
				part[char_idx - start_pos] = '\0';
				parts.push_back(part);
			}

			start_pos = char_idx + 1;
		}

		char_idx++;
	}

	return parts;
}


char * remove_character(const char * string, char token)
{
	unsigned int token_count = 0;

	unsigned int char_idx = 0;
	unsigned int string_len = 0;

	while (true)
	{
		char c = string[char_idx];

		if (c == '\0')
		{
			string_len = char_idx;
			break;
		}

		if (c == token)
		{
			token_count++;
		}

		char_idx++;
	}

	char * cleaned_string = (char *)malloc((string_len - token_count + 1) * sizeof(char));
	char_idx = 0;
	unsigned int cleaned_string_idx = 0;

	while (true)
	{
		char c = string[char_idx];

		if (c != token)
		{
			cleaned_string[cleaned_string_idx] = c;
			cleaned_string_idx++;
		}

		if (c == '\0')
		{
			break;
		}

		char_idx++;
	}

	return cleaned_string;
}