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
	printf("Enter your file name: ");
	scanf("%s", filename);
	fp = fopen(filename, "rt");
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

void jsonNameList(char * jsonstr, jsmntok_t * t, int tokcount){
	int r, i, j = 1;
	int parentindex = 999;

	//printf("********* Name List *********\n");
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
	for (i = 0; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
			if(t[0].type == JSMN_OBJECT && t[2].type == JSMN_ARRAY){		//data4 에서 사용될 곳
				if (i == 0)
							continue;					// 0인 경우는 뛰어넘 1부터 시작
				if (t[i].type == JSMN_OBJECT) {							//object를 만났을때 parentindex를 업데이트
					parentindex = i;
				}
				if(t[i].parent == parentindex){								//업데이트한 parentindex가 parent값인 토큰만 출력
					printf("[[Name %d]%.*s \n", j, t[i].end-t[i].start, jsonstr + t[i].start);
					j += 1;
				}
	}
	else if(t[0].type == JSMN_OBJECT && t[2].type != JSMN_ARRAY){		//data1, data2에서 사용될 곳
		if (t[i].type == JSMN_OBJECT && t[i].parent == -1)						// 맨 처음 object만
		{
			parentindex = i;																						//parent로 등록
		}
		if (t[i].parent == parentindex){														//parent로 등록한 값을 가지고
			printf("[[Name %d]%.*s \n", j, t[i].end-t[i].start, jsonstr + t[i].start);	//토큰 출력
			j += 1;
		}
	}
	else if(t[0].type == JSMN_ARRAY){		//data3에서 사용될 곳
		if (t[i].type == JSMN_OBJECT )
		{
			parentindex = i;																						//parent로 등록
		}
		if (t[i].parent == parentindex){														//parent로 등록한 값을 가지고
			printf("[[Name %d]%.*s \n", j, t[i].end-t[i].start, jsonstr + t[i].start);	//토큰 출력
			j += 1;
		}
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
		int parentindex = 999;


		if (tokcount < 0){
			printf("Failed to parse JSON: %d\n", tokcount);
		}
		for (i = 0; i < tokcount; i++) {
				/* We may use strndup() to fetch string value */
				if(t[0].type == JSMN_OBJECT && t[2].type == JSMN_ARRAY){//data3에서 사용될 곳
					if (i == 0)
								continue;
					if (t[i].type == JSMN_OBJECT) {
						parentindex = i;
					}
					if(t[i].parent == parentindex){
						nameTokIndex[j] = i;
						j += 1;
					}
		}
		else if(t[0].type == JSMN_OBJECT && t[2].type != JSMN_ARRAY){										//data1, data2에서 사용될 곳
			if (t[i].type == JSMN_OBJECT && t[i].parent == -1)
			{
				parentindex = i;
			}
			if (t[i].parent == parentindex){
				nameTokIndex[j] = i;
				j += 1;
			}
		}
		else if(t[0].type == JSMN_ARRAY){		//data3에서 사용될 곳
			if (t[i].type == JSMN_OBJECT )
			{
				parentindex = i;																						//parent로 등록
			}
			if (t[i].parent == parentindex){														//parent로 등록한 값을 가지고
				nameTokIndex[j] = i;
				j += 1;
			}
		}

	}
}

void printNameList(char * jsonstr, jsmntok_t * t, int * nameTokIndex){
	int i, j = 1;
	//for (i = 0; i < 100; i++){
	//	printf("%d --\n", nameTokIndex[i]);
	//}
	printf("********* Name List2 *********\n");
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
	int parentindex = 999;
	//for (i = 0; i < 100; i++){
	//	printf("%d --\n", nameTokIndex[i]);
	//}
	printf("********* Object List *********\n");


	for (i = 0; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */

			if (t[0].type == JSMN_OBJECT && t[2].type == JSMN_ARRAY)	//data4에서만 사용될 곳
			{
				if(i == 0)			//0인 경우는 건너뛰고 1부터 시작할 것임
					continue;

				if (t[i].type == JSMN_OBJECT){							//object이면 그 곳의 첫번째 value만 출력
				printf("[Name %d]%.*s \n", j, t[i+2].end-t[i+2].start, jsonstr + t[i+2].start);
						objectIndex[j] = i;										//objectIndex 배열에도 값 저장
				j += 1;
				}
			}
			else if (t[0].type == JSMN_ARRAY){					//data3에서만 사용될 곳
				if (t[i].type == JSMN_OBJECT){							//object이면 그 곳의 첫번째 value만 출력
				printf("[Name %d]%.*s \n", j, t[i+2].end-t[i+2].start, jsonstr + t[i+2].start);
						objectIndex[j] = i;										//objectIndex 배열에도 값 저장
				j += 1;
				}
			}
			else																									// data1, data2에서만 사용될 곳
			{
				if (t[i].type == JSMN_OBJECT && t[i].parent == -1){	    //첫번 째 object만 고려
				printf("[Name %d]%.*s \n", j, t[i+2].end-t[i+2].start,jsonstr + t[i+2].start);
						objectIndex[j] = i;												//objectIndex 배열에도 값 저장
				j += 1;
				}
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
//printf("]]]]]]]%s", json);
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
	//printNameList(JSON, t, nameTokIndex);
//	printf("\n");
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
	return EXIT_SUCCESS;
}
