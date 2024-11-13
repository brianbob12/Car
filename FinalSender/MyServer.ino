#include "MyServer.h"

const char* ssid = "Kepler 186-f";  // AP mode SSID, no password needed

const char body[] PROGMEM = R"===( 
<!DOCTYPE html>  
<html><body>        
<h1>Motor Control</h1>
<p>Adjust the slider to set motor speed and use WASD keys to control direction (Space to stop).</p>
<label for='speed'>Speed (0 to 100%): </label>
<input type='range' id='speed' min='0' max='100' value='50' oninput='updateSpeed()'><span id='speedValue'>50</span>%<br><br>
<div id="controls">
  <p>Controls:</p>
  <ul>
    <li>W - Forward</li>
    <li>S - Backward</li>
    <li>A - Left</li>
    <li>D - Right</li>
    <li>Space - Stop</li>
  </ul>
</div>
<script>
  function updateSpeed() {
    let speed = document.getElementById('speed').value;
    document.getElementById('speedValue').innerText = speed;
    fetch(`/update?speed=${speed}`);
  }
  function updateDirection(direction) {
    fetch(`/update?direction=${direction}`);
  }
  document.addEventListener('keydown', function(event) {
    switch(event.key.toLowerCase()) {
      case 'w': updateDirection(0); break;  // Forward
      case 's': updateDirection(1); break;  // Backward
      case 'a': updateDirection(2); break;  // Left
      case 'd': updateDirection(3); break;  // Right
      case ' ': updateDirection(4); break;  // Stop (space)
    }
  });
</script>
</body></html>  
)===";

WiFiServer server(80);    // HTTP server on port 80

#define SPEED_MAX 3 //max speed in Hz

float serverSpeed = 0;
int serverDirection = FORWARD;

void setup_server(){
  WiFi.softAP(ssid, "");  // Start WiFi in AP mode, no password required

  server.begin();

}


void handleRequest(WiFiClient client, String requestLine){
  Serial.println(requestLine);
  if(requestLine.length() == 0){
    client.print(body);
    return;
  }
  if(requestLine.startsWith("GET /update?")){
    if(requestLine.indexOf("speed=") > -1){
      int newSpeed = requestLine.substring(requestLine.indexOf("speed=") + 6).toInt();
      serverSpeed = newSpeed / 100.0 * SPEED_MAX;
    }
    if(requestLine.indexOf("direction=") > -1){
      serverDirection = requestLine.substring(requestLine.indexOf("direction=") + 10).toInt();
    }
    onUpdate(serverSpeed, serverDirection);
  }
  Serial.println("Unknown request");
}

void loop_server(){
  WiFiClient client = server.accept();
  if(client.connected()){
    String requestLine = "";
    while(client.available()){
      char c = client.read();
      requestLine += c;
      if(c == '\n'){
        break;
      }
    }
    handleRequest(client, requestLine);
  }
}
