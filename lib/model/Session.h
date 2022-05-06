#include <Arduino.h>
#include <time.h>

extern Preferences preferences;

class Session
{
private:
    int _sessionId;
    int _hostId;
    String _accesstoken;
    int _accesstokenexpiry;
    String _refreshtoken;
    int _refreshtokenexpiry;
    int _lastLoginTime;

public:
    Session(int id = 0, int hostId = 0, String accesstoken = "", int accesstokenexpiry = 0, String refreshtoken = "", int refreshtokenexpiry = 0, int lastLoginTime = 0)
    {
        this->setId(id);
        this->setHostId(hostId);
        this->setAccesstoken(accesstoken);
        this->setAccesstokenExpiry(accesstokenexpiry);
        this->setRefreshtoken(refreshtoken);
        this->setRefreshtokenExpiry(refreshtokenexpiry);
        this->setLoginTime(lastLoginTime);
    };

    void setId(int id)
    {
        preferences.putInt("sessionid", id);
        _sessionId = id;
    }

    void setHostId(int hostId)
    {
        preferences.putInt("hostid", hostId);
        _hostId = hostId;
    }

    void setAccesstoken(String accesstoken)
    {
        preferences.putString("accesstoken", accesstoken);
        _accesstoken = accesstoken;
    }

    void setAccesstokenExpiry(int accesstokenexpiry)
    {
        preferences.putInt("atokenexpiry", accesstokenexpiry);
        _accesstokenexpiry = accesstokenexpiry;
    }

    void setRefreshtoken(String refreshtoken)
    {
        preferences.putString("refreshtoken", refreshtoken);
        _refreshtoken = refreshtoken;
    }

    void setRefreshtokenExpiry(int refreshtokenexpiry)
    {
        preferences.putInt("rtokenexpiry", refreshtokenexpiry);
        _refreshtokenexpiry = refreshtokenexpiry;
    }

    void setLoginTime(int lastLoginTime)
    {
        preferences.putInt("logintime", lastLoginTime);
        _lastLoginTime = lastLoginTime;
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

    int getLoginTime()
    {
        return _lastLoginTime;
    }
};
