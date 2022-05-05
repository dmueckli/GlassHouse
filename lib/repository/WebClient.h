#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "Constants.h"

class WebClientException : virtual public std::exception
{

protected:
    int error_number;     ///< Error number
    String error_message; ///< Error message

public:
    /** Constructor (C++ STL string, int, int).
     *  @param err_num Error number
     *  @param msg The error message
     */
    explicit WebClientException(int err_num, const String &msg) : error_number(err_num), error_message(msg)
    {
    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~WebClientException() throw() {}

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

class WebClient
{
    int _statusCode = 0;

public:
    int makePostRequest(String json, String apiEndpoint = "/glasshouse_api/v1/controller/weatherData.php", String contentType = "application/json")
    {
        WiFiClient wifi;
        HttpClient client = HttpClient(wifi, serverAddress);

        try
        {
            Serial.println("");

            Serial.print("Making POST request: ");

            client.post(apiEndpoint, contentType, json);

            _statusCode = client.responseStatusCode();

            if (_statusCode < 0 || _statusCode != 200)
            {
                /* code */
                String response = client.responseBody();

                Serial.print("Failed to make POST request to ");
                Serial.print(serverAddress);
                Serial.println(apiEndpoint);
                Serial.print("Status code: ");
                Serial.println(_statusCode);
                Serial.print("Response: ");
                Serial.println(response);
                throw WebClientException(_statusCode, "Failed to make POST request!");
            }
            else
            {
                /* code */
                Serial.println("Data successfully sent!");
                Serial.print("Status code: ");
                Serial.println(_statusCode);
                return _statusCode;
            }
        }
        catch (const WebClientException &wce)
        {

            Serial.print(wce.getErrorNumber());
            Serial.print(": ");
            Serial.println(wce.what());
            return _statusCode;
        }
    }
};
