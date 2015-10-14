#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<jansson.h>


int main(int argc,char* argv[])
{
	char *str;
	json_t *root;

	root = json_pack("{s:s, s:i}", "greeting", "Hello, World!", "number", 42);
	str = json_dumps(root, JSON_ENCODE_ANY);

	printf("str = %s\n", str);
	return 0;

}