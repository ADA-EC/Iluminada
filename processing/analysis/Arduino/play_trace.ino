int i;
byte colors[7];

void setup() {
    // LEDs output
    pinMode(11, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(2, OUTPUT);
    
    // Init serial
    Serial.begin(115200);
}

void loop() {
    // Get serial data
    if(Serial.available() > 0) {
        Serial.readBytes(colors, 7);
        for(i = 0; i < 7; i++)
            colors[i] = 255 - colors[i];
    }
    
    // Set LEDs PWM
    analogWrite(11, colors[0]);
    analogWrite(10, colors[1]);
    analogWrite(9, colors[2]);
    analogWrite(6, colors[3]);
    analogWrite(5, colors[4]);
    analogWrite(3, colors[5]);
}