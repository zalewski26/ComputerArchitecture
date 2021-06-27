#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void myScanf(char*, ...);
void myPrintf(char*, ...);
char* convert(unsigned int num, int base);
int deconvert(char* string, int base);

int main(){
	int n1, n2 = 11, n3;
	char* string;
	myPrintf("Podaj kolejno: dziesiętną, binarną, szesnastkową, napis: \n");
	myScanf("%d %b %x %s", &n1, &n2, &n3, &string);
	//scanf("%d %x %s", &n1, &n3, string);
	myPrintf("\nPierwsza liczba:\ndziesiętnie = %d\nbinarnie = %b\nszesnastkowo = %x\n", n1, n1, n1); 
	myPrintf("\nDruga liczba:\ndziesiętnie = %d\nbinarnie = %b\nszesnastkowo = %x\n", n2, n2, n2); 
	myPrintf("\nTrzecia liczba:\ndziesiętnie = %d\nbinarnie = %b\nszesnastkowo = %x\n\nNapis: %s\n", n3, n3, n3, string); 

	return 0;
}

void myScanf(char* format, ...){
	char* args = (char*) &format + sizeof format;

	for (char* current = format; *current != '\0'; current++){
		while (*current != '%' && *current != '\0')
			current++;

		if (*current == '\0')
			break;	
		current++;

		if (*current == 'd' || *current == 'b' || *current == 'x' || *current == 's'){
			char** address_str = (char**) (*(char**) args);
			int* address = (int*) *((int*) args);
			char* buff = malloc(sizeof(1024));
			int read_result;

			int counter = 0;
			while(read_result = read(0, buff + counter, 1) != 0){
				if (buff[counter] == '\n' || buff[counter] == ' '){
					buff[counter] = '\0';
					break;
				}
				counter++;
			}

			if (*current == 's'){
				*address_str = buff;
			}
			else{
				int base;
				switch (*current){
					case 'b':
						base = 2;
						break;
					case 'd':
						base = 10;
						break;
					case 'x':
						base = 16;
						break;
				}
				*address = deconvert(buff, base);
			}
			
			args += sizeof (int*);
		}
	}
	args = 0;
}

void myPrintf(char* format, ...){
	char* arg = (char*) &format + sizeof format;			//va_list arg;      va_start(arg, format);

	for (char* current = format; *current != '\0'; current++){
		while (*current != '%' && *current != '\0'){
			write(1, current, 1);
			current++;
		}
		if (*current == '\0')
			break;

		current++;

		int i;
		switch (*current){
			case 's': ;
				char* string;
				string = *((char**) arg);
				arg += sizeof (char*);
				for (; *string != '\0'; string++)
					write(1, string, 1);
				break;
			case 'd': ;
				i = *((int*) arg);				//i = va_arg(arg, int);
				arg += sizeof (int);
				write(1, convert(i, 10), 50);
				break;
			case 'x': ;
				i = *((int*) arg);
				arg += sizeof (int);
				write(1, convert(i, 16), 50);
				break;
			case 'b': ;
				i = *((int*) arg);
				arg += sizeof (int);
				write(1, convert(i, 2), 50);
				break;			
		}
	}
	arg = 0;
}

char* convert(unsigned int num, int base){ 
	static char representation[]= "0123456789ABCDEF";
	static char buffer[50]; 
	char *ptr; 
	
	ptr = &buffer[49]; 
	*ptr = '\0'; 
	
	do{ 
		*--ptr = representation[num%base]; 
		num /= base; 
	} while(num != 0); 
	
	return(ptr); 
}

int deconvert(char* string, int base) {
	static char representation[]= "0123456789ABCDEF";
	int result = 0;
	int multiplier = 1;

	for(int i = strlen(string) - 1; i >= 0; i--){
		for (int j = 0; j < base + 1; j++){
			if (string[i] == representation[j]){
				result += j * multiplier;
				continue;
			}
		}
		multiplier *= base;
	}

	return result;
}

/*switch (*current){
			case 'd': ;
				int* address = (int*) *((int*) args);
				char* buff = malloc(sizeof(50));
				int read_result = read(0, buff, 50);
				int num = 0;
				for (int j = 0; j < read_result - 1; j++){
					num = num*10 + buff[j] - '0';
				}

				*address = num;
				//args += sizeof address;
				break;

			case 's': ;
				char** address2 = (char**) (*(char**) args);
				char* buff2 = malloc(sizeof(50));
				int read_result2 = read(0, buff2, 50);
				*address2 = buff2;
				//args += sizeof *address2;
		}
		args += sizeof (char*);*/

/*int* result = malloc(4 * sizeof (int));
		int multiplier = 1;
		int cleaner = 0x000000ff;

		for (int j = 0; j < sizeof args; j++){
			result[j] = args[j] * multiplier & cleaner;
			multiplier *= 256;
			cleaner = cleaner << 8;
		}
		int address = result[0] | result[1] | result[2] | result[3];*/
