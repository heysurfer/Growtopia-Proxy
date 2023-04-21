class HttpS_Server
{
public:

	

	HttpS_Server(std::string IP,int port)
	{
        auto decodeBase64 = [=](const std::string& base64Text)
        {
            const char PADDING_CHAR = '=';
            const uint8_t DECODED_ALPHBET[128] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,0,0,0,0 };

            const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

            if (base64Text.empty())
                return std::string{""};


            uint32_t numPadding = (*std::prev(base64Text.end(), 1) == PADDING_CHAR) + (*std::prev(base64Text.end(), 2) == PADDING_CHAR);

            std::string decoded((base64Text.size() * 3 >> 2) - numPadding, '.');

            union
            {
                uint32_t temp;
                char tempBytes[4];
            };
            const uint8_t* bytes = reinterpret_cast<const uint8_t*>(base64Text.data());

            std::string::iterator currDecoding = decoded.begin();

            for (uint32_t i = 0, lim = (base64Text.size() >> 2) - (numPadding != 0); i < lim; ++i, bytes += 4)
            {
                temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12 | DECODED_ALPHBET[bytes[2]] << 6 | DECODED_ALPHBET[bytes[3]];
                (*currDecoding++) = tempBytes[2];
                (*currDecoding++) = tempBytes[1];
                (*currDecoding++) = tempBytes[0];
            }

            switch (numPadding)
            {
            case 2:
                temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12;
                (*currDecoding++) = tempBytes[2];
                break;

            case 1:
                temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12 | DECODED_ALPHBET[bytes[2]] << 6;
                (*currDecoding++) = tempBytes[2];
                (*currDecoding++) = tempBytes[1];
                break;
            }

            return decoded;
        };
		std::string cerf1 = ("LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSUQxRENDQXJ5Z0F3SUJBZ0lFZE5jN2NEQU5CZ2txaGtpRzl3MEJBUXNGQURDQmdqRUxNQWtHQTFVRUJoTUMKVkZJeEd6QVpCZ05WQkFNTUVuZDNkeTVuY205M2RHOXdhV0V4TG1OdmJURUxNQWtHQTFVRUNBd0NXRmd4Q3pBSgpCZ05WQkFjTUFuaDRNUkl3RUFZRFZRUUtEQWxJWlhsVGRYSm1aWEl4S0RBbUJna3Foa2lHOXcwQkNRRVdHV2hsCmVYTjFjbVpsY25OQWNISnZkRzl1YldGcGJDNWpiMjB3SGhjTk1qTXdNek13TVRrek9EUXdXaGNOTXpNd016STMKTVRrek9EUXdXakNCZ2pFTE1Ba0dBMVVFQmhNQ1ZGSXhHekFaQmdOVkJBTU1FbmQzZHk1bmNtOTNkRzl3YVdFeApMbU52YlRFTE1Ba0dBMVVFQ0F3Q1dGZ3hDekFKQmdOVkJBY01Bbmg0TVJJd0VBWURWUVFLREFsSVpYbFRkWEptClpYSXhLREFtQmdrcWhraUc5dzBCQ1FFV0dXaGxlWE4xY21abGNuTkFjSEp2ZEc5dWJXRnBiQzVqYjIwd2dnRWkKTUEwR0NTcUdTSWIzRFFFQkFRVUFBNElCRHdBd2dnRUtBb0lCQVFEQlA1azBaSitkajQwL2ZGZ1pXYUV3bGx2egpNL3JTYjFNbnVvS2ZuOXQ0aDdlQXJpcVQ4dHhGak9WU05kSE0vNXQ2eTRqb1pyZFJkczMzZnArZGY0eVdaSEJXCmhhUFM2TTFDcU16a092WEpxbXAvQTNwNzFIOHdmRk1hMUo2VC96cjJta0hrZ2ZtckFQaktMV2FFWUovUHJRVVkKdnZpTW5YcTRkVEFyTGNVMnFOcHBmUHNYeWhMR21NTEpEdTlucjV5NDI4YXZHN2xJYlpzdGZVODUxT1FEVDhYTgpOaFp4aC9JTVVaQkpTRjcxVFV1UXBuQzVmSlJ4eTFCZkFrWUZpMWY1MEtsVFVlRW1hMkRva0U1cVB6TWllelZtCmdHWXNMSFFjSXR3aXU1YlRoNHJpdStVVHRMTmNKUjcvcWhLVkxXOCt5MFJVSUdwRG1LdGdzMEszNUZickFnTUIKQUFHalVEQk9NQjBHQTFVZERnUVdCQlQ5QUh5VVVpVThvYzRCTEQ2aTUraUxTdWVkempBZkJnTlZIU01FR0RBVwpnQlQ5QUh5VVVpVThvYzRCTEQ2aTUraUxTdWVkempBTUJnTlZIUk1FQlRBREFRSC9NQTBHQ1NxR1NJYjNEUUVCCkN3VUFBNElCQVFCZXdhMGNWb1VlTHZwWisySktCaC9mR3g5cXRnRFJsYkNPTGg0ZFAxQU9MYTZwaUhZUXQxUFcKRGZaUWZwYXBsZUtVOVRjbVYxMHZNeHNBVmQrSWJ2NXdwS0Rnd1NHV1dleDFuYzhsTDNKZW5xaC9HODg4QVBxegpBbWtTd2R0dk1lUUY0TnVaZndZZDZKWmpzbE9WNVJaSW1oRmFTOWI3b21XYUZXQUdtbjR5Skd3MGdNYUE3TEs3CkhzQmwwdGc2ZG9qNHdYSEkwMWJ1enpibEpuVlBPSzZDZkxyZGVxOWFRT3pqYlBoN003dDZMaEZocmVCZnJIeFAKN3FjQUhhR25GbFg0bm9FYzlIbUtaYTJYL3N1NFVYWTRiYkZWVE1OSytzbmJwR29aNitmRDN3dHhiMVZXY3k1bAplRnJmSjBINUlud0lRQU1ITUZBOTRVcnFBU1U5WkpCOAotLS0tLUVORCBDRVJUSUZJQ0FURS0tLS0t");
		std::string cerf2 = ("LS0tLS1CRUdJTiBQUklWQVRFIEtFWS0tLS0tCk1JSUV2Z0lCQURBTkJna3Foa2lHOXcwQkFRRUZBQVNDQktnd2dnU2tBZ0VBQW9JQkFRREJQNWswWkorZGo0MC8KZkZnWldhRXdsbHZ6TS9yU2IxTW51b0tmbjl0NGg3ZUFyaXFUOHR4RmpPVlNOZEhNLzV0Nnk0am9acmRSZHMzMwpmcCtkZjR5V1pIQldoYVBTNk0xQ3FNemtPdlhKcW1wL0EzcDcxSDh3ZkZNYTFKNlQvenIybWtIa2dmbXJBUGpLCkxXYUVZSi9QclFVWXZ2aU1uWHE0ZFRBckxjVTJxTnBwZlBzWHloTEdtTUxKRHU5bnI1eTQyOGF2RzdsSWJac3QKZlU4NTFPUURUOFhOTmhaeGgvSU1VWkJKU0Y3MVRVdVFwbkM1ZkpSeHkxQmZBa1lGaTFmNTBLbFRVZUVtYTJEbwprRTVxUHpNaWV6Vm1nR1lzTEhRY0l0d2l1NWJUaDRyaXUrVVR0TE5jSlI3L3FoS1ZMVzgreTBSVUlHcERtS3RnCnMwSzM1RmJyQWdNQkFBRUNnZ0VBTDhvWmloSmtRcU93YTRYRmg2ZFpyRmxTcmVjcDNRWjR5dmRkdkRuZEhZWjYKZzlFT2FINERxUk41MW9YR3QycHl1Y0JCd0M4K29GKy8rYmVQbzNSYWdRcWZtVlg5T0hnbjRwYzU3Q3NFZzNXbwpWbmEyd0k3QlVXMDlaQUkyb1NUYUlRZnV5YUsyNm1vWUhHOXp6Y0xUWXBsSURHR1Z0N2h1RTlXdkxzNW4yZWNqCngxUzJEVWg1eDJ2NmRNRVdueG5nckxqZC9ZN0xYaFV0am8zbHh0dE56S0tLaGZmNUFYcmFwWGNjOVdoK2tLa3gKZ3d1cW1OM2xucDg1UVkvTnJFWkY1WmlzajVYbjM5WkFqaGxTaS95NmxlSHFpdmJicXVreTlqREtwNTdVZVVlVApBWEJPZ2xIWEZySXlrVUl3cFZKbGVuVnhsRFQ3QXFCdmVndHZtR2hPQVFLQmdRRHErSU1jUmR3WlU4cEtvNy8zCjVJdXlJZk82V3FHemZ1WnBDejB0NnVIcDNHYVQ2TnQ4N3dZNkxRZ0E2Yk1Uc0FuUDBrS2E2THRxRHdQSW55RHcKcXFZZTkzOXRNWEJrdzBsdlQzc3NIK0JYclhNR0ZFOVl0WkpzUWdIK3R2eWV5ck5pTCs1THdmSDgrZXJ3YVNxRwpYeWNCZXlGSXFoZ1NqbEE4OUFxdTIxWGxBUUtCZ1FEU2l5eGM3cnVGcXdMaGxmeHBzaXY5MGJCenVMZ2QwSGcyCkltS3ZqVk1waWpaMjlWT09JVGs2NUZ5T05rbEZINlJuR0ZQYkRSb1dYVlVYdnZxYVRVTzl1ZUtobllHRkpGL2YKVFJOcTloeTZSdEpnejRvZFB5MUhtTjFiVmxYUTJBRHZaRU1pbHVTQ044KzdJVnlQMWdDeXR1OFdvZ1B1cEVlZgp4enRyYVZBZjZ3S0JnUURWTkUxek9BaHdpdEVCVFBnQkxNVjlDc1dta0ZvdERyWnZGaDJiTG9keXRlUDF6ZEpxCjJFL2tOYndQWi81dm5Nb3FZblRCTHUvdldsUnFrOGxGMXZJSkl6WGlsSHVSVW9tdGtJY0pFSnQxc3hLdHIzd1gKMkJsV1ZjYU5vdWd5QldjamJxNWcyMGlCN1JzNnJaSkNmdEJiemFmejBUTXFreXRIUTREMkRGc3lBUUtCZ1FDOQoyUjdrb2FSWDhJOUZpUEExQXFNbVdXbEp2OXZOam9pSEQ5UjZ4ZTJpUEczR01JcnFVMHorcktKRTNmQ05zdTJVCnhOd0FYcjBTUTVDU08vUlBKTEticHIwVHpDblpPNlp6ZlZXd0VrSnZPYU5FQm9CY3hQWDRaSC9kZDJra1MxSDAKaXl6NlNLTytROE05MHVSYkVyWkljQ29BOTVDaUpHSXRVZ0pqWlhBSmh3S0JnRFhwMmF0c2FPSVRpM0lRcWxsQwpsL00xREhOOFBZNWVIRllpVk0wZSs5b2J5eHVwOUpVL0hUM215R1J4V2lWT0dCa0h6ciswS05QeE9Ub2xucHlLCkFvWEYydjdwTVAyU3hTMDRnL0o1NnozV0RWNDZDRDh3cGJOT3J3SW93ZUhlY1Y2RkRlSWY1clBJVzhIZmRua0MKR2sycFlnSlVDRzdPT01vR1dZMkh6di8xCi0tLS0tRU5EIFBSSVZBVEUgS0VZLS0tLS0=");
		std::string CertPem = decodeBase64(cerf1);
		std::string KeyPem = decodeBase64(cerf2);
		utils::writeFile(utils::getTemp() + "\\Cert.Pem", CertPem);
		utils::writeFile(utils::getTemp() + "\\Key.Pem", KeyPem);
        	server = std::make_shared< httplib::SSLServer>((utils::getTemp() + "\\Cert.Pem").c_str(), (utils::getTemp() + "\\Key.Pem").c_str());
		char server_data[] = {
		"server|%s\n"
		"port|%s\n"
		"type|1\n"
		"#maint|Under mainteance.\n"
		"beta_server|%s\n"
		"beta_port|17192\n"
		"beta_type|1\n"
		"meta|ni.com\n"
		"RTENDMARKERBS1001\n"
		};
		server->Post("/growtopia/server_data.php", [=](const httplib::Request& req, httplib::Response& res) {
            Print("Received Post Request %s ", req.path.c_str());
			res.set_content(FormatStr(server_data, IP.c_str(), std::to_string(port).c_str(), IP.c_str()), "text/html");
		});
		server->Get("/growtopia/server_data.php", [=](const httplib::Request& req, httplib::Response& res) {
            Print("Received Get Request %s ", req.path.c_str());
			res.set_content(FormatStr(server_data, IP.c_str(), std::to_string(port).c_str(), IP.c_str()), "text/html");
		});
        remove((utils::getTemp() + "\\Cert.Pem").c_str());
        remove((utils::getTemp() + "\\Key.Pem").c_str());
	}
	void Listen()
	{
        Print("HTTP(S) Server Listening on %s:%i", "127.0.0.1", 443);
		server->listen("127.0.0.1", 443);
	}
    void Close()
    {
        if (server) {
            server->stop();
	    server.reset();
        }
    }
private:
	std::shared_ptr< httplib::SSLServer> server;


};