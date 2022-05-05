#include <Arduino.h>

class Session
{
private:
    int _sessionId;
    int _hostId;
    String _accesstoken;
    int _accesstokenexpiry;
    String _refreshtoken;
    int _refreshtokenexpiry;

public:
    Session(int id = 0, int hostId = 0, String accesstoken = "", int accesstokenexpiry = 0, String refreshtoken = "", int refreshtokenexpiry = 0)
    {
        this->setId(id);
        this->setHostId(hostId);
        this->setAccesstoken(accesstoken);
        this->setAccesstokenExpiry(accesstokenexpiry);
        this->setRefreshtoken(refreshtoken);
        this->setRefreshtokenExpiry(refreshtokenexpiry);
    };

    void setId(int id)
    {
        _sessionId = id;
    }

    void setHostId(int hostId)
    {
        _hostId = hostId;
    }

    void setAccesstoken(String accesstoken)
    {
        _accesstoken = accesstoken;
    }

    void setAccesstokenExpiry(int accesstokenexpiry)
    {
        _accesstokenexpiry = accesstokenexpiry;
    }

    void setRefreshtoken(String refreshtoken)
    {
        _refreshtoken = refreshtoken;
    }

    void setRefreshtokenExpiry(int refreshtokenexpiry)
    {
        _refreshtokenexpiry = refreshtokenexpiry;
    }

    int getId()
    {
        return _sessionId;
    }

    int getHostId()
    {
        return _hostId;
    }

    String getAccesstoken()
    {
        return _accesstoken;
    }

    int getAccesstokenExpiry()
    {
        return _accesstokenexpiry;
    }

    String getRefreshtoken()
    {
        return _refreshtoken;
    }

    int getRefreshtokenExpiry()
    {
        return _refreshtokenexpiry;
    }
};
