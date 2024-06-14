#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // Read input from serial monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    // Parse M-Pesa message
    parseMpesaMessage(input);
  }
}

void parseMpesaMessage(String message) {
  // Regular expression pattern to extract information
  String pattern = "([A-Z0-9]+) Confirmed\\.You have received Ksh([0-9,]+\\.\\d{2}) from (.+?) (\\d{10}) on (\\d{1,2}/\\d{1,2}/\\d{2}) at (\\d{1,2}:\\d{2} [AP]M)";
  // Match the pattern
  if (message.indexOf("Confirmed") >= 0) {
    int startIndex = message.indexOf("Confirmed");
    String match = message.substring(startIndex);
    if (match.length() > 0) {
      int idIndex = match.indexOf("Confirmed.") + 10;
      int amountIndex = match.indexOf("Ksh") + 3;
      int senderIndex = match.indexOf("from ") + 5;
      int phoneIndex = match.indexOf(" ", senderIndex);
      int dateIndex = match.indexOf(" on ") + 4;
      int timeIndex = match.indexOf(" at ") + 4;

      String transaction_id = match.substring(idIndex, amountIndex - 6);
      String amount = match.substring(amountIndex, senderIndex - 1);
      String sender_name = match.substring(senderIndex, phoneIndex - 1);
      String sender_phone = match.substring(phoneIndex, phoneIndex + 10);
      String date = match.substring(dateIndex, timeIndex - 1);
      String time = match.substring(timeIndex);

      // Print parsed information on serial monitor
      Serial.println("Transaction ID: " + transaction_id);
      Serial.println("Amount: " + amount);
      Serial.println("Sender Name: " + sender_name);
      Serial.println("Sender Phone: " + sender_phone);
      Serial.println("Date: " + date);
      Serial.println("Time: " + time);
      Serial.println();
    } else {
      Serial.println("Failed to parse the message.");
    }
  } else {
    Serial.println("Not an M-Pesa message.");
  }
}
