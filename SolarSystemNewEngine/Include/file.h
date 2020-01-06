#pragma warning(disable:4996)
#include <fstream>
#include <sstream>

static char* ParseShader(const std::string& filepath) {
		FILE* fp;
		char* content = NULL;

		size_t count = 0;

		if (filepath != "") {
			fp = fopen(filepath.c_str(), "r");

			if (fp != NULL) {
				fseek(fp, 0, SEEK_END);//Lee o file desde 0 ate o fim
				count = ftell(fp);//Obtem o numero de bytes do file
				rewind(fp);//Set o file para o inicio

				if (count > 0) {
					content = (char*)malloc(sizeof(char) * (count + 1));
					if (content != 0) {
						count = fread(content, sizeof(char), count, fp);
						content[count] = '\0';
					}
				}
				fclose(fp);
			}
		}
		return content;
}


