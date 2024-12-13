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

  <h3>Angle Offsets</h3>
  Main Angle Offset: <input type="number" id="mainOffset" value="0" step="0.1">
  <button onclick="setMainOffset()">Set Main Offset</button><br><br>
  Secondary Angle Offset: <input type="number" id="secondaryOffset" value="0" step="0.1">
  <button onclick="sendSecondaryOffset()">Set Secondary Offset</button>

  <h3>Obsticle Avoidance</h3>
  <button onclick="setObsticleAvoidance(1)">Enable</button>
  <button onclick="setObsticleAvoidance(0)">Disable</button>

  <h3>Start Activity</h3>
  <button onclick="startActivity()">Start Activity</button>

  <h3>Wall Following</h3>
  <button onclick="setWallFollowing(1)">Enable</button>
  <button onclick="setWallFollowing(0)">Disable</button>

  <h3>Go Forward</h3>
  <button onclick="goForward()">Go Forward</button>

  <h3>Go Backward</h3>
  <button onclick="goBackward()">Go Backward</button>

  <h3>Turn Left</h3>
  <button onclick="turnLeft()">Turn Left</button>

  <h3>Turn Right</h3>
  <button onclick="turnRight()">Turn Right</button>

  <h3>Set X Offset</h3>
  X: <input type="number" id="setXOffset" value="0">
  <button onclick="setXOffset()">Set X Offset</button>

  <h3>Set Y Offset</h3>
  Y: <input type="number" id="setYOffset" value="0">
  <button onclick="setYOffset()">Set Y Offset</button>
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

  function setMainOffset() {
    const offset = document.getElementById('mainOffset').value;
    fetch(`/update?cmd=3&offset=${offset}`);  // SET_MAIN_ANGLE_OFFSET_COMMAND_CODE
  }
  
  function sendSecondaryOffset() {
    const offset = document.getElementById('secondaryOffset').value;
    fetch(`/update?cmd=4&offset=${offset}`);  // SET_SECONDARY_ANGLE_OFFSET_COMMAND_CODE
  }

  function setObsticleAvoidance(enabled) {
    fetch(`/update?cmd=5&enabled=${enabled}`);  // SET_OBSTICLE_AVOIDANCE_COMMAND_CODE
  }

  function startActivity() {
    fetch(`/update?cmd=11`);  // START_ACTIVITY_COMMAND_CODE
  }

  function setWallFollowing(enabled) {
    fetch(`/update?cmd=6&enabled=${enabled}`);  // SET_WALL_FOLLOWING_ENABLED_COMMAND_CODE
  }

  function goForward() {
    fetch(`/update?cmd=7`);  // GO_FORWARD_COMMAND_CODE
  }

  function goBackward() {
    fetch(`/update?cmd=8`);  // GO_BACKWARD_COMMAND_CODE
  }

  function turnLeft() {
    fetch(`/update?cmd=9`);  // TURN_LEFT_COMMAND_CODE
  }

  function turnRight() {
    fetch(`/update?cmd=10`);  // TURN_RIGHT_COMMAND_CODE
  }

  function setXOffset() {
    const offset = document.getElementById('setXOffset').value;
    fetch(`/update?cmd=12&offset=${offset}`);  // SET_X_OFFSET_COMMAND_CODE
  }

  function setYOffset() {
    const offset = document.getElementById('setYOffset').value;
    fetch(`/update?cmd=13&offset=${offset}`);  // SET_Y_OFFSET_COMMAND_CODE
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
      
      case SET_MAIN_ANGLE_OFFSET_COMMAND_CODE: {
        float offset = requestLine.substring(requestLine.indexOf("offset=") + 7).toFloat();
        serverOnUpdate(SET_MAIN_ANGLE_OFFSET_COMMAND_CODE, offset, 0, 0);
        break;
      }
      
      case SET_SECONDARY_ANGLE_OFFSET_COMMAND_CODE: {
        float offset = requestLine.substring(requestLine.indexOf("offset=") + 7).toFloat();
        serverOnUpdate(SET_SECONDARY_ANGLE_OFFSET_COMMAND_CODE, offset, 0, 0);
        break;
      }

      case SET_OBSTICLE_AVOIDANCE_COMMAND_CODE: {
        int enabled = requestLine.substring(requestLine.indexOf("enabled=") + 7).toInt();
        serverOnUpdate(SET_OBSTICLE_AVOIDANCE_COMMAND_CODE, enabled, 0, 0);
        break;
      }

      case START_ACTIVITY: {
        serverOnUpdate(START_ACTIVITY, 0, 0, 0);
        break;
      }

      case SET_WALL_FOLLOWING_ENABLED_COMMAND_CODE: {
        int enabled = (requestLine.indexOf("enabled=1") != -1) ? 1 : 0;
        Serial.printf("Setting wall following enabled to %d\n", enabled);
        serverOnUpdate(SET_WALL_FOLLOWING_ENABLED_COMMAND_CODE, enabled, 0, 0);
        break;
      }

      case GO_FORWARD_COMMAND_CODE: {
        serverOnUpdate(GO_FORWARD_COMMAND_CODE, 0, 0, 0);
        break;
      }

      case GO_BACKWARD_COMMAND_CODE: {
        serverOnUpdate(GO_BACKWARD_COMMAND_CODE, 0, 0, 0);
        break;
      }

      case TURN_LEFT_COMMAND_CODE: {
        serverOnUpdate(TURN_LEFT_COMMAND_CODE, 0, 0, 0);
        break;
      }

      case TURN_RIGHT_COMMAND_CODE: {
        serverOnUpdate(TURN_RIGHT_COMMAND_CODE, 0, 0, 0);
        break;
      }
      
      case SET_X_OFFSET: {
        float offset = requestLine.substring(requestLine.indexOf("offset=") + 7).toFloat();
        serverOnUpdate(SET_X_OFFSET, offset, 0, 0);
        break;
      }

      case SET_Y_OFFSET: {
        float offset = requestLine.substring(requestLine.indexOf("offset=") + 7).toFloat();
        serverOnUpdate(SET_Y_OFFSET, offset, 0, 0);
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
