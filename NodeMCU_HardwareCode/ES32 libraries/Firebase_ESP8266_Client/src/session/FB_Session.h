/**
 * Google's Firebase Data class, FB_Session.h version 1.2.11
 * 
 * This library supports Espressif ESP8266 and ESP32
 * 
 * Created January 18, 2022
 * 
 * This work is a part of Firebase ESP Client library
 * Copyright (c) 2022 K. Suwatchai (Mobizt)
 * 
 * The MIT License (MIT)
 * Copyright (c) 2022 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "FirebaseFS.h"

#ifndef FIREBASE_SESSION_H
#define FIREBASE_SESSION_H
#include <Arduino.h>
#include "Utils.h"

#include "rtdb/stream/FB_Stream.h"
#include "rtdb/stream/FB_MP_Stream.h"
#include "rtdb/QueueInfo.h"
#include "rtdb/QueueManager.h"

#include "signer/Signer.h"

using namespace mb_string;

#if defined(FIREBASE_ESP32_CLIENT) || defined(FIREBASE_ESP8266_CLIENT)

enum fb_esp_fcm_msg_type
{
  msg_single,
  msg_multicast,
  msg_topic
};

class FCMObject
{

#if defined(ESP32) || defined(ESP8266)
  friend class FIREBASE_CLASS;
#endif
  friend class FirebaseData;

public:
  FCMObject();
  ~FCMObject();

  /** Store Firebase Cloud Messaging's authentication credentials.
   * 
   * @param serverKey Server key found on Console: Project settings > Cloud Messaging
   * @param spi_ethernet_module SPI_ETH_Module struct data, optional for ESP8266 use with Ethernet module. 
   * 
   * SPI_ETH_Module struct data is for ESP8266 Ethernet supported module lwip interface.
   * The usage example for Ethernet.
   * 
   * #include <ENC28J60lwIP.h>
   * 
   * #define ETH_CS_PIN 16 //GPIO 16 connected to Ethernet module (ENC28J60) CS pin
   * 
   * ENC28J60lwIP eth(ETH_CS_PIN);
   * 
   * FirebaseData fbdo;
   * 
   * SPI_ETH_Module spi_ethernet_module;
   * spi_ethernet_module.enc28j60 = &eth;
   * 
   * fbdo.fcm.begin(FIREBASE_FCM_SERVER_KEY, &spi_ethernet_module);
   * 
   */
  template <typename T = const char *>
  void begin(T serverKey, SPI_ETH_Module *spi_ethernet_module = NULL) { mBegin(toStringPtr(serverKey), spi_ethernet_module); }

  /** Add recipient's device registration token or instant ID token.
   * 
   * @param deviceToken Recipient's device registration token to add that message will be sent to.
   */
  template <typename T = const char *>
  void addDeviceToken(T deviceToken) { mAddDeviceToken(toStringPtr(deviceToken)); }

  /** Remove the recipient's device registration token or instant ID token.
   * 
   * @param index Index (start from zero) of the recipient's device registration token that added to FCM Data Object of Firebase Data object.
   */
  void removeDeviceToken(uint16_t index);

  /** Clear all recipient's device registration tokens.
  */
  void clearDeviceToken();

  /** Set the notify message type information.
   * 
   * @param title The title text of notification message.
   * @param body The body text of notification message.
   */
  template <typename T1 = const char *, typename T2 = const char *>
  void setNotifyMessage(T1 title, T2 body) { mSetNotifyMessage(toStringPtr(title), toStringPtr(body)); }

  /** Set the notify message type information.
   * 
   * @param title The title text of notification message.
   * @param body The body text of notification message.
   * @param icon The name and/or included URI/URL of the icon to show on notifying message.
   */
  template <typename T1 = const char *, typename T2 = const char *, typename T3 = const char *>
  void setNotifyMessage(T1 title, T2 body, T3 icon) { mSetNotifyMessage(toStringPtr(title), toStringPtr(body), toStringPtr(icon)); }

  /** Set the notify message type information.
   * 
   * @param title The title text of notification message.
   * @param body The body text of notification message.
   * @param icon The name and/or included URI/URL of the icon to show on notifying message.
   * @param click_action The URL or intent to accept click event on the notification message.
   */
  template <typename T1 = const char *, typename T2 = const char *, typename T3 = const char *, typename T4 = const char *>
  void setNotifyMessage(T1 title, T2 body, T3 icon, T4 click_action) { mSetNotifyMessage(toStringPtr(title), toStringPtr(body), toStringPtr(icon), toStringPtr(click_action)); }

  /** add the custom key/value in the notify message type information.
   * 
   * @param key The key field in notification message.
   * @param value The value field in the notification message.
  */
  template <typename T1 = const char *, typename T2 = const char *>
  void addCustomNotifyMessage(T1 key, T2 value) { mAddCustomNotifyMessage(toStringPtr(key), toStringPtr(value)); }

  /** Clear all notify message information.
  */
  void clearNotifyMessage();

  /** Set the custom data message type information.
   * 
   * @param jsonString The JSON structured data string.
  */
  template <typename T = const char *>
  void setDataMessage(T jsonString) { mSetDataMessage(toStringPtr(jsonString)); }

  /** Set the custom data message type information.
   * 
   * @param json The FirebaseJson object.
  */
  void setDataMessage(FirebaseJson &json);

  /** Clear custom data message type information.
  */
  void clearDataMessage();

  /** Set the priority of the message (notification and custom data).
   * 
   * @param priority The priority string i.e. normal and high.
  */
  template <typename T = const char *>
  void setPriority(T priority) { mSetPriority(toStringPtr(priority)); }

  /** Set the collapse key of the message (notification and custom data).
   * 
   * @param key String of collapse key.
  */
  template <typename T = const char *>
  void setCollapseKey(T key) { mSetCollapseKey(toStringPtr(key)); }

  /** Set the Time To Live of the message (notification and custom data).
   * 
   * @param seconds Number of seconds from 0 to 2,419,200 (4 weeks).
  */
  void setTimeToLive(uint32_t seconds);

  /** Set the topic of the message will be sent to.
   * 
   * @param topic Topic string.
  */
  template <typename T = const char *>
  void setTopic(T topic) { mSetTopic(toStringPtr(topic)); }

  /** Get the send result.
   * 
   * @return string of payload returned from the server.
  */
  const char *getSendResult();

private:
  bool waitResponse(FirebaseData &fbdo);

  bool handleResponse(FirebaseData *fbdo);

  void rescon(FirebaseData &fbdo, const char *host);

  void fcm_begin(FirebaseData &fbdo);

  bool fcm_send(FirebaseData &fbdo, fb_esp_fcm_msg_type messageType);

  int fcm_sendHeader(FirebaseData &fbdo, size_t payloadSize);

  void fcm_preparePayload(FirebaseData &fbdo, fb_esp_fcm_msg_type messageType);

  void clear();

  void mBegin(MB_StringPtr serverKey, SPI_ETH_Module *spi_ethernet_module = NULL);

  void mAddDeviceToken(MB_StringPtr deviceToken);

  void mSetNotifyMessage(MB_StringPtr title, MB_StringPtr body);

  void mSetNotifyMessage(MB_StringPtr title, MB_StringPtr body, MB_StringPtr icon);

  void mSetNotifyMessage(MB_StringPtr title, MB_StringPtr body, MB_StringPtr icon, MB_StringPtr click_action);

  void mAddCustomNotifyMessage(MB_StringPtr key, MB_StringPtr value);

  void mSetDataMessage(MB_StringPtr jsonString);

  void mSetPriority(MB_StringPtr priority);

  void mSetCollapseKey(MB_StringPtr key);

  void mSetTopic(MB_StringPtr topic);

  bool prepareUtil();

  MB_String result;
  MB_String raw;
  MB_String idTokens;
  int _ttl = -1;
  uint16_t _index = 0;
  uint16_t _port = FIREBASE_PORT;
  UtilsClass *ut = nullptr;
  SPI_ETH_Module *_spi_ethernet_module = NULL;
  bool intUt = false;
};

#endif

class FirebaseData
{

#ifdef ENABLE_RTDB
  friend class FB_RTDB;
#endif
  friend class UtilsClass;

#if defined(FIREBASE_ESP_CLIENT)

#ifdef ENABLE_FCM
  friend class FB_CM;
#endif
#ifdef ENABLE_FB_STORAGE
  friend class FB_Storage;
#endif
#ifdef ENABLE_FIRESTORE
  friend class FB_Firestore;
#endif
#ifdef ENABLE_FB_FUNCTIONS
  friend class FB_Functions;
#endif
#ifdef ENABLE_GC_STORAGE
  friend class GG_CloudStorage;
#endif

#elif defined(FIREBASE_ESP32_CLIENT) || defined(FIREBASE_ESP8266_CLIENT)
#if defined(ESP32) || defined(ESP8266)
  friend class FIREBASE_CLASS;
#endif
#ifdef ENABLE_FCM
  friend class FCMObject;
#endif
#endif

#if defined(FIREBASE_ESP_CLIENT)
  friend class Firebase_ESP_Client;
#endif

public:
#ifdef ENABLE_RTDB
  typedef void (*StreamEventCallback)(FIREBASE_STREAM_CLASS);
  typedef void (*MultiPathStreamEventCallback)(FIREBASE_MP_STREAM_CLASS);
  typedef void (*StreamTimeoutCallback)(bool);
  typedef void (*QueueInfoCallback)(QueueInfo);
#endif

  FirebaseData();
  ~FirebaseData();

#if defined(ESP8266)
  /** Set the receive and transmit buffer memory size for secured mode BearSSL WiFi client.
   * 
   * @param rx The number of bytes for receive buffer memory for secured mode BearSSL (512 is minimum, 16384 is maximum).
   * @param tx The number of bytes for transmit buffer memory for secured mode BearSSL (512 is minimum, 16384 is maximum).
   * 
   * @note Set this option to false to support get large Blob and File operations.
  */
  void setBSSLBufferSize(uint16_t rx, uint16_t tx);
#endif

  /** Set the HTTP response size limit.
   * 
   * @param len The server response buffer size limit. 
  */
  void setResponseSize(uint16_t len);

  /** Set the Root certificate for a FirebaseData object.
   * 
   * @param ca PEM format certificate string.
  */
  void setCert(const char *ca);

  /** Pause/Unpause WiFiClient from all Firebase operations.
   * 
   * @param pause The boolean to set/unset pause operation.
   * @return Boolean type status indicates the success of operation.
  */
#ifdef ENABLE_RTDB
  bool pauseFirebase(bool pause);
#endif

  /** Check the pause status of FirebaseData object.
   * 
   * @return Boolean type value of pause status.
  */
#ifdef ENABLE_RTDB
  bool isPause();
#endif

  /** Get a WiFi client instance.
   * 
   * @return WiFi client instance.
  */
  WiFiClientSecure *getWiFiClient();

  /** Close the keep-alive connection of the internal WiFi client.
   * 
   * @note This will release the memory used by internal WiFi client.
  */
  void stopWiFiClient();

  /** Get the data type of payload returned from the server (RTDB only).
   * 
   * @return The one of these data type e.g. string, boolean, int, float, double, json, array, blob, file and null.
  */
#ifdef ENABLE_RTDB
  String dataType();
#endif

  /** Get the data type of payload returned from the server (RTDB only).
   * 
   * @return The enumeration value of fb_esp_rtdb_data_type.
   * fb_esp_rtdb_data_type_null or 1,
   * fb_esp_rtdb_data_type_integer or 2,
   * fb_esp_rtdb_data_type_float or 3,
   * fb_esp_rtdb_data_type_double or 4,
   * fb_esp_rtdb_data_type_boolean or 5,
   * fb_esp_rtdb_data_type_string or 6,
   * fb_esp_rtdb_data_type_json or 7,
   * fb_esp_rtdb_data_type_array or 8,
   * fb_esp_rtdb_data_type_blob or 9,
   * fb_esp_rtdb_data_type_file or 10
  */
#ifdef ENABLE_RTDB
  uint8_t dataTypeEnum();
#endif

  /** Get the event type of stream. (RTDB only)
   * 
   * @return The one of these event type e.g. put, patch, cancel, and auth_revoked.
   * 
   * @note The event type "put" indicated that data at an event path relative to the stream path was completely changed. The event path can be Getd by dataPath().
   * 
   * The event type "patch" indicated that data at the event path relative to stream path was updated. The event path can be Getd by dataPath().
   * 
   * The event type "cancel" indicated something wrong and cancel by the server.
   * 
   * The event type "auth_revoked" indicated the provided Firebase Authentication Data (Database secret) is no longer valid.
  */
#ifdef ENABLE_RTDB
  String eventType();
#endif

  /** Get the unique identifier (ETag) of RTDB data. (RTDB only)
   * 
   * @return String of unique identifier.
  */
#ifdef ENABLE_RTDB
  String ETag();
#endif

  /** Get the current stream path. (RTDB only)
   * 
   * @return The database streaming path.
  */
#ifdef ENABLE_RTDB
  String streamPath();
#endif

  /** Get the current data path. (RTDB only)
   * 
   * @return The database path which belongs to the server's returned payload.
   * 
   * @note The database path returned from this function in case of stream, also changed upon the child or parent's stream 
   * value changes.
  */
#ifdef ENABLE_RTDB
  String dataPath();
#endif
#if defined(FIREBASE_ESP_CLIENT)

  /** Get the metadata of a file in the Firebase storage data bucket.
   * 
   * @return The FileMetaInfo data of file.
   * 
   * @note The FileMetaInfo properties are
   * name - The file name
   * bucket - The storage bucket id
   * generation - The timestamp (millisecond) of file
   * contentType - The content type or mime type of file
   * size - The size of file in byte
   * etag - The ETag of file
   * crc32c - The CRC32 of file
   * downloadTokens - The download token
  */
#if defined(ENABLE_GC_STORAGE) || defined(ENABLE_FB_STORAGE)
  FileMetaInfo metaData();
#endif

  /** Get the files info in the Firebase storage data bucket.
   * 
   * @return The pointer to FileItem array (FileList) of files in the data bucket.
   * 
   * @note The FileItem properties are
   * name - The file name
   * bucket - The storage bucket id
  */
#ifdef ENABLE_FB_STORAGE
  FileList *fileList();
#endif

  /** Get the download URL from the currently uploaded file in the Firebase storage data bucket.
   * 
   * @return The URL to download file.
  */
#if defined(ENABLE_FB_STORAGE) || defined(ENABLE_GC_STORAGE)
  String downloadURL();
#endif

#endif

  /** Get the error reason String from the process.
   * 
   * @return The error description string (String object).
  */
  String errorReason();

  /** Return the integer data of server returned payload (RTDB only).
   * 
   * @return integer value.
  */
#ifdef ENABLE_RTDB
  int intData();
#endif

  /** Return the float data of server returned payload (RTDB only).
   * 
   * @return Float value.
  */
#ifdef ENABLE_RTDB
  float floatData();
#endif

  /** Return the double data of server returned payload (RTDB only).
   * 
   * @return double value.
  */
#ifdef ENABLE_RTDB
  double doubleData();
#endif

  /** Return the Boolean data of server returned payload (RTDB only).
   * 
   * @return Boolean value.
  */
#ifdef ENABLE_RTDB
  bool boolData();
#endif

  /** Return the String data of server returned payload (RTDB only).
   * 
   * @return String (String object).
  */
#ifdef ENABLE_RTDB
  String stringData();
#endif

  /** Return the JSON String data of server returned payload (RTDB only).
   * 
   * @return String (String object).
  */
#ifdef ENABLE_RTDB
  String jsonString();
#endif

  /** Return the Firebase JSON object of server returned payload (RTDB only).
   * 
   * @return FirebaseJson object.
  */
#ifdef ENABLE_RTDB
  FirebaseJson &jsonObject();
#endif

  /** Return the Firebase JSON object pointer of server returned payload (RTDB only).
   * 
   * @return FirebaseJson object pointer.
  */
#ifdef ENABLE_RTDB
  FirebaseJson *jsonObjectPtr();
#endif

  /** Return the Firebase JSON Array object of server returned payload (RTDB only).
   * 
   * @return FirebaseJsonArray object.
  */
#ifdef ENABLE_RTDB
  FirebaseJsonArray &jsonArray();
#endif

  /** Return the Firebase JSON Array object pointer of server returned payload (RTDB only).
   * 
   * @return FirebaseJsonArray object pointer.
  */
#ifdef ENABLE_RTDB
  FirebaseJsonArray *jsonArrayPtr();
#endif

  /** Return the pointer to blob data (uint8_t) array of server returned payload (RTDB only).
   * 
   * @return Dynamic array of 8-bit unsigned integer i.e. std::vector<uint8_t>.
  */
#ifdef ENABLE_RTDB
  std::vector<uint8_t> *blobData();
#endif

  /** Return the file stream of server returned payload (RTDB only).
   * 
   * @return the file stream.
  */
#ifdef ENABLE_RTDB
  fs::File fileStream();
#endif

  /**
   * Get the value by specific type from FirebaseJsonData object (RTDB only).
   * This should call after parse or get function.
  */
#ifdef ENABLE_RTDB
  template <typename T>
  auto to() -> typename enable_if<is_num_int<T>::value || is_num_float<T>::value || is_bool<T>::value, T>::type
  {
    if (_ss.rtdb.resp_data_type == fb_esp_data_type::d_string)
      setRaw(true); //if double quotes string, trim it.

    if (_ss.rtdb.raw.length() > 0)
    {
      if (_ss.rtdb.resp_data_type == fb_esp_data_type::d_boolean)
        mSetResBool(strcmp(_ss.rtdb.raw.c_str(), num2Str(true, -1)) == 0);
      else if (_ss.rtdb.resp_data_type == fb_esp_data_type::d_integer || _ss.rtdb.resp_data_type == fb_esp_data_type::d_float || _ss.rtdb.resp_data_type == fb_esp_data_type::d_double)
      {
        mSetResInt(_ss.rtdb.raw.c_str());
        mSetResFloat(_ss.rtdb.raw.c_str());
      }
    }

    if (_ss.rtdb.req_data_type == d_timestamp)
    {
      if (is_num_uint64<T>::value)
        return iVal.uint64;
      if (is_num_int32<T>::value || is_num_uint32<T>::value || is_num_int64<T>::value || is_num_uint64<T>::value)
        return iVal.uint64 / 1000;
      else
        return 0;
    }

    if (is_bool<T>::value)
      return iVal.int32 > 0;
    else if (is_num_int8<T>::value)
      return iVal.int8;
    else if (is_num_uint8<T>::value)
      return iVal.uint8;
    else if (is_num_int16<T>::value)
      return iVal.int16;
    else if (is_num_uint16<T>::value)
      return iVal.uint16;
    else if (is_num_int32<T>::value)
      return iVal.int32;
    else if (is_num_uint32<T>::value)
      return iVal.uint32;
    else if (is_num_int64<T>::value)
      return iVal.int64;
    else if (is_num_uint64<T>::value)
      return iVal.uint64;
    else if (is_same<T, float>::value)
      return fVal.f;
    else if (is_same<T, double>::value)
      return fVal.d;
    else
      return 0;
  }

  template <typename T>
  auto to() -> typename enable_if<is_const_chars<T>::value || is_std_string<T>::value || is_arduino_string<T>::value || is_mb_string<T>::value, T>::type
  {
    if (_ss.rtdb.resp_data_type == fb_esp_data_type::d_string)
      setRaw(true);
    return _ss.rtdb.raw.c_str();
  }

  template <typename T>
  auto to() -> typename enable_if<is_same<T, FirebaseJson *>::value, FirebaseJson *>::type
  {
    if (!_ss.jsonPtr)
      _ss.jsonPtr = new FirebaseJson();

    if (_ss.rtdb.resp_data_type == d_json)
    {
      _ss.jsonPtr->clear();
      if (_ss.arrPtr)
        _ss.arrPtr->clear();
      _ss.jsonPtr->setJsonData(_ss.rtdb.raw.c_str());
    }
    return _ss.jsonPtr;
  }

  template <typename T>
  auto to() -> typename enable_if<is_same<T, FirebaseJsonData *>::value, FirebaseJsonData *>::type
  {
    if (!_ss.dataPtr)
      _ss.dataPtr = new FirebaseJsonData();
    return _ss.dataPtr;
  }

  template <typename T>
  auto to() -> typename enable_if<is_same<T, FirebaseJson>::value, FirebaseJson &>::type
  {
    return *to<FirebaseJson *>();
  }

  template <typename T>
  auto to() -> typename enable_if<is_same<T, FirebaseJsonArray *>::value, FirebaseJsonArray *>::type
  {
    if (!_ss.arrPtr)
      _ss.arrPtr = new FirebaseJsonArray();

    if (_ss.rtdb.resp_data_type == d_array)
    {
      if (_ss.jsonPtr)
        _ss.jsonPtr->clear();
      _ss.arrPtr->clear();
      _ss.arrPtr->setJsonArrayData(_ss.rtdb.raw.c_str());
    }

    return _ss.arrPtr;
  }

  template <typename T>
  auto to() -> typename enable_if<is_same<T, FirebaseJsonArray>::value, FirebaseJsonArray &>::type
  {
    return *to<FirebaseJsonArray *>();
  }

  template <typename T>
  auto to() -> typename enable_if<is_same<T, std::vector<uint8_t> *>::value, std::vector<uint8_t> *>::type
  {
    return _ss.rtdb.blob;
  }

#if defined(FLASH_FS)
  template <typename T>
  auto to() -> typename enable_if<is_same<T, fs::File>::value, fs::File>::type
  {
    if (_ss.rtdb.resp_data_type == fb_esp_data_type::d_file && init())
    {
      int ret = ut->mbfs->open(pgm2Str(fb_esp_pgm_str_184), mbfs_type mem_storage_type_flash, mb_file_open_mode_read);
      if (ret < 0)
        _ss.http_code = ret;
    }

    return ut->mbfs->getFlashFile();
  }
#endif

#endif

  /** Return the new appended node's name or key of server returned payload when calling pushXXX function (RTDB only).
   * 
   * @return String (String object).
  */
#ifdef ENABLE_RTDB
  String pushName();
#endif

  /** Get the stream connection status (RTDB only).
   * 
   * @return Boolean type status indicates whether the Firebase Data object is working with a stream or not.
  */
#ifdef ENABLE_RTDB
  bool isStream();
#endif

  /** Get the server connection status.
   * 
   * @return Boolean type status indicates whether the Firebase Data object is connected to the server or not.
  */
  bool httpConnected();

  /** Get the timeout event of the server's stream (30 sec is the default) (RTDB only). 
   * Nothing to do when stream connection timeout, the stream connection will be automatically resumed.
   * 
   * @return Boolean type status indicates whether the stream was a timeout or not.
  */
#ifdef ENABLE_RTDB
  bool streamTimeout();
#endif

  /** Get the availability of data or payload returned from the server (RTDB only).
   * 
   * @return Boolean type status indicates whether the server return the new payload or not.
  */
#ifdef ENABLE_RTDB
  bool dataAvailable();
#endif

  /** Get the availability of stream event-data payload returned from the server (RTDB only).
   * 
   * @return Boolean type status indicates whether the server returns the stream event-data 
   * payload or not.
  */
#ifdef ENABLE_RTDB
  bool streamAvailable();
#endif

  /** Get the matching between data type that intends to get from/store to database and the server's return payload data type (RTDB only).
   * 
   * @return Boolean type status indicates whether the type of data being get from/store to database 
   * and the server's returned payload is matched or not.
   * 
   * @note Data type checking was disable by default, which can be enabled via the Firebase Config e.g.
   * config.rtdb.data_type_stricted = true
  */
#ifdef ENABLE_RTDB
  bool mismatchDataType();
#endif

  /** Get the HTTP status code returned from the server.
   * 
   * @return integer number of HTTP status.
  */
  int httpCode();

  /** Get the HTTP payload length returned from the server.
   * 
   * @return integer number of payload length.
  */
  int payloadLength();

  /** Get the maximum size of HTTP payload length returned from the server.
   * 
   * @return integer number of max payload length.
  */
  int maxPayloadLength();

  /** Check the overflow of the returned payload data buffer (RTDB only).
   * 
   * @return The overflow status. 
   * 
   * @note Total default HTTP response buffer size is 400 bytes which can be set through FirebaseData.setResponseSize.
  */
  bool bufferOverflow();

  /** Get the name (full path) of the backup file in SD card/Flash memory (RTDB only).
   * 
   * @return String (String object) of the file name that stores on SD card/Flash memory after backup operation.
  */
#ifdef ENABLE_RTDB
  String getBackupFilename();
#endif

  /** Get the size of the backup file.
   * 
   * @return Size of backup file in byte after backup operation.
  */
#ifdef ENABLE_RTDB
  size_t getBackupFileSize();
#endif

  /** Clear or empty data in Firebase Data object.
  */
  void clear();

  /** Get the error description for file transferring (push file, set file, backup and restore) (RTDB only).
   * 
   * @return Error description string (String object).
  */
  String fileTransferError();

  /** Return the server's payload data.
   * 
   * @return Payload string (String object).
  */
  String payload();

  FB_TCP_Client tcpClient;

#if defined(FIREBASE_ESP32_CLIENT) || defined(FIREBASE_ESP8266_CLIENT)
#ifdef ENABLE_FCM
  FCMObject fcm;
#endif
#endif

private:
#ifdef ENABLE_RTDB
  StreamEventCallback _dataAvailableCallback = NULL;
  MultiPathStreamEventCallback _multiPathDataCallback = NULL;
  StreamTimeoutCallback _timeoutCallback = NULL;
  QueueInfoCallback _queueInfoCallback = NULL;
#endif
#if defined(FIREBASE_ESP_CLIENT)
#ifdef ENABLE_FB_FUNCTIONS
  FunctionsOperationCallback _functionsOperationCallback = NULL;
#endif
#endif

  UtilsClass *ut = nullptr;
  MB_File *mbfs = nullptr;
  bool intCfg = false;
  unsigned long last_reconnect_millis = 0;

  uint16_t reconnect_tmo = 10 * 1000;

  SPI_ETH_Module *_spi_ethernet_module = NULL;

#ifdef ENABLE_RTDB
  QueueManager _qMan;
  union IVal
  {
    std::uint64_t uint64;
    std::int64_t int64;
    std::uint32_t uint32;
    std::int32_t int32;
    std::int16_t int16;
    std::uint16_t uint16;
    std::int8_t int8;
    std::uint8_t uint8;
  };

  struct FVal
  {
    double d = 0;
    float f = 0;
    void setd(double v)
    {
      d = v;
      f = static_cast<float>(v);
    }

    void setf(float v)
    {
      f = v;
      d = static_cast<double>(v);
    }
  };

  IVal iVal = {0};
  FVal fVal;
#endif
  struct fb_esp_session_info_t _ss;

  void ethDNSWorkAround(SPI_ETH_Module *spi_ethernet_module, const char *host, int port);
  bool ethLinkUp(SPI_ETH_Module *spi_ethernet_module = NULL);
  void closeSession();
  bool handleStreamRead();
  void checkOvf(size_t len, struct server_response_data_t &resp);
  int tcpSend(const char *data);
  int tcpSendChunk(const char *data, int &index, size_t len);
  bool reconnect(unsigned long dataTime = 0);
  MB_String getDataType(uint8_t type);
  MB_String getMethod(uint8_t method);
  bool tokenReady();
  void setTimeout();
  void setSecure();
  bool validRequest(const MB_String &path);
  void addQueue(struct fb_esp_rtdb_queue_info_t *qinfo);
#ifdef ENABLE_RTDB
  void clearQueueItem(QueueItem *item);
  void sendStreamToCB(int code);
  void mSetResInt(const char *value);
  void mSetResFloat(const char *value);
  void mSetResBool(bool value);
#endif

  bool init();
  void setRaw(bool trim);
};

#endif
