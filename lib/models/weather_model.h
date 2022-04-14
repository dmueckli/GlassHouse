/// Basic humidity and temperature model.
class WeatherModel
{
public:
    float _humidity;
    int _earthMoist;
    float _tempCelsius;
    float _heatIndexCelsius;
    float _tempFahrenheit;
    float _heatIndexFahrenheit;

    WeatherModel(float humidity = 0.0, int earthMoist = 0.0, float tempCelsius = 0.0, float heatIndexCelsius = 0.0, float tempFahrenheit = 0.0, float heatIndexFahrenheit = 0.0)
    {
        _humidity = humidity;
        _earthMoist = earthMoist;
        _tempCelsius = tempCelsius;
        _heatIndexCelsius = heatIndexCelsius;
        _tempFahrenheit = tempFahrenheit;
        _heatIndexFahrenheit = heatIndexFahrenheit;
    }
};