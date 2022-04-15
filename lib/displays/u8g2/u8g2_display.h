#include <U8g2lib.h>
#include <functions.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

int g_lineHeight = 0;

void drawInterface(WeatherModel data)
{

    for (size_t i = 0; i < 104; i++)
    {
        /* code */
        fpsDStart();

        u8g2.setFont(u8g2_font_profont12_tf);
        g_lineHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
        u8g2.clearBuffer();

        u8g2.setCursor(0, g_lineHeight - 20 + i);
        u8g2.printf("Humidity: %.1lf", data._humidity);
        u8g2.print(" %");

        u8g2.setCursor(0, g_lineHeight * 2 - 20 + i);
        u8g2.printf("Earth moist: %u", data._earthMoist);
        u8g2.print(" %");

        u8g2.setCursor(0, g_lineHeight * 3 - 20 + i);
        u8g2.printf("Temperature: %.1lf C", data._tempCelsius);

        u8g2.setCursor(0, g_lineHeight * 4 - 20 + i);
        u8g2.printf("Heat index: %.1lf C", data._heatIndexCelsius);
        // u8g2.setCursor(0, g_lineHeight);
        // u8g2.printf("FPS: %.1lf", fps);
        // u8g2.setCursor(0, g_lineHeight * 2);
        // u8g2.printf("Power: %u mW", milliwatts);
        u8g2.sendBuffer();
        fpsDEnd();
        fpsCalculate();
    }
}