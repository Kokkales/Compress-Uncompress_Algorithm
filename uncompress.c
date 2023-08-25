#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//list's functions archetypes and structs

typedef struct
{
	char c;
	int count;
}run;

struct _node			//struct node is the struct which contains the data and the address of the next stuct(node)
{
	run data;
	struct _node* next;
};
typedef struct _node node;  //making a synonym to _node

struct _list		//struct list is  the struct which contains the address of the last added node
{
	node* head;
};
typedef struct _list list;  //maing a synonym to _list

list* createList(); 				//this archetype is a function that creates the list we need
void addRunAtTheEnd(list*l,run r);			//this archetype is a function that add nodes at the end of the list
void  addRunByChoice(list* l);		//this archetype is a function that add nodes wherever the user wants depending on his choice
void deleteRunByChoice(list* l);	//this archetype is a function that delete node wherever the user wants depending on his choice
void destroyNode(node* n);			//this archetype is a function that helps deleteRunByChoice(list*l); to delete node completly
void changeData();					//this archetype is a function that changes data wherever the user wants depending on his choice
void printRuns(list* l);			//this archetype is a function that just print the list 




///----UNCOMPRESS PROGRAMM----

//this functions prints only the menu
void printMenu()
{
	printf("\nCHOICE MENU\n");
	printf("\t1.Add data.\n");
	printf("\t2.Change data.\n");
	printf("\t3.Delete data.\n");
	printf("\t4.Exit.\n");
	printf("My option:");
}

//i need this function so as to at the end of the programm the compressed file to get updated (because it doesn't update durring the running of the programm)
void renewCompressedFile(list* l,const char* filename)
{
	FILE* com=fopen(filename,"w");  //com stands for compressed ,.....i open the compressed file to write the new data
	if(com==NULL)
	{
		printf("Error finding file!");   //checking if exists
		abort();
	}
	
	int i;
	run r;
	node* curr=l->head;
	while(curr!=NULL)  //i run nodes 1-1 so as to print every data of each node from list in the compressed file
	{
		r=curr->data;   //saving data to a struct
		fprintf(com,"%d,%c",r.count,r.c);   //print every struct in the file
		curr=curr->next;  //making in each loop the list to show at the next node of the list 
	}
	
	fclose(com); //closing file
	
}


//this function uncompress the file
void uncompress(list* l,const char* filename){
	if(l->head==NULL)							//checks if the list is empty
	{
		printf("Empty list.Nothing to uncompress\n");
		printf("Thank you!\n");
		abort();
	}
	
	FILE* uncom=fopen(filename,"w+");		//open the uncompessed file so as to write at it the list
	if(uncom==NULL)				//checking if the file exists
	{
		printf("There isn't this file.");
		abort();
	}
	
	//1.take the data of last node every time and set them to a struct..
	int numberOfChars;
	char character;
	int size=0;
	node* curr=l->head;
	while(curr!=NULL)
	{
		run r=curr->data;
		numberOfChars=r.count;
		character=r.c;
		
		//2.set the data to uncom(count times the c).......... 
		int i;
		for(i=0;i<numberOfChars;i++)
		{
			fprintf(uncom,"%d%c",numberOfChars,character);
			size++;	   //counting the size,because later I want to put the data into a table and reverse it
		}
		curr=curr->next; //making in each loop the list to show at the next node of the list 
	}
	
	/*Because the list prints in reverse the text we had as input ,we have to reverse it again as it was at the beggining,so:
		1.I have to insert all the characters to a table
		2.write the characther of the new table in com
	*/
	char* table=(char*)malloc(size*sizeof(char));  //creating space for the table tha
	if(table==NULL)		//checkin if there is space to create the table
	{
		printf("allocation memmory error!!");
		abort();
	}
	int i;
	fseek(uncom,0,SEEK_SET);  //initialization of uncom pointer
	for(i=size-1;i>=0;i--)		//using size we start to save the characters from the end to start so as to be reversed
	{
		fread(table+i,sizeof(char),1,uncom);  //saving in a every loop the character to the table
	}
	
	fseek(uncom,0,SEEK_SET);  //i initialize the uncom again to use it for writing the characters 1-1 in the uncompressed file
	for(i=0;i<size;i++)
	{
		fwrite(table+i,sizeof(char),1,uncom);		//-----!!!!fprintf OR fwrite??????------
	}
		
	fclose(uncom);  //closing file
	free(table);		//free memory that is not usefull anymore
	printf("\nFile UNCOMPRESSED\n");
}



int main(int argc,char* argv[]){
	
	if(!strcmp("-e",argv[1])) //checking how to set the argv[] depending on how many arguments th user gave
	{
	
		//firstly i have to save from the compressed all the runs in the list
		FILE* com=fopen(argv[2],"r");  //com stands for compressed
		if(com==NULL){
			printf("THERE IS NOT THIS FILE!!");
			abort();
		}
		
		//create list
		list* l=createList();
		if(l==NULL){
			printf("There is no space for the list!");
			abort();
		}
		
		/*the following algorithm:
		  in each loop 
		  I use fscanf so as to set values to numberOfChars and character variables (the fscanf seperates the integer and the char of the file)
		  I set in struct(run) the number of characters and the charachter
		  I add run to the list i created before
		  */
		run r;  //create a struct
		int numberOfChars;
		char character;
		while(fscanf(com,"%d%c",&numberOfChars,&character)!=EOF)  //the loop ends when the file ends
		{
			r.count=numberOfChars;
			r.c=character;
			addRunAtTheEnd(l,r);
		}
	
		printRuns(l);
		printf("\n");
		fclose(com);
		
		//----------
		
		int option;
		printf("\tWELCOME\n");
		printMenu();
		
		do{
			scanf("%d",&option);
			if(option>4 || option<1)
			{
				printf("\nWRONG input.Please provide an integer between 1-4.\n");
				printf("My choice:");
			}
		}while(option>4 || option<1);
		
		while(option!=4){
			switch(option){
				case 1: //add data anywhere
					addRunByChoice(l);
					printf("\n\nCurrentList:\n");
					printRuns(l);
					break;
				case 2: //change data
					changeData(l);
					printf("\n\nCurrentList:\n");
					printRuns(l);
					break;
				case 3: //delete data
					deleteRunByChoice(l);
					printf("\n\nCurrentList:\n");
					printRuns(l);
					break;
			}
			printMenu();
			do{
				scanf("%d",&option);
				if(option>4 || option<1)
				{
					printf("\nWRONG input.Please provide an integer between 1-4.\n");
					printf("My choice:");
				}
			}while(option>4 || option<1);
		}
		uncompress(l,argv[1]);
		renewCompressedFile(l,argv[2]);
		
		
		//printing the size
		FILE* com1=fopen(argv[2],"r");
		if(com1==NULL)
		{
			printf("file not found\n");
			abort();
		}
		fseek(com1,0,SEEK_END);
		long com1Size=ftell(com);
		printf("\n\ncompressed size:%ld\n",com1Size);
		fclose(com1);
		
		FILE* uncom=fopen(argv[3],"r");
		if(uncom==NULL)
		{
			printf("file not found\n");
			abort();
		}
		fseek(uncom,0,SEEK_END);
		long uncomSize=ftell(uncom);
		printf("uncompressed size:%ld",uncomSize);
		fclose(uncom);
		
	}
	else
	{
		//firstly i have to save from the compressed all the runs in the list
		FILE* com=fopen(argv[1],"r");  //com stands for compressed
		if(com==NULL){
			printf("THERE IS NOT THIS FILE!!");
			abort();
		}
		
		//create list
		list* l=createList();
		if(l==NULL){
			printf("There is no space for the list!");
			abort();
		}
		
		/*the following algorithm:
		  in each loop 
		  I use fscanf so as to set values to numberOfChars and character variables (the fscanf seperates the integer and the char of the file)
		  I set in struct(run) the number of characters and the charachter
		  I add run to the list i created before
		  */
		run r;  //create a struct
		int numberOfChars;
		char character;
		while(fscanf(com,"%d%c",&numberOfChars,&character)!=EOF)  //the loop ends when the file ends
		{
			r.count=numberOfChars;
			r.c=character;
			addRunAtTheEnd(l,r);
		}
	
		printRuns(l);
		printf("\n");
		fclose(com);
		
		uncompress(l,argv[2]);
		renewCompressedFile(l,argv[1]);
		
		//printing the size
		FILE* com1=fopen(argv[2],"r");
		if(com1==NULL)
		{
			printf("file not found\n");
			abort();
		}
		fseek(com1,0,SEEK_END);
		long com1Size=ftell(com);
		printf("\n\ncompressed size:%ld\n",com1Size);
		fclose(com1);
		
		FILE* uncom=fopen(argv[3],"r");
		if(uncom==NULL)
		{
			printf("file not found\n");
			abort();
		}
		fseek(uncom,0,SEEK_END);
		long uncomSize=ftell(uncom);
		printf("uncompressed size:%ld",uncomSize);
		fclose(uncom);
	}
	printf("\nTHANK YOU!\n");
	
	

	return 0;
}


//END OF UNCOMPRESS PROGRAMM---
//L---IST PROGRAMM

//this function creates the list we need an sets the head
list* createList(){
	list* l=(list*)malloc(sizeof(list)); //creates space for one list struct-retruns a pointer to list
	if(l==NULL)
	{
		return NULL;
	}
	l->head=NULL; //initiallization of head
	return l;
}

//add runs at the end-----
//for some reason the data insert normally in this function but they aren't saved to the list in the list and the whole programm is saboatged 
void addRunAtTheEnd(list* l,run r)
{
	if(l==NULL){    //checking if there is any list
		printf("There is no list\n");
		abort(); 
	}
	
	node* temp=(node*)malloc(sizeof(node));	//creating space for the new node
	if(temp==NULL){							//checking if there is space for the new node
		printf("Allocation memmory ERROR in addRUN\n");
		abort();
	}

	temp->data=r;
	temp->next=l->head;  //setting next to point to previews node
	l->head=temp;  //setting head to point to last added node

}

//add a new node at list(add a new run) wherever the user wants
void  addRunByChoice(list* l){
	if(l==NULL){
		printf("There is no list\n");
		abort(); 
	}
	run r;
	int choice;
	printf("ADD DATA\n");
	
	printf("Character:");
	scanf(" %c",&r.c);
	printf("Amount:");
	scanf("%d",&r.count);
	printf("\n");
	if(l->head==NULL)
	{
		addRunAtTheEnd(l,r);
	}else{
		printRuns(l);
		printf("I want to add data before number:");
		scanf("%d",&choice);
		
		node* temp=(node*)malloc(sizeof(node));	//creating space for the new node
		if(temp==NULL){
			printf("Allocation memmory ERROR in addRUN\n");
			abort();
		}
		node* curr=l->head;
		node* prev=NULL;
		int i=0;
		while(curr!=NULL)
		{
			i++;
			if(i==choice)
			{
				temp->data=r;
				temp->next=prev->next;
				prev->next=temp;
				printf("addition succed!\n");
				break;
			}
			
			curr=curr->next;
		}
		if(choice>i)
		{
			addRunAtTheEnd(l,r);
		}
	}
	
}


//delete any run the user wants depending on choice
void deleteRunByChoice(list* l){
	
	if(l==NULL)
	{
		printf("There is no List");
		abort();
	}
	
	if(l->head==NULL)
	{
		printf("The list is empty.Nothing to delete!!");
		return;
	}
	
	printf("\nSelect which sequence you would like to delete:\n");
	printRuns(l);
	int choice;
	printf("My choice:");
	scanf("%d",&choice);
	
	node* curr=l->head; //curr points to last added struct(Run)
	node* prev=NULL;  //prev=Null because there isn't previews node at first
	
	//to delete any node i need to make the previews node (of the one i want to delete), to point to another node 
	int i=0;
	while(curr!=NULL)
	{
		i++;
		if(i==choice)  //when the choice found:
		{
			if(prev==NULL)
			{
				l->head=NULL;  //if the list has only one struct(run) then l->head=NULL
			}else{

				prev->next=curr->next;
			}
			destroyNode(curr);
			printf("\nSuccessfully deleted!\n");
			break;
		}
		prev=curr;  //in every loop prev must follow curr 
		curr=curr->next; //in every loop curr must point to next node 
 
	}
}

void destroyNode(node* n){				//free node's space
	if(n==NULL){
		printf("\nThere isn't this node\n");
		abort();
	}
	free(n);   //free memmory completely after delete
}


//change data wherever the user wants to
void changeData(list* l)
{
	if(l==NULL)  //checking if there is any list
	{
		printf("\nList not found!\n");
		abort();
	}
	if(l->head==NULL)  //checking if the list is empty
	{
		printf("\nList is empty you cant change the text.\n");
		return ;
	}
	int choice;
	printf("CHANGE DATA\n");
	printRuns(l);						
	printf("I want to change data in number:");
	scanf("%d",&choice);
	
	node* curr=l->head;  //initialization of curr so as to show at the last added node in the list
	
	int i=0;
	run r;
	while(curr!=NULL)  //the loop will stop when it will run the whole list and there will not there are other nodes
	{
		i++;
		if(i==choice)
		{
			printf("The new amount of this is:");
			scanf("%d",&r.count);
			printf("The new character is:");
			scanf(" %c",&r.c);
			
			curr->data=r;				//here becomes the change according the data user gave
			printf("\nSucces data change\n");
			break;
		}
		curr=curr->next;		//making in each loop the list to show at the next node of the list
	}
	
}

//print all the runs
void printRuns(list* l){
	if(l->head=NULL)
	{
		printf("Empty list.Nothing to print!\n");
		abort();
	}
	int i=1;		//this is used for the message to be more understoodable from the user
	node* curr=l->head;
	while(curr!=NULL){		//the loop will stop when the list end
		printf("\t<%d>  %d%c\n",i,curr->data.count,curr->data.c);
		curr=curr->next;  //increasing every time the curr node by 1
		i++;
	}
}

/*SOURCES OF LIST CODE
	
	the biggest part of the code is based on the lecture "Prog2class 2021 05 21"
	but there are some modifications according to the excersise 	
*/

/*SOURCES OF UNCOMPRESS CODE
	
	the whole code of uncompress.c is based on the lectures,the labs 
	I watched during the semester and the slides that  are uploaded in eclass
	
*/



