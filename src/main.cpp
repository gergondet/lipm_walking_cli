#include <mc_control/ControllerClient.h>

#include <iostream>
#include <sstream>
#include <unordered_map>

struct CLIClient : public mc_control::ControllerClient
{
  CLIClient()
  : mc_control::ControllerClient("ipc:///tmp/mc_rtc_pub.ipc", "ipc:///tmp/mc_rtc_rep.ipc")
  {
  }

  void category(const std::vector<std::string> & /*parent*/, const std::string & /*category*/) override {}

  bool start_standing(std::stringstream &)
  {
    this->send_request({{"Walking", "Main"}, "Start standing"});
    return true;
  }

  bool load_plan(std::stringstream & args)
  {
    std::string plan;
    args >> plan;
    if(!args)
    {
      std::cerr << "Require one argument\n";
      return false;
    }
    this->send_request({{"Walking", "Main"}, "Footstep plan"}, plan);
    return true;
  }

  bool start_walking(std::stringstream &)
  {
    this->send_request({{"Walking", "Main"}, "Start walking"});
    return true;
  }

  bool pause_walking(std::stringstream &)
  {
    this->send_request({{"Walking", "Main"}, "Pause walking"});
    return true;
  }

  void run()
  {
    bool running = true;
    using callback_t = bool(CLIClient::*)(std::stringstream &);
    using fun_t = std::pair<callback_t, std::string>;
    std::unordered_map<std::string, fun_t> callbacks =
    {
      {"start_standing", {&CLIClient::start_standing, "Start standing"}},
      {"load_plan", {&CLIClient::load_plan, "Load the provided plan"}},
      {"start_walking", {&CLIClient::start_walking, "Start walking"}},
      {"pause_walking", {&CLIClient::pause_walking, "Pause walking"}},
    };
    while(running)
    {
      std::string ui;
      std::cout << "(command) " << std::flush;
      std::getline(std::cin, ui);
      std::stringstream ss;
      ss << ui;
      std::string token;
      ss >> token;
      if(token == "stop")
      {
        running = false;
      }
      else if(token == "help")
      {
        for(const auto & cb : callbacks)
        {
          std::cout << "- " << cb.first << " -- " << cb.second.second << "\n";
        }
        std::cout << "- help -- Display this message\n";
      }
      else if(callbacks.count(token))
      {
        auto fn = callbacks[token].first;
        (this->*fn)(ss);
      }
      else
      {
        std::cerr << "No such functions, type help to get a list of available commands\n";
      }
    }
  }
private:
};

int main(int, char * [])
{
  CLIClient client;
  client.run();
  return 0;
}
