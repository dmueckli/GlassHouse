#include <Arduino.h>
#include <ArduinoJson.h>
#include <Constants.h>
#include <WiFi.h>
#include <HTTPClient.h>

extern AirHumiditySensor airHumiditySensor;
extern MoistureSensor moistureSensor;
extern TouchSensor touchSensor;

class Icinga2RepositoryException : virtual public std::exception
{

protected:
    int error_number;     ///< Error number
    String error_message; ///< Error message

public:
    /** Constructor (C++ STL string, int, int).
     *  @param err_num Error number
     *  @param msg The error message
     */
    explicit Icinga2RepositoryException(int err_num, const String &msg) : error_number(err_num), error_message(msg)
    {
    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~Icinga2RepositoryException() throw() {}

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

struct Icinga2Repository
{

    int statusCode;
    String protocol = "https://";
    String serverName = "94.16.105.193";
    String port = ":5665";

    void sendHostalive()
    {
        sendCheckresult("Host", "", 0, "OK - " + (String)hostname + " ist erreichbar!", "");
    }
    void sendSensorData() {
        sendHumidity();
        sendTemperature();
        sendHeatindex();
        sendMoisture();
    }

    void sendHumidity()
    {
        float humidity = airHumiditySensor.getHumidity();

        if (!(isnan(humidity)))
        {
            sendCheckresult("Service", "Humidity", 0, "OK - Humidity " + (String)humidity + " %", "humidity="+String(humidity)+";0.0");
        } else {
            sendCheckresult("Service", "Humidity", 3, "CRITICAL - Humidity " + (String)humidity + " %", "");
        }
    }

    void sendTemperature()
    {
        float temperature = airHumiditySensor.getTemperature();

        if (!(isnan(temperature)))
        {
            sendCheckresult("Service", "Temperature", 0, "OK - Temperature " + (String)temperature + " °C", "temperature="+String(temperature)+";0.0");
        } else {
            sendCheckresult("Service", "Temperature", 3, "CRITICAL - Temperature " + (String)temperature + " °C", "");
        }
    }

    void sendHeatindex()
    {
        float heatindex = airHumiditySensor.getHeatIndex();

        if (!(isnan(heatindex)))
        {
            sendCheckresult("Service", "Heatindex", 0, "OK - Heatindex " + (String)heatindex + " °C", "heatindex="+String(heatindex)+";0.0");
        } else {
            sendCheckresult("Service", "Heatindex", 3, "CRITICAL - Heatindex  " + (String)heatindex + " °C", "");
        }
    }

    void sendMoisture()
    {
        float moisture = moistureSensor.getMoistureValue(true);

        if (!(isnan(moisture)))
        {
            sendCheckresult("Service", "Moisture", 0, "OK - Moisture " + (String)moisture + " %", "heatindex="+String(moisture)+";0.0");
        } else {
            sendCheckresult("Service", "Moisture", 3, "CRITICAL - Moisture " + (String)moisture + " %", "");
        }
    }

    void sendCheckresult(String type = "", String service = "", int exitStatus = 1, String pluginOutput = "", String performanceData = "", bool pretty = true)
    {
        try
        {
            WiFiClient wifi;
            HTTPClient client;
            String apiEndpoint = "/v1/actions/process-check-result";
            // String apiEndpoint = "/v1";
            String url = protocol + serverName + port + apiEndpoint;

            DynamicJsonDocument doc(200);
            String json;
            String host = icingahost;

            String filter;

            if (type == "" || isnan(exitStatus) || pluginOutput == "")
            {
                throw Icinga2RepositoryException(999, "Mandatory values not given!");
                return;
            }

            if (service != "" && service != NULL)
            {
                filter = "host.name==\"" + host + "\" && service.name==\"" + service + "\"";
            }
            else
            {
                filter = "host.name==\"" + host + "\"";
            }

            doc["type"] = type;
            doc["filter"] = filter;
            doc["exit_status"] = exitStatus;
            doc["plugin_output"] = pluginOutput;
            if (performanceData != "")
            {
                doc["performance_data"] = performanceData;
            }

            doc["check_source"] = (String)icingahost;
            doc["pretty"] = pretty;

            serializeJsonPretty(doc, json);

            client.begin(url);

            client.setAuthorization("esp32_test", "Geheim#2022");
            client.addHeader("Accept", "application/json");

            statusCode = client.POST(json);

            Serial.print("Sending passive checkresult to icinga. ");
            Serial.print("Status code: ");
            Serial.println(statusCode);

            String postResponse;

            // Check if the Session has been created
            if (statusCode != 200)
            {
                if (statusCode == -1) // HTTPC_ERROR_CONNECTION_REFUSED
                {
                    client.end();
                    throw Icinga2RepositoryException(-1, "Could not connect to the server - Connection refused!");
                    return;
                }
                else if (statusCode == -2) // HTTPC_ERROR_SEND_HEADER_FAILED
                {
                    client.end();
                    throw Icinga2RepositoryException(-2, "Could not connect to the server - failed sending headers!");
                    return;
                }
                else if (statusCode == -3) // HTTPC_ERROR_SEND_PAYLOAD_FAILED
                {
                    client.end();
                    throw Icinga2RepositoryException(-3, "Could not connect to the server - failed sending body!");
                    return;
                }
                else if (statusCode == -4) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-4, "Could not connect to the server!");
                    return;
                }
                else if (statusCode == -5) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-5, "Could not connect to the server - connection lost!");
                    return;
                }
                else if (statusCode == -6) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-6, "Could not connect to the server - no stream available!");
                    return;
                }
                else if (statusCode == -7) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-7, "Could not connect to the server - no server available!");
                    return;
                }
                else if (statusCode == -8) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-8, "Could not connect to the server - no server available!");
                    return;
                }
                else if (statusCode == -9) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-9, "Could not connect to the server - wrong encoding!");
                    return;
                }
                else if (statusCode == -10) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-10, "Could not connect to the server - error writing stream!");
                    return;
                }
                else if (statusCode == -11) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(-11, "Could not connect to the server - timeout reading from server!");
                    return;
                }
                else if (statusCode == 401) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(401, "Unauthorized!");
                    return;
                }
                else if (statusCode == 404) // HTTPC_ERROR_NOT_CONNECTED
                {
                    client.end();
                    throw Icinga2RepositoryException(404, "Not found!");
                    return;
                }
                else
                {
                    postResponse = client.getString();
                    client.end();
                    if (postResponse == "" || postResponse == NULL)
                    {
                        throw Icinga2RepositoryException(500, "Body contains no data!");
                        return;
                    }

                    // Deserialization of the login response
                    DynamicJsonDocument doc2(200);

                    DeserializationError error = deserializeJson(doc2, postResponse);

                    // Test if parsing succeeds.
                    if (error)
                    {
                        Serial.print(F("deserializeJson() failed: "));
                        Serial.println(error.f_str());
                        throw Icinga2RepositoryException(1, "deserializeJson() failed");
                        return;
                    }
                    String message = doc2["messages"];

                    Serial.println(message);
                    throw Icinga2RepositoryException(401, "Checkresult could not be sent!");
                    return;
                }
            }

            postResponse = client.getString();
            client.end();
            if (postResponse == "" || postResponse == NULL)
            {
                throw Icinga2RepositoryException(500, "Body contains no data!");
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
                throw Icinga2RepositoryException(1, "deserializeJson() failed");
                return;
            }

            String messages = doc3["results"][0]["status"];

            Serial.println(messages);
            Serial.println();
        }
        catch (Icinga2RepositoryException &ire)
        {
            Serial.println(ire.what());
        }
        catch (const std::exception &e)
        {
            Serial.print(e.what());
            Serial.println();
        }
    }
};