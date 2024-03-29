/**
 * client2.c : 簡易檔案傳輸工具程式 v2
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

#include "client2.h"


int client_mode(char* server, char mode, char* filename) {
    
    DEBUG("除錯模式啟動\n");

    /*  建立客戶端 socket  */

    if ((client_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket() 呼叫失敗");
        exit(EXIT_FAILURE);
    }

    /*  設定客戶端 socket  */

    bzero(&server_address, sizeof (struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server);
    server_address.sin_port = htons(PORT);
    len = sizeof (server_address);

    /* 與伺服器建立連線 */

    if ((result = connect(client_sockfd, (struct sockaddr *) &server_address, len)) < 0) {
        perror("connect() 呼叫失敗"); 
        close(client_sockfd);
        exit(EXIT_FAILURE);

    } else {
        printf("連線中...\n");
    }
    
    FD_ZERO(&readfds);
    FD_SET(client_sockfd, &readfds);
    
    /* 開始傳送接收資料 */
    
    bzero(bytebuf, 1);           
    bzero(buf, BUFF_LEN);
    bzero(tmp, BUFF_LEN);
    
    /* 通知伺服器目前的模式 (Download/Upload) */
    
    if (send(client_sockfd, &mode, sizeof (char), 0)) {
        DEBUG("mode 訊息已送出\n");
    }
    
    if (mode == 'D') {    /* 下載模式 */
    
        /* 要求伺服器提供使用者指定的檔案 */

        /* 傳送檔案名稱給伺服器 */
    
        if (send(client_sockfd, filename, BUFF_LEN, 0)) {
            DEBUG("Filename 訊息已送出\n");
        }
    
        /* 處理檔案路徑 */
        /* malloc two buffs */
        char* _path = malloc(BUFF_LEN);
        char*_fname = malloc(BUFF_LEN);
        
        split_path_file(&_path, &_fname, filename);
        strcpy(filename, _fname);
        chdir("./downloads");
        
        DEBUG("File name: %s\n", filename);

        /* 接收檔案大小 */
        
        if (recv(client_sockfd, &nread, sizeof (int), 0)) {
            DEBUG("Filesize 訊息已接收\n");
            printf("檔案大小為 %d bytes\n", nread);
        }
        
        if (nread <= 0) {
            printf("伺服器上沒有這個檔案喔\n"); 
            close(client_sockfd);
            exit(EXIT_FAILURE);
        }
        
        fp = fopen(filename, "wb, ccs=UTF-8");
        
         /* 顯示進度 */

        for (i = 0, cread = 1, per = 0; i < nread; i = i + 1, cread = cread + 1) { 
            
            bzero(bytebuf, sizeof (wchar_t*));
            
            st = recv(client_sockfd, bytebuf, 1, 0);
            
            per = ((float) cread / (float) nread) * 100;
            j = i % DOT_LEN;
            if (i == nread - 1) {
                j = DOT_LEN - 1;
            }
            
            /* 處理取得的資料 */
            
            fwrite(bytebuf, 1, 1, fp);
            
            cls();
            printf("\r進度：%3d %c %8s 正在下載 \"%s\" %d / %d bytes\n", per, '%', dot[j], filename, cread, nread);
            usleep(250);    /* 避免 CPU 佔用過高與畫面閃爍 */

        }
        
        fclose(fp);
        
        printf("傳輸完成\n");
        
        chdir("..");
    }
    
    if (mode == 'U') {    /* 上傳模式 */
    
        /* 處理檔案路徑 */
        /* malloc two buffs */
        char* _path = malloc(BUFF_LEN);
        char*_fname = malloc(BUFF_LEN);
        
        split_path_file(&_path, &_fname, filename);
        strcpy(filename, _fname);
        chdir(_path);
        
        DEBUG("File name: %s\n", filename);
        
        fp = fopen(filename, "rb, ccs=UTF-8");
        if (!fp) {
            printf("找不到 %s%s\n", _path, filename);
            close(fd);
            close(client_sockfd);
            exit(EXIT_FAILURE);
        }

        /* 開始傳送資料 */

        /* 取得檔案大小 */
        
        fseek(fp, 0L, SEEK_END);
        nread = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        
        DEBUG("File size: %d\n", nread);
        
        /* 傳送檔案名稱與檔案大小給對方 */
        
        if (send(client_sockfd, filename, BUFF_LEN, 0)) {
            DEBUG("Filename 訊息已送出\n");
        }
        
        if (send(client_sockfd, &nread, sizeof (int), 0)) {
            DEBUG("Filesize 訊息已送出\n");
        }

        for (i = 0, cread = 1, per = 0; i < nread; i = i + 1, cread = cread + 1) { 
            
            bzero(bytebuf, sizeof (wchar_t*));
            
            if (!feof(fp)) {
                st = fread(bytebuf, 1, 1, fp);
            }
                   
            st = send(client_sockfd, bytebuf, 1, 0);
            
            per = ((float) cread / (float) nread) * 100;
            j = i % DOT_LEN;
            if (i == nread - 1) {
                j = DOT_LEN - 1;
            }
            
            cls();
            printf("\r進度：%3d %c %8s 正在上傳 \"%s\" %d / %d bytes\n", per, '%', dot[j], filename, cread, nread);
            usleep(250);    /* 避免 CPU 佔用過高與畫面閃爍 */

        }
        
        fclose(fp);
    
        printf("傳輸完成\n");
    
        chdir("..");
    }
    
    
    /* 關閉 socket */
    
    close(client_sockfd);
    exit(EXIT_SUCCESS);
    
    return EXIT_SUCCESS;
}


