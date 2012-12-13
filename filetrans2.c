/**
 * filetrans2.c : 簡易檔案傳輸工具程式 v2
 *
 * Copyright (c) 2012 鄭新霖 aka Hsin-lin Cheng <lancetw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "filetrans2.h"
#include "server2.h"
#include "client2.h"


/* 預設監聽接收檔案 (Server mode)，引數指定檔案上傳或下載 (Client mode) */

int main(int argc, char *argv[]) {
                    
    bzero(&filename, BUFF_LEN);
    
    if (argc > 3) {
        /* 使用參數模式 */
        strcpy(server, argv[1]);
        printf("正在連線到 %s:%d ...\n", server, PORT);
        
        if (strlen(argv[2]) == 1) {
            strcpy(&mode, argv[2]);
        }
        
        if (strlen(argv[3]) > 0) {
            strcpy(filename, argv[3]);
        }
        
        if (mode == 'D' || mode == 'U') {
            client_mode(server, mode, filename);
        } else {
            printf("請提供正確的上傳或下載模式（U/D）！\n");
            exit(EXIT_FAILURE); 
        }
        
    } else {
        /* 使用預設伺服器資訊 */
        strcpy(server, SERVER);
        
        if (argc > 2) {
            
            if (strlen(argv[1]) == 1) {
                strcpy(&mode, argv[1]);
            }
            
            if (strlen(argv[2]) > 0) {
                strcpy(filename, argv[2]);
            }
            
            if (mode == 'D' || mode == 'U') {
                client_mode(server, mode, filename);
            } else {
                printf("請提供正確的上傳或下載模式（U/D）！\n");
                exit(EXIT_FAILURE); 
            }
            
            client_mode(server, mode, filename);
            
        } else if (argc == 2) {
            printf("請提供檔案名稱、上傳或下載模式（U/D）！\n");
            exit(EXIT_FAILURE);  
          
        } else {
            printf("以檔案伺服器模式執行...\n");

            server_mode();
        }
        
    }
    
    return EXIT_SUCCESS;
}


/* work on ANSI terminals, demands POSIX. */

void cls()
{
    const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREE_ANSI, 12);
}


void split_path_file(char** p, char** f, char* pf) {
    char *slash = pf, *next;
    while ((next = strpbrk(slash + 1, "\\/"))) slash = next;
    if (pf != slash) slash++;
    *p = strndup(pf, slash - pf);
    *f = strdup(slash);
}
