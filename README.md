# BHTS - Bee Hive Temperature Sensor

The project goal is to save temperature inside different kind of hive to determine with isoliation is the best.

## 🛠️ Hardware

- arduino mkr 1010 wifi
- sensor tmp36

## 🧐 Features

- Read temperature with arduino mkr 1010 wifi + tmp36 sensor

## 🛠️ Installation

1. Modify [sketch/arduino_secrets.h.sample](sketch/arduino_secrets.h.sample) and rename it to `arduino_secrets.h`
2. Modify [.env.sample](.env.sample) and rename it to `.env`

## 🙇 Acknowledgements

- [https://docs.influxdata.com/influxdb/cloud/api-guide/client-libraries/arduino/](https://docs.influxdata.com/influxdb/cloud/api-guide/client-libraries/arduino/)
- [https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)