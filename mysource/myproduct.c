#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../myproduct.h"

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
	fp = fopen("myproduct.json", "rt");
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

void printProduct(product_t * p[], int pcount){
	int i;

	printf("************************************************************************************************\n");
	printf("번호\t\t제조사\t\t\t제품명\t\t가격\t\t갯수\t\t총가격\n");
	printf("************************************************************************************************\n");
	 for (i = 0; i < pcount; i++) {
			/* We may use strndup() to fetch string value */
	//printf("[%d]", nameTokInfo[i].tokindex);
	printf("%d\t\t%s\t\t%s\t\t%d\t\t%d\t\t%d\t\t"
		,i+1, p[i]->name, p[i]->company, p[i]->price, p[i]->count, (p[i]->price)*(p[i]->count));
	printf("\n");
}

}

int makeProduct(const char *json, jsmntok_t *t, int tokcount, product_t *p[]){
	int i, k = -1;
	char word[20];
	printf("히히\n");

		// printf("%d\n", tokcount);

	for (i = 1; i < tokcount; i++) {
			/* We may use strndup() to fetch string value */
				if (t[i].type == JSMN_OBJECT) {
					k++;
				}
				if (jsoneq(json, &t[i], "company") == 0) {
					/* We may use strndup() to fetch string value */
					sprintf(p[k]->company, "%.*s", t[i+1].end-t[i+1].start,
							json + t[i+1].start);
					// printf("%s\n", p[k]->company);
				} else if (jsoneq(json, &t[i], "name") == 0) {
					/* We may additionally check if the value is either "true" or "false" */
					sprintf(p[k]->name, "%.*s", t[i+1].end-t[i+1].start,
							json + t[i+1].start);
				} else if (jsoneq(json, &t[i], "price") == 0) {
					/* We may want to do strtol() here to get numeric value */
					sprintf(word, "%.*s", t[i+1].end-t[i+1].start,
							json + t[i+1].start);
					p[k]->price = atoi(word);
				} else if (jsoneq(json, &t[i], "count") == 0) {
					sprintf(word, "%.*s", t[i+1].end-t[i+1].start,
							json + t[i+1].start);
					p[k]->count = atoi(word);
				}
			}
k++;
return k;

}


int main() {
	int i;
	int r, k;
	int nameTokIndex[100] = {0};
	int objectIndex[20] = {0};
	jsmn_parser p;
	char *JSON;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	product_t *snacklist[10];

	for(i = 0; i < 10; i++){
		snacklist[i] = (product_t *)malloc(sizeof(product_t));
	}

	JSON = readJSONFile();
	//printf("%s", JSON);

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON, strlen(JSON), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
	 	printf("Failed to parse JSON: %d\n", r);
	 	return 1;
	}
//	printf("안녕\n", k);
	k = makeProduct(JSON, t, r, snacklist);
	printProduct(snacklist, k);
	// printf("안녕\n");
	// printf("%d\n", k);
	// jsonNameList(JSON, t, r, &nameTokInfo);
	printf("\n");

	/* Assume the top-level element is an object */
	return EXIT_SUCCESS;
}
