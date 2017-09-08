#include <stdio.h>
#include <string.h>

int main(int argc, char**argv){
	char string[4];
	if(fgets(string, sizeof(string), stdin)) printf("%s\n", string );

	for(int i = 0; i < argc; i++){
		printf("%d-%s\n",i, argv[i]);
	}

}
