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


int main() {
	int i;
	int r;
	jsmn_parser p;
	char *JSON;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	JSON = readJSONFile();
	//printf("%s", JSON);




	jsmn_init(&p);
	r = jsmn_parse(&p, JSON, strlen(JSON), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON, &t[i], "name") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- Name: %.*s\n", t[i+1].end-t[i+1].start,
					JSON + t[i+1].start);
			i++;
		} else if (jsoneq(JSON, &t[i], "keywords") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Keywords: %.*s\n", t[i+1].end-t[i+1].start,
					JSON + t[i+1].start);
			i++;
		} else if (jsoneq(JSON, &t[i], "description") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON + t[i+1].start);
			i++;
		} else if (jsoneq(JSON, &t[i], "examples") == 0) {
			int j;
			printf("- Examples:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("");
		}
	}


	return EXIT_SUCCESS;
}
