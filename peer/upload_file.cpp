#include "headers.h"
#include "cmd_defs.h"

static int status_ok(char* payload) {
    console_write(payload);
    return 0;
}

static int invalid_format(char* payload) {
    console_write(payload);
    return 0;
}

static int not_found(char* payload) {
    console_write(payload);
    return 0;
}

static int forbidden(char* payload) {
    console_write(payload);
    return 0;
}

static int exists(char* payload) {
    console_write(payload);
    return 0;
}

static int unauthorized(char* payload) {
    console_write(payload);
    return 0;
}

static map<string, int(*)(char*)> response_handler = {
    { STATUS_OK_CODE,       status_ok },
    { INVALID_FORMAT_CODE,  invalid_format },
    { NOT_FOUND_CODE,       not_found },
    { FORBIDDEN_CODE,       forbidden },
    { CONFLICT_CODE,        exists },
    { UNAUTHORIZED_CODE,    unauthorized }
};

/*
    uploads file to tracker in format upload_file <file_path> <group_id>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT), 404(NOT_FOUND), 403(FORBIDDEN), 409(CONFLICT_CODE), 401(UNAUTHORIZED)
*/
int upload_file(char* cmd) {
    char* temp;
    strcpy(temp, cmd);
    string file_path = get_tokens(temp, WHITESPACE)[1];
    long long file_size = get_file_size(file_path.c_str());
    if(file_size == -1) {
        console_write("File not found.\n");
        return 0;
    }

    long long chunks_num = (file_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    string file_name = file_path.substr(file_path.find_last_of('/') + 1);
    file_paths[file_name] = file_path;
    
    if(send(peer_sock, (string(cmd) + " " + to_string(chunks_num)).c_str(), SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");
    
    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.\n");
    
    char* response_code = strtok(response, WHITESPACE);
    if(!response_code) {
        console_write("Unexpected response from tracker. No response code found.\n");
        return 0;
    }
    
    char* payload = strtok(NULL, "");
    if(!payload) {
        console_write("Unexpected response from tracker. No payload found.\n");
        return 0;
    }

    if(response_handler.find(response_code) == response_handler.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[response_code](payload);
}