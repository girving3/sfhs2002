#include "bake.h"

bool check_build_status(char *target, char * dependency);
void action_building(char *action);
int file_mod(char *filename);
bool is_url(char *line);
//code for breaking up dependencies (tested)
	/*	char *med;  
		med=strtok(deps, " "); //set whatever string u want
		//loop that puts each individual dependency into pointer vector
		while(med!=NULL) {
			dep_words=realloc(dep_words, (dep+1) *sizeof(dep_words[0]);
			dep_wrods[dep]=strdup(med);
			++dep;

		//	printf("%s\n", med); //prints out the the current word its pointing too
		//	do something with curr word here or just put into a pointer vector
			med=strtok(NULL, " "); //sets the pointer to the next word
			} */



//code for checking the modifcation date of target and dependencies
//
void build(LISTITEM *list) {
	//loop for traversing through list of structures that containt target and assoc. deps & actions
	LISTITEM *p=list;
	while(p!=NULL) {
	char * target=p->target;
	char * dependency=p->dependencies;
	char * action=p->action;

	
	if(check_build_status(target, dependency)) {
		continue;
	} else {
	action_building(action);
	}
			
	

	//end of stuff cont loop to next target
	p=p->next;
	}
}

bool check_build_status(char *target, char * dependency) {
//pointer to target and pointer to dependency line passed in as parameters (tested)
	
	//break up into pointer vector
	char ** dep_words=NULL;
	int dep=0;

	char *med;  
		med=strtok(dependency, " "); //set whatever string u want /what is deps? meant 
		//loop that puts each individual dependency into pointer vector
		while(med!=NULL) {
			dep_words=realloc(dep_words, (dep+1) *sizeof(dep_words[0]));
			dep_words[dep]=strdup(med);
			++dep;

			med=strtok(NULL, " "); //sets the pointer to the next word
			}


int targ_mod=file_mod(target);

//checks the modification of targets dependencies against its own
//checks if the dependency exists   <----
//classifies it
for(int i=0; i<dep; i++) {
	int comp;
	LISTITEM* item = find_item(list,dep_words[i]);
	if(item == NULL) { //check if the dependency is a target and if is then check build
	build(item);
	comp=file_mod(dep_words[i]);

	} else if(is_url(dep_words[i])) {
	//curl thingo for mod date then compare to targ
	should_rebuild_url(dep_words[i]); //returns a string with the date
	//******TO DO: convert the string with the date e.g Fri ... GMT to int to compare
	comp = 0;
	} else {
	comp=file_mod(dep_words[i]);
	}
	if(targ_mod<comp) { //check if int value of target is less (=less recently modified)
		return false;
		break;
	}
}
return true;
}



//fork and wait while it completes and then return the 
	//rebuild target passed in 
	//pass action lines to shell 
	//need find_item function from other file

void action_building(char *action) {
//code for breaking up action lines based upon the "|"
//int length=strlen(action);
char ** actions=NULL;
int act=0;

char *med;
med=strtok(action, "|");

while(med!=NULL) {
	actions=realloc(actions, (act+1) *sizeof(actions[0]));
	actions[act]=strdup(med);
	++act;

	med=strtok(NULL, "|");
}

for(int i=0; i<act; i++) {
	rebuild(actions[i]);
}


}

//void(char [] action);



//code for returning file modification time as an integer (where the larger the int the more recent the mod time as its counted from epoch 1970 time) from lec 17ish (tested but only with full pathnames?)
char *progname;

int file_mod(char *filename) { //just need to pass in unspecified target/dependency

	struct stat stat_buffer;

	if(stat(filename, &stat_buffer) != 0) { //can we stat the files attributes
		perror(progname);
		exit(EXIT_FAILURE);
	}
	else if( S_ISREG( stat_buffer.st_mode) ) {
		printf("file was modified on %i\n", (int)stat_buffer.st_mtime);
		printf("which was %s", ctime(&stat_buffer.st_mtime));
	//	int time=(int)stat_buffer.st_mtime;
		return  (int)stat_buffer.st_mtime;

	}
	return 0;
}

int url_modification(char *url); // function to get the modification date of the url
//function for determinining if a dependency is a url

bool is_url(char *line) { //untested
	//int length=strlen(line);

	char hs[]="https://";
	int length1=strlen(hs);

	char f[]="file://";
	int length2=strlen(f);

	char h[]="http://";
	int length3=strlen(h);


	bool first=true; 
	bool second=true;
	bool third=true;

	for(int i=0; i<length1; i++){
		if(line[i]!=hs[i]) {
			first=false;
			break;
		}
	}

		for(int i=0; i<length2; i++){
		if(line[i]!=f[i]) {
			second=false;
			break;
		}
	}
	
		for(int i=0; i<length3; i++){
		if(line[i]!=h[i]) {
			third=false;
			break;
		}
	}
	
	return first || second || third;
}

int main(int argc, char* argv[]) {
	//pass in file as argv 
	//store in struct using store struct func - does it need to return the list maybe?
	//then pass that struct into build 
	 if(argc > 1) {
        FILE *fp        = fopen(argv[1], "r");

        if(fp == NULL) {
            perror(argv[1]);
            return 1;
        }
       /*while(!feof(fp)) {
            char *line = nextline(fp);  // HANDLES CONTINUATION LINES

            if(line) {
                printf("%8s()\t%s\n", __func__, line);
                free(line);
            }
       }*/
	LISTITEM * list=store_target(fp);
	build(list);


        
        fclose(fp);                     // WE OPENED IT, SO WE CLOSE IT
    }
    return 0;

}


		


