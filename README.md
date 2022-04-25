# Dashboard untuk Arduino IDE

Pustaka ini digunakan Arduino IDE untuk menghubungkan perangkat ke platform Dashboard menggunakan protokol MQTT berdasarkan [PubSubClient](https://github.com/knolleary/pubsubclient)

## Contoh (Examples)

Pustaka menyertakan contoh kode program. Lihat Berkas -> Contoh -> Dashboard (File -> Example -> Dashboard) pada Arduino IDE.

## Limitasi (Limitations)

 - Hanya dapat melakukan publish dengan QoS 0. Dapat melakukan subscribe pada QoS 0 atau 1.
 - Ukuran maksimum data yang dikirim termasuk header adalah **256 byte** secara bawaan (default). Dapat diatur dengan memanggil `PubSubClient::setBufferSize(size)`.
 - Interval keepalive diatur ke 15 detik secara bawaan (default). Dapat diatur dengan memanggil `PubSubClient::setKeepAlive(keepAlive)`.
 - Klien menggunakan MQTT 3.1.1 secara bawaan (default).


## Perangkat Yang Didukung

Pustaka menggunakan API Arduino Ethernet Client untuk berinteraksi dengan perangkat keras jaringan. Artinya pustaka ini dapat digunakan pada perangkat keras apapun yang memiliki interaktifitas API tersebut termasuk papan dan shield seperti:

 - Arduino Ethernet
 - Arduino Ethernet Shield
 - Arduino YUN – gunakan include `YunClient` di `EthernetClient`, dan pastikan untuk melakukan `Bridge.begin()` dahulu
 - Arduino WiFi Shield - jika Anda ingin mengirim data > 90 bytes dengan shield ini,
   aktifkan the `MQTT_MAX_TRANSFER_SIZE` di header.
 - Sparkfun WiFly Shield – [library](https://github.com/dpslwk/WiFly)
 - TI CC3000 WiFi - [library](https://github.com/sparkfun/SFE_CC3000_Library)
 - Intel Galileo/Edison
 - ESP8266
 - ESP32

## License

Kode program dilisensikan dibawah GNU GENERAL PUBLIC LICENSE
