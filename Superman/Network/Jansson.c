#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<jansson.h>


int main(int argc,char* argv[])
{
	char *str;
	char *buf1,*buf2,*buf3;
	int a;
	json_error_t error;
	json_t *root;
	json_t *test;

	root = json_pack("{s:s,s:s,s:s,s:i}","P_H", "greeting","P_F", "Hello, World!","P_S", "number","P_X", 42);
	str = json_dumps(root, JSON_ENCODE_ANY);


	json_unpack(root,"{s:s,s:s,s:s,s:i}","P_H",&buf1,"P_F",&buf2,"P_S",&buf3,"P_X",&a);

	printf("test: %s %s %s %d\n",buf1,buf2,buf3,a);

	printf("str = %s\n", str);

	//test=json_string(str);

	test=json_loads(str,JSON_DECODE_ANY,&error);

	str = json_dumps(test, JSON_ENCODE_ANY|JSON_ESCAPE_SLASH);

	printf("str = %s\n", str);
	

	return 0;

}
