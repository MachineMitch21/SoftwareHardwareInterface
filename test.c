
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int		_int;
	char* 	_str;
} Test;

void show_int(int i)
{
	show_bytes((char*) &i, sizeof(int));
}

void show_test(Test* t)
{
	show_bytes((char*) t, sizeof(Test));
}

void show_bytes(char* start, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("Byte {%d}: {%p, 0x%.8x}\n", i, start + i, *(start + i));
	}
}

int write_bytes_to_disk(char* file_name, char* start, int len)
{
	FILE* fp;
	int rv = 0;

	fp = fopen(file_name, "w+");

	if (fp != NULL)
	{
		for (int i = 0; i < len; i++)
		{
			fprintf(fp, "0x%.8x\n", *(start + i));
		}
	}
	else 
	{
		rv = -1;
	}
	
	fclose(fp);
	return rv;
}

int write_test_to_disk(char* file_name, Test* t)
{
	return write_bytes_to_disk(file_name, (char*) t, sizeof(Test));
}

int load_bytes_from_disk(char* file_name, char* start, int len)
{
	FILE* fp;	
	int rv = 0;

	fp = fopen(file_name, "r");

	if (fp != NULL)
	{
		for (int i = 0; i < len; i++)
		{
			char buf[10];
			fscanf(fp, "%s", buf);
			start[i] = (char)strtol(buf, NULL, 0);
		}
	}
	else 
	{
		rv = -1;
	}
	
	fclose(fp);
	return rv;
}

int load_test_from_disk(char* file_name, Test* t)
{	
	return load_bytes_from_disk(file_name, (char*) t, sizeof(Test));
}

int load_int_from_disk(char* file_name, int* i)
{
	return load_bytes_from_disk(file_name, (char*) i, sizeof(int));
}	

int load_int_arr_from_disk(char* file_name, int* start, int len)
{
	return load_bytes_from_disk(file_name, (char*) start, sizeof(int) * len);
}

int main(int argc, char** argv)
{
	int a = 12345;

	printf("int a = %d\n", a);
	
	show_int(a);

	int big_num = 1220619400;

	printf("\nint big_num = %d\n", big_num);

	show_int(big_num);

	Test t;
	t._str = "Some test string";
	t._int = 12345;
	
	printf("\nTest: {%s, %d}\n", t._str, t._int);

	show_test(&t);

	if (write_test_to_disk("test.txt", &t) == 0)
	{
		printf("\nTest saved to disk\n");
	}

	Test t1;

	if (load_test_from_disk("test.txt", &t1) == 0)
	{
		printf("Test after loading from disk: {%s, %d}\n", t1._str, t1._int);
	}
	else 
	{
		printf("\nTest could not be loaded from disk\n");
	}

	int b = 0;

	if (load_int_from_disk("test.txt", &b) == 0)
	{
		printf("Integer from first four bytes in test.txt == {%d}\n", b);
	}

	int arr[4];

	if (load_int_arr_from_disk("test.txt", arr, 4) == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			printf("Integer from bytes {%d} to {%d} in test.txt == {%d}\n",(i * 4) ,((i + 1) * 4) - 1, arr[i]); 
		}
	}

	return 0;
}
