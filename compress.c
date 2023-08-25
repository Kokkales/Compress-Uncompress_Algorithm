/*SOURCES OF CODE
	
	the whole code of compress.c is based on the lectures,the labs 
	I watched during the semester and the slides that  are uploaded in eclass
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
	char c; // character we count;
	int count; //number of the characters we count  
}run;    

int main(int argc,char* argv[])
{
	FILE* uncom=fopen(argv[1],"r");  //uncom stands for uncommpresed
	if(uncom==NULL)  //checking existence of file
	{
		printf("ERROR FILE NOT FOUND!\n");
		abort();
	}
	
	//finding number of characters
	fseek(uncom,0,SEEK_END);
	long numChar=ftell(uncom);
	
	
	//using numChar to find how many structs there are 
	int i;
	char k,l;
	int numRun=1;  //the numRun is the number of the structs
	fseek(uncom,0,SEEK_SET); //reseting the position of uncom
	/*i will count the structs by comparing the side-by-side charachters.
	   when the two charachters which are compared are not the same means that i have a new struct(run)
	   for example:::file with content:"aabcaa"
	   				  int count=1	
	   		          'a'='a',
					  'a'!='b' -> count++ ,
					  'b'!='c' -> count++ ,
					  'c'!='a' -> count++ ,
					  'a'='a'
		Finally the variable count takes value 4  (count=4) ,which means i will havw 4 structs(run) */
	for(i=0;i<numChar-1;i++) 		//here is -1 because the file ends and thre is no other char to compair
	{
		fread(&k,sizeof(char),1,uncom);
		fread(&l,sizeof(char),1,uncom);
		if(k!=l)
		{
			numRun++;
		}
		fseek(uncom,-1,SEEK_CUR);  
	}
	
	//here i have to save data to each struct(run)
	//1.	allocate memory for numRun structs(run)
	//2.	initialization of uncom
	
	int j;
	run *r;
	r=(run*)malloc(numRun*sizeof(run));
	if(r==NULL)
	{
		printf("Allocation memmory ERROR!!\n");
		abort();
	}
	
	
	fseek(uncom,0,SEEK_SET);  //initializing uncom
	for(i=0;i<numRun;i++){
		fread(&k,sizeof(char),1,uncom);  //setting k the first char of the file1.txt
		r[i].c=k;  //charchter that is compared in every struct(run)
		r[i].count=1; //counter of this(k) charachter
		for(j=ftell(uncom);j<numChar;j++)
		{
			fread(&l,sizeof(char),1,uncom);
			if(r[i].c==l)
			{
				r[i].count++;
			}
			else{
				break;
			}
		}
		fseek(uncom,-1,SEEK_CUR);
		k=l;
		
	}
	
	fclose(uncom);
	
	//I have saved the data to each struct..now i have to copy them to file1.compressed
	FILE* com;
	com=fopen(argv[2],"w");
	if(com==NULL)
	{
		printf("ERROR FILE NOT FOUND!!\n");
		abort();
	}
	
	for(i=0;i<numRun;i++)
	{
		fprintf(com,"%d%c",r[i].count,r[i].c);
	}
	
	fseek(com,0,SEEK_END);
	long comSize=ftell(com);
	
	printf("\n\tRESULTS\n");
	for(i=0;i<numRun;i++)
	{
		fprintf(stdout,"%d%c",r[i].count,r[i].c);
	}
	
	printf("\n\nSize of uncompressed file: %ld\n",numChar);
	printf("Size of compressed file: %ld\n",comSize);
	
	free(r);
	fclose(com);
	
	
	return 0;
}
