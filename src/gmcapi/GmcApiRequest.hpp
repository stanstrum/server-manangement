class GmcApiRequest {
public:
  enum Method {
    GET,
    POST,
  };

  virtual const char* path() = 0;
  virtual const char* referrer() = 0;
  virtual Method method() = 0;
};
