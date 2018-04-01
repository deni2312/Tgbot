#include <string>
#include <tgbot/bot.h>
#include <iostream>

#define FORMAT "video/mpeg"
#define MP4 ".mp4"
#define OUTVID "example.mp4"

using namespace tgbot;
using namespace types;
using namespace methods;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void video(std::string link)//link converter
{
  CURL *curl;
  CURLcode res;
  static const char *pagefilename = OUTVID;
  FILE *pagefile;

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

}

void groupInfo(const Message message, const Api& api){
  std::string linkMessage;
  try{
    linkMessage=*(message.text);
    if(linkMessage.find(MP4)!=std::string::npos){
      video(linkMessage);
      api.sendVideo(std::to_string(message.chat.id),OUTVID,tgbot::methods::types::FileSource::LOCAL_UPLOAD,FORMAT);
    }else{
      api.sendMessage(std::to_string(message.chat.id), "Error mp4 file does not exist");
    }
  }catch(TelegramException& excp){
    std::cerr<<"Exception catched, it says:" << excp.what() << "\n";
  }
}
int main() {
        LongPollBot bot("token");
        bot.callback(groupInfo);
        bot.start();
        //unreachable code
}
