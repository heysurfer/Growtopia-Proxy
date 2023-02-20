class PuzzleSolver {
public:
    bool Solved = false;
    std::string LatestAnswer = "";
    std::string Error;
    PuzzleSolver(std::string g) : SecretKey(g) {}
    void AddParams(std::string Key, std::string Value)
    {
        Params.append(Key + "=" + Value + "&");
    }
    std::string GetAnswer(std::string CaptchaUID)
    {
     
        AddParams("Action", "Solve");
        AddParams("Puzzle", CaptchaUID);
        AddParams("Secret", SecretKey);
        AddParams("Format", "txt");
        Path =  "/" + Path + Params;
       
        httplib::Client cli(API);
        cli.enable_server_certificate_verification(false);
        cli.set_connection_timeout(10, 0);
        
        auto res = cli.Get(Path.c_str());
      
        if (res.error() == httplib::Error::Success) {
            std::string captchaAnswer = res->body;
            Solved = captchaAnswer.find("Failed") == std::string::npos && captchaAnswer.length() > 6 && res->status == 202;
            if (!Solved)
                Error = res->body;
            return LatestAnswer = (captchaAnswer.length() > 6) ? captchaAnswer.erase(0, 7) : "Failed";
        }
        else
            Print("Requested Failed Error : %s", httplib::to_string(res.error()).c_str());
        return "Failed";
    }
private:
    std::string SecretKey;
    const char* API = "http://api.surferwallet.net";
    std::string Path = "Captcha?";
    std::string Params = "";
   
};