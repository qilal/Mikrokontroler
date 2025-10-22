import tkinter as tk
from tkinter import ttk, messagebox
import serial
import time
import threading

# Ganti COM7 sesuai port ESP32 kamu
PORT = 'COM7'
BAUD = 115200

try:
    esp32 = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)
except Exception as e:
    messagebox.showerror("Error", f"Gagal membuka port {PORT}\n{e}")
    esp32 = None


def kirim_data(data):
    """Kirim data ke ESP32 via serial"""
    if esp32 and esp32.is_open:
        esp32.write((str(data) + '\n').encode())  # kirim dengan newline
        print(f"Dikirim: {data}")
    else:
        messagebox.showerror("Error", "ESP32 tidak terhubung!")


def motor_on():
    kirim_data('1')
    status_label.config(text="Motor ON ✅", foreground="green")


def motor_off():
    kirim_data('0')
    status_label.config(text="Motor OFF ⛔", foreground="red")


def ubah_kecepatan(val):
    nilai = int(float(val))
    kirim_data(nilai)
    speed_value_label.config(text=f"{nilai}")


def baca_serial():
    """Baca data dari ESP32"""
    while True:
        if esp32 and esp32.in_waiting > 0:
            data = esp32.readline().decode().strip()
            if data:
                print(f"[ESP32] {data}")


# Jalankan thread pembacaan serial
if esp32:
    threading.Thread(target=baca_serial, daemon=True).start()


# ---------------- GUI ----------------
root = tk.Tk()
root.title("Kontrol Motor ESP32")
root.geometry("350x300")
root.resizable(False, False)

frame = ttk.Frame(root, padding=20)
frame.pack(fill="both", expand=True)

# Judul
title_label = ttk.Label(frame, text="Kontrol Motor ESP32", font=("Segoe UI", 14, "bold"))
title_label.pack(pady=10)

# Tombol ON/OFF
btn_frame = ttk.Frame(frame)
btn_frame.pack(pady=10)

btn_on = ttk.Button(btn_frame, text="Motor ON", command=motor_on)
btn_on.grid(row=0, column=0, padx=10)

btn_off = ttk.Button(btn_frame, text="Motor OFF", command=motor_off)
btn_off.grid(row=0, column=1, padx=10)

# Slider Kecepatan
slider_label = ttk.Label(frame, text="Atur Kecepatan Motor (PWM 0–255):")
slider_label.pack(pady=(20, 5))

speed_slider = ttk.Scale(frame, from_=0, to=255, orient="horizontal", command=ubah_kecepatan)
speed_slider.pack(fill="x", padx=10)

speed_value_label = ttk.Label(frame, text="0", font=("Segoe UI", 12))
speed_value_label.pack()

# Status label
status_label = ttk.Label(frame, text="Motor OFF ⛔", font=("Segoe UI", 12), foreground="red")
status_label.pack(pady=15)

# Tombol keluar
btn_exit = ttk.Button(frame, text="Keluar", command=root.destroy)
btn_exit.pack(pady=10)

root.mainloop()

# Tutup serial saat keluar
if esp32 and esp32.is_open:
    esp32.close()
