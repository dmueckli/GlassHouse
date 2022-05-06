#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include <Constants.h>
#include <WiFi.h>
#include "TimeRepository.h"

#pragma once
extern Preferences preferences;
extern Session session;
extern Timeinfo timeinfo;

extern const char *serverAddress;

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
    const char *contentType = "application/json";

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
            int logintime = preferences.getInt("logintime", 0);

            if (sessionid == 0 || hostid == 0 || accesstoken == "" || accesstoken_expires_in == 0 || refreshtoken == "" || refreshtoken_expires_in == 0 || logintime == 0)
            {
                logIn();
                Serial.println("Session could not be restored!");
            }
            else
            {
                session.setId(sessionid);
                session.setHostId(hostid);
                session.setAccesstoken(accesstoken);
                session.setAccesstokenExpiry(accesstoken_expires_in);
                session.setRefreshtoken(refreshtoken);
                session.setRefreshtokenExpiry(refreshtoken_expires_in);
                session.setLoginTime(logintime);
                Serial.println("Session has been restored!");
            }

            checkTokenExpiry();
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
            int statusCode;
            WiFiClient wifi;
            HttpClient client = HttpClient(wifi, serverAddress);
            String apiEndpoint = "/glasshouse_api_dev/v1/controller/Sessions.php";

            DynamicJsonDocument doc(200);
            String json;

            doc["username"] = username;
            doc["password"] = userpassword;

            serializeJsonPretty(doc, json);

            Serial.print("Loggin in to  ");
            Serial.println(apiEndpoint);
            Serial.print("with json data:");
            Serial.println(json);

            client.post(apiEndpoint, contentType, json);

            statusCode = client.responseStatusCode();

            Serial.print("Status code: ");
            Serial.println(statusCode);

            // Check if the Session has been created
            if (statusCode != 201)
            {
                throw ApiRepositoryException(401, "Username or password is incorrect!");
                return;
            }

            // Deserialization of the login response
            DynamicJsonDocument doc2(500);
            String loginResponse = client.responseBody();

            DeserializationError error = deserializeJson(doc2, loginResponse);

            // Test if parsing succeeds.
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                throw ApiRepositoryException(1, "deserializeJson() failed");
                return;
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
            session.setLoginTime(timeinfo.getTime());

        }
        catch (ApiRepositoryException &are)
        {
            Serial.println(are.what());
        }
    }

    /** Refreshes a user session.
     *
     */
    void refresh()
    {
        try
        {
            int statusCode;
            WiFiClient wifi;
            HttpClient client = HttpClient(wifi, serverAddress);
            String apiEndpoint = "/glasshouse_api_dev/v1/controller/Sessions.php?sessionid=";

            // create the json body
            DynamicJsonDocument doc(200);
            String json;

            // only the refresh token is needed, the accesstoken will be sent in the header
            doc["refresh_token"] = session.getRefreshtoken();

            serializeJsonPretty(doc, json);

            Serial.print("Refreshing session on endpoint: ");
            Serial.println(apiEndpoint + session.getId());
            // Serial.print("with json data:");
            // Serial.println(json);

            String atoken = session.getAccesstoken();
            const char *cstr = atoken.c_str();
            // Serial.print("Accesstoken: ");
            // Serial.println(atoken);

            String url = apiEndpoint + session.getId();
            // Serial.print("Url: ");
            // Serial.println(url);

            client.beginRequest();

            client.patch(url.c_str(), contentType, json.length(), (const byte *)json.c_str(), atoken.c_str());
            client.endRequest();

            statusCode = client.responseStatusCode();

            Serial.print("Status code: ");
            Serial.println(statusCode);
            Serial.println();

            // Check if the Session has been created
            if (statusCode != 200)
            {
                // Deserialization of the login response
                DynamicJsonDocument doc2(500);
                String loginResponse = client.responseBody();

                DeserializationError error = deserializeJson(doc2, loginResponse);

                // Test if parsing succeeds.
                if (error)
                {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());
                    throw ApiRepositoryException(1, "deserializeJson() failed");
                    return;
                }
                String message = doc2["messages"];

                Serial.println(message);
                logIn();
                throw ApiRepositoryException(401, "Session could not be refreshed - please login again!");
                return;
            }

            // Deserialization of the login response
            DynamicJsonDocument doc3(500);
            String loginResponse = client.responseBody();

            DeserializationError error = deserializeJson(doc3, loginResponse);

            // Test if parsing succeeds.
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                throw ApiRepositoryException(1, "deserializeJson() failed");
                return;
            }

            String message = doc3["messages"];
            Serial.println(message);
            Serial.println();

            int sessionid = doc3["data"]["session_id"];
            // int hostid = doc2["data"]["host_id"];
            String accesstoken = doc3["data"]["access_token"];
            // TODO: Make all variable names the same on the api
            int accesstoken_expires_in = doc3["data"]["access_token_expiry"];
            String refreshtoken = doc3["data"]["refresh_token"];
            int refreshtoken_expires_in = doc3["data"]["refresh_token_expiry"];

            session.setId(sessionid);
            // session.setHostId(hostid);
            session.setAccesstoken(accesstoken);
            session.setAccesstokenExpiry(accesstoken_expires_in);
            session.setRefreshtoken(refreshtoken);
            session.setRefreshtokenExpiry(refreshtoken_expires_in);
            session.setLoginTime(timeinfo.getTime());
        }
        catch (ApiRepositoryException &are)
        {
            Serial.println(are.what());
        }
    }

    void checkTokenExpiry()
    {
        try
        {
            if ((timeinfo.getTime() - session.getLoginTime()) > session.getAccesstokenExpiry() || (timeinfo.getTime() - session.getLoginTime()) > session.getRefreshtokenExpiry())
            {
                if (timeinfo.getTime() == 0)
                {
                    // Serial.print("Could not get time!");
                    throw ApiRepositoryException(500, "Error obtaining time!");
                    return;
                }
                else if ((timeinfo.getTime() - session.getLoginTime()) > session.getRefreshtokenExpiry())
                {
                    Serial.print("Current Time: ");
                    Serial.println(timeinfo.getTime());
                    Serial.print("Login Time: ");
                    Serial.println(session.getLoginTime());
                    Serial.print("Expiry Time: ");
                    Serial.println(session.getRefreshtokenExpiry());
                    Serial.println("Refresh Token expired! Logging in again...");
                    Serial.println();

                    logIn();
                }
                else if ((timeinfo.getTime() - session.getLoginTime()) > session.getAccesstokenExpiry())
                {
                    Serial.print("Current Time: ");
                    Serial.println(timeinfo.getTime());
                    Serial.print("Login Time: ");
                    Serial.println(session.getLoginTime());
                    Serial.print("Expiry Time: ");
                    Serial.println(session.getAccesstokenExpiry());
                    Serial.println("Access Token expired! Refreshing...");
                    Serial.println();
                    refresh();
                }
            }
            else
            {
                long int loginTime = session.getLoginTime();
                long int currentTime = timeinfo.getTime();
                long int atokenexpiry = (((currentTime - loginTime) - session.getAccesstokenExpiry()) * -1);
                long int rtokenexpiry = (((currentTime - loginTime) - session.getRefreshtokenExpiry()) * -1);

                // Serial.print("Current Time: ");
                // Serial.println(currentTime);
                // Serial.print("Login Time: ");
                // Serial.println(loginTime);
                Serial.print("Accesstoken expires in: ");
                Serial.print(atokenexpiry);
                Serial.println("s");
                // Serial.println(session.getAccesstokenExpiry());
                Serial.print("Refreshtoken expires in: ");
                Serial.print(rtokenexpiry);
                Serial.println("s");
                // Serial.println(session.getRefreshtokenExpiry());
                Serial.println("All tokens OK!");
                Serial.println();
            }
        }
        catch (ApiRepositoryException &e)
        {
            Serial.printf(e.what());
            Serial.println();
            return;
        }
    }

    void postSensorData()
    {
        try
        {
            int statusCode;
            WiFiClient wifi;
            HttpClient client = HttpClient(wifi, serverAddress);
            String apiEndpoint = "/glasshouse_api_dev/v1/controller/GlassHouse.php";

            // create the json body
            DynamicJsonDocument doc(300);
            String json;

            // only the refresh token is needed, the accesstoken will be sent in the header
            doc["sensor_data"]["humidity"] = airHumiditySensor.getHumidity();
            doc["sensor_data"]["soil_moisture"] = moistureSensor.getMoistureValue(true);
            doc["sensor_data"]["temperature"] = airHumiditySensor.getTemperature();
            doc["sensor_data"]["heat_index"] = airHumiditySensor.getHeatIndex();

            // For °F
            // doc["sensor_data"]["temperature"] = airHumiditySensor.getTemperature(true);
            // doc["sensor_data"]["heat_index"] = airHumiditySensor.getHeatIndex(true);

            serializeJsonPretty(doc, json);

            Serial.print("Posting sensor data to endpoint: ");
            Serial.println(apiEndpoint);
            // Serial.print("with json data:");
            // Serial.println(json);

            String atoken = session.getAccesstoken();

            client.beginRequest();

            client.post(apiEndpoint.c_str(), contentType, json.length(), (const byte *)json.c_str(), atoken.c_str());
            client.endRequest();

            statusCode = client.responseStatusCode();

            Serial.print("Status code: ");
            Serial.println(statusCode);
            Serial.println();

            String postResponse = client.responseBody();

            // Check if the Session has been created
            if (statusCode != 200)
            {
                // Deserialization of the login response
                DynamicJsonDocument doc2(200);

                DeserializationError error = deserializeJson(doc2, postResponse);

                // Test if parsing succeeds.
                if (error)
                {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());
                    throw ApiRepositoryException(1, "deserializeJson() failed");
                    return;
                }
                String message = doc2["messages"];

                Serial.println(message);
                logIn();
                throw ApiRepositoryException(401, "Sensor data could not be inserted to database!");
                return;
            }

            // Deserialization of the login response
            DynamicJsonDocument doc3(500);

            DeserializationError error = deserializeJson(doc3, postResponse);

            // Test if parsing succeeds.
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                throw ApiRepositoryException(1, "deserializeJson() failed");
                return;
            }

            String messages = doc3["messages"];

            Serial.println(messages);
            Serial.println();
        }
        catch (ApiRepositoryException &are)
        {
            Serial.println(are.what());
        }
        catch (const std::exception &e)
        {
            Serial.print(e.what());
            Serial.println();
        }
    }

    // TODO: Add method to create, update host.
};