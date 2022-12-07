#include &lt; Wire.h & gt;
#include &lt; Servo.h & gt;
#include &lt; SPI.h & gt;
#include &lt; MFRC522.h & gt;
#include &lt; LiquidCrystal_I2C.h & gt;
// Set the LCD address to 0x27 for a 16 x 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 4                   // define green LED pin
#define LED_R 5                   // define red LED
#define BUZZER 2                  // buzzer pin
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
Servo myServo;                    // servo name
int button = 6;

void setup()
{
    Serial.begin(9600); // Initiate a serial communication
    SPI.begin();        // Initiate SPI bus
    mfrc522.PCD_Init(); // Initiate MFRC522
    myServo.attach(3);  // servo pin
    myServo.write(5);   // servo start position
    pinMode(LED_G, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);
    pinMode(button, INPUT);

    Serial.println(&quot; Place your card on reader... & quot;);
    Serial.println();
    // initialize the LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0); // column, row
    lcd.print(&quot; Place Your Card & quot;);
    lcd.setCursor(0, 1); // column, row
    lcd.print(&quot; *Locked * &quot;);
}
void loop()
{
    if (digitalRead(button) == HIGH)
    {
        lcd.setCursor(0, 1); // column, row
        lcd.print(&quot; *Locked * &quot;);
        tone(BUZZER, 2000);
        delay(100);
        noTone(BUZZER);
        delay(50);
        myServo.write(5);
        delay(200);
    }
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    // Show UID on serial monitor
    Serial.print(&quot; UID tag
                 : &quot;);
    String content = &quot;
    &quot;
    ;
    byte letter;
    for (byte i = 0; i & lt; mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] & lt; 0x10 ? &quot; 0 & quot; : &quot; &quot;);
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] & lt; 0x10 ? &quot; 0 & quot; : &quot; &quot;));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print(&quot; Message
                 : &quot;);
    content.toUpperCase();

    if (content.substring(1) == &quot; 09 E5 8C 6E & quot;) // change here the UID of card/cards or tag/tags that you
        want to give access
        {
            Serial.println(&quot; Access Granted & quot;);
            Serial.println();

            delay(500);
            digitalWrite(LED_G, HIGH);
            lcd.setCursor(0, 1); // column, row
            lcd.print(&quot; *Un - Locked * &quot;);
            tone(BUZZER, 2000);
            delay(100);
            noTone(BUZZER);
            delay(50);
            tone(BUZZER, 2000);
            delay(100);
            noTone(BUZZER);

            myServo.write(120);
            delay(300);
            myServo.write(105);
            delay(1000);
            digitalWrite(LED_G, LOW);
        }

    else
    {
        lcd.setCursor(0, 1); // column, row
        lcd.print(&quot; Wrong Card / Tag & quot;);
        Serial.println(&quot; Access denied & quot;);
        digitalWrite(LED_R, HIGH);
        tone(BUZZER, 1500);
        delay(500);
        digitalWrite(LED_R, LOW);

        noTone(BUZZER);
        delay(100);
        digitalWrite(LED_R, HIGH);
        tone(BUZZER, 1500);
        delay(500);
        digitalWrite(LED_R, LOW);
        noTone(BUZZER);
        delay(100);
        digitalWrite(LED_R, HIGH);
        tone(BUZZER, 1500);
        delay(500);
        digitalWrite(LED_R, LOW);
        noTone(BUZZER);
        lcd.setCursor(0, 1); // column, row
        lcd.print(&quot; *Locked * &quot;);
    }
}