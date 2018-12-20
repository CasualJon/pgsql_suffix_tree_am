#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define NUMOFDATA 70001 //NUMOFDATA = Number of tuples in file + 1

typedef struct tuple {
    int id;
    char group[64];
    char val[64];
    struct tuple *nextTuple;
} tuple;
// Compare function from a stack overflow  user, see paper for citation
bool Compare(const char *value, const char *input, int v, int i);
// Separate search functions and wrappers for Compare, strstr, and strcmp
double search(tuple* tup, char* input);
double searchWrapper(tuple* tup, char* input);
double searchStrstr(tuple* tup, char* input);
double searchWrapperStrstr(tuple* tup, char* input);
double searchStrcmp(tuple* tup, char* input);
double searchWrapperStrcmp(tuple* tup, char* input);

int main(int argc, char *argv[]){
    tuple *data_array = malloc(NUMOFDATA*sizeof(tuple));
    int data_count = 0;
    char line[128];
    FILE *fp;
    fp=fopen("SynthData70K.txt", "r");  //Define input file here!
    printf("Total Tuples:\n");
    while(fgets(line, sizeof(line), fp)){
        int idtmp;
        char grouptmp[64];
	char valtmp[64];
        if(sscanf(line, "%d %s %s", &idtmp, grouptmp, valtmp)==3){
            data_array[data_count].id = idtmp;
            if (data_count > 0) {
                data_array[data_count-1].nextTuple=&data_array[data_count];
            }
            strcpy(data_array[data_count].group,grouptmp);
	    strcpy(data_array[data_count].val,valtmp);
            //printf("%d, \"%s\", \"%s\"\n",data_array[data_count].id,data_array[data_count].group,data_array[data_count].val);
            if(++data_count == NUMOFDATA)break;
        }
    }
    fclose(fp);
    // Here are all of our test cases
    printf("wn: %f\n",searchWrapper(&data_array[0], "%n"));
    printf("wo: %f\n",searchWrapper(&data_array[0], "%o"));
    printf("we: %f\n",searchWrapper(&data_array[0], "%e"));
    printf("wone: %f\n",searchWrapper(&data_array[0], "%one"));
    printf("wthree: %f\n",searchWrapper(&data_array[0], "%three"));
    printf("wfour: %f\n",searchWrapper(&data_array[0], "%four"));
    printf("woneone: %f\n",searchWrapper(&data_array[0], "%oneone"));
    printf("woneoneone: %f\n",searchWrapper(&data_array[0], "%oneoneone"));
    printf("wonew: %f\n",searchWrapper(&data_array[0], "%one%"));
    printf("woneonew: %f\n", searchWrapper(&data_array[0], "%oneone%"));
    printf("woneoneonew: %f\n", searchWrapper(&data_array[0], "%oneoneone%"));
    printf("fw: %f\n",searchWrapper(&data_array[0], "f%"));
    printf("ow: %f\n",searchWrapper(&data_array[0], "o%"));
    printf("oneonew: %f\n",searchWrapper(&data_array[0], "oneone%"));
    printf("oneoneonew: %f\n",searchWrapper(&data_array[0], "oneoneone%"));
    printf("one: %f\n",searchWrapper(&data_array[0], "one"));
    printf("oneone: %f\n",searchWrapper(&data_array[0], "oneone"));
    printf("oneoneone: %f\n",searchWrapper(&data_array[0], "oneoneone"));
    printf("strstr wonew: %f\n",searchWrapperStrstr(&data_array[0], "one"));
    printf("strstr woneonew: %f\n", searchWrapperStrstr(&data_array[0], "oneone"));
    printf("strstr woneoneonew: %f\n", searchWrapperStrstr(&data_array[0], "oneoneone"));
    printf("strcmp oneoneone: %f\n",searchWrapperStrcmp(&data_array[0], "oneoneone"));
    printf("<empty>: %f\n",searchWrapper(&data_array[0], ""));

    return 0;
}
double searchWrapper(tuple *tup, char *input) {
    int j;
    int x = 5;
    double tot;
    for (j=0; j < x; j++) {
        tot=tot+search(tup, input);
}
    //printf("%f\n",tot);
    return tot/x;
}
double searchWrapperStrstr(tuple *tup, char *input) {
    int j;
    int x = 5;
    double tot;
    for (j=0; j < x; j++) {
        tot=tot+searchStrstr(tup, input);
}
    //printf("%f\n",tot);
    return tot/x;
}
double searchWrapperStrcmp(tuple *tup, char *input) {
    int j;
    int x = 5;
    double tot;
    for (j=0; j < x; j++) {
        tot=tot+searchStrcmp(tup, input);
}
    //printf("%f\n",tot);
    return tot/x;
}

double search(tuple *tup, char *input) {
   // printf("Returned Tuples:\n");
	int total=0;
        clock_t startTime = clock();
	while(tup != NULL) {
           if (Compare(tup->val,input,0,0)){
	        total++;
	 	//printf("%d, \"%s\", \"%s\"\n",tup->id,tup->group,tup->val);
	    }
            tup=tup->nextTuple;
	}
        //printf("Search Input: %s\n", input);
        clock_t endTime = clock();
        double totTime =  ((double)(endTime - startTime) / CLOCKS_PER_SEC);
        printf("%d %f\n", total,totTime);
    return totTime;
}

double searchStrstr(tuple *tup, char *input) {
   // printf("Returned Tuples:\n");
        int total=0;
        clock_t startTime = clock();
        while(tup != NULL) {
           if (strstr(tup->val,input) != NULL) {
                total++;
                //printf("%d, \"%s\", \"%s\"\n",tup->id,tup->group,tup->val);
            }
            tup=tup->nextTuple;
        }
        //printf("Search Input: %s\n", input);
        clock_t endTime = clock();
        double totTime =  ((double)(endTime - startTime) / CLOCKS_PER_SEC);
        printf("%d %f\n", total,totTime);
    return totTime;
}

double searchStrcmp(tuple *tup, char *input) {
  // printf("Returned Tuples:\n");
        int total=0;
        clock_t startTime = clock();
        while(tup != NULL) {
           if (strcmp(tup->val,input) == 0 ) {
                total++;
                //printf("%d, \"%s\", \"%s\"\n",tup->id,tup->group,tup->val);
            }
            tup=tup->nextTuple;
        }
        //printf("Search Input: %s\n", input);
        clock_t endTime = clock();
        double totTime =  ((double)(endTime - startTime) / CLOCKS_PER_SEC);
        printf("%d %f\n", total,totTime);
    return totTime;
}

bool Compare(const char *value, const char *input, int v, int i) {
  if (input[i] == '\0') {
    return value[v] == '\0';
  } else if (input[i] == '%') {
    for (; value[v] != '\0'; v++) {
      if (Compare(value, input, v, i+1))
        return true;
    }
    return Compare(value, input, v, i+1);
  } else if (input[i] != '_' && input[i] != value[v]) {
    return false;
  }  else {
    return Compare(value, input, v+1, i+1);
  }
}
