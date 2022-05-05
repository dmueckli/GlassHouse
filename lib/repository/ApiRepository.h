#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include <Constants.h>
#include <WiFi.h>
#include "Session.h"

extern Preferences preferences;
extern Session session;

extern const char *serverAddress;

int statusCode;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress);

class ApiRepositoryException : virtual public std::exception
{

protected:
    int error_number;     ///< Error number
    String error_message; ///< Error message

public:
    /** Constructor (C++ STL string, int, int).
     *  @param err_num Error number
     *  @param msg The error message
     */
    explicit ApiRepositoryException(int err_num, const String &msg) : error_number(err_num), error_message(msg)
    {
    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~ApiRepositoryException() throw() {}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *  is in possession of the HttpException object. Callers must
     *  not attempt to free the memory.
     */
    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }

    /** Returns error number.
     *  @return #error_number
     */
    virtual int getErrorNumber() const throw()
    {
        return error_number;
    }
};

class ApiRepository
{
public:
    String contentType = "application/json";

    /** Beginning of the API connection.  
     *  It first checks for stored session data from "preferences.h"
     *  and tries to log in to the api if none session data could be restored.
     * 
     *  TODO: Add method to check if accesstoken has been expired.
     *  TODO: Add method to check if refreshtoken has been expired.
     *  TODO: Implement method for refreshing the session
    */
    void begin()
    {
        try
        {
            int sessionid = preferences.getInt("sessionid", 0);
            int hostid = preferences.getInt("hostid", 0);
            String accesstoken = preferences.getString("accesstoken", "");
            int accesstoken_expires_in = preferences.getInt("atokenexpiry", 0);
            String refreshtoken = preferences.getString("refreshtoken", "");
            int refreshtoken_expires_in = preferences.getInt("rtokenexpiry", 0);

            if (sessionid == 0 || hostid == 0 || accesstoken == "" || accesstoken_expires_in == 0 || refreshtoken == "" || refreshtoken_expires_in == 0)
            {
                logIn();
                Serial.println("Session could not be restored!");
            }
            else
            {
                Serial.println("Session has been restored!");
                session.setId(sessionid);
                session.setHostId(hostid);
                session.setAccesstoken(accesstoken);
                session.setAccesstokenExpiry(accesstoken_expires_in);
                session.setRefreshtoken(refreshtoken);
                session.setRefreshtokenExpiry(refreshtoken_expires_in);
            }

            Serial.print("Session ID: ");
            Serial.println(session.getId());
            Serial.print("Host ID: ");
            Serial.println(session.getHostId());
            Serial.print("Accesstoken: ");
            Serial.println(session.getAccesstoken());
            Serial.print("Accesstoken expiry: ");
            Serial.println(session.getAccesstokenExpiry());
            Serial.print("Refreshtoken: ");
            Serial.println(session.getRefreshtoken());
            Serial.print("Refreshtoken expiry: ");
            Serial.println(session.getRefreshtokenExpiry());

            
        }
        catch (ApiRepositoryException &are)
        {
            Serial.println(are.what());
        }

        // logIn();
    }

    void logIn()
    {
        try
        {
            String apiEndpoint = "/glasshouse_api_dev/v1/controller/Sessions.php";
            // String jsondata = "{\"username\": \"muecklichd\",\"password\": \"Hans-Dieter2\"}";

            DynamicJsonDocument doc(200);
            String json;

            doc["username"] = "XXXXXXXXXX";
            doc["password"] = "XXXXXXXXXX";

            serializeJsonPretty(doc, json);

            Serial.print("Loggin in to  ");
            Serial.println(apiEndpoint);
            Serial.print("with json data:");
            Serial.println(json);

            client.post(apiEndpoint, contentType, json);

            statusCode = client.responseStatusCode();

            Serial.print("Status code: ");
            Serial.println(statusCode);
            // client.responseBody();

            DynamicJsonDocument doc2(500);
            String loginResponse = client.responseBody();

            DeserializationError error = deserializeJson(doc2, loginResponse);

            // Test if parsing succeeds.
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                throw ApiRepositoryException(1, "deserializeJson() failed");
            }

            int sessionid = doc2["data"]["session_id"];
            int hostid = doc2["data"]["host_id"];
            String accesstoken = doc2["data"]["accesstoken"];
            int accesstoken_expires_in = doc2["data"]["accesstoken_expires_in"];
            String refreshtoken = doc2["data"]["refreshtoken"];
            int refreshtoken_expires_in = doc2["data"]["refreshtoken_expires_in"];

            // session = Session(sessionid, accesstoken, accesstoken_expires_in, refreshtoken, refreshtoken_expires_in);
            session.setId(sessionid);
            session.setHostId(hostid);
            session.setAccesstoken(accesstoken);
            session.setAccesstokenExpiry(accesstoken_expires_in);
            session.setRefreshtoken(refreshtoken);
            session.setRefreshtokenExpiry(refreshtoken_expires_in);

            preferences.putInt("sessionid", sessionid);
            preferences.putInt("hostid", hostid);
            preferences.putString("accesstoken", accesstoken);
            preferences.putInt("atokenexpiry", accesstoken_expires_in);
            preferences.putString("refreshtoken", refreshtoken);
            preferences.putInt("rtokenexpiry", refreshtoken_expires_in);
        }
        catch (ApiRepositoryException &are)
        {
            Serial.println(are.what());
        }
    }

    // TODO: Add method to refresh a session.
    // TODO: Add method to create, update host. 
    // TODO: Add method to post sensor data to the API. 
};