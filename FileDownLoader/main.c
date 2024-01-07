#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(contents, size, nmemb, stream);
    return written;
}

// 进度条回调函数
static int ProgressCallback(void *clientp, double  dltotal, double  dlnow, double  ultotal, double  ulnow) {
printf("\rDownload Progress: %.2f%%", (dlnow/dltotal) * 100);
    return 0;
}

void downLoad(const char *url, const char *filename) {
    char *response = malloc(1); // 初始分配
	response[0] = '\0';  // 确保初始状态是空字符串
	
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl = curl_easy_init();
	if (curl != NULL) {

        FILE *fp = fopen(filename, "wb");
        if(fp == NULL) {
			fprintf(stderr, "Cannot open file %s\n", filename);
            return;
       	}
   		curl_easy_setopt(curl, CURLOPT_URL, url);
   		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); //循环调用回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);   //传递参数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);          //传递参数

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);

   		CURLcode res = curl_easy_perform(curl);
   		if (res != 0) {
   			printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(response);
            response = NULL;
   		}

    	fclose(fp);
   		curl_easy_cleanup(curl);
   }


   curl_global_cleanup();
}

int main() {

	const char* url = "https://mirror.bjtu.edu.cn/kernel/linux/kernel/v1.0/linux-1.0.tar.bz2";
	const char* filename = "./linux-1.0.tar.bz2";
	downLoad(url, filename);

	return 0;
}
