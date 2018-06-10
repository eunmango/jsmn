#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
void printAllObject(char * jsonstr, jsmntok_t * t, int tokcount);
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
	char filename[20];
	fp = fopen("data4.json", "rt");
	if (fp == NULL){
		printf("file open failed\n");
		exit(0);
	}
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


int Call(char * jsonstr, jsmntok_t * t, int tokcount, NameTokInfo *nameTokInfo){

	int r, i, j = 1, k = 0;
	int parentindex = 999;
	int tokenindex;
	char *json;
	char str[500], word[10];
	int priceint, countint;
	jsmn_parser p;
	jsmntok_t t1[128];



	// printf("%d ==", nameTokInfo->tokindex);
	sprintf(str, "%.*s", t[nameTokInfo->tokindex].end - t[nameTokInfo->tokindex].start, jsonstr + t[nameTokInfo->tokindex].start);
	// printf("%s", str);

	//printf("%.*s",t[nameTokInfo->tokindex].end - t[nameTokInfo->tokindex].start, jstr + t[nameTokInfo->tokindex].start);
	jsmn_init(&p);
	r = jsmn_parse(&p, str, strlen(str), t1, sizeof(t1)/sizeof(t1[0]));
	//printf("%d", r);
	j = 1;
	for(i = 0; i < r; i++){
		 // printf("// %.*s //\t", t1[i].end-t1[i].start, str + t1[i].start);

		if (jsoneq(str, &t1[i], "company") == 0) {
			/* We may use strndup() to fetch string value */
			printf("%6.*s\t\t", t1[i+1].end-t1[i+1].start,
					str + t1[i+1].start);
		} else if (jsoneq(str, &t1[i], "name") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("%6.*s\t\t", t1[i+1].end-t1[i+1].start,
					str + t1[i+1].start);
		} else if (jsoneq(str, &t1[i], "price") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("%6.*s\t\t", t1[i+1].end-t1[i+1].start,
					str + t1[i+1].start);
					sprintf(word, "%.*s", t1[i+1].end-t1[i+1].start, str + t1[i+1].start);
					priceint = atoi(word);
		} else if (jsoneq(str, &t1[i], "count") == 0) {
			printf("%6.*s \t\t", t1[i+1].end-t1[i+1].start,
					str + t1[i+1].start);
			sprintf(word, "%.*s", t1[i+1].end-t1[i+1].start, str + t1[i+1].start);
			countint = atoi(word);
			printf("%d", priceint * countint);

				}
			}
}



void jsonNameList(char * jsonstr, jsmntok_t * t, int tokcount, NameTokInfo *nameTokInfo){
	int r, i, j = 1, k = 0;
	int parentindex = 999;
	char *json;
	char str[500];

	nameTokInfo = (NameTokInfo*)malloc(sizeof(NameTokInfo)*20);

	// for(i = 0; i < 20; i++){
	// 	nameTokInfo[i] = malloc(sizeof(NameTokInfo));
	// 	//memset(nameTokInfo[i], 0, sizeof(NameTokInfo));
	// }



	//printf("********* Name List *********\n");
	// printf("ddd[%d]", nameTokInfo[0].tokindex);
	if (tokcount < 0){
		printf("Failed to parse JSON: %d\n", tokcount);
	}
	 for (i = 0; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
				printf("[%d][%d][%d][Name %d]%.*s \n", t[i].size, t[i].type, t[i].parent, j, t[i].end-t[i].start,
					jsonstr + t[i].start);
			j += 1;
		}

j = 1;
	printf("\n\n********* Name List1 *********\n");
	if (tokcount < 0){
		printf("Failed to parse JSON: %d\n", tokcount);
	}
	for (i = 1; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
				if (t[i].type == JSMN_OBJECT) {
					nameTokInfo[k].objectindex = k;
					nameTokInfo[k].tokindex = i;
					printf("[Name %d]%.*s \n", j, t[nameTokInfo[k].tokindex].end-t[nameTokInfo[k].tokindex].start,
						jsonstr + t[nameTokInfo[k].tokindex].start);
					//	printf("[%d][%d]", nameTokInfo[k].tokindex, i);
					k++;
				}
			}

			printf("****************************************************************************************\n");
			printf("번호\t\t제조사\t\t제품명\t\t가격\t\t갯수\t\t총가격\n");
			printf("****************************************************************************************\n");
			 for (i = 0; i < k; i++) {
			 		/* We may use strndup() to fetch string value */
			//printf("[%d]", nameTokInfo[i].tokindex);
			printf("%d\t\t", i+1);
			Call(jsonstr, t, tokcount, &nameTokInfo[i]);
			printf("\n");

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
int main() {
	int i;
	int r;
	int nameTokIndex[100] = {0};
	int objectIndex[20] = {0};
	jsmn_parser p;
	char *JSON;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	NameTokInfo nameTokInfo[20];

	JSON = readJSONFile();
	//printf("%s", JSON);



	jsmn_init(&p);
	r = jsmn_parse(&p, JSON, strlen(JSON), t, sizeof(t)/sizeof(t[0]));

	jsonNameList(JSON, t, r, &nameTokInfo);
	printf("\n");

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	return EXIT_SUCCESS;
}
