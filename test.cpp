#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
using namespace std;

const char *url = "http://localhost:6666/upload";

void uploadWithFileUsePost(std::string fileName)
{
    CURL* easy_handler = curl_easy_init();
    if(easy_handler == NULL){
        printf("curl easy init failed!");
        curl_global_cleanup();
        return;
    }
    curl_easy_setopt(easy_handler,CURLOPT_URL,url);


    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

    curl_formadd(&post, &last,
        CURLFORM_COPYNAME, "file",     
        CURLFORM_BUFFER, "face11.jpg",                        
        CURLFORM_FILE, fileName.c_str(),               
        CURLFORM_END);

    curl_easy_setopt(easy_handler,CURLOPT_HTTPPOST,post);
    
    auto res = curl_easy_perform(easy_handler);
  
    if (res != CURLE_OK){
        printf("curl_easy_perform() failed，error code is:%s\n", curl_easy_strerror(res));
    }
    curl_formfree(post);
    curl_easy_cleanup(easy_handler);
    curl_global_cleanup();

    printf("\n");
}

size_t read_data(void* buffer,size_t size,size_t nmemb,void* user_p){
    FILE* fp = (FILE *)user_p;
    return fread(buffer,size,nmemb,fp);
}


void uploadWithBinaryUsePost(std::string fileName)
{
    CURLcode code;
    code = curl_global_init(CURL_GLOBAL_ALL);
    if (code != CURLE_OK){
        cerr<<"curl global init failed!"<<endl;
        return ;
    }
 
    FILE *fp = fopen(fileName.c_str(),"rb");
    if(NULL == fp){
        cerr<<"fopen failed!"<<endl;
        curl_global_cleanup();
        return ;
    }
    
    fseek(fp,0,2);
    int file_size = ftell(fp);
    rewind(fp);
 
    CURL *easy_handler = curl_easy_init();
    if(easy_handler == NULL){
        cerr<<"curl easy init failed!"<<endl;
        fclose(fp);
        curl_global_cleanup();
        return ;
    }

    curl_easy_setopt(easy_handler,CURLOPT_URL,url);




    uint8_t *pData = new uint8_t[file_size];
    read_data(pData,sizeof(uint8_t),file_size,fp);

    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

    curl_formadd(&post, &last,
        CURLFORM_COPYNAME, "file",                  
        CURLFORM_BUFFER, "face.jpg",                 
        CURLFORM_BUFFERPTR, pData,      
        CURLFORM_BUFFERLENGTH, file_size,   
        CURLFORM_END);

    curl_easy_setopt(easy_handler,CURLOPT_HTTPPOST,post);
 
    code = curl_easy_perform(easy_handler);
    if (code != CURLE_OK){
        printf("curl_easy_perform() failed，error code is:%s\n", curl_easy_strerror(code));
    }
 
    fclose(fp);
    curl_easy_cleanup(easy_handler);
    curl_global_cleanup();

    printf("\n");
}

int main(int argc,char**argv)
{
    if(argc <2){
        printf("add upload file!\n");
        return 0;
    }

    std::string fileName = argv[1];
    printf("upload file name:%s\n",fileName.c_str());

    uploadWithFileUsePost(fileName);
    uploadWithBinaryUsePost(fileName);
}