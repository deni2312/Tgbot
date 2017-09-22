
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include <exception>
#include <signal.h>
using namespace std;
using namespace TgBot;
bool sigintGot = false;
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
void image(string link)//link converter
{
CURL *curl;
CURLcode res;
static const char *pagefilename = "example.mp4";
  FILE *pagefile;
curl_global_init(CURL_GLOBAL_DEFAULT);
curl = curl_easy_init();
if(curl) 
{
  curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
#ifdef SKIP_PEER_VERIFICATION
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
pagefile = fopen(pagefilename, "wb");
    if(pagefile) 
      {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
        curl_easy_perform(curl);
        fclose(pagefile);
      }
  }
curl_global_cleanup();
}
int main() {
  bool i=false;
  string test;
  string mp4="mp4";
  const string videoFilePath = "example.mp4";
	const string videoMimeType = "video/mp4";
   TgBot::Bot bot("INSERT HERE YOUR TOKEN");
   bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
          bot.getApi().sendMessage(message->chat->id, "I'm a bot!");
   });
    bot.getEvents().onAnyMessage([&bot, &i, &videoFilePath, &videoMimeType,&test,&mp4](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        if(i==true)
        {
        test=message->text;
        if(test.find(mp4)!=std::string::npos)
        {
        image(test);
        bot.getApi().sendVideo(message->chat->id, InputFile::fromFile(videoFilePath, videoMimeType));
        }else
          {
            bot.getApi().sendMessage(message->chat->id, "Error mp4 file does not exist");
          }
          i=false;
        }
        if(message->text=="/convert")
        {
          bot.getApi().sendMessage(message->chat->id, "Write here the link to convert:");
          i=true;
        }
        cout<<message->chat->id<<"\n";
        
        });
	remove("example.mp4");
       signal(SIGINT, [](int s) {
            printf("SIGINT got");
            sigintGot = true;
          });
          TgLongPoll longPoll(bot);
          while(!sigintGot)
          {
            try
            {
              printf("Long poll started\n");
              longPoll.start();
            }catch(exception& e){
              printf("error %s\n",e.what());
            }
          }
    return 0;
}
