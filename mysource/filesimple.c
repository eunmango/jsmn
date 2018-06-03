#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char * readJSONFile()	{
	FILE * fp;
	fp = fopen("data.json", "rt");
	int count;
	char str[256], *json;
	json = (char *)malloc(sizeof(str));

	while(1){
		fgets(str, sizeof(str) ,fp);
		if(feof(fp))
			break;
		count += strlen(str);
		realloc(json, count+1);
		strcat(json, str);
	}

	fclose(fp);
	return json;
}

void jsonNameList(char * jsonstr, jsmntok_t * t, int tokcount){
	int r, i, j = 1;

	//printf("********* Name List *********\n");
	if (tokcount < 0){
		printf("Failed to parse JSON: %d\n", tokcount);
		return 1;
	}
	if (tokcount < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	 for (i = 0; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
				printf("[%d][%d][%d][Name %d]%.*s \n", t[i].size, t[i].type, t[i].parent, j, t[i].end-t[i].start,
					jsonstr + t[i].start);
			j += 1;
		}

j = 1;
	printf("\n\n********* Name List *********\n");
	if (tokcount < 0){
		printf("Failed to parse JSON: %d\n", tokcount);
		return 1;
	}
	if (tokcount < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	for (i = 1; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
			if(t[i].parent == 0){
					printf("[[Name %d]%.*s \n", j, t[i].end-t[i].start,
					jsonstr + t[i].start);
			j += 1;
		}
	}
}



/*
if (t[i].size == 1)
{
	printf("[%d][Name %d]%.*s \n", t[i].size, j, t[i].end-t[i].start,
		jsonstr + t[i].start);
j += 1;
}
*/
void jsonNameList2(char * jsonstr, jsmntok_t * t, int tokcount, int * nameTokIndex){

		int r, i, j = 1;
		if (tokcount < 0){
			printf("Failed to parse JSON: %d\n", tokcount);
			return 1;
		}
		if (tokcount < 1 || t[0].type != JSMN_OBJECT) {
			printf("Object expected\n");
			return 1;
		}
		for (i = 1; i < tokcount; i++) {
				/* We may use strndup() to fetch string value */
				if (t[i].size == 1)
				{
					nameTokIndex[j] = i;
					j++;
			}
		}
}

void printNameList(char * jsonstr, jsmntok_t * t, int * nameTokIndex){
	int i, j = 1;
	//for (i = 0; i < 100; i++){
	//	printf("%d --\n", nameTokIndex[i]);
	//}
	printf("********* Name List *********\n");
	for (i = 0; i < 100; i++){

	if (nameTokIndex[i] != 0){
	printf("[Name %d]%.*s \n", j, t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,
			jsonstr + t[nameTokIndex[i]].start);
			j++;
		}
	}
}

void selectNameList(char * jsonstr, jsmntok_t * t, int * nameTokIndex){
	int no, i;

//	for (i = 0; i < 20; i++){
//		printf("%d", nameTokIndex[i]);
//}

	while(1){
	printf("Select Name's no(exit 0): " );
	scanf("%d", &no);
	if (no == 0)
		break;

		printf("[Name %d]%.*s \n", no, t[nameTokIndex[no]].end-t[nameTokIndex[no]].start,
				jsonstr + t[nameTokIndex[no]].start);
	 printf("%.*s \n", t[nameTokIndex[no]+1].end-t[nameTokIndex[no]+1].start,
				jsonstr + t[nameTokIndex[no]+1].start);
			}

}

void printObject(char * jsonstr, jsmntok_t * t, int tokcount, int * objectIndex){
	int i, j = 1;
	//for (i = 0; i < 100; i++){
	//	printf("%d --\n", nameTokIndex[i]);
	//}
	printf("********* Object List *********\n");


	for (i = 0; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
			if (t[i].type == JSMN_OBJECT && t[i].parent == -1)
			{
				printf("[Name %d]%.*s \n", j, t[i+2].end-t[i+2].start,
					jsonstr + t[i+2].start);
					objectIndex[j] = i;
			j += 1;
		}
	}
	objectIndex[j] = 999;
}

void selectObject(char * jsonstr, jsmntok_t * t, int * objectIndex){
	int no, i, r, j = 1;
	char str[256], *json;
	jsmn_parser p;
	jsmntok_t t1[128];

	json = (char *)malloc(sizeof(str));

	while(1){
	//p == NULL;
	printf("Select object's no(exit 0): " );
	scanf("%d", &no);
	if (no == 0)
		break;

	sprintf(json, "%.*s",t[objectIndex[no]].end - t[objectIndex[no]].start, jsonstr + t[objectIndex[no]].start);
  //printf("%.*s",t[objectIndex[no]].end - t[objectIndex[no]].start, jsonstr + t[objectIndex[no]].start);

	jsmn_init(&p);
	r = jsmn_parse(&p, json, strlen(json), t1, sizeof(t1)/sizeof(t1[0]));

	//jsonNameList(json, t1, r);
	printAllObject(json, t1, r);


	/*for (i = 0; i < r; i++) {
		if (t1[i].size == 1)
		//if (t[i] == '\0') break; //0으로 초기화 되어있고 0은 토큰의 특성상 0을가진 토큰은 없어야됨
		printf("[%.*s] %.*s\n", t1[i].end	- t1[i].start, jsonstr + t1[i].start, t1[i + 1].end - t1[i + 1].start, jsonstr + t1[i + 1].start);
	}*/

	}
}

void printAllObject(char * jsonstr, jsmntok_t * t, int tokcount){
	int i;

		printf("***** Object List *****\n");
		for (i = 1; i < tokcount; i++) {
				/* We may use strndup() to fetch string value */
				if (t[i].size == 1)
				{
					printf("[%.*s] %.*s\n", t[i].end-t[i].start,
						jsonstr + t[i].start, t[i+1].end-t[i+1].start,
							jsonstr + t[i+1].start);
			}
		}
		printf("\n");


}


int main() {
	int i;
	int r;
	int nameTokIndex[100] = {0};
	int objectIndex[20] = {0};
	jsmn_parser p;
	char *JSON;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	JSON = readJSONFile();
	//printf("%s", JSON);



	jsmn_init(&p);
	r = jsmn_parse(&p, JSON, strlen(JSON), t, sizeof(t)/sizeof(t[0]));

	jsonNameList(JSON, t, r);
	printf("\n");
	jsonNameList2(JSON, t, r, nameTokIndex);
	printNameList(JSON, t, nameTokIndex);
	printf("\n");
	selectNameList(JSON, t, nameTokIndex);
	printf("\n");
	printObject(JSON, t, r, objectIndex);
	printf("\n");
	selectObject(JSON, t, objectIndex);

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	return EXIT_SUCCESS;
}
