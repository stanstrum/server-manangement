#include "Paths.hpp"
#include <sstream>
#include <iostream>

const char* GmcCsrfInitialization::path() { return "/login/"; };
const char* GmcCsrfInitialization::referrer() { return 0; };
GmcApiRequest::Method GmcCsrfInitialization::method() { return HEAD; };

void GmcCsrfInitialization::finalize(CURL* curl) {};
void GmcCsrfInitialization::consume_response(std::string response) {};

const char* GmcAuthentication::path() { return "/login/auth/"; };
const char* GmcAuthentication::referrer() { return "/login/"; };
GmcApiRequest::Method GmcAuthentication::method() { return POST; };

void GmcAuthentication::finalize(CURL* curl) {
  std::ostringstream oss;

  const char *username = curl_easy_escape(curl, this->username.c_str(), this->username.length());
  const char *password = curl_easy_escape(curl, this->password.c_str(), this->password.length());
  const char *rememberme = this->rememberme ? "true" : "false";

  if (!username || !password) {
    throw std::runtime_error("Form data escape failed");
  };

  oss << "username=" << username << "&password=" << password << "&rememberme=" << rememberme;
  std::string post_data = oss.str();

  std::cout << "formdata: " << post_data << std::endl;

  curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_data.c_str());
};

void GmcAuthentication::consume_response(std::string response) {
  // todo, parse response and do stuff
};

GmcAuthentication::GmcAuthentication(
  std::string username,
  std::string password,
  bool rememberme
) :
  username(username),
  password(password),
  rememberme(rememberme)
{}
