#include "ServoResource.h"
#include <vector>

std::vector<ServoResource> servoData;

void getAllServos(AsyncWebServerRequest *request)
{
  StaticJsonDocument<1024> doc;
  JsonArray array = doc.to<JsonArray>();
  
  for (ServoResource servoResource : servoData) {
    JsonObject obj = array.createNestedObject();
    obj["id"] = servoResource.id;
    obj["angle"] = servoResource.angle;
  }
  
  String message;
  serializeJson(doc, message);
  
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
  addCorsHeaders(response);
  request->send(response);
}
void getServoById(AsyncWebServerRequest *request)
{
  int id = GetIdFromURL(request, "/servos/");

  StaticJsonDocument<200> doc;
  
  for (ServoResource servoResource : servoData) {
    if (servoResource.id == id) {
      doc["id"] = servoResource.id;
      doc["angle"] = servoResource.angle;
      break;
    }
  }
  
  String message;
  serializeJson(doc, message);
  
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
  addCorsHeaders(response);
  request->send(response);
}

void getServo(AsyncWebServerRequest *request) {
  
  if(request->url().indexOf("/servos/") != -1)
  {
    getServoById(request);
  }
  else {
    getAllServos(request);
  }
}
void createServo(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{ 
  String bodyContent = GetBodyContent(data, len);
  
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  int id = doc["id"];
  float angle = doc["angle"];
  ServoResource servoResource(id, angle);
  servoData.push_back(servoResource);
  
  String message = "Create servo with id " + String(id) + " and angle " + String(angle);
  Serial.println(message);
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
  addCorsHeaders(response);
  request->send(response);
}

void updateServo(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{
  int id = GetIdFromURL(request, "/servos/");
  String bodyContent = GetBodyContent(data, len);
   
  StaticJsonDocument<200> doc;
  StaticJsonDocument<200> outputDoc;

  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  float angle = doc["angle"];
  
  for (ServoResource& servoResource : servoData) {
    if (servoResource.id == id) {
      servoResource.angle = angle;

      outputDoc["id"] = servoResource.id;
      outputDoc["angle"] = servoResource.angle;
      break;
    }
  }
  String message;
  serializeJson(outputDoc, message);

  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", message);
  addCorsHeaders(response);
  request->send(response);
}

void deleteServo(AsyncWebServerRequest *request) {
  int id = GetIdFromURL(request, "/servos/");

  for (auto it = servoData.begin(); it != servoData.end(); ++it) {
    if (it->id == id) {
      servoData.erase(it);
      break;
    }
  }
  
  String message = "Delete servo with id " + String(id);
  Serial.println(message);
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
  addCorsHeaders(response);
  request->send(response);
}

