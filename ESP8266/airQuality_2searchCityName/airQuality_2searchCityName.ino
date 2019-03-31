/*
 * ESP8266 강좌 시리즈
 * 미제먼지 데이터 받아오기 2편
 * 
 * 본 저작물은 '한국환경공단'에서 실시간 제공하는 '한국환경공단_대기오염정보'를 이용하였습니다.
 * https://www.data.go.kr/dataset/15000581/openapi.do
 */

#include <ESP8266WiFi.h> // ESP 8266 와이파이 라이브러리
#include <ESP8266HTTPClient.h> // HTTP 클라이언트

String sido = "서울"; // 서울, 부산, 대구, 인천, 광주, 대전, 울산, 경기, 강원, 충북, 충남, 전북, 전남, 경북, 경남, 제주, 세종 중 입력
String key = "";
String url = "http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getCtprvnMesureSidoLIst?sidoName=" + sido + "&searchCondition=HOUR&pageNo=1&numOfRows=200&ServiceKey=" + key;

float so2, co, o3, no2, pm10, pm25 = 0; // 아황산가스(이산화황), 일산화탄소, 오존, 이산화질소, 미세먼지, 초미세먼지
int score = 0;

void setup()
{
  // 시리얼 세팅
  Serial.begin(115200);
  Serial.println();

  // 와이파이 접속
  WiFi.begin("SSID", "00000000"); // 공유기 이름과 비밀번호

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // 와이파이 접속하는 동안 "." 출력
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) // 와이파이가 접속되어 있는 경우
  {
    WiFiClient client; // 와이파이 클라이언트 객체
    HTTPClient http; // HTTP 클라이언트 객체

    if (http.begin(client, url)) {  // HTTP
      // 서버에 연결하고 HTTP 헤더 전송
      int httpCode = http.GET();

      // httpCode 가 음수라면 에러
      if (httpCode > 0) { // 에러가 없는 경우
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString(); // 받은 XML 데이터를 String에 저장
          int cityIndex = payload.indexOf("영등포구");
          so2 = getNumber(payload, "<so2Value>", cityIndex); // 아황산가스(이산화황)
          co = getNumber(payload, "<coValue>", cityIndex); // 일산화탄소
          o3 = getNumber(payload, "<o3Value>", cityIndex); // 오존
          no2 = getNumber(payload, "<no2Value>", cityIndex); // 이산화질소
          pm10 = getNumber(payload, "<pm10Value>", cityIndex); // 미세먼지
          pm25 = getNumber(payload, "<pm25Value>", cityIndex); // 초미세먼지
        }
      } else {
        Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP] 접속 불가\n");
    }
    Serial.println(so2); // 아황산가스(이산화황)
    Serial.println(co); // 일산화탄소
    Serial.println(o3); // 오존
    Serial.println(no2); // 이산화질소
    Serial.println(pm10); // 미세먼지
    Serial.println(pm25); // 초미세먼지
    delay(600000);
  }
}

float getNumber(String str, String tag, int from) {
  float num = -1;
  int f = str.indexOf(tag, from) + tag.length();
  int t = str.indexOf("<", f);
  String s = str.substring(f, t);
  return s.toFloat();
}
