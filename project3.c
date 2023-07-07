#include "objdb.h"
#include <stdio.h>

int main(int argc ,char*argv[]){
	FILE *db_ptr = NULL;
	char dbname[256] , fname[256], objname[256];
	char choice ,**find_array;
	int res,i;

	while(1){
		
		scanf(" %c",&choice);
		switch (choice)
		{
		case 'o':
			scanf(" %s",dbname);
			res = open_db(&db_ptr,dbname);
			if(res == 1){
				//SUCCESS//
			}
			else if(res == 0){
				printf("\nInvalid db file %s.\n",dbname);
			}
			else{// res == -1
				printf("\nError opening %s.\n",dbname);
			}
			break;
        case 'i':
			scanf(" %s %s",fname,objname);
			res = import_obj(&db_ptr,fname,objname);
			if(res == 1){
				//SUCCESS
			}
			else if(res == 0){
				printf("\nObject %s already in db.\n",objname);
			}
			else if(res == -2){
				printf("\nFile %s not found.\n",fname);
			}
			else{// res == -3
				printf("\nNo open db file.\n");
			}
			break;
		case 'f':
			scanf(" %s",objname);
			find_array = find_similar_obj(&db_ptr,objname);
			if(find_array){
				printf("\n##\n");
				for(i=0 ;find_array[i] ;i++){
					printf("%s\n",find_array[i]);	
				}
				find_array_clean(find_array);
			}
			else{
				printf("\nNo open db file.\n");
			}
			break;
		case 'e':
			scanf(" %s %s",objname,fname);
			res = export_obj(&db_ptr,objname,fname);
			if(res == 1){
				//SUCCESS
			}
			else if(res == 0){
				printf("\nObject %s not in db.\n",objname);
			}
			else if(res == -2){
				printf("\nCannot open file %s.\n",fname);
			}
			else{//res == -3
				printf("\nNo open db file.\n");
			}
			break;
		case'd':
			scanf(" %s",objname);
			res = delete_obj(&db_ptr,dbname,objname);
			if(res == 1){
				//SUCCESS
			}
			else if(res == -2){
				printf("\nObject %s not in db.\n",objname);
			}
			else{//res == -3
				printf("\nNo open db file.\n");
			}
			break;
		case'c':
			res = close_db(&db_ptr);
			if(res == 1){
				//SUCCESS
			}
			else{//res == 0
				printf("\nNo open db file.\n");
			}
			break;
		case'q':
			close_db(&db_ptr);
			return(0);		
		}


    }

	return(0);
}
