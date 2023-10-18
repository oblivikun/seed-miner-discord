#include <iostream>
#include <random>
#include <string>
#include <curl/curl.h>

std::string generateInviteCode(int size = 7, const std::string& chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQrstuvwxyz0123456789") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    std::string code;
    for (int i = 0; i < size; ++i) {
        code += chars[dis(gen)];
    }
    return code;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void checkInvite(const std::string& code) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    long http_code = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ("https://discordapp.com/api/v6/invites/" + code).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    // Check the HTTP status code
    if (http_code == 200) {
        std::cout << "Found valid invite: " << code << std::endl;
    }
}

int main() {
    while (true) {
        std::string codes = generateInviteCode();
        checkInvite(codes);
    }
    return 0;
}