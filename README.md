# pAsync-Keylogger
An extremely small keylogger that uses an indirect call to 'GetAsyncKeyState' and exfils to a python server

# Features
- Randomly generated XOR encryption key for every log sent (no hard-coded key)
- Server bruteforces key upon reciept
- Indirect calls to GetAsyncKeyState to lower detection
- Exfiltrates data every minute to IP:Port (Default: 127.0.0.1:6782)
- 18.5kb size
