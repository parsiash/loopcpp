#include "file_utility.h"

char * get_file_content(const char * file_path)
{
	FILE * file_ptr = fopen(file_path, "r");

	//compute the file length
	fseek(file_ptr, 0, SEEK_END);
	int file_len = ftell(file_ptr);
	fseek(file_ptr, 0, SEEK_SET);

	char * file_content = (char *)malloc((file_len + 1) * sizeof(char));
	int content_index = 0;

	while (content_index < file_len)
	{
		int c = getc(file_ptr);

		if (c == EOF)
		{
			break;
		}

		file_content[content_index] = c;
		content_index++;
	}

	file_content[content_index] = '\0';

	fclose(file_ptr);
	return file_content;
}