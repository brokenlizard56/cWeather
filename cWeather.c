#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <json/json.h>
void parseJSON(json_object * json)
{ 
	json_object *item = json_object_object_get(json_object_object_get(json_object_object_get(json_object_object_get(json, "query"), "results"), "channel"), "item");
	json_object *current = json_object_object_get(item, "condition");
	json_object *forecast = json_object_object_get(item, "forecast");
	json_object *day;
	int index;
	printf("%s\n", json_object_get_string(json_object_object_get(item, "title")));
	printf("%s\n", json_object_get_string(json_object_object_get(current, "date")));
	//Current Weather Printing
	printf("Temperature: %d°F\n", json_object_get_int(json_object_object_get(current, "temp")));
	printf("Current Condition: %s\n", json_object_get_string(json_object_object_get(current, "text")));
	printf("Forecast:\n");
	for(index = 0; index < json_object_array_length(forecast); index++)
	{
		day = json_object_array_get_idx(forecast, index);
		printf("    %s,", json_object_get_string(json_object_object_get(day, "day"))); 
		printf(" %s\n", json_object_get_string(json_object_object_get(day, "date"))); 
		printf("	Temperature: ↑%d°F ↓%d°F\n", json_object_get_int(json_object_object_get(day, "high")), json_object_get_int(json_object_object_get(day, "low")));
		printf("	Conditions:%s\n", json_object_get_string(json_object_object_get(day, "text")));
	}
}
size_t createJSON(void *ptr, size_t size, size_t nmemb, void *stream)
{
	json_object *json;
	char **response = (char**)stream;
	*response = strndup(ptr, (size_t)(size *nmemb));
	json = json_tokener_parse(*response);
	if(json_object_to_json_string(json) != NULL)
	{
		parseJSON(json);
		json_object_put(json);
	}
	else
	{
		printf("Couldn't get Weather Information\n");
	}
}
static void getJSON()
{
	CURL *curl;
	char* response = NULL;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if(curl) 
	{
		
		curl_easy_setopt(curl, CURLOPT_URL, "http://query.yahooapis.com/v1/public/yql?q=select%20item%20from%20weather.forecast%20where%20location%3D%2207646%22&format=json");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, createJSON);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}
int main(void)
{
	getJSON();
	return 0;
}
