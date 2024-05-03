#include "GmcCsrfInitialization.hpp"

GmcCsrfInitialization::GmcCsrfInitialization(std::string path) : m_path(path) {};

const char* GmcCsrfInitialization::path() {
  return this->m_path.c_str();
};

const char* GmcCsrfInitialization::referrer() {
  return nullptr;
};

GmcApiRequest::Method GmcCsrfInitialization::method() {
  return HEAD;
};

void GmcCsrfInitialization::finalize(CURL* curl) {};
void GmcCsrfInitialization::consume_response(std::string&& response) {};
