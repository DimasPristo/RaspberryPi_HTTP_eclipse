/*
 * main.c
 *
 *  Created on: 26 Agt 2021
 *      Author: pi
 */

#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>

#define FTPBODY "ftp-list.json"
#define SERVER "https://skin-peoplecontrol.cubeacon.com/api/dispenser/wjNPpe4kxq/1625553967"

void read_server(void){
	CURL *curl;
	CURLcode res;
	FILE *ftpfile;

	/* local file name to store the file as */
	ftpfile = fopen(FTPBODY, "wb"); /* b is binary, needed on win32 */

	curl = curl_easy_init();
	if(curl) {
	/* Get a file listing from sunet */
	curl_easy_setopt(curl, CURLOPT_URL, SERVER);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, ftpfile);

	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",
			  curl_easy_strerror(res));

	/* always cleanup */
	curl_easy_cleanup(curl);
	}

	fclose(ftpfile); /* close the local file */
}

void read_response(void){
	FILE *ftpfile;
	char buffer[1024];

	struct json_object *parsed_json;
	struct json_object *success;
	struct json_object *qr;
	struct json_object *rfid;

	ftpfile = fopen(FTPBODY, "r");
	fread(buffer, 1024, 1, ftpfile);
	fclose(ftpfile);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json,"success", &success);
	json_object_object_get_ex(parsed_json,"qr", &qr);
	json_object_object_get_ex(parsed_json,"rfid", &rfid);

	printf("Success : %d \n",json_object_get_int(success));
	printf("QR : %s \n",json_object_get_string(qr));
	printf("RFID : %s \n",json_object_get_string(rfid));

}

int main(void)
{
	printf("GET SERVER\n");
	read_server();
	printf("READ FILE\n");
	read_response();
    return 0;
}
