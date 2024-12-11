#include "MyServer.h"
#include "Defaults.h"
#include "Vive.h"

const char* ssid = "Kepler 186-f";  // AP mode SSID, no password needed

const char body[] PROGMEM = R"===( 
<!DOCTYPE html>  
<html><body>        
<h1>Robot Control</h1>

<div id="position-display">
  <h3>Current Position</h3>
  <p>X: <span id="currentX">--</span></p>
  <p>Y: <span id="currentY">--</span></p>
  <p>Angle: <span id="currentAngle">--</span>°</p>
</div>

<div id="controls">
  <button onclick="sendCommand('abort')">ABORT</button><br><br>
  
  <h3>Go To Position</h3>
  X: <input type="number" id="gotoX" value="0">
  Y: <input type="number" id="gotoY" value="0">
  Speed: <input type="number" id="gotoSpeed" value="1" min="0" max="3">
  <button onclick="sendGoto()">GO TO</button><br><br>
  
  <h3>Turn To Angle</h3>
  Angle: <input type="number" id="turnAngle" value="0">
  <button onclick="sendTurnTo()">TURN TO</button>
</div>

<script>
  function sendCommand(cmd) {
    if (cmd === 'abort') {
      fetch('/update?cmd=0');  // ABORT_COMMAND_CODE
    }
  }
  
  function sendGoto() {
    const x = document.getElementById('gotoX').value;
    const y = document.getElementById('gotoY').value;
    const speed = document.getElementById('gotoSpeed').value;
    fetch(`/update?cmd=1&x=${x}&y=${y}&speed=${speed}`);  // GOTO_COMMAND_CODE
  }
  
  function sendTurnTo() {
    const angle = document.getElementById('turnAngle').value;
    fetch(`/update?cmd=2&angle=${angle}`);  // TURNTO_COMMAND_CODE
  }

  // Add position update functionality
  async function updatePosition() {
    try {
      const response = await fetch('/position');
      const data = await response.json();
      document.getElementById('currentX').textContent = data.x;
      document.getElementById('currentY').textContent = data.y;
      document.getElementById('currentAngle').textContent = data.angle.toFixed(1);
    } catch (e) {
      console.error('Failed to update position:', e);
    }
  }

  // Update position every second
  setInterval(updatePosition, 1000);
  updatePosition(); // Initial update
</script>
</body></html>  
)===";

WiFiServer server(80);    // HTTP server on port 80

#define SPEED_MAX 3 //max speed in Hz

void (*serverOnUpdate)(int command, float arg1, float arg2, float arg3);

void setup_server(void (*onUpdate)(int command, float arg1, float arg2, float arg3)){
  Serial.println("Setting up server");
  serverOnUpdate = onUpdate;
  WiFi.softAP(ssid, "");
  server.begin();
  Serial.printf("Server started on %s\n", WiFi.softAPIP().toString().c_str());
}

void handleRequest(WiFiClient client, String requestLine){
  Serial.println(requestLine);
  if(requestLine.length() == 0){
    client.print(body);
    return;
  }
  
  if(requestLine.startsWith("GET /position")) {
    // Send position data as JSON
    String json = "{\"x\":" + String(car_position_x) + 
                 ",\"y\":" + String(car_position_y) + 
                 ",\"angle\":" + String(car_angle) + "}";
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(json);
    return;
  }
  
  if(requestLine.startsWith("GET /update?")){
    int cmd = requestLine.substring(requestLine.indexOf("cmd=") + 4).toInt();
    
    switch(cmd) {
      case ABORT_COMMAND_CODE:
        serverOnUpdate(ABORT_COMMAND_CODE, 0, 0, 0);
        break;
        
      case GOTO_COMMAND_CODE: {
        float x = requestLine.substring(requestLine.indexOf("x=") + 2).toFloat();
        float y = requestLine.substring(requestLine.indexOf("y=") + 2).toFloat();
        float speed = requestLine.substring(requestLine.indexOf("speed=") + 6).toFloat();
        serverOnUpdate(GOTO_COMMAND_CODE, x, y, speed);
        break;
      }
      
      case TURNTO_COMMAND_CODE: {
        float angle = requestLine.substring(requestLine.indexOf("angle=") + 6).toFloat();
        serverOnUpdate(TURNTO_COMMAND_CODE, angle, DEFAULT_TURN_SPEED, 0);
        break;
      }
    }
    return;
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
