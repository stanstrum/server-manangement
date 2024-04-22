class GmcApiRequest {
  public:
    virtual const char* path() = 0;
    virtual const char* referrer() = 0;
    virtual const char* method() = 0;
};
