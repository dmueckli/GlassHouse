#include <U8g2lib.h>
#include "GlassHouseRepository.h"
#include "Functions.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

int g_lineHeight = 0;

class GHDisplay
{
public:
    void drawInterface(GlassHouseMk1 data)
    {

        /* code */
        fpsDStart();
        int statusCode = data.getStatusCode();
        String statusMessage;
        if (statusCode == 200)
        {
            /* code */
            statusMessage = String(statusCode) + " OK!";
        }
        else
        {
            /* code */
            statusMessage = String(statusCode) + " Error!";
        }

        u8g2.setFont(u8g2_font_profont12_tf);
        g_lineHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
        u8g2.clearBuffer();

        u8g2.setCursor(0, g_lineHeight);
        u8g2.printf("Humidity: %.1lf", data.getHumidity());
        u8g2.print(" %");

        u8g2.setCursor(0, g_lineHeight * 2);
        u8g2.printf("Earth moist: %.1lf", data.getMoistureValue(true));
        u8g2.print(" %");

        u8g2.setCursor(0, g_lineHeight * 3);
        u8g2.printf("Temperature: %.1lf C", data.getTemperature());

        u8g2.setCursor(0, g_lineHeight * 4);
        u8g2.printf("Heat index: %.1lf C", data.getHeatIndex());

        u8g2.setCursor(0, g_lineHeight * 5);
        u8g2.printf("Post data: %s", statusMessage);

        u8g2.setCursor(0, g_lineHeight * 6);
        u8g2.print(currentTime());
        // u8g2.setCursor(0, g_lineHeight);
        // u8g2.printf("FPS: %.1lf", fps);
        // u8g2.setCursor(0, g_lineHeight * 2);
        // u8g2.printf("Power: %u mW", milliwatts);
        u8g2.sendBuffer();
        fpsDEnd();
        fpsCalculate();
    }
};
