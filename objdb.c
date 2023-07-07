#include "objdb.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 512
#define NAMELEN 256

// Helpful Inner Utilities //

void ferror_checker(FILE **fptr,const int LINE){
	if(ferror(*fptr)){
		perror("Error");
		fprintf(stderr,"at line %d\n",LINE);
		clearerr(*fptr);
		exit(-1);
	}
}

// dynamic memory error checker
void dmerror_checker(char **ptr,const int LINE){
	if(*ptr == NULL){
		fprintf(stderr,"Error at line %d\n",LINE);
		free(*ptr);
		*ptr = NULL;
		exit(-1);
	}
}

int db_size_calc(FILE **db_ptr){
	int size = 0;
	int total = (int) sizeof(long); //Include the db id bytes
	
	fseek(*db_ptr,sizeof(long),SEEK_SET); //surpass the db id bytes
	ferror_checker(db_ptr,__LINE__-1);
	fread(&size,sizeof(int),1,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);
	while(!feof(*db_ptr)){
		fseek(*db_ptr,size-sizeof(int),SEEK_CUR);
		ferror_checker(db_ptr,__LINE__-1);
		total += size;
		fread(&size,sizeof(int),1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
	}

	return(total);
}

void find_array_clean(char **array){
    int i,array_size;

	//Array size calc
    for(i=0 ;array[i] != NULL ;i++){
		//empty
	}

	if(i%10){
		array_size = ((i/10) + 1 ) *10;
	}
	else{
		array_size = i;
	}
    //

	//Array destruction
    for(i=0 ;i<array_size ;i++){
        free(array[i]);
    }
    free(array);
	array = NULL;
	//
	
    return;
}

int fopen_err_checker(FILE **fptr,const int LINE){
	if( *fptr == NULL){
		return(0);
	}
	else if(ferror(*fptr)){
		perror("Error");
		fprintf(stderr,"at line %d\n",LINE);
		clearerr(*fptr);
		return(-1);
	}
	return(0);
}

int fclose_err_checker(FILE **fptr,int res,const int LINE){
	if(res){
		perror("fclose error:");
		fprintf(stderr,"at line %d\n",__LINE__);
		clearerr(*fptr);
		*fptr = NULL;
		return(-1);//error found 
	}
	return(0);//error not found
}

int find_obj(FILE **db_ptr ,const char *OBJNAME ,int *size ,int *namelen){
	char *buffer;

	//buffer init
	buffer = malloc(NAMELEN);
	dmerror_checker(&buffer,__LINE__-1);
	//

	//Stream set to first object
	rewind(*db_ptr);
	fseek(*db_ptr,sizeof(long),SEEK_SET);
	ferror_checker(db_ptr,__LINE__-1);
	//

	//Find obj
	fread(size,sizeof(int),1,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);
	while(!feof(*db_ptr)){
		fread(namelen,sizeof(int),1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
		fread(buffer,1,*namelen+1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);

		if(strncmp(OBJNAME,buffer,*namelen+1) == 0){
			free(buffer);
			return(1);//SUCCESS
		}

		fseek(*db_ptr,(*size -(*namelen+1+2*sizeof(int))),SEEK_CUR);
		ferror_checker(db_ptr,__LINE__-1);
		fread(size,sizeof(int),1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
	}
	//

	free(buffer);
	return(0);
}

// Main programm Utilities //

int open_db(FILE **db_ptr,const char *DBNAME){
	int res;
	long dbcode = 0;
	long DBCODE = 2459790149966373410;

	//Closing any open db file
	if(*db_ptr){
		res = fclose(*db_ptr);
		if(fclose_err_checker(db_ptr,res,__LINE__-1)){
			return(-1);
		}
		*db_ptr = NULL;
	}
	//	

	*db_ptr = fopen(DBNAME,"rb+"); //open db file
	if(fopen_err_checker(db_ptr,__LINE__-1))
		return(-1);

	//if file doesn't exist make it
	if(*db_ptr == NULL){
		*db_ptr = fopen(DBNAME,"wb+");
		if(fopen_err_checker(db_ptr,__LINE__-1))
			return(-1);
		fwrite(&(DBCODE),8,1,*db_ptr);
		if(fopen_err_checker(db_ptr,__LINE__-1))
			return(-1);
		return(1);
	}
	//

	//Check if it is db file
	fread(&dbcode,sizeof(long),1,*db_ptr);
	if(dbcode != DBCODE){
		fclose(*db_ptr);
		*db_ptr = NULL;
		return(0);
	}
	//

	return(1); //SUCCESS
}

int import_obj(FILE **db_ptr ,const char *FNAME ,const char *OBJNAME){
	int namelen ,read,size;
	char *buffer;
	FILE *objfile_ptr;

	//Checking that the db is open
	if(*db_ptr == NULL){
		return(-3);
	}
	//

	//Checking that the file exists (and opening it) 
	objfile_ptr = fopen(FNAME,"rb");
	if(objfile_ptr == NULL){
		return(-2);
	}
	//

	if(find_obj(db_ptr,OBJNAME,&size,&namelen)){
		fclose(objfile_ptr);
		objfile_ptr = NULL;
		return(0);
	}

	//Copying objname metadata and objname to database
	namelen = strlen(OBJNAME);
	fseek(*db_ptr,1*sizeof(int),SEEK_END);
	ferror_checker(db_ptr,__LINE__-1);
	
	fwrite(&namelen,sizeof(int),1,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);

	fwrite(OBJNAME,1,namelen+1,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);
	//

	//init buffer
	buffer = malloc(BUFFER_SIZE);
	dmerror_checker(&buffer,__LINE__-1);
	//

	//Copying obj to db
	size = 2*sizeof(int) + namelen + 1;
	do{
		read = fread(buffer,1,BUFFER_SIZE,objfile_ptr);
		ferror_checker(db_ptr,__LINE__-1);//free(buffer); //needed

		fwrite(buffer,1,read,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);

		fflush(*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);

		size +=read;
	}while(!feof(objfile_ptr));
	//

	//writing the obj size metadata
	fseek(*db_ptr,-size,SEEK_CUR);
	ferror_checker(db_ptr,__LINE__-1);

	fwrite(&size,sizeof(int),1,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);//free(buffer); //needed
	//

	free(buffer);
	buffer = NULL;
	fclose(objfile_ptr);
	objfile_ptr = NULL;
	return(1);
}

char** find_similar_obj(FILE **db_ptr ,const char *OBJNAME){
	char ** obj_names;
	char *name_holder;
	unsigned int rows = 10;
	int i,obj_size,namelen;

	//Checking that the db is open
	if(!(*db_ptr) ){
		return(NULL);
	}
	//

	//obj_names array init
	obj_names = (char**) calloc(rows,sizeof(char*));
	//

	//Setting the db_ptr to the start of the first object
	fseek(*db_ptr,sizeof(long),SEEK_SET);
	ferror_checker(db_ptr,__LINE__-1);
	//

	//Return all object names (Special feature)
	if(strcmp(OBJNAME,"*") == 0){
		fread(&obj_size,sizeof(int),1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
		for(i=0 ;!feof(*db_ptr) ;i++){
			fread(&namelen,sizeof(int),1,*db_ptr);
			ferror_checker(db_ptr,__LINE__-1);

			if(i == rows){
				rows += 10;
				obj_names = (char**) realloc(obj_names,rows*sizeof(char*));
			}

			obj_names[i] = malloc(namelen+1);
			fread(obj_names[i],1,namelen+1,*db_ptr);
			ferror_checker(db_ptr,__LINE__-1);
			fseek(*db_ptr,(obj_size -(namelen+1+2*sizeof(int))),SEEK_CUR);
			ferror_checker(db_ptr,__LINE__-1);
			fread(&obj_size,sizeof(int),1,*db_ptr);
			ferror_checker(db_ptr,__LINE__-1);
		}
		return(obj_names);
	}
	//

	//Init name_holder
	name_holder = (char*) malloc(NAMELEN);
	dmerror_checker(&name_holder,__LINE__-1);
	//

	
	//Actual Utility
	fread(&obj_size,sizeof(int),1,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);

	for(i=0 ;!feof(*db_ptr) ;){
		fread(&namelen,sizeof(int),1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);

		fread(name_holder,1,namelen+1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);

		if(strstr(name_holder,OBJNAME)){
			if(i == rows){
				rows += 10;
				obj_names = (char**) realloc(obj_names,rows*sizeof(char*));
			}
			obj_names[i] = malloc(namelen+1);
			strncpy(obj_names[i],name_holder,namelen+1);
			i++;
		}
		fseek(*db_ptr,(obj_size -(namelen+1+2*sizeof(int))),SEEK_CUR);
		ferror_checker(db_ptr,__LINE__-1);
		fread(&obj_size,sizeof(int),1,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
	}
	obj_names[i] = NULL;
	free(name_holder);
	name_holder = NULL;
	return(obj_names);
	//
}

int export_obj(FILE **db_ptr ,const char *OBJNAME ,const char *FNAME){
	FILE *objfile_ptr; 
	int size,namelen,read;
	char *buffer;

	//Checking if the db is not open
	if(*db_ptr == NULL){
		return(-3);
	}
	//

	//Checking if the file that the data will be exported already exists
	objfile_ptr = fopen(FNAME,"rb");
	if(objfile_ptr != NULL){
		return(-2);
	}
	//

	//Check if object not found
	if(!find_obj(db_ptr,OBJNAME,&size,&namelen)){
		return(0);
	}
	//

	//Creating the file that the data will be exported
	objfile_ptr = fopen(FNAME,"wb");
	ferror_checker(db_ptr,__LINE__-1);
	//

	//buffer init
	buffer = malloc(BUFFER_SIZE);
	dmerror_checker(&buffer,__LINE__-1);
	//

	//Copying obj data to the new file (exporting it)
	size = size -(namelen + 1 +2*sizeof(int));
	while(1){
		read = fread(buffer,1,BUFFER_SIZE,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
		if(read < size){
			fwrite(buffer,1,read,objfile_ptr);
			ferror_checker(&objfile_ptr,__LINE__-1);
			
		}else{
			fwrite(buffer,1,size,objfile_ptr);
			ferror_checker(&objfile_ptr,__LINE__-1);
			break;
		}
		
		fflush(objfile_ptr);
		ferror_checker(&objfile_ptr,__LINE__-1);
		size -=read;
	}
	//

	free(buffer);
	fclose(objfile_ptr);
	return(1);
}

int delete_obj(FILE **db_ptr,const char *DBNAME,const char *OBJNAME){
	char *buffer;
	int namelen,size,read,db_size = 0;
	
	if(*db_ptr == NULL){
		return(-3);
	}

	db_size = db_size_calc(db_ptr);

	if(!find_obj(db_ptr,OBJNAME,&size,&namelen)){
		return(-2);
	}

	//buffer init
	buffer = malloc(BUFFER_SIZE);
	dmerror_checker(&buffer,__LINE__-1);
	//
	
	//Ignore the object that will be removed and move the other objects backwards
	fseek(*db_ptr,size-(namelen+1+2*sizeof(int)),SEEK_CUR);
	ferror_checker(db_ptr,__LINE__-1);
	read = fread(buffer,1,BUFFER_SIZE,*db_ptr);
	ferror_checker(db_ptr,__LINE__-1);
	while(read){
		fseek(*db_ptr,-(size+read),SEEK_CUR);
		ferror_checker(db_ptr,__LINE__-1);

		fwrite(buffer,1,read,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
		
		fflush(*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
		
		fseek(*db_ptr,size,SEEK_CUR);
		ferror_checker(db_ptr,__LINE__-1);
		
		read = fread(buffer,1,BUFFER_SIZE,*db_ptr);
		ferror_checker(db_ptr,__LINE__-1);
	}
	//

	db_size =db_size - size; //recalc db size

	fclose(*db_ptr);
	truncate(DBNAME,db_size); //cut db to it's new smaller size
	*db_ptr = fopen(DBNAME,"rb+");

	free(buffer);
	return(1);
}

int close_db(FILE **db_ptr){
	if(*db_ptr == NULL){
		return(0);
	}

	if(fclose(*db_ptr)){
		ferror_checker(db_ptr,__LINE__-1);
	}
	else{
		*db_ptr = NULL;
	}
	return(1);
}
