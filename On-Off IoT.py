import paho.mqtt.client as mqtt
import serial
import time

# ----- SERIAL -----
arduino = serial.Serial('COM7', 115200, timeout=1)
time.sleep(2)

# ----- MQTT -----
broker = "broker.emqx.io"
port = 1883
topic_control = "iot/motor/control"
topic_speed = "iot/motor/speed"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Terhubung ke broker MQTT")
        client.subscribe(topic_control)
        client.subscribe(topic_speed)
        print(f"Berlangganan ke: {topic_control}, {topic_speed}")
    else:
        print(f"❌ Gagal terhubung, kode error: {rc}")

def on_message(client, userdata, msg):
    pesan = msg.payload.decode().strip().upper()
    topic = msg.topic

    if topic == topic_control:
        if pesan == "ON":
            arduino.write(b'1')
            print("Motor ON")
        elif pesan == "OFF":
            arduino.write(b'0')
            print("Motor OFF")

    elif topic == topic_speed:
        try:
            speed_value = int(pesan)
            if 0 <= speed_value <= 255:
                arduino.write(str(speed_value).encode())  # kirim angka ke Arduino
                print(f"⚙️ Kecepatan motor diatur: {speed_value}")
            else:
                print("⚠️ Nilai kecepatan harus antara 0–255")
        except ValueError:
            print("⚠️ Pesan kecepatan tidak valid:", pesan)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker, port, 60)

print("Menghubungkan ke broker MQTT...")
try:
    client.loop_forever()
except KeyboardInterrupt:
    arduino.close()
    print("❌ Program dihentikan.")
